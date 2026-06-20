
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "gtagml-parse-state.h"

#include "tile/gtagml-tile.h"
#include "kernel/graph/gtagml-graph.h"
#include "tag-command/gtagml-tag-command.h"
#include "whitespace/gtagml-whitespace.h"
#include "kernel/gtagml-root.h"
#include "tile/gtagml-attribute-tile.h"
#include "tile/gtagml-paralex-tile.h"
#include "tile/gtagml-raw-tile.h"

#include "kernel/grammar/gtagml-parser.h"

#include "annotation/gtagml-annotation-tile.h"

#include "kernel/document/gtagml-document-info.h"
//#include "kernel/document/gtagml-document-light-xml.h"

#include "kans.h"

#include <QDebug>


USING_KANS(GTagML)

GTagML_Parse_State::GTagML_Parse_State(GTagML_Graph& g, GTagML_Document_Info& document_info)
 : Flags(0), markup_position_(g.root_node()), acc_mode_(Acc_Mode::Main_Tile), document_info_(document_info),
   current_parsing_mode_(GTagML_Parsing_Modes::GTagML), //?current_annotation_tile_(nullptr),
   parser_(nullptr), tile_acc_length_adjustment_(0),
   tile_acc_qts_(&tile_acc_), string_literal_acc_qts_(&string_literal_acc_),
   current_raw_format_("latex"), held_semantic_mark_mode_(0)

   ,streams_(this)

   ,current_exs_group_number_(0)
   ,current_exs_number_(0)
   ,current_exs_offset_(0)

   ,current_slash_line_plus_count_(0)

   ,current_paragraph_type_(Paragraph_Types::N_A)
   ,current_paragraph_count_(0), current_paragraph_bridge_(0)
   //?,streams_.primary_acc()stream_(&streams_.primary_acc())
   ,sentences_text_stream_(&sentences_text_)
   ,sentence_gaps_stream_(&sentence_gaps_)
   ,sentences_section_heading_stream_(&sentences_section_heading_)
   ,latex_section_heading_stream_(&latex_section_heading_)
   ,heading_counts_({0,0})
   ,sentence_nesting_depth_(1)
   //,section_id_(0)
   ,sentence_id_(0)
   ,paragraph_id_(0)
   ,footnote_id_(0)
   ,current_section_number_(0)
   ,current_item_count_(0)
   ,pseudo_paragraph_counts_({0,0})
   ,fn_count_(0)
   ,parameter_count_(0)
   ,last_parameter_kind_(Parameter_Kinds::N_A)
//   ,jats_stream_(&jats_)
{

// flags.use_latex_sdi_all_markers = true;
// flags.use_latex_sdi_paragraph_markers = true;
}

void GTagML_Parse_State::auto_closed_tag_command_leave(QString post)
{
 parse_context_.flags.after_auto_closed_tag_command = false;
}

void GTagML_Parse_State::enter_tag_command_parameter(Parameter_Kinds pk, QString post)
{
 reset_primary();

 if(parameter_count_ > 0)
 {
  if(last_parameter_kind_ & Parameter_Kinds::Mandatory)
    streams_.latex("}");
  else if(last_parameter_kind_ & Parameter_Kinds::Optional)
    streams_.latex("]");
 }
 else
   streams_.chop_latex(1);

// if(post.isEmpty())
// {
//  ++parameter_count_;
// }

 if(post.isEmpty())
 {
  last_parameter_kind_ = pk;
  ++parameter_count_;
 }
 else
 {
  last_parameter_kind_ = Parameter_Kinds::N_A;
  parameter_count_ = 0;
 }


 if(pk & Parameter_Kinds::Mandatory)
 {
  streams_.latex("{");
 }
 else if(pk & Parameter_Kinds::Optional)
 {
  streams_.latex("[");
 }
}


void GTagML_Parse_State::outer_tag_command_leave(QString pre, QString inter, QString orig, QString post)
{
 reset_primary();

 // //  check for encloser mismatch?

 QString no_auto_paragraph_flag;

 Tag_Command_Name_Info pop_info = tag_command_name_stack_.pop();
 QString pop = pop_info.name;

 u2 pop_parts = 0;
 if(pop_info.flags & Tag_Command_Name_Flags::Multi_Transpile)
   pop_parts = pop.count("/") + 1;

//?
 if(pop.contains("`*`"))
 {
  QStringVector pops = pop.split("`*`");
  pop = pops.takeLast();
  no_auto_paragraph_flag = pops.join("`") + "*";
 }

 u2 paragraph_bridge = 0;

 bool orig_star = false;
 bool orig_no_star = false;

 bool inter_star = false;

 if(inter.endsWith("*"))
   inter_star = true;

//?   paragraph_bridge = (u2) Paragraph_Bridge_Flags::Double_New_Line + 1;

 else if(inter.endsWith(";"))
   paragraph_bridge = (u2) Paragraph_Bridge_Flags::Normal + 1;

 if(no_auto_paragraph_flag.endsWith("*"))
   paragraph_bridge = (u2) Paragraph_Bridge_Flags::Double_New_Line + 1;


 if(orig.endsWith("*"))
 {
//?  paragraph_bridge = (u2) Paragraph_Bridge_Flags::Double_New_Line + 1;
  orig_star = true;
  orig.chop(1);
 }

 QStringList pres = pre.split("`");
 pre = pres.takeFirst();

// if(!orig.isEmpty())
//   pres.push_back(orig);

 if(pop.startsWith("-`*`"))
 {
  pop = pop.mid(4);
 }

 if(!orig.isEmpty())
 {
  orig_no_star = !orig_star;
  if(pop.startsWith(","))
    streams_.latex_stream() << "\n%\n\\end{" + pop.mid(1) + "}\n";
 }
 else
 {
  if(pres.size() == 0)
  {
   if(pop_parts == 0)
     streams_.latex_stream() << "}";
   else
     streams_.latex_stream() << QString(pop_parts, QChar::fromLatin1('}'));

//  if(no_auto_paragraph_flag.endsWith("*"))
//  {
//   streams_.latex_stream() << "\n";
//   set_paragraph_bridge_ = 1;
//  }
  }
  else
  {
   if(pres.size() == 1)
   {
    if(pop.startsWith(",<GT>"))
      streams_.latex_stream() << "\n%\n\\end{GT" + pop.mid(5) + "}\n";
    else if(pop.startsWith(","))
      streams_.latex_stream() << "\n%\n\\end{" + pop.mid(1) + "}\n";
    // //? pop_parts here?
    else if(pop_parts == 0)
      streams_.latex_stream() << "}";
    else
      streams_.latex_stream() << QString(pop_parts, QChar::fromLatin1('}'));
   }

//?
//  if(no_auto_paragraph_flag.endsWith("*"))
//    streams_.latex_stream() << "\n";

  }

 }

 if(pre == "+" || orig_star || inter_star)
 {
  if(inter_star)
    streams_.latex("\n");
  else
    leave_subparagraph_with_continue();

  if(orig_star || inter_star)
    if(parse_context_.flags.auto_paragraph_mode)
      parse_context_.flags.paragraph_continuation = true;

  //?++paragraph_continuation_count_;
  streams_.latex_stream() << "\n";
 }
 else if(pre == "%" || orig_no_star)
 {
  streams_.latex_stream() << " %! (subparagraph ends paragraph)";
  leave_subparagraph_with_reset();
 }


 if(paragraph_bridge)
   set_paragraph_bridge(paragraph_bridge - 1);
}

void GTagML_Parse_State::resolve_tag_command_name_transform()
{
 tag_command_name_transforms_[current_auto_closed_tag_command_] =
   current_tag_command_name_transform_;
 current_tag_command_name_transform_.clear();
 parse_context_.flags.inside_tag_command_name_transform = false;
}

void GTagML_Parse_State::tag_command_name_transform_acc(QString text)
{
 if(text.size() == 1)
 {
  if((text == "]" && active_encloser_ == "[")
    || (text == "}" && active_encloser_ == "{")
    || (text == ")" && active_encloser_ == "("))
  {
   resolve_tag_command_name_transform();
   return;
  }
 }

 current_tag_command_name_transform_ += text;
}

void GTagML_Parse_State::tag_command_name_transform_entry()
{
 parse_context_.flags.after_auto_closed_tag_command = false;
 parse_context_.flags.inside_tag_command_name_transform = true;
}


void GTagML_Parse_State::item_marker(QString pre)
{
 reset_primary();

 streams_.latex_stream() << "\n\n\\" << GT_item_;
 if(pre.isEmpty())
   streams_.latex_stream() << "Item{} ";
 else
   streams_.latex_stream() << "Item[" << pre << "] ";
}


void GTagML_Parse_State::outer_tag_command_entry(QString blank_lines,
  QString outer,
  QString pre, QString main, QStringVector supl, QString post, QString at_flag)
{
 u2 nlcount = blank_lines.count(QLatin1Char('\n'));

 QString return_to_acc;
 if(main.endsWith("/"))
 {
  // //  maybe a kind of endsWith that returns the count of the char at the end
  if(!main.endsWith("//"))
  {
   if(post == "." || post == ",")
   {
    main.chop(1);
    return_to_acc = post;
   }
  }
 }

 QString no_auto_paragraph_flag;

//? qDebug() << "\n\n1= " << streams_.get_latex();

 if(nlcount > 1)
 {
  if(pre.endsWith("*"))
  {
   no_auto_paragraph_flag = pre.chopped(1) + "`*`";
   if(pre.startsWith("-"))
     primary_acc("\n\n");
   else
   {
    auto_close_paragraph();
    primary_acc("\n\n");
   }
  }
  else if(parse_context_.flags.auto_paragraph_mode && !pre.endsWith("%"))
  {
   if(tag_command_name_stack_.isEmpty())
     auto_new_paragraph();
   else
   {
    Tag_Command_Name_Info top_info = tag_command_name_stack_.top();
    if(top_info.flags & Tag_Command_Name_Flags::Temp_Suppress_Auto_Paragraphs)
      streams_.latex("\n\n");
    else
      auto_new_paragraph();
   }
  }
 }
 else if(nlcount == 1)
 {
  primary_acc(blank_lines);
 }

 //
 reset_primary();

 primary_acc(return_to_acc);

//? qDebug() << "\n\n2= " << streams_.get_latex();

 active_encloser_ = outer;

 QString alt;

 main = main.simplified();

 QStringList tokens = main.split("//");

 if(main.contains(" "))
 {
  QStringList qsl = main.split(" ");
  main = qsl.takeFirst();

  QString arrow;
  for(u1 i = 0; i < qsl.size(); ++i)
  {
   if(i % 2)
   {
    QString repl = qsl[i];
    if(arrow.startsWith("->"))
      latex_command_name_transforms_[main] = repl;
   }
   else
     arrow = qsl[i];
  }

  if(post == ";" && outer == "{")
  {
   QString next_char = parser_->skip(1);
   if(next_char != "}")
   {
    // //  problem?
   }
   return;
  }

 }

 QStringVector latex_tokens;
 QStringVector tsom_tokens;

 auto convert_double_dollar = [](QString& name)
 {
  if(name == "$$")
    name = "quasi@desc@item";
 };

 for(QString token : tokens)
 {
  QString latex = latex_command_name_transforms_.value(token, token);

  QString tsom = latex;

  // //  should this be optional?
  latex.replace("-@", "@");
  latex.replace("---", "@@@%%%");
  latex.replace("--", "@@%%");
  latex.replace("-", "");
  latex.replace("@@@%%%", "--");
  latex.replace("@@%%", "-");

  latex.replace("+", "PLUS");

  if(latex != tsom) // i.e., latex has changed
  {
   latex.replace("0", "ZERO");
   latex.replace("1", "ONE");
   latex.replace("2", "TWO");
   latex.replace("3", "THREE");
   latex.replace("4", "FOUR");
   latex.replace("5", "FIVE");
   latex.replace("6", "SIX");
   latex.replace("7", "SEVEN");
   latex.replace("8", "EIGHT");
   latex.replace("9", "NINE");
  }

  if(at_flag.contains("/@"))
  {
   tsom.replace("-@", "@");
   tsom.replace("---", "--");
   tsom.replace("--", "-");
  }

  convert_double_dollar(latex);

  latex_tokens.push_back(latex);
  tsom_tokens.push_back(tsom);
 }

 QString& ltf = latex_tokens[0];

 QRegularExpression num_end("([^\\d].*)(\\d+)");
 QRegularExpressionMatch m = num_end.match(ltf);
 if(m.hasMatch())
 {
  QString m1 = m.captured(1);
  QString m2 = m.captured(2);
  ltf = m1 + "|" + m2 + "|";

  if(m1 == "s")
    section_command(m2.toUInt());
 }

 QStringList ltf_parts;

 if(ltf.contains("/"))
   ltf_parts = ltf.split("/");

 QString prepend;
 QString lprepend;

 QString append;

 if(pre.endsWith("%"))
 {
  streams_.latex("\n%\n");

  pre = pre.mid(1);
  prepend = "GT";
  lprepend = "<GT>";
  enter_implicit_subparagraph(pre, ltf);

  append = "\n %! (subparagraph)";

  //parse_context_.flags.ignore_blank_lines = true;
 }


 if(post == ",")
 {
  Tag_Command_Name_Flags check_Temp_Suppress_Auto_Paragraphs
    = parse_context_.flags.auto_paragraph_mode?
    Tag_Command_Name_Flags::Temp_Suppress_Auto_Paragraphs : Tag_Command_Name_Flags::N_A;

  if(ltf_parts.isEmpty())
  {
   convert_double_dollar(ltf);

   streams_.latex_stream() << "\\begin{" << prepend << ltf << "}";
   tag_command_name_stack_.push({no_auto_paragraph_flag + post + lprepend + main,
     check_Temp_Suppress_Auto_Paragraphs | Tag_Command_Name_Flags::Normal});
  }
  else
  {
   for(QString ltfp : ltf_parts)
   {
    convert_double_dollar(ltfp);

    streams_.latex_stream() << "\\begin{" << prepend << ltfp << "}";
   }
   tag_command_name_stack_.push({no_auto_paragraph_flag + post + lprepend + main,
     check_Temp_Suppress_Auto_Paragraphs | Tag_Command_Name_Flags::Multi_Transpile});
  }
 }
 else if(ltf_parts.isEmpty())
 {
  QString ltl = latex_tokens.takeLast();
  if(latex_tokens.size() > 1 && post == "`")
  {
   QString ltf = latex_tokens.takeFirst();
   convert_double_dollar(ltf);
   for(QString token : latex_tokens)
   {
    streams_.latex_stream() << "\\" << prepend << ltf << "{"
      << token << "}/";
   }
   streams_.latex_stream() << "\\" << prepend << ltf << "{"
     << ltl << "}";

   QString next_char = parser_->skip(1);
   if(next_char != ")" && next_char != "]")
   {
    // //  problem?
   }
  }
  else
  {
   convert_double_dollar(ltl);

   for(QString token : latex_tokens)
   {
    convert_double_dollar(token);
    streams_.latex_stream() << "\\" << prepend << token << "{}/";
   }

   streams_.latex_stream() << "\\" << prepend << ltl << "{";
   if(post == ";")
   {
    parse_context_.flags.after_auto_closed_tag_command = true;
    streams_.latex_stream() << "}";
    current_auto_closed_tag_command_ = no_auto_paragraph_flag + lprepend + ltl;
   }
   else if(post == ".")
   {
    tag_command_name_stack_.push({no_auto_paragraph_flag + post + lprepend + ltl,
      Tag_Command_Name_Flags::Normal});
   }
  }
 }
 else
 {
  u1 sz = ltf_parts.size();
  for(QString ltfp : ltf_parts)
  {
   convert_double_dollar(ltfp);

   streams_.latex_stream() << "\\" << prepend << ltfp << "{";
  }
  if(post == ";")
  {
   parse_context_.flags.after_auto_closed_tag_command = true;
   streams_.latex(QString(sz, QChar::fromLatin1('}')));
   current_auto_closed_tag_command_ = no_auto_paragraph_flag + lprepend + ltf;
  }
  else if(post == ".")
  {
   tag_command_name_stack_.push({no_auto_paragraph_flag + post + lprepend + ltf,
     Tag_Command_Name_Flags::Multi_Transpile});
  }

 }

 if(!append.isEmpty())
   streams_.latex(append);
}


void GTagML_Parse_State::insert_latex_item()
{
 streams_.latex("\n\n\\item{}  ");
}


void GTagML_Parse_State::parse_processing_instruction(QString instruction, QString lrcode)
{
 reset_primary();

 if(instruction == "spar")
 {
  if(lrcode == "0111")
    enter_auto_paragraph_mode();
  else if(lrcode == "1110")
    parse_context_.flags.auto_paragraph_mode = false;

  return;
 }

 if(instruction == "end")
 {
  if(lrcode == "1111")
  {
   // //  same lines as /// // -- refactor?
   prepare_end_document();

//   reset_primary();
//   check_close_paragraph();

//   streams_.latex_stream() << "\n\n";
//   parse_context_.flags.auto_paragraph_mode = false;

//   end_document();

   return;
  }
 }
}


void GTagML_Parse_State::enter_special_section(QString text)
{
 if(text == "Abstract")
   enter_abstract();

 else if(text == "pa")
 {
  auto_new_paragraph("pa.1");

//  reset_primary();

//  check_close_paragraph();

//  current_paragraph_type_ = Paragraph_Types::Paragraph_Addendum;
 }
}

void GTagML_Parse_State::init(GTagML_Parser* parser)
{
 parser_ = parser;

 streams_.init();
}

void GTagML_Parse_State::enter_abstract()
{
 reset_primary();

 check_close_paragraph();

 current_paragraph_type_ = Paragraph_Types::Abstract;

 streams_.enter_abstract();

}




void GTagML_Parse_State::primary_acc(QString text)
{
 if(flags.heading_acc)
 {
  if(!flags.latex_only)
    sentences_section_heading_stream_ << text;

  if(!flags.sentences_only)
    latex_section_heading_stream_ << text;

  return;
 }


 if(flags.just_ended_sentence)
 {
  flags.just_ended_sentence = false;

  QString space;

  s4 ix = text.indexOf(QRegularExpression("\\S"));

  if(ix == -1)
    space = text;
  else
    space = text.left(ix);

  u4 count = space.count('\n');

  if(count < 2)
  {
   if(flags.use_latex_sdi_all_markers)
     streams_.latex(" \\> ");

   ++sentence_id_;
   streams_.sentences_sdi_stream() << "\n\n--- Sentence/switch\nid: "
     << sentence_id_ << "\nr#  "
     << document_info_.line_and_column_string(parser_->current_position())
     << "\n";

   if(text.startsWith(" "))
     text.chop(1);
  }

 }

 else
 {
  if(flags.await_paragraph_start)
  {
   flags.await_paragraph_start = false;
   if(flags.use_latex_sdi_all_markers || flags.use_latex_sdi_paragraph_markers)
     streams_.latex("\\:");

   ++paragraph_id_;
   streams_.sentences_sdi_stream() << "\n\n--- Paragraph/start\nid: " << paragraph_id_
     << "\nr#  "
     << document_info_.line_and_column_string(parser_->current_position())
     << "\n";

  }

  if(flags.await_sentence_start)
  {
   flags.await_sentence_start = false;
   if(flags.use_latex_sdi_all_markers)
     streams_.latex("\\+");

   ++sentence_id_;
   streams_.sentences_sdi_stream() << "\n\n--- Sentence/start\nid: " << sentence_id_
     << "\nr#  "
     << document_info_.line_and_column_string(parser_->current_position());
  }
 }

 streams_.primary(text);
}

void GTagML_Parse_State::reset_primary()
{
 auto handle_sentences = [this]()
 {
  if(flags.sentences_latex_filter)
  {
   QString pa = streams_.primary_acc();
   QRegularExpression rx ("\\\\\\w+");
   while(true)
   {
    QRegularExpressionMatch m = rx.match(pa);
    if(m.hasMatch())
      pa.replace(m.capturedStart(), m.capturedEnd() - m.capturedStart(), "");
    else
      break;
   }
   pa.replace("{", "");
   pa.replace("}", "");
   pa.replace("\\", "");
   sentences_text_stream_ << pa;
  }
  else
    sentences_text_stream_ << streams_.primary_acc();
 };

 if(flags.sentences_only)
 {
  handle_sentences();
 }

 else if(flags.latex_only)
 {
  streams_.latex_stream() << streams_.primary_acc();
  u4 pos = parser_->current_position();

  sentence_gaps_stream_ << "\n +" << line_and_column_string_tight(pos);

  pos += streams_.primary_acc().size();

  sentence_gaps_stream_ << ":" << streams_.primary_acc().size();
  sentence_gaps_stream_ << "=" << line_and_column_string_tight(pos);
 }

 else
 {
  streams_.latex_stream() << streams_.primary_acc();
  streams_.xml_writer().writeCharacters(streams_.primary_acc());
  handle_sentences();
 }

 streams_.primary_acc().clear();
}


//void GTagML_Parse_State::subsection_heading(QString stext, QString ltext, u1 level)
//{
//// reset_primary();
//// check_close_paragraph();

// streams_.xml_writer().writeTextElement("s2", stext);
// streams_.latex_stream() << "\n\n\\s|2|{" << ltext << "}\n";

// streams_.sentences_sdi_stream() << "\n\n--- Section/start\n-l  2\n-t" << stext << "\n";

// set_paragraph_bridge();
//}


void GTagML_Parse_State::section_heading(QString stext, QString ltext, u1 level)
{
 ++current_section_number_;

// reset_primary();
// check_close_paragraph();

 if(current_section_number_ > 1)
 {
  streams_.xml_writer().writeCharacters("\n\n@=/sec=@\n\n");
 }

 streams_.xml_writer().writeCharacters("\n\n");

 streams_.xml_writer().writeTextElement("s1", stext);
 streams_.latex_stream() << "\n\n\\s|" << level << "|{" << ltext << "}\n";

 ++section_ids_[level];

 streams_.sentences_sdi_stream() << "\n\n--- Section/start\n-l  " << level << "\n-i  "
   << section_ids_[level] << "\n-t  " << stext << "\n";

 set_paragraph_bridge();
}

void GTagML_Parse_State::blank_line_as_visible_space()
{
 reset_primary();

 streams_.latex_stream() << "\n\n \\visbreak{}\n";
}

// <%- \ifnum\presetStretch=2\vspace*{9pt}\else\fi -%>


//void GTagML_Parse_State::latex_only(QString text)
//{
// streams_.latex_stream() << text;
//}


void GTagML_Parse_State::enter_latex_only_to_space(QString match, QString space)
{
 primary_acc(space);

 enter_latex_only_to_space(match);

 //streams_.latex_stream() << text;
}

void GTagML_Parse_State::enter_latex_only_to_space(QString match)
{
 enter_latex_only(match);

 parse_context_.flags.latex_only_to_space = true;
}

void GTagML_Parse_State::leave_latex_only_to_space(QString match)
{
 leave_latex_only(match);

 parse_context_.flags.latex_only_to_space = false;
}


void GTagML_Parse_State::enter_latex_only(QString match)
{
 reset_primary();

 sentence_gaps_stream_ << "\n @l(" << match << ") +" << line_and_column_string_tight();

 parse_context_.flags.latex_only = true;
 flags.latex_only = true;
}

void GTagML_Parse_State::leave_latex_only(QString match)
{
 reset_primary();

 sentence_gaps_stream_ << "\n @l(" << match << ") -" << line_and_column_string_tight();

 parse_context_.flags.latex_only = false;
 flags.latex_only = false;
}


void GTagML_Parse_State::enter_sentences_only(QString open, QString pre_space)
{
 u1 count = open.size();
 if(pre_space.contains("\n"))
   primary_acc(" ");

 reset_primary();

 parse_context_.flags.sentences_only = true;
 flags.sentences_only = true;
}

void GTagML_Parse_State::leave_sentences_only(QString close, QString post_space)
{
 reset_primary();

 parse_context_.flags.sentences_only = false;
 flags.sentences_only = false;

 u1 count = close.size();

 if(count == 1)
 {
  if(post_space.contains("\n"))
    primary_acc(" ");
  else
    primary_acc(post_space);
 }
 else if(count == 2)
 {
  if(post_space.contains("\n"))
    primary_acc("\n");
  else
    primary_acc(post_space);
 }
}

void GTagML_Parse_State::force_switch_sentence()
{
 end_sentence("");
}

void GTagML_Parse_State::ell_2_nonbreak()
{
 reset_primary();

 if(!flags.sentences_only)
   streams_.latex_stream() << ".~";

 if(!flags.latex_only)
   sentences_text_stream_ << ". ";
}

void GTagML_Parse_State::ell_count(u1 count, QString follow)
{
 reset_primary();

 static QStringList latex = {".\\@", "\\ellThree{2pt}{2pt}",
   "\\ellFour{2pt}{2pt}"};

 static QStringList sentences = {".", "...",
   "...."};

 if(!flags.sentences_only)
   streams_.latex_stream() << latex.value(count - 2) << follow;

 if(!flags.latex_only)
   sentences_text_stream_ << sentences.value(count - 2) << follow;


// if(count == 3)
// {
//  if(!flags.sentences_only)
//    streams_.latex_stream() << ;

//  if(!flags.latex_only)
//    sentences_text_stream_ << "...";
// }

// else if(count == 4)
// {
//  if(!flags.sentences_only)
//    streams_.latex_stream() << "\\ellFour{2pt}{2pt}";

//  if(!flags.latex_only)
//    sentences_text_stream_ << "....";
// }

}

void GTagML_Parse_State::noindent_marker()
{
 if(!flags.sentences_only)
   streams_.latex_stream() << "\\noindent{}";
}


void GTagML_Parse_State::footnote_marker(QString text)
{
 reset_primary();

 if(!flags.latex_only)
   sentences_text_stream_ << "\\" << text;

 if(!flags.sentences_only)
   streams_.latex_stream() << "\\fnm{" << text << "}";
}


void GTagML_Parse_State::end_sentence(QString punctuation,
  u1 nesting_code, QVector<QPair<QString, QString>> supplements)
{
 flags.just_ended_sentence = true;

 if(nesting_codes_by_depth_.contains(sentence_nesting_depth_))
 {
  if(nesting_code == Nesting_Codes::Signal_Default)
    nesting_code = nesting_codes_by_depth_.take(sentence_nesting_depth_);
  else
    nesting_code |= nesting_codes_by_depth_.take(sentence_nesting_depth_);
 }

 reset_primary();

 streams_.sentences_sdi_stream() << "\n\n--- Sentence//end";

 streams_.sentences_sdi_stream() << "\nid: " << sentence_id_;

 streams_.sentences_sdi_stream() << "\nr#  " <<
   document_info_.line_and_column_string(parser_->current_position());

 if(!sentence_gaps_.isEmpty())
 {
  streams_.sentences_sdi_stream() << "\ng. " << sentence_gaps_to_string();
  sentence_gaps_.clear();
 }

 streams_.sentences_sdi_stream() << "\np: " << punctuation;

 u1 nc = sentence_nesting_depth_;

 if(nesting_code != Nesting_Codes::Signal_Default)
   nc |= nesting_code;

 if(nc != 1)
   streams_.sentences_sdi_stream() << "\nN: " << nc;

 for(auto pr : supplements)
 {
  streams_.sentences_sdi_stream() << "\n" << pr.first << ": "
    << pr.second;
 }

 streams_.sentences_sdi_stream() << "\nt. " << sentences_text_to_string() << "\n";

 sentences_text_.clear();

 if(!flags.sentences_only)
 {
  streams_.xml_writer().writeCharacters(punctuation);
  streams_.latex_stream() << punctuation; // << "  ";
 }

  //? streams_.latex_stream() << punctuation << "@ ";

// streams_.primary_acc()stream_ << punctuation << "  ";
}




void GTagML_Parse_State::heading(u1 count, QString stext, QString ltext)
{
 if(count == 3)
 {
  static QString text_default = "Section %1";
  ++current_section_counts_[1];
  if(stext.isEmpty())
    stext = text_default.arg(current_section_counts_[1]);

  if(ltext.isEmpty())
    ltext = text_default.arg(current_section_counts_[1]);

  section_heading(stext, ltext, 1);
 }

 else if(count == 4)
 {
  streams_.latex_stream() << "\n\n\\addendum{";
 }

}

void GTagML_Parse_State::prepare_bibliography()
{
 streams_.xml_writer().writeCharacters("\n\n");
 //<sec><title>Introduction</title> <ref

// streams_.xml_writer().writeStartElement("sec");
// streams_.xml_writer().writeTextElement("title", "Bibliography");

 streams_.xml_writer().writeStartElement("back");
 streams_.xml_writer().writeStartElement("ref-list");
 streams_.xml_writer().writeTextElement("title", "Bibliography");
 streams_.xml_writer().writeComment("bib:here");
 streams_.xml_writer().writeEndElement();

 streams_.xml_writer().writeEndElement();
// streams_.xml_writer().writeEndElement();
}


void GTagML_Parse_State::end_inner_document()
{
 reset_primary();

 streams_.latex_stream() << "\n%END-TEMPLATE%\n";

}

void GTagML_Parse_State::end_document()
{
// // //  for now
// end_inner_document();
// return;

 reset_primary();


//?? check_close_paragraph();

 streams_.latex_stream() << "\n%END-TEMPLATE%\n";

// streams_.latex_stream() << "\n\\end{document}";

//? streams_.xml_writer().writeEndElement();
//? streams_.xml_writer().writeComment("prepare end document");

 streams_.xml_writer().writeCharacters("\n\n@=/sec=@\n");
 streams_.xml_writer().writeComment("end section");

 streams_.xml_writer().writeEndElement();
 streams_.xml_writer().writeComment("ended body");

 streams_.xml_writer().writeComment("inserting bibliography");
 prepare_bibliography();

 streams_.xml_writer().writeEndElement();
 streams_.xml_writer().writeEndDocument();
 streams_.xml_writer().writeComment("ended document");

 parse_context_.flags.auto_paragraph_mode = false;
}

void GTagML_Parse_State::set_paragraph_bridge(u1 flag)
{
 current_paragraph_bridge_ =
   (current_paragraph_count_ << 2) + flag + 1;
}

void GTagML_Parse_State::leave_footnote(QString pretext, QString space)
{
 reset_primary();

 QString latex_space;

 if(pretext.endsWith("_"))
 {
  latex_space = space;
  pretext.chop(1);
 }

 if(pretext.isEmpty())
   end_sentence("");

 else if(pretext == "~")
 {
  end_sentence("");
  --sentence_nesting_depth_;
  nesting_codes_by_depth_[sentence_nesting_depth_] = Nesting_Codes::Continue_At_Start;
 }

 if(!flags.latex_only)
 {
  streams_.xml_writer().writeEndElement();
  streams_.xml_writer().writeComment("leave footnote");
  streams_.xml_writer().writeCharacters("\n\n");
 }

 if(!flags.xml_only)
   streams_.latex_stream() << latex_space << "}";

}

void GTagML_Parse_State::enter_footnote(QString pretext, QString space)
{
 reset_primary();

 QString latex_space;

 QString sdi_footnote_mark = "{<%1>}"_qt.arg(++footnote_id_);

 if(pretext.endsWith("_"))
 {
  latex_space = space;
  pretext.chop(1);
 }

 if(pretext.isEmpty())
 {
  end_sentence({{"F", sdi_footnote_mark}});
 }

 else if(pretext == "~")
 {
  sentences_text_stream_ << sdi_footnote_mark;
  end_sentence(Nesting_Codes::Continue_At_End);
  ++sentence_nesting_depth_;
 }

 ++fn_count_;

 if(!flags.latex_only)
 {
  streams_.xml_writer().writeCharacters("\n\n");
//  streams_.xml_writer().writeStartElement("doc-footnote");
  streams_.xml_writer().writeStartElement("fn");

  streams_.xml_writer().writeAttribute("id", "fn-%1"_qt.arg(fn_count_));
  streams_.xml_writer().writeCharacters("@fn-hold@");
 }

 if(!flags.xml_only)
   streams_.latex_stream() << "\\docFootnote{" << latex_space;
}

void GTagML_Parse_State::latex_command_via_semantic_annotation(QString concept, QString text)
{
 reset_primary();

 streams_.xml_writer().writeTextElement("SA-%1"_qt.arg(concept), text);
 streams_.latex_stream() << "\\SA" << concept << "{" << text << "}";
}


QString GTagML_Parse_State::line_and_column_string_tight()
{
 return line_and_column_string_tight(parser_->current_position());
}

QString GTagML_Parse_State::line_and_column_string()
{
 return line_and_column_string(parser_->current_position());
}


QString GTagML_Parse_State::line_and_column_string_tight(u4 pos)
{
 return document_info_.line_and_column_string(pos, "/");
}

QString GTagML_Parse_State::line_and_column_string(u4 pos)
{
 return document_info_.line_and_column_string(pos);
}

void GTagML_Parse_State::enter_sentences_latex_filter(QString pretext)
{
 reset_primary();

 sentence_gaps_stream_ << "\n @f(" << pretext << ") +" << line_and_column_string_tight();

 if(pretext == "~~")
 {
  end_sentence("");
  ++sentence_nesting_depth_;
 }

 else if(pretext == "~")
 {
  end_sentence("", Nesting_Codes::Continue_At_End);
  ++sentence_nesting_depth_;
//  parent_sentences_.push_back(sentences_text_);
//  sentences_text_.clear();
 }

 flags.sentences_latex_filter = true;
}

void GTagML_Parse_State::leave_sentences_latex_filter(QString pretext)
{
 reset_primary();

 sentence_gaps_stream_ << "\n @f(" << pretext << ") -" << line_and_column_string_tight();

 if(pretext == "~~")
 {
  end_sentence("");
  --sentence_nesting_depth_;
 }

 else if(pretext == "~")
 {
  end_sentence("");
  --sentence_nesting_depth_;
  nesting_codes_by_depth_[sentence_nesting_depth_] = Nesting_Codes::Continue_At_Start;
 }

 flags.sentences_latex_filter = false;
}

void GTagML_Parse_State::prepare_end_document()
{
 reset_primary();
 check_close_paragraph();

 streams_.latex_stream() << "\n\n";
 parse_context_.flags.auto_paragraph_mode = false;

 end_document();
}


void GTagML_Parse_State::section_command(u1 depth)
{

}


void GTagML_Parse_State::enter_heading(u1 count1, u1 count2)
{
 reset_primary();
 check_close_paragraph();

 heading_counts_ = {count1, count2};

 if(heading_counts_ == QPair<u1, u1> {3, 2})
 {
  streams_.latex_stream() << "\n\n";
  parse_context_.flags.auto_paragraph_mode = false;

  end_document();

  return;
 }


 flags.heading_acc = true;
 parse_context_.flags.heading_acc = true;
}


void GTagML_Parse_State::leave_heading()
{
 heading(heading_counts_.first, heading_counts_.second,
   sentences_section_heading_, latex_section_heading_);
 sentences_section_heading_.clear();
 latex_section_heading_.clear();
 flags.heading_acc = false;
 parse_context_.flags.heading_acc = false;
}


void GTagML_Parse_State::enter_justline(QString pretext)
{
 reset_primary();

 static QMap<QString, QString> static_map {
   {"--", "\\semijust{"},
   {"-", "\\justline{"},
 };

 QString code = static_map.value(pretext);

 if(!flags.sentences_only)
 {
  if(flags.heading_acc)
    latex_section_heading_stream_ << code;
  else
    streams_.latex_stream() << code;
 }

 parse_context_.flags.heading_acc = false;
 parse_context_.flags.justline = true;
}

void GTagML_Parse_State::leave_justline(QString pretext, QString follow)
{ 
 follow = follow.simplified().replace(' ', "");

 if(follow.startsWith("="))
   follow = follow.mid(1);

 QString code = "}";

 if(!follow.isEmpty())
   code += "(" + follow + ")";

 reset_primary();

 if(pretext != "|-")
 {
  parse_context_.flags.justline = false;
  parse_context_.flags.heading_acc = flags.heading_acc;
 }

 if(!flags.sentences_only)
 {
  if(flags.heading_acc)
    latex_section_heading_stream_ << code;
  else
    streams_.latex_stream() << code;
 }
}


void GTagML_Parse_State::heading(u1 count1, u1 count2, QString stext, QString ltext)
{
// reset_primary();
// qDebug() <<  latex_;
// check_close_paragraph();
// qDebug() << latex_;
// set_paragraph_bridge();

 if(count2 == 0)
 {
  heading(count1, stext, ltext); return;
 }

 if(count1 == 2)
 {
  if(count2 == 1)
  {
   streams_.latex_stream() << "\n\n\\notesdiv{" << ltext << "}\n";
   set_paragraph_bridge();
  }
  else
    section_heading(stext, ltext, count2);
 }

 if(count1 == 3)
 {
  if(count2 == 2)
    end_document();
 }
}

void GTagML_Parse_State::enter_auto_paragraph_mode()
{
 set_paragraph_bridge();
 parse_context_.flags.auto_paragraph_mode = true;
}

void GTagML_Parse_State::close_paragraph()
{
 if(flags.in_pa_1)
 {
  flags.in_pa_1 = false;
 }
 else
 {
  streams_.xml_writer().writeCharacters("\n");
  streams_.xml_writer().writeEndElement();
  streams_.xml_writer().writeComment("end of p1 paragraph");
  streams_.xml_writer().writeCharacters("\n\n");
 }

 if(flags.just_ended_sentence)
 {
  if(flags.use_latex_sdi_all_markers)
    streams_.latex_stream() << "\\<";

  streams_.sentences_sdi_stream() << "\n--- Sentence/end \nid: "
    << sentence_id_ << "\nr#  "
    << line_and_column_string() << "\n";

  if(!sentence_gaps_.isEmpty())
  {
   streams_.sentences_sdi_stream() << "g. " << sentence_gaps_to_string() << "\n";
   sentence_gaps_.clear();
  }
 }

 if(flags.use_latex_sdi_all_markers || flags.use_latex_sdi_paragraph_markers)
   streams_.latex_stream() << "\\;";

 streams_.sentences_sdi_stream() << "\n--- Paragraph/end \nid: " << paragraph_id_
   << "\ny: " << current_paragraph_type_to_string() << "\n";


 if(current_paragraph_type_ == Paragraph_Types::Abstract)
 {
  current_paragraph_type_ = Paragraph_Types::N_A;
  streams_.latex_stream() << "\n\\end{docAbstract}] \n";
  set_paragraph_bridge();
 }
 else
 {
//  if(flags.use_latex_sdi_paragraph_markers)
//    streams_.latex_stream() << "\n}\\;% end paragraph \n";
//  else
    streams_.latex_stream() << "\n}% end paragraph \n"; // pLevel
    ; //streams_.latex_stream() << "\n} % end paragraph";

 }

 flags.just_ended_sentence = false;
}

void GTagML_Parse_State::check_close_paragraph()
{
 if(current_paragraph_bridge_)
 {
  --current_paragraph_bridge_;
  Paragraph_Bridge_Flags flag = (Paragraph_Bridge_Flags) (current_paragraph_bridge_ & 3);
  if(flag == Paragraph_Bridge_Flags::Double_New_Line)
  {
    streams_.latex("\n\n");
//    streams_.latex("!!!");
//    streams_.latex("\n\n");
  }
  else if(flag == Paragraph_Bridge_Flags::Single_New_Line)
    streams_.latex("\n");
  current_paragraph_bridge_ = 0;
 }
 else
   close_paragraph();
}

void GTagML_Parse_State::show_latex()
{
 //?qDebug() << "\n" << latex_ << "\n";
}

void GTagML_Parse_State::paragraph_continuation()
{
 //?streams_.latex("% paragraph continuation\n");

//? n8 fl1 = parse_context_.Flags;

// if(parse_context_.flags.auto_paragraph_mode)
//   qDebug() << "APM1";

 parse_context_.flags.paragraph_continuation = false;
 current_paragraph_bridge_ = 0;

// n8 fl2 = parse_context_.Flags;

// if(parse_context_.flags.auto_paragraph_mode)
//   qDebug() << "APM2";

}


void GTagML_Parse_State::auto_close_paragraph()
{
 reset_primary();

 check_close_paragraph();
}

void GTagML_Parse_State::auto_new_paragraph()
{
// QString lat = streams_.get_latex();

// qDebug() << "\n\n lat = " << lat << "\n\n";

 auto_new_paragraph("p.1");
}

void GTagML_Parse_State::auto_new_paragraph(QString cmd)
{ // p.1
 auto_close_paragraph();

 current_slash_line_plus_count_ = 0;

 bool skip_flags = false;

 if(cmd == "p.1")
 {
  ++current_paragraph_count_;
  streams_.xml_writer().writeCharacters("\n");
  streams_.xml_writer().writeComment("enter " + cmd);
  streams_.xml_writer().writeStartElement(cmd);
  streams_.xml_writer().writeAttribute("id", QString::number(current_paragraph_count_).prepend("Para-"));
 }
 else if(cmd == "pa.1")
 {
  skip_flags = true;
  //   qDebug() << " \n\n   ! " << cmd;
  //   streams_.xml_writer().writeComment("bib:here");
  flags.in_pa_1 = true;
 }

 streams_.latex_stream() << "\n\\" << cmd << "{%\n";

// if(flags.use_latex_sdi_paragraph_markers)
//   streams_.latex_stream() << "\\:";


//? streams_.latex_stream() << "\n\n\\pLevelOne{";

//? streams_.latex_stream() << "\n\n\\pLevelOne{";

 if(skip_flags)
   return;

 flags.await_paragraph_start = true;
 flags.await_sentence_start = true;
}

void GTagML_Parse_State::pseudo_paragraph()
{
 reset_primary();

 streams_.xml_writer().writeCharacters("\n");
 streams_.xml_writer().writeEndElement();
 streams_.xml_writer().writeComment("end of list paragraph");

 ++pseudo_paragraph_counts_.second;

 streams_.xml_writer().writeStartElement("p");
// streams_.xml_writer().writeAttribute("paragraph-container", "list");

 streams_.xml_writer().writeAttribute("id", "P-list_%1-%2"_qt
   .arg(pseudo_paragraph_counts_.first).arg(pseudo_paragraph_counts_.second));

 //? streams_.xml_writer().writeTextElement("<pp>", "");
 streams_.latex_stream() << "\n\n\\pseudoIndent{} ";
}


void GTagML_Parse_State::enter_implicit_subparagraph(QString pre, QString text)
{
 GT_item_ = "GT" + text;

 //?streams_.latex_stream() << "\n\n\\begin{" << text << "}\n";
 //?parse_context_.flags.read_numbered_items = true;
 parse_context_.flags.ignore_blank_lines = true;

 parse_context_.flags.read_numbered_items = true;

}

void GTagML_Parse_State::enter_subparagraph(QString text, QString sup)
{
 reset_primary();

 current_item_count_ = 0;

 if(text == "exs")
 {
  streams_.latex_stream() << "\n\n\\begin{exsGroup}\n";
  streams_.xml_writer().writeStartElement("exs-group");
  parse_context_.flags.read_parens_as_label = true;
  parse_context_.flags.read_parens_as_ref = false;
 }

 else if(text == "desc")
 {
  if(sup.isEmpty())
    streams_.latex_stream() << "\n\n\\begin{docDescription}\n";
  else
    streams_.latex_stream() << "\n\n\\begin{docDescription}[leftmargin=" << sup << "]\n";

//  streams_.xml_writer().writeStartElement("exs-group");
  parse_context_.flags.read_desc_label = true;
  parse_context_.flags.ignore_blank_lines = true;

  streams_.xml_writer().writeCharacters("\n\n");

  streams_.xml_writer().writeStartElement("list");
  streams_.xml_writer().writeAttribute("list-type", "simple");
  streams_.xml_writer().writeAttribute("list-content", "description");
  streams_.xml_writer().writeCharacters("\n");


//  parse_context_.flags.read_parens_as_ref = false;
 }

 else if(text == "enumn")
 {
  if(sup.isEmpty())
    streams_.latex_stream() << "\n\n\\begin{docEnumerate}[1)]\n";
  else
    streams_.latex_stream() << "\n\n\\begin{docEnumerate}[\\hspace{" << sup << "}1)]\n";

  streams_.xml_writer().writeStartElement("enumn");
  parse_context_.flags.read_numbered_items = true;
  parse_context_.flags.ignore_blank_lines = true;
 }

 else if(text == "enums")
 {
//?  streams_.latex_stream() << "\n\n\\begin{enums}\n";
  streams_.latex_stream() << "\n\n\\begin{docEnumerate}\n";
  streams_.xml_writer().writeStartElement("enums");
  parse_context_.flags.read_numbered_items = true;
  parse_context_.flags.ignore_blank_lines = true;
 }

 else if(text == "itemz")
 {
  streams_.latex_stream() << "\n\n\\begin{itemz}\n";
  streams_.xml_writer().writeStartElement("itemz");
  parse_context_.flags.read_bulleted_items = true;
  parse_context_.flags.ignore_blank_lines = true;
 }

 else if(text == "block")
 {
  streams_.latex_stream() << "\n\n\\begin{blockQuote}\n";
  streams_.xml_writer().writeStartElement("block-quote");
  parse_context_.flags.ignore_blank_lines = true;

  held_paragraph_types_.push(current_paragraph_type_);
  current_paragraph_type_ = Paragraph_Types::Block_Quote;
 }

 else if(text == "nblock")
 {
  streams_.latex_stream() << "\n\n\\ndnblockQuote{%\n";
//  streams_.xml_writer().writeStartElement("block-quote");
  parse_context_.flags.ignore_blank_lines = true;

  held_paragraph_types_.push(current_paragraph_type_);
  current_paragraph_type_ = Paragraph_Types::Endnote_Block_Quote;
 }


}

void GTagML_Parse_State::check_blank_line()
{
 if(current_paragraph_type_ == Paragraph_Types::Block_Quote)
 {
  reset_primary();
//?  streams_.latex_stream() << "\n\\parbreak.2{}\n";
  streams_.latex_stream() << "\n\\parbreak{}\n";
 }

 else if(current_paragraph_type_ == Paragraph_Types::Endnote_Block_Quote)
 {
  reset_primary();
//?  streams_.latex_stream() << "\n\\parbreak.2{}\n";
  streams_.latex_stream() << "\n\\parbreak{}\n";
 }

 else if(parse_context_.flags.read_bulleted_items)
 {
  reset_primary();
  //?qDebug() << "\n\n" << latex_ << "\n\n";
  streams_.latex_stream() << "\n";
 }
}


void GTagML_Parse_State::leave_subparagraph_with_reset()
{
 if(parse_context_.flags.read_desc_label)
 {
  parse_context_.flags.read_desc_label = false;
  parse_context_.flags.ignore_blank_lines = false;
 }

 else if(parse_context_.flags.read_parens_as_label)
 {
  parse_context_.flags.read_parens_as_label = false;
  parse_context_.flags.read_parens_as_ref = true;
 }

 else if(parse_context_.flags.read_numbered_items)
 {
  parse_context_.flags.read_numbered_items = false;
  parse_context_.flags.ignore_blank_lines = false;
 }

 else if(parse_context_.flags.read_bulleted_items)
 {
  parse_context_.flags.read_bulleted_items = false;
  parse_context_.flags.ignore_blank_lines = false;
 }


 else if(current_paragraph_type_ == Paragraph_Types::Block_Quote)
 {
  parse_context_.flags.ignore_blank_lines = false;
  current_paragraph_type_ = held_paragraph_types_.pop();
 }

 else if(current_paragraph_type_ == Paragraph_Types::Endnote_Block_Quote)
 {
  parse_context_.flags.ignore_blank_lines = false;
  current_paragraph_type_ = held_paragraph_types_.pop();
 }

}

void GTagML_Parse_State::leave_subparagraph_with_continue()
{
 static QString letters = "abcdefghijklmnopqrstuvwxyz";

// streams_.latex_stream() << "\n\\nip";

 streams_.latex_stream() << "\n\\nip\\clrmMid{"
   << letters[current_slash_line_plus_count_] << "}%";

 ++current_slash_line_plus_count_;
}

void GTagML_Parse_State::single_slash_line_plus()
{
 single_slash_line();

 leave_subparagraph_with_continue();
}

void GTagML_Parse_State::single_slash_line()
{
 reset_primary();

 streams_.xml_writer().writeEndElement();

 if(parse_context_.flags.read_desc_label)
 {
//  streams_.xml_writer().writeEndElement();
  streams_.xml_writer().writeComment("end of list paragraph");
  streams_.xml_writer().writeCharacters("\n");
  streams_.xml_writer().writeCharacters("@=/list-item=@");
  streams_.xml_writer().writeCharacters("\n");

  streams_.xml_writer().writeEndElement();
  streams_.xml_writer().writeComment("end of description");


  streams_.xml_writer().writeCharacters("\n\n");

  streams_.latex_stream() << "\n\\end{docDescription}\n";
  parse_context_.flags.read_desc_label = false;
  parse_context_.flags.ignore_blank_lines = false;
 }

 else if(parse_context_.flags.read_parens_as_label)
 {
  streams_.latex_stream() << "\n\\end{exsGroup}\n";
  parse_context_.flags.read_parens_as_label = false;
  parse_context_.flags.read_parens_as_ref = true;
 }

 else if(parse_context_.flags.read_numbered_items)
 {
  streams_.latex_stream() << "\n\\end{docEnumerate}\n";
//?  streams_.latex_stream() << "\n\\end{enums}\n";
  parse_context_.flags.read_numbered_items = false;
  parse_context_.flags.ignore_blank_lines = false;
 }

 else if(parse_context_.flags.read_bulleted_items)
 {
  streams_.latex_stream() << "\n\\end{itemz}\n";
  parse_context_.flags.read_bulleted_items = false;
  parse_context_.flags.ignore_blank_lines = false;
 }


 else if(current_paragraph_type_ == Paragraph_Types::Block_Quote)
 {
  streams_.latex_stream() << "\\end{blockQuote}\n";
  parse_context_.flags.ignore_blank_lines = false;
  current_paragraph_type_ = held_paragraph_types_.pop();
 }

 else if(current_paragraph_type_ == Paragraph_Types::Endnote_Block_Quote)
 {
  streams_.latex_stream() << "}\\newpage{}\n";
  parse_context_.flags.ignore_blank_lines = false;
  current_paragraph_type_ = held_paragraph_types_.pop();
 }

}



void GTagML_Parse_State::paren_ref_global(u2 number, QString text)
{
 reset_primary();

 QString r = QString::number(number);

 streams_.latex_stream() << "\\exsRef(" << r << ")" ;
 streams_.xml_writer().writeTextElement("-exsRef", "r");
}


void GTagML_Parse_State::paren_ref(u2 number, QString text)
{
 reset_primary();

 QString r = QString::number(number + current_exs_offset_);

 streams_.latex_stream() << "\\exsRef(" << r << ")" ;
 streams_.xml_writer().writeTextElement("-exsRef", "r");
}


void GTagML_Parse_State::latex_command_auto_closed(QString command_name, QString arg)
{
 reset_primary();

 if(arg.isEmpty())
 {
  streams_.latex_stream() << "\\" << command_name << "{}";

   // //  streams_.xml_writer().writeCharacters("!%1%!"_qt.arg(command_name));
 }
 else
 {
  streams_.latex_stream() << "\\" << command_name << "{" << arg << "}";
   // //  streams_.xml_writer().writeCharacters("!%1:%2%!"_qt.arg(command_name).arg(arg));
 }
}

void GTagML_Parse_State::citation(QString full_match, QString label, QString locator)
{
 u2 ref_index = ref_labels_.indexOf(label);

 reset_primary();

 sentences_text_stream_ << full_match;

 QStringList qsl = locator.split("==");
 locator = qsl.takeFirst();

 if(!qsl.isEmpty())
 {
  streams_.xml_writer().writeTextElement("attrib", qsl.join(", "));
 }

 streams_.xml_writer().writeCharacters("[");
 streams_.xml_writer().writeStartElement("xref");
 streams_.xml_writer().writeAttribute("ref-type", "bibr");
//? streams_.xml_writer().writeAttribute("rid", label);

 streams_.xml_writer().writeAttribute("rid", "B%1"_qt.arg(ref_index + 1));

 streams_.xml_writer().writeCharacters("%1"_qt.arg(ref_index + 1));

 if(locator.isEmpty())
 {
  streams_.latex_stream() << "\\citeLabel{" << label << "}";
//?  streams_.xml_writer().writeCharacters("!%1%!"_qt.arg(command_name));
 }
 else if(locator.startsWith(":"))
 {
  QStringList qsl = locator.mid(1).simplified().split("-");
  if(qsl.size() == 1)
  {
   streams_.latex_stream() << "\\citePage{" << label << "}"
     << "(" << qsl.first() << ")";

//   streams_.xml_writer().writeAttribute("page", qsl.first());
   streams_.xml_writer().writeCharacters(", p. %1"_qt.arg(qsl.first()));
  }
  else
  {
   streams_.latex_stream() << "\\citePages{" << label << "}"
     << "(" << qsl.join(", ") << ")";

   streams_.xml_writer().writeCharacters(", pp. %1"_qt.arg(qsl.join("&#x2013;")));
  }
 }

 else if(locator.startsWith(";"))
 {
  streams_.latex_stream() << "\\citeLocator{" << label << "}"
    << "(" << locator.mid(1) << ")";

  QString loc = locator.mid(1);
  loc.replace("pp.", "pp. ");
  loc.replace("--", "@=#x2013;");
//  streams_.xml_writer().writeAttribute("locator", locator.mid(1));
  streams_.xml_writer().writeCharacters(loc.prepend(", "));
 }

 streams_.xml_writer().writeEndElement();
 streams_.xml_writer().writeCharacters("]");
}

void GTagML_Parse_State::bulleted_item(QString symbol, QString supp)
{
 reset_primary(); //? qDebug() << "\n\n" << latex_ << "\n\n";

 streams_.latex_stream() << "\n\\symItem{} ";
 streams_.xml_writer().writeTextElement("sym-item", "");
}


void GTagML_Parse_State::enums_item(u2 number, QString text, QString follow)
{
 reset_primary();

 if(follow == ")")
  //? streams_.latex_stream() << "\n\\enumsItem{}[)] ";
  //? streams_.latex_stream() << "\n\\item[" << text << ")] ";
   streams_.latex_stream() << "\n\\item{}";

 else
   streams_.latex_stream() << "\n\\enumsItem{} ";

// streams_.xml_writer().writeTextElement("enums-item", "");

 streams_.xml_writer().writeComment("enums-item");
}

void GTagML_Parse_State::desc_item_with_multiline_label(QString text)
{
 reset_primary();

 streams_.latex_stream() << "\n\n\\descItemBlank{} \\descMLabel{" << text << "}";
//? streams_.xml_writer().writeTextElement("exs-item", "");

 ++current_item_count_;

 if(current_item_count_ == 1)
 {
  streams_.xml_writer().writeCharacters("\n");
 }
 else
 {
  streams_.xml_writer().writeCharacters("\n");
  streams_.xml_writer().writeEndElement();
  streams_.xml_writer().writeComment("end of list paragraph");
  streams_.xml_writer().writeCharacters("\n\n");
 }

 ++pseudo_paragraph_counts_.first;
 pseudo_paragraph_counts_.second = 0;

 if(pseudo_paragraph_counts_.first > 1)
   streams_.xml_writer().writeCharacters("@=/list-item=@");

 streams_.xml_writer().writeTextElement("list-item", text);

 ++pseudo_paragraph_counts_.second;

 streams_.xml_writer().writeStartElement("p");
// streams_.xml_writer().writeAttribute("paragraph-container", "list");
 streams_.xml_writer().writeAttribute("id", "P-list_%1-%2"_qt
   .arg(pseudo_paragraph_counts_.first).arg(pseudo_paragraph_counts_.second));


 end_sentence();
}

void GTagML_Parse_State::desc_item(QString text)
{
 reset_primary();

 ++current_item_count_;

 ++pseudo_paragraph_counts_.first;
 pseudo_paragraph_counts_.second = 0;

 if(current_item_count_ == 1)
 {
  streams_.xml_writer().writeCharacters("\n");
 }
 else
 {
  streams_.xml_writer().writeCharacters("\n");
  streams_.xml_writer().writeEndElement();
  streams_.xml_writer().writeComment("end of list paragraph");
  streams_.xml_writer().writeCharacters("\n\n");

  streams_.xml_writer().writeCharacters("@=/list-item=@");
 }

 // if(pseudo_paragraph_counts_.first > 1)
 streams_.xml_writer().writeTextElement("list-item", text);

 streams_.xml_writer().writeStartElement("p");


 ++pseudo_paragraph_counts_.second;

// streams_.xml_writer().writeStartElement("p");
// streams_.xml_writer().writeAttribute("paragraph-container", "list");

 streams_.xml_writer().writeAttribute("id", "P-list_%1-%2"_qt
   .arg(pseudo_paragraph_counts_.first).arg(pseudo_paragraph_counts_.second));

// streams_.xml_writer().writeAttribute("paragraph-container", "list");

 streams_.latex_stream() << "\n\n\\descItem{" << text << "}";
//? streams_.xml_writer().writeTextElement("exs-item", "");

 end_sentence();
}


void GTagML_Parse_State::exs_item(u2 number, QString text)
{
 reset_primary();

 if(number == 1)
 {
  current_exs_offset_ = current_exs_number_;
  ++current_exs_group_number_;
 }

 ++current_exs_number_;

 streams_.latex_stream() << "\n\\exsItem{} ";
 streams_.xml_writer().writeTextElement("exs-item", "");

 end_sentence();
}

void GTagML_Parse_State::enter_block_float_mode()
{
 reset_primary();
 streams_.latex_stream() << "\\ndntext{";
 parse_context_.flags.block_float_mode = true;

//? parse_context_.flags.heading_acc = false;
}

void GTagML_Parse_State::leave_block_float_mode()
{
 reset_primary();

 parse_context_.flags.block_float_mode = false;

 streams_.latex_stream() << "}";
}


void GTagML_Parse_State::enter_italics_mode()
{
 reset_primary();

 streams_.xml_writer().writeStartElement("i");

 streams_.latex_stream() << "\\textit{";

 parse_context_.flags.italics_mode = true;
}

void GTagML_Parse_State::leave_italics_mode()
{
 reset_primary();

 parse_context_.flags.italics_mode = false;

 streams_.xml_writer().writeEndElement();
 streams_.latex_stream() << "}";

}

void GTagML_Parse_State::hyperlink_2(QString text, QString link)
{
 reset_primary();

 streams_.xml_writer().writeTextElement("hlink2", text + link);
 streams_.latex_stream() << "\\hlinkTwo{" << text << "}{" + link + "}";
}

void GTagML_Parse_State::hyperlink_1(QString text)
{
 reset_primary();

 streams_.xml_writer().writeTextElement("hlink1", text);
 streams_.latex_stream() << "\\hlinkOne{" << text << "}";
}

void GTagML_Parse_State::emph_symbolic(QString text)
{
 reset_primary();

 streams_.xml_writer().writeTextElement("eS", text);
 streams_.latex_stream() << "\\eS{" << text << "}";
}

void GTagML_Parse_State::emph_acronym(QString text)
{
 reset_primary();

 QString version;

 if(text == text.toLower())
   version = "AllLower";
 else if(text != text.toUpper())
   version = "SomeLower";

// QString eA = flags.in_ql? "iq_eA" : "eA";
 QString eA;

 if(flags.in_ql)
 {
  eA = "iq_eA";
 }
 else
  eA = "eA";

 streams_.xml_writer().writeTextElement(eA + version, text);
 streams_.latex_stream() << "\\eA" << version << "{" << text << "}";
}

void GTagML_Parse_State::enter_double_quote_mode(QString pre)
{
 reset_primary();

 parse_context_.flags.double_quote_mode = true;

 if(pre == "d")
   streams_.xml_writer().writeStartElement("disp-quote");
 else if(pre == "!")
   streams_.xml_writer().writeStartElement("ql");
 else
   streams_.xml_writer().writeStartElement("q");

 if(pre == "!")
 {
  streams_.latex_stream() << "\\ql{";
  flags.in_ql = true;
 }
 else
   streams_.latex_stream() << "\\q{";
}

void GTagML_Parse_State::leave_double_quote_mode()
{
 reset_primary();

 parse_context_.flags.double_quote_mode = false;

 if(flags.in_ql)
 {
  // //  anything?
  flags.in_ql = false;
 }

 streams_.xml_writer().writeEndElement();
 streams_.latex_stream() << "}";
}

void GTagML_Parse_State::enter_single_quote_mode()
{
 reset_primary();
 parse_context_.flags.single_quote_mode = true;

 streams_.latex_stream() << "\\sq{";
}

void GTagML_Parse_State::leave_single_quote_mode()
{
 reset_primary();

 parse_context_.flags.single_quote_mode = false;
 streams_.latex_stream() << "}";
}

void GTagML_Parse_State::enter_single_quote_mode_doubled()
{
 reset_primary();
 parse_context_.flags.single_quote_mode_doubled = true;

 streams_.latex_stream() << "\\sqq{";
}

void GTagML_Parse_State::leave_single_quote_mode_doubled()
{
 reset_primary();
 parse_context_.flags.single_quote_mode_doubled = false;

 streams_.latex_stream() << "}";
}

void GTagML_Parse_State::enter_single_quote_mode_trebled()
{
 parse_context_.flags.single_quote_mode_trebled = true;

}

void GTagML_Parse_State::leave_single_quote_mode_trebled()
{
 parse_context_.flags.single_quote_mode_trebled = false;

}

void GTagML_Parse_State::enter_acronym_mode(u1 size)
{
 if(size == 2)
 {
  enter_sample_mode();
  return;
 }

 reset_primary();

 parse_context_.flags.acronym_mode = true;

 held_macro_string_ = "eA";
// streams_.xml_writer().writeStartElement("eA");
// streams_.latex_stream() << "\\eA{";
}


void GTagML_Parse_State::leave_acronym_mode()
{
 QString version;

 if(streams_.primary_acc() == streams_.primary_acc().toLower())
   version = "AllLower";
 else if(streams_.primary_acc() != streams_.primary_acc().toUpper())
 {
  // //  what to do
  if(streams_.primary_acc().endsWith("s"))
  {
   QString t = streams_.primary_acc();
   t.chop(1);
   if(t == t.toUpper())
     goto skip_this;
  }
  version = "SomeLower";
 }

skip_this:

 if(flags.in_ql)
   streams_.xml_writer().writeStartElement("iq_" + held_macro_string_ + version);
 else
   streams_.xml_writer().writeStartElement(held_macro_string_ + version);

 streams_.latex_stream() << "\\" + held_macro_string_ << version << "{";

 reset_primary();

 parse_context_.flags.acronym_mode = false;

 streams_.xml_writer().writeEndElement();
 streams_.latex_stream() << "}";
}

void GTagML_Parse_State::enter_sample_mode()
{
 reset_primary();

 parse_context_.flags.emph_sample_mode = true;

 streams_.xml_writer().writeStartElement("eS");
 streams_.latex_stream() << "\\eS{";
}


void GTagML_Parse_State::leave_sample_mode()
{
 reset_primary();

 parse_context_.flags.emph_sample_mode = false;

 streams_.xml_writer().writeEndElement();
 streams_.latex_stream() << "}";
}


void GTagML_Parse_State::short_macro(QString text, u1 size)
{
 if(size == 2)
 {
  enter_highlight_mode();
  streams_.primary_acc() = text;
  leave_highlight_mode();
  return;
 }
 enter_short_macro_mode(1);
 streams_.primary_acc() = text;
 leave_short_macro_mode();
}

void GTagML_Parse_State::short_acronym(QString text, u1 size)
{
 if(size == 2)
 {
  enter_sample_mode();
  streams_.primary_acc() = text;
  leave_sample_mode();
  return;
 }

 enter_acronym_mode(1);
 streams_.primary_acc() = text;
 leave_acronym_mode();
}

void GTagML_Parse_State::short_emph_sample(QString text)
{
 enter_sample_mode();
 streams_.primary_acc() = text;
 leave_sample_mode();
}

void GTagML_Parse_State::enter_highlight_mode()
{
 reset_primary();

 parse_context_.flags.emph_highlight_mode = true;

 streams_.xml_writer().writeStartElement("eH");
 streams_.latex_stream() << "\\eH{";
}

void GTagML_Parse_State::leave_highlight_mode()
{
 reset_primary();

 parse_context_.flags.emph_highlight_mode = false;

 streams_.xml_writer().writeEndElement();
 streams_.latex_stream() << "}";
}

void GTagML_Parse_State::enter_short_macro_mode(u1 size)
{
 if(size == 2)
 {
  enter_highlight_mode();
  return;
 }

 reset_primary();

 parse_context_.flags.short_macro_mode = true;

 streams_.xml_writer().writeStartElement("sM");
 streams_.latex_stream() << "\\";
}


void GTagML_Parse_State::leave_short_macro_mode()
{
 QString latex = streams_.primary_acc();
 latex.replace("0", "Zero");
 latex.replace("1", "One");
 latex.replace("2", "Two");
 latex.replace("3", "Three");
 latex.replace("4", "Four");
 latex.replace("5", "Fice");
 latex.replace("6", "Six");
 latex.replace("7", "Seven");
 latex.replace("8", "Eight");
 latex.replace("9", "Nine");

 parse_context_.flags.short_macro_mode = false;

 streams_.xml_writer().writeCDATA(streams_.primary_acc());
 streams_.xml_writer().writeEndElement();

 streams_.primary_acc().clear();

 streams_.latex_stream() << latex << "{}";
}


void GTagML_Parse_State::enter_emph_italics_mode(QString mid)
{
 reset_primary();

 parse_context_.flags.emph_italics_mode = true;

 if(mid == "=")
 {
  streams_.xml_writer().writeStartElement("eIbox");
  streams_.latex_stream() << "\\eIbox{";
 }
 else
 {
  streams_.xml_writer().writeStartElement("eI");
  streams_.latex_stream() << "\\eI{";
 }
}

void GTagML_Parse_State::leave_emph_italics_mode()
{
 reset_primary();

 parse_context_.flags.emph_italics_mode = false;

 streams_.xml_writer().writeEndElement();
 streams_.latex_stream() << "}";
}


void GTagML_Parse_State::special_character_sequence(QString text)
{
 auto process = [this](QString latex, QString sentences, QString xml)
 {
  if(flags.heading_acc)
  {
   if(!flags.latex_only)
     sentences_section_heading_stream_ << sentences;

   if(!flags.sentences_only)
     latex_section_heading_stream_ << latex;

   return;
  }

  reset_primary();

  if(flags.latex_only)
    streams_.latex_stream() << latex;

  else if(flags.sentences_only)
    sentences_text_stream_ << sentences;

  else
  {
   streams_.latex_stream() << latex;
   sentences_text_stream_ << sentences;
   streams_.xml_writer().writeCharacters(xml);
  }
 };

 QMap<QString, QStringList> static_map {{
   { "%--", {"\\mdash{}", " - ", "@=#x2014;"}}, //&mdash
   { "%-", {"\\ndash{}", "--", "@=#x2013;"}},
   { "^:", {"\\raiseColon{}", ":", ":"}},
   { "%$", {"\\$", "$", "$"}},
   { "%%", {"\\%", "%", "%"}},
   { "%&", {"\\&", "&", "&"}},
   { "%#", {"\\#", "#", "#"}},
   }};

 auto it = static_map.find(text);

 if(it != static_map.end())
 {
  auto v = it.value();
  process(v[0], v[1], v[2]);
 }
// if(text == "%--")
// {
//  process("\\mdash{}", " - ", "&mdash;");
// }

// else if(text == "%-")
// {
//  process("\\ndash{}", "--", "&ndash;");
// }

// else if(text == "^:")
// {
//  process("\\raiseColon{}", ":", ":");
// }
}



void GTagML_Parse_State::enter_multiline_comment(QString semis, QString tildes)
{
 markup_position_.enter_multiline_comment(cutmax(semis.length()), cutmax(tildes.length()));
//? parse_context_.flags.inside_multiline_comment = true;
}

void GTagML_Parse_State::check_leave_multiline_comment(QString semis, QString tildes)
{
 if(markup_position_.check_leave_multiline_comment(cutmax(tildes.length()),
  cutmax(tildes.length())));
//?  parse_context_.flags.inside_multiline_comment = false;
}

void GTagML_Parse_State::tile_acc(QString str)
{
 tile_acc_qts_ << str;
}

//void GTagML_Parse_State::spm_acc(QString str)
//{
// spm_acc_qts_ << str;
//}


void GTagML_Parse_State::tag_command_annotation(QString annotation)
{
 //?
 // current_annotation_tile_ = new GTagML_Annotation_Tile("");
 // current_annotation_tile_->set_connector(annotation);
 // caon_ptr<tNode> node = make_new_node(current_annotation_tile_);
 // markup_position_.tag_command_annotation(node);
}

void GTagML_Parse_State::annotation_entry(QString flag, QString text, QString rel, QString tile, QString follow)
{
 check_tile_acc();

 QString annotation_text = text;
 if(flag == "*")
 {
  annotation_text.prepend("*:");
 }

 caon_ptr<GTagML_Tile> ntile = new GTagML_Tile(annotation_text);
 caon_ptr<GTagML_Annotation_Tile> atile = new GTagML_Annotation_Tile;
 //atile->set_subject(text);
 caon_ptr<tNode> node = make_new_node(ntile);
 caon_ptr<tNode> anode = make_new_node(atile);
 CAON_PTR_DEBUG(tNode ,node)
 if(rel.isEmpty())
 {
  markup_position_.annotation_entry(node, anode);
  flags.active_annotation_entry = true;
  if(!tile.isEmpty())
  {
   atile->set_tile(tile);
  }
  if(follow == ";")
  {
   markup_position_.close_annotation();
   flags.active_annotation_entry = false;
  }
  else
   flags.active_annotation_tile = true;
 }
 else
 {
  atile->set_connector(rel);
  //?current_annotation_tile_->set_connector(rel);
  markup_position_.annotation_entry(node, anode);
  if(!tile.isEmpty())
  {
   atile->set_tile(tile);
   //?current_annotation_tile_->set_tile(tile);
  }
  if(follow == ";")
  {
   markup_position_.close_annotation();
   flags.active_annotation_entry = false;
  }
  else
   flags.active_annotation_tile = true;
 }
}

void GTagML_Parse_State::check_tile_acc(Acc_Mode new_mode)
{
 if(tile_acc_.isEmpty())
 {
  acc_mode_ = new_mode;
  return;
 }

 QString trim = tile_acc_.trimmed();

 if(trim.isEmpty())
 {
  attach_whitespace(tile_acc_);
  tile_acc_qts_.reset();
  tile_acc_.clear();
  acc_mode_ = new_mode;
  return;
 }

 caon_ptr<tNode> ws_defer_node = nullptr;

 if(trim == "%-%" || trim == "---")
   trim.clear();

 else if(trim.endsWith("-."))
 {
  trim.chop(2);
  trim = trim.trimmed();
  ws_defer_node = markup_position_.current_tag_command_node();

 }

 caon_ptr<GTagML_Tile> new_tile = nullptr;

 switch(acc_mode_)
 {
 case Acc_Mode::Main_Tile:
  attach_left_whitespace();
  check_add_words();
  if(new_mode == Acc_Mode::Raw)
  {
   new_tile = add_tile(trim + "\n");
  }
  else
  {
   new_tile = add_tile(trim);
   attach_right_whitespace();
  }
  tile_acc_length_adjustment_ = 0;
  tile_acc_qts_.reset();
  tile_acc_.clear();

  break;

 case Acc_Mode::Arg_Tile:
  attach_left_whitespace();
  new_tile = add_tile(trim);
  attach_right_whitespace();
  tile_acc_length_adjustment_ = 0;
  tile_acc_qts_.reset();
  tile_acc_.clear();
  break;

 case Acc_Mode::Attribute:
  add_attribute_tile(tile_acc_);
  tile_acc_length_adjustment_ = 0;
  tile_acc_qts_.reset();
  tile_acc_.clear();
  break;

 case Acc_Mode::Raw:
  add_raw_tile(tile_acc_);
  tile_acc_length_adjustment_ = 0;
  tile_acc_qts_.reset();
  tile_acc_.clear();
  break;


 default: break;
 }

 if(new_tile)
   new_tile->set_whitespace_defer(ws_defer_node.raw_pointer());

 acc_mode_ = new_mode;
}

void GTagML_Parse_State::check_add_words()
{
 //?QStringList strings = tile_acc_.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
 QStringList strings; // = tile_acc_.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
 for(QString s : strings)
 {
  QRegularExpression rx_start("\\A\\$(\\w+)\\$");
  QRegularExpressionMatch rxm_start = rx_start.match(s);

  QRegularExpression rx_end("\\$(\\w+)\\$\\W*\\Z");
  QRegularExpressionMatch rxm_end = rx_end.match(s);

  if(rxm_start.hasMatch())
  {
   QString key = rxm_start.captured(1);
   s = s.mid(rxm_start.capturedEnd());
   if(key == "LDQ")
   {
    quote_acc_ += s + " ";
   }
  }
  else if(rxm_end.hasMatch())
  {
   QString key = rxm_end.captured(1);
   s = s.mid(0, rxm_end.capturedStart());
   if(key == "RDQ")
   {
    quote_acc_ += s;
    document_info_.add_quote(quote_acc_);
    quote_acc_.clear();
   }
  }
  else if(!quote_acc_.isEmpty())
  {
   quote_acc_ += s + " ";
  }


  if(s.contains('@'))
   continue;
  if(s.contains("//"))
   continue;
  if(s.count('.') > 1)
   continue;
  if(s.contains('/') && s.contains('.'))
   continue;

  if(s.contains(QRegularExpression("\\d")))
   continue;

  s.replace("&apos;", "");
  s.replace(QRegularExpression("&\\w+;"), "");
  s.replace(QRegularExpression("[^\\w']"), "");
  if(!s.isEmpty())
  {
   document_info_.add_word(s);
  }
 }
}


void GTagML_Parse_State::mark_attribute_tile()
{
 check_tile_acc(Acc_Mode::Attribute);
}



void GTagML_Parse_State::attach_left_whitespace()
{
 if(get_light_xml())
 {
  return;
 }
 int index = tile_acc_.indexOf(QRegularExpression("\\S"));

 if(index > 0)
 {
  // // is this hackneyed?
  if(index < 2)
    markup_position_.check_flag_right_whitespace();

  attach_whitespace(tile_acc_.left(index));
 }
}

void GTagML_Parse_State::attach_right_whitespace()
{
 if(get_light_xml())
 {
  return;
 }

 int index = tile_acc_.lastIndexOf(QRegularExpression("\\S\\s+\\Z"));
 if(index != -1)
 {
  attach_whitespace(tile_acc_.mid(index + 1));
 }
}


void GTagML_Parse_State::attach_whitespace(QString whitespace)
{
 if(get_light_xml())
 {
  return;
 }
#ifndef NO_CAON
 if(caon_ptr<tNode> node = markup_position_.current_node())
 {
  CAON_PTR_DEBUG(tNode ,node)
  caon_ptr<GTagML_Whitespace_Holder> nwh = node->as<GTagML_Whitespace_Holder>();
  nwh->attach_whitespace(whitespace);
 }
#endif //NO_CAON
}

#ifdef HIDE
void GTagML_Parse_State::end_khif_tile(QString connector_prefix, QString connectors)
{
 end_khif_tile();

 bool string_follow = (connector_prefix.startsWith('+'));
 bool subject_claim = (connector_prefix.endsWith('['));
 markup_position_.load_khif_connectors(connectors);

 parse_context_.flags.inside_khif_tile = true;

}

void GTagML_Parse_State::attach_khif_tile()
{
 if(!khif_tile_.isEmpty())
 {
  QString kt = khif_tile_.trimmed();
  if(!kt.isEmpty())
  {
   if(markup_position_.expecting_khif_attribute_tile())
   {
    QRegularExpression rx("(\\S+)\\s*(\\S*)");
    QRegularExpressionMatch match = rx.match(kt);
    QString attribute = match.captured(1);
    QString value = match.captured(2);
    caon_ptr<GTagML_Attribute_Tile> tile = make_new_attribute_tile(attribute, value);
    khif_tile_.clear();
    CAON_PTR_DEBUG(GTagML_Attribute_Tile ,tile)
    caon_ptr<tNode> node = make_new_node(tile);
    markup_position_.add_khif_tile_node(node);
   }
   else
   {
    caon_ptr<GTagML_Tile> tile = make_new_tile(kt);
    CAON_PTR_DEBUG(GTagML_Tile ,tile)
    khif_tile_.clear();
    caon_ptr<tNode> node = make_new_node(tile);
    markup_position_.add_khif_tile_node(node);
   }
  }
 }
}

void GTagML_Parse_State::end_khif_tile()
{
 //? parse_context.flags.inside_khif_tile = false;
 attach_khif_tile();
 markup_position_.khif_tag_command_leave();
}
#endif //def HIDE


void GTagML_Parse_State::enter_tag_command_with_predicate_vector(QString tag_command,
 QString connector_prefix, QString connectors)
{
 bool string_follow = (connector_prefix.startsWith('+'));
 bool subject_claim = (connector_prefix.endsWith('['));

 caon_ptr<GTagML_Tag_Command> gtc = make_new_tag_command(tag_command, {});
 caon_ptr<tNode> node = make_new_node(gtc);

 markup_position_.tag_command_entry(node);

 markup_position_.load_khif_connectors(connectors);

  //?  parse_context_.flags.inside_khif_tile = true;

}


//void GTagML_Parse_State::khif_tile_acc(QString m)
//{
// khif_tile_ += m;
//}

void GTagML_Parse_State::attach_predicate_vector(QString connector_prefix, QString connectors)
{
// attach_khif_tile();
 bool string_follow = (connector_prefix.startsWith('+'));
 bool subject_claim = (connector_prefix.endsWith('['));
 markup_position_.load_khif_connectors(connectors);
}


caon_ptr<GTagML_Tag_Command> GTagML_Parse_State::html_tag_instruction(QString prefix,
 QString tag_command, QString argument)
{
 check_tile_acc();

 caon_ptr<GTagML_Tag_Command> gtc = make_new_tag_command(tag_command, {});
 if(prefix == "!")
  gtc->flags.is_tag_assertion = true;
 else if(prefix == "?")
  gtc->flags.is_tag_query = true;

 gtc->flags.is_html = true;
 gtc->flags.is_understood_auto_closed = true;

 caon_ptr<tNode> node = make_new_node(gtc);

 caon_ptr<tNode> current_node = markup_position_.tag_command_instruction(node);
 tag_body_leave();
 tile_acc(argument);
 check_tile_acc();
 markup_position_.restore_current_node(current_node);

 return gtc;

}



caon_ptr<GTagML_Tag_Command> GTagML_Parse_State::html_tag_command_entry(QString prefix, QString tag_command)
{
 check_tile_acc();

 if(get_light_xml())
 {
  // get_light_xml()->tag_entry(tag_command);
  parse_context_.flags.inside_html_tag_body = true;
  return nullptr;
 }

 caon_ptr<GTagML_Tag_Command> gtc = make_new_tag_command(tag_command, {});
 caon_ptr<tNode> node = make_new_node(gtc);

 markup_position_.tag_command_entry(node);
 parse_context_.flags.inside_html_tag_body = true;
 gtc->flags.is_html = true;

 return gtc;

}

void GTagML_Parse_State::check_non_or_left_wrapped(QString wmi, caon_ptr<GTagML_Tag_Command> gtc)
{
 if(wmi.startsWith(':'))
 {
  if(wmi.startsWith("::"))
    gtc->flags.is_non_wrapped = true;
  else
    gtc->flags.is_left_wrapped = true;
 }
 if(wmi.endsWith('.'))
 {
  gtc->flags.has_non_wrapped_space = true;
 } 
}

caon_ptr<GTagML_Tag_Command> GTagML_Parse_State::tag_command_entry(QString wmi, 
  QString prefix, QString tag_command, QString argument, QString parent_tag_type)
{
 if(held_semantic_mark_mode_)
 {
  document_info_.marks()[held_semantic_mark_mode_].last() += tag_command;
  held_semantic_mark_mode_ = 0;
 }

 check_tile_acc();
 caon_ptr<GTagML_Tag_Command> gtc = make_new_tag_command(tag_command,
   argument, parent_tag_type);

 check_non_or_left_wrapped(wmi, gtc);

//?
// if(prefix == "/")
//   gtc->flags.is_environment = true;

 caon_ptr<tNode> node = make_new_node(gtc);

 RELAE_SET_NODE_LABEL(node, tag_command);
 markup_position_.tag_command_entry(node);
 parse_context_.flags.inside_tag_body = true;

 return gtc;
}


void GTagML_Parse_State::html_tag_command_attribute_entry(QString pre_space,
 QString attribute, QString s_or_d)
{
 current_html_attribute_ = attribute;
//? if(s_or_d == "'")
//?  parse_context_.flags.inside_html_tag_attribute_single_quote = true;
//? else if(s_or_d == "\"")
//?  parse_context_.flags.inside_html_tag_attribute_double_quote = true;
}

void GTagML_Parse_State::html_tag_command_attribute_acc(QString str)
{
 tile_acc_ += str;
}

caon_ptr<GTagML_Attribute_Tile> GTagML_Parse_State::complete_html_tag_command_attribute()
{
 if(get_light_xml())
 {
  // get_light_xml()->add_attribute_tile(current_html_attribute_, tile_acc_);
  tile_acc_.clear();
  current_html_attribute_.clear();
  document_info_.check_light_xml_href();
 }


 caon_ptr<GTagML_Attribute_Tile> tile =  make_new_attribute_tile(current_html_attribute_, tile_acc_);

 CAON_PTR_DEBUG(GTagML_Attribute_Tile ,tile)
 if(tile->key() == "href")
 {
  QString tag = markup_position_.current_tag_command_name();
  document_info_.add_href(tag, tile->value());
 }
 caon_ptr<tNode> node = make_new_node(tile);
 markup_position_.add_attribute_tile_node(node);

 current_html_attribute_ = QString();
 tile_acc_ = QString();

//? parse_context_.flags.inside_html_tag_attribute_double_quote = false;
//? parse_context_.flags.inside_html_tag_attribute_single_quote = false;
 parse_context_.flags.inside_html_tag_body = true;

 return tile;

}

GTagML_Document_Light_Xml* GTagML_Parse_State::get_light_xml()
{
 return nullptr;
 //return document_info_.light_xml();
}

void GTagML_Parse_State::multi_arg_transition_to_main_tile()
{
 multi_arg_transition({}, {}, {}, "-->", "=>");
  // // need to mark as main tile somehow ...
}

void GTagML_Parse_State::multi_arg_transition(QString wmi, QString inner_wmi,
  QString fiat, QString arg_marker, QString carried_arg_marker)
{
 if(flags.active_attribute_sequence)
 {
  flags.active_attribute_sequence = false;
  if(caon_ptr<tNode> cn = markup_position_.current_node())
  {
   if(caon_ptr<GTagML_Tag_Command> gtc = cn->GTagML_tag_command())
   {
    CAON_PTR_DEBUG(GTagML_Tag_Command ,gtc)
    check_tile_acc();
    markup_position_.attribute_sequence_leave();
    gtc->flags.is_multi_parent = true;

    //if(gtc->flags.is_provisional_multi_parent_semis)
    //?gtc->flags.is_multi_parent_semis = true;
    if(gtc->flags.anticipate_semis)
      parse_context_.flags.inside_multi_parent_semis = true;
    else
      parse_context_.flags.inside_multi_parent = true;
    tag_command_entry_inside_multi(wmi, {}, fiat, gtc->name(),
      arg_marker);
    return;
   }
  }
 }
 tag_command_leave();
 QString tag_command = markup_position_.current_tag_command_name();
 tag_command_entry_inside_multi(wmi, inner_wmi, fiat, tag_command,
   arg_marker, &carried_arg_marker);
}

void GTagML_Parse_State::tag_command_entry_with_layer(QString tag_command, QString layer_marker)
{
 caon_ptr<GTagML_Tag_Command> gtc = tag_command_entry({}, {}, tag_command, {});
   //make_new_tag_command(tag_command, {});

 if(layer_marker == "->>")
 {
  gtc->flags.is_layer_optional = true;
 }
 else if(layer_marker == "-->")
 {
  gtc->flags.is_layer_mandatory = true;
 }
 else if(layer_marker == "->")
 {
  gtc->flags.is_layer_main_mandatory = true;
 }
 else if(layer_marker == "-->>")
 {
  gtc->flags.is_layer_main_optional = true;
 }

 tag_body_leave();
}

void GTagML_Parse_State::tag_command_entry_inside_multi(QString wmi,
  QString inner_wmi,
  QString fiat,
  QString tag_command, QString arg_marker, 
  QString* carried_arg_marker, QString argument, QString name)
{
 QString nn = name;
 if(nn.isEmpty())
   // //  this "name" will never appear but it's a placeholder 
    //    for debugging ...
   nn = QString("%1 %2").arg(tag_command).arg(arg_marker);

 caon_ptr<GTagML_Tag_Command> gtc = make_new_tag_command(nn, argument);
 if(name.isEmpty())
   gtc->flags.autogen_multi_name = true;

 if(carried_arg_marker && (*carried_arg_marker == "=>"))
   gtc->flags.marked_main = true;

 check_non_or_left_wrapped(wmi, gtc);

 if(fiat == '=')
 {
  gtc->flags.is_fiat = true;
  if(!gtc->flags.is_left_wrapped)
    gtc->flags.is_non_wrapped = true;
 }

 if(inner_wmi == ".")
   gtc->flags.tile_is_quasi_fiat = true;

 caon_ptr<tNode> node = make_new_node(gtc);

 if(arg_marker == "->>")
 {
  gtc->flags.is_multi_optional = true;
  gtc->flags.multi_arg_layer = true;
  markup_position_.await_optional(node);
  acc_mode_ = Acc_Mode::Arg_Tile;
 }
 else if(arg_marker == "-->")
 {
  gtc->flags.is_multi_mandatory = true;
  gtc->flags.multi_main_layer = true;
  markup_position_.await_mandatory(node);
  acc_mode_ = Acc_Mode::Main_Tile;
 }
 else if(arg_marker == "->")
 {
  gtc->flags.is_multi_mandatory = true;
  gtc->flags.multi_arg_layer = true;
  markup_position_.await_mandatory(node);
  acc_mode_ = Acc_Mode::Arg_Tile;
 }
 else if(arg_marker == "-->>")
 {
  gtc->flags.is_multi_optional = true;
  gtc->flags.multi_main_layer = true;
  markup_position_.await_optional(node);
  acc_mode_ = Acc_Mode::Main_Tile;
 }
}

void GTagML_Parse_State::tag_command_entry_multi(QString wmi,
  QString inner_wmi, QString tag_command,
  QString tag_body_follow, QString fiat, QString first_arg_wmi, QString first_arg_marker)
{
 Tag_Body_Follow_Mode m = tag_body_follow.isEmpty() ? Normal
   : parse_tag_body_follow(tag_body_follow);

 caon_ptr<GTagML_Tag_Command> gtc = tag_command_entry(wmi, {},
   tag_command, {});

 if(m == Region)
   gtc->flags.is_region = true;

 gtc->flags.has_entry = true;


 tag_body_leave();

 if(tag_body_follow.isEmpty())
   gtc->flags.anticipate_semis = true;

 if(first_arg_marker == "@")
 {
  gtc->flags.is_provisional_multi_parent = true;
  markup_position_.prepare_attribute_sequence();
  parse_context_.flags.inside_attribute_sequence = true;
  parse_context_.flags.inside_multi_generic = true;
  flags.active_attribute_sequence = true;
  acc_mode_ = Acc_Mode::Attribute;
 }
 else
 {
  gtc->flags.is_multi_parent = true;
  tag_command_entry_inside_multi(first_arg_wmi, inner_wmi, fiat, tag_command, first_arg_marker);

  parse_context_.flags.inside_multi_generic = true;
  if(tag_body_follow.isEmpty())
    parse_context_.flags.inside_multi_parent_semis = true;
  else
    parse_context_.flags.inside_multi_parent = true;
 }
}

void GTagML_Parse_State::tag_command_entry_inline(QString wmi,
  QString inner_wmi, QString fiat, QString tag_command,
  QString tag_body_follow, QString argument)
{
 Tag_Body_Follow_Mode m = parse_tag_body_follow(tag_body_follow);
 QString prefix;

 caon_ptr<GTagML_Tag_Command> gtc = tag_command_entry(wmi, prefix,
   tag_command, argument);

 CAON_PTR_DEBUG(GTagML_Tag_Command ,gtc)

 if(fiat == "=")
   gtc->flags.is_fiat = true;

// if(inner_wmi == ".")
//   gtc->flags.tile_is_quasi_fiat = true;


//?
// else if(fiat == "==")
//   gtc->flags.is_fiat_inherited = true;

 switch(m)
 {
 case Normal:
  tag_body_leave();
  break;
 case Region:
  gtc->flags.is_region = true;
  tag_body_leave();
  break;
 case Empty:
  gtc->flags.is_self_closed = true;
  tag_body_leave();
  tag_command_leave(); break;
 }
}

void GTagML_Parse_State::gtag_command_entry_inline(QString tag_command,
 QString tag_body_follow)
{
 Tag_Body_Follow_Mode m = parse_tag_body_follow(tag_body_follow);
 QString prefix;

 caon_ptr<GTagML_Tag_Command> gtc = tag_command_entry({}, prefix,
   tag_command, {});

 switch(m)
 {
 case Normal:
  tag_body_leave();
  break;
 case Region:
  gtc->flags.is_region = true;
  tag_body_leave();
  break;
 case Empty:
  gtc->flags.is_self_closed = true;
  tag_body_leave();
  tag_command_leave(); break;
 }
}


caon_ptr<GTagML_Tag_Command> GTagML_Parse_State::make_new_tag_command(QString name, QString argument, QString parent_tag_type)
{
 GTagML_Tag_Command* result;

 QString opt;
 if(argument.startsWith('%'))
   argument = argument.mid(1);
 else
 {
  argument = argument.trimmed();
  if(argument.startsWith('['))
  {
   int be = argument.indexOf(']');
   opt = argument.mid(1, be - 1).trimmed();
   argument = argument.mid(be + 1).trimmed();
  }
  else if(argument.startsWith('('))
  {
   int be = argument.indexOf(')');
   opt = argument.mid(1, be - 1).trimmed();
   opt.prepend('(');
   opt.append(')');
   argument = argument.mid(be + 1).trimmed();
  }
 }

 if(name.startsWith('='))
 {
  result = new GTagML_Tag_Command(name.mid(1), argument, parent_tag_type);
  result->flags.is_fiat = true;
  result->flags.is_non_wrapped = true;
 }
 else
   result = new GTagML_Tag_Command(name, argument, parent_tag_type);

 if(!opt.isEmpty())
   result->set_opt_argument(opt);
   
 return result;
}


caon_ptr<GTagML_Tile> GTagML_Parse_State::add_tile(QString tile_str)
{
 if(get_light_xml())
 {
  // get_light_xml()->add_tile(tile_str);
  return nullptr;
 }

 caon_ptr<GTagML_Tile> tile = make_new_tile(tile_str);
 tile->set_length_adjustment(tile_acc_length_adjustment_);
 caon_ptr<tNode> node = make_new_node(tile);
 //?
 markup_position_.add_tile_node(node);
 return tile;
}

caon_ptr<GTagML_Raw_Tile> GTagML_Parse_State::add_raw_tile(QString tile_str)
{
  // // current latex is the only raw format ...
 caon_ptr<GTagML_Raw_Tile> tile = make_new_raw_tile(current_raw_format_, tile_str);
 CAON_PTR_DEBUG(GTagML_Raw_Tile ,tile)
 caon_ptr<tNode> node = make_new_node(tile);
 markup_position_.add_raw_tile_node(node);
 return tile;
}

caon_ptr<GTagML_Attribute_Tile> GTagML_Parse_State::add_attribute_tile(QString tile_str)
{
 if(get_light_xml())
 {
  // get_light_xml()->add_attribute_tile(tile_str);
  return nullptr;
 }

 caon_ptr<GTagML_Attribute_Tile> tile = make_new_attribute_tile(tile_str);
 CAON_PTR_DEBUG(GTagML_Attribute_Tile ,tile)
 caon_ptr<tNode> node = make_new_node(tile);
 markup_position_.add_attribute_tile_node(node);
 return tile;
}


void GTagML_Parse_State::add_string_literal_tile(QString str)
{
 //? caon_ptr<GTagML_Tile> tile = add_tile(str);
 //? tile->flags.is_string_literal = true;
}

void GTagML_Parse_State::tag_command_leave(QString connector_to_load, QString tag_command)
{

}

void GTagML_Parse_State::block_gtag_command_leave()
{
 if(caon_ptr<tNode> node = markup_position_.tag_command_leave())
 {
  CAON_PTR_DEBUG(tNode ,node)
  check_tile_acc();
  markup_position_.confirm_tag_command_leave(node);
  check_multi_parent_reset();
 }
}

void GTagML_Parse_State::inline_tag_command_leave()
{
 if(caon_ptr<tNode> node = markup_position_.tag_command_leave())
 {
  CAON_PTR_DEBUG(tNode ,node)
  check_tile_acc();
  markup_position_.confirm_tag_command_leave(node);
  check_multi_parent_reset();
 }
}

void GTagML_Parse_State::tag_command_leave_multi(QString tag_command)
{
 if(caon_ptr<tNode> node = markup_position_.tag_command_leave())
 {
  CAON_PTR_DEBUG(tNode ,node)
  check_tile_acc();
  markup_position_.confirm_tag_command_leave(node);

  if(caon_ptr<GTagML_Tag_Command> gtc = node->GTagML_tag_command())
  {
   CAON_PTR_DEBUG(GTagML_Tag_Command ,gtc)

   if( (gtc->flags.is_multi_optional) || (gtc->flags.is_multi_mandatory) )
   {
     // //  two leaves here because the first is an arg ...
    if(caon_ptr<tNode> node1 = markup_position_.tag_command_leave())
    {
     markup_position_.confirm_tag_command_leave(node1);
//?
//     check_multi_parent_reset();
//     return;
    }
   }
  }
  check_multi_parent_reset();
 }
}

void GTagML_Parse_State::tag_command_leave()
{
 if(caon_ptr<tNode> node = markup_position_.tag_command_leave())
 {
  CAON_PTR_DEBUG(tNode ,node)
  check_tile_acc();
  markup_position_.confirm_tag_command_leave(node);
  check_multi_parent_reset();
 }
}

void GTagML_Parse_State::tag_command_instruction_leave(caon_ptr<tNode> node)
{
 check_tile_acc();
 markup_position_.restore_current_node(node);
}

void GTagML_Parse_State::check_html_tag_command_leave(QString tag_command, QString match_text)
{
 check_tile_acc();

 if(get_light_xml())
 {
  // get_light_xml()->tag_leave(tag_command);
  return;
 }

 markup_position_.rewind_tag_command_leave(tag_command);
}

void GTagML_Parse_State::check_multi_parent_reset()
{
 if(caon_ptr<GTagML_Tag_Command> gtc = markup_position_.get_current_tag_command())
 {
  CAON_PTR_DEBUG(GTagML_Tag_Command ,gtc)
  if(gtc->flags.is_multi_parent || gtc->flags.is_multi_parent_inherited)
  {
   parse_context_.flags.inside_multi_generic = true;
   if(gtc->flags.anticipate_semis)
     parse_context_.flags.inside_multi_parent_semis = true;
   else
     parse_context_.flags.inside_multi_parent = true;
  }
  else
  {
   parse_context_.flags.inside_multi_generic = false;
   parse_context_.flags.inside_multi_parent = false;
   parse_context_.flags.inside_multi_parent_semis = false;
  }
 }
 else
 {
  parse_context_.flags.inside_multi_generic = false;
  parse_context_.flags.inside_multi_parent = false;
  parse_context_.flags.inside_multi_parent_semis = false;
 }
}

void GTagML_Parse_State::check_tag_command_leave(QString tag_command, QString match_text)
{
 // //  If the tag command does not match the current node, treat the whole match
  //    as just text to accumulate
 if(caon_ptr<tNode> node = markup_position_.check_tag_command_leave(tag_command))
 {
  CAON_PTR_DEBUG(tNode ,node)
  check_tile_acc();
  markup_position_.confirm_tag_command_leave(node);
  check_multi_parent_reset();
 }
 else
 {
  tile_acc(match_text);
 }
}

//void GTagML_Parse_State::add_string_literal_tile()
//{
// if(!tile_acc_.isEmpty())
// {
//  // //  This will happen if a string literal is preceeded by a
//  //     string without whitespace; that could be used to create
//  //     special kinds of string literals.
//  //     This could be a spot for a callback which takes the
//  //     tile_acc_ string before clearing it.
//  tile_acc_.clear();
//  tile_acc_qts_.reset();
// }
// add_string_literal_tile(string_literal_acc_);
// string_literal_acc_.clear();
// qts_string_literal_acc_.reset();
//}

caon_ptr<GTagML_Attribute_Tile> GTagML_Parse_State::make_new_attribute_tile(QString tile)
{
 QRegularExpression rx("(\\S+)\\s+(.+)", QRegularExpression::DotMatchesEverythingOption);
 QRegularExpressionMatch rxm = rx.match(tile);
 if(rxm.hasMatch())
 {
  return caon_ptr<GTagML_Attribute_Tile>(
   new GTagML_Attribute_Tile(rxm.captured(1), rxm.captured(2)) );
 }
}

caon_ptr<GTagML_Attribute_Tile> GTagML_Parse_State::make_new_attribute_tile(QString key, QString value)
{
 return caon_ptr<GTagML_Attribute_Tile>(
   new GTagML_Attribute_Tile(key, value) );
}

caon_ptr<GTagML_Raw_Tile> GTagML_Parse_State::make_new_raw_tile(QString format, QString value)
{
 return caon_ptr<GTagML_Raw_Tile>(
   new GTagML_Raw_Tile(format, value) );
}

caon_ptr<GTagML_Tile> GTagML_Parse_State::make_new_tile(QString tile)
{
 return caon_ptr<GTagML_Tile>( new GTagML_Tile(tile) );
}


caon_ptr<GTagML_Paralex_Tile> GTagML_Parse_State::make_new_paralex_tile(QString tile,
  u1 kind, u1 w_or_a)
{
 return caon_ptr<GTagML_Paralex_Tile>( new GTagML_Paralex_Tile(tile, 
   (GTagML_Paralex_Tile::Kind) kind, w_or_a) );
}


caon_ptr<GTagML_Parse_State::tNode> GTagML_Parse_State::make_new_node(caon_ptr<GTagML_Tile> tile)
{
 CAON_PTR_DEBUG(GTagML_Tile ,tile)
 caon_ptr<tNode> result = caon_ptr<tNode>( new tNode(tile) );
 #ifdef RELAE_LABEL_NODES
 result->set_label(tile->thumbnail());
 #endif
 return result;
}

caon_ptr<GTagML_Parse_State::tNode> GTagML_Parse_State::make_new_node(caon_ptr<GTagML_Raw_Tile> tile)
{
 CAON_PTR_DEBUG(GTagML_Raw_Tile ,tile)
 caon_ptr<tNode> result = caon_ptr<tNode>( new tNode(tile) );
 #ifdef RELAE_LABEL_NODES
 result->set_label(tile->thumbnail());
 #endif
 return result;
}

void GTagML_Parse_State::check_nonstandard_special_character_sequence
  (QString match_text, QString& esc, u1& mode, QString& sup_text)
{
 static QMap<QString, std::pair<QString, u1>> static_map {
  {"%--", {"-", 2}},
  {"%_", {"_", 1}},
  {"%#", {"#", 1}},
  {"->-", {"/", 2}},
  {"%..", {"...", 2}},
  {"%...", {"...", 2}},
 };

 static QMap<QString, QString> supp_map {
  {"%...", "."},
 };

 auto it = static_map.find(match_text);
 if(it != static_map.end())
 {
  std::tie(esc, mode) = it.value();
  sup_text = supp_map.value(match_text);
//  esc = it.value().first;
//  mode = it.value().second;
 }
}

void GTagML_Parse_State::semantic_mark(QString match_text, QString sem, u1 mode)
{
 QString acc;

 int ix = sem.indexOf("-/");
 if(ix == -1)
 {
  acc = sem;
 }
 else
 {
  acc = sem.mid(0, ix);
  sem = sem.mid(ix + 2);
 }

 if(sem.endsWith("/+"))
 {
  // //  Signals that the following tag-command should be added to the mark ...
  sem.chop(2);
  acc = sem;
  held_semantic_mark_mode_ = mode;
 }

 QString text = QString("%1%2^").arg(mode).arg(acc) + "%9";
 text.prepend("^%");
 tile_acc(text);
 document_info_.marks()[mode].push_back(sem);
}



void GTagML_Parse_State::special_character_sequence(QString match_text, 
  QString esc, u1 mode)
{
 QString sup_text;
 check_nonstandard_special_character_sequence(match_text, esc, mode, sup_text);

 QString text;
 GTagML_Paralex_Tile::Kind k = GTagML_Paralex_Tile::N_A;
 u1 w = 0;

 switch (mode)
 {
 case 0: text = match_text; 
  break;
 case 1: 
 case 2: 
 case 3: 
 case 4: 
  text = esc;
  w = mode;
  k = GTagML_Paralex_Tile::Alt_Interpretation;
  break;  
 case 5:
  if(match_text == "||")
  {
    // // preserve the escape?
   tile_acc("||");
   ++tile_acc_length_adjustment_;
   return;
  }
  text = esc; 
  k = GTagML_Paralex_Tile::Pipe_Escape;
  if( (text[0] == '>') || (text[0] == '+') )
    w = 2;
  else if(text[0] == '&') 
    w = 3;
  else
    w = 1;
  break;
 }

 int adj = 0;
 // // maybe we just acc everything ...
 switch(k)
 {
 case GTagML_Paralex_Tile::Alt_Interpretation:
  //?tile_acc_length_adjustment_ += (text.size() + 3) - w;
  //?adj = text.size();
  //?adj += 2;
  tile_acc_length_adjustment_ += 3;
  switch (mode)
  {
  case 1:
   tile_acc(QString("`(%1)%2").arg(text).arg(sup_text));
   return;
  case 2:
   tile_acc(QString("`{%1}%2").arg(text).arg(sup_text));
   return;
  case 3:
   tile_acc(QString("`[%1]%2").arg(text).arg(sup_text));
   return;
  case 4:
   tile_acc(QString("`<%1>%2").arg(text).arg(sup_text));
   return;
  }
  break;
 case GTagML_Paralex_Tile::Pipe_Escape:
  tile_acc_length_adjustment_ += (text.size() + 1) - w;
  tile_acc(QString("|%1").arg(text));
  return; 
 default:
  break;
 }

 check_tile_acc();
 
 caon_ptr<GTagML_Paralex_Tile> xtile = make_new_paralex_tile(text, k, w);
 caon_ptr<tNode> node = make_new_node(xtile);
 markup_position_.add_tile_node(node);
}

caon_ptr<GTagML_Parse_State::tNode> GTagML_Parse_State::make_new_node(caon_ptr<GTagML_Annotation_Tile> tile)
{
 caon_ptr<tNode> result = new tNode(tile);
 result->set_label(tile->thumbnail());
 return result;
}

caon_ptr<GTagML_Parse_State::tNode> GTagML_Parse_State::make_new_node(caon_ptr<GTagML_Annotation_Tile> tile, QString label)
{
 caon_ptr<tNode> result = new tNode(tile);
 result->set_label(label);
 return result;
}

caon_ptr<GTagML_Parse_State::tNode> GTagML_Parse_State::make_new_node(caon_ptr<GTagML_Attribute_Tile> tile)
{
 CAON_PTR_DEBUG(GTagML_Attribute_Tile ,tile)
 caon_ptr<tNode> result = caon_ptr<tNode>( new tNode(tile) );
 #ifdef RELAE_LABEL_NODES
 result->set_label(tile->thumbnail());
 #endif
 return result;
}

caon_ptr<GTagML_Parse_State::tNode> GTagML_Parse_State::make_new_node(caon_ptr<GTagML_Paralex_Tile> tile)
{
 CAON_PTR_DEBUG(GTagML_Paralex_Tile ,tile)
 caon_ptr<tNode> result = caon_ptr<tNode>( new tNode(tile) );
 #ifdef RELAE_LABEL_NODES
 result->set_label(tile->thumbnail());
 #endif
 return result;
}

caon_ptr<GTagML_Parse_State::tNode> GTagML_Parse_State::make_new_node(caon_ptr<GTagML_Tag_Command> gtc)
{
 CAON_PTR_DEBUG(GTagML_Tag_Command ,gtc)
 caon_ptr<tNode> result = caon_ptr<tNode>( new tNode(gtc) );
 #ifdef RELAE_LABEL_NODES
 result->set_label(gtc->name());
 #endif
 return result;
}

void GTagML_Parse_State::attribute_sequence_leave()
{
 check_tile_acc();
 parse_context_.flags.inside_attribute_sequence = false;
 parse_context_.flags.inside_multi_generic = false;
 markup_position_.attribute_sequence_leave();
}

void GTagML_Parse_State::tag_body_leave(QString match)
{
 check_tile_acc();
 markup_position_.tag_body_leave();
 parse_context_.flags.inside_tag_body = false;
 if(match == "/>")
  tag_command_leave();
}

void GTagML_Parse_State::enter_special_parse_mode(QString spm)
{
 if(spm.startsWith("raw-"))
 {
  current_raw_format_ = spm.mid(4);
 }
 else if(spm != "raw")
 {
  // // no other spm current recognized ...
  return;
 }

 prior_parsing_modes_.push({current_parsing_mode_, acc_mode_});
 check_tile_acc(Acc_Mode::Raw);
 current_parsing_mode_ = GTagML_Parsing_Modes::Raw;
}

void GTagML_Parse_State::leave_special_parse_mode(QString spm)
{
 if(prior_parsing_modes_.isEmpty())
   current_parsing_mode_ = GTagML_Parsing_Modes::Parse_Error;
 else
 {
  current_parsing_mode_ = prior_parsing_modes_.top().first;
  check_tile_acc(prior_parsing_modes_.pop().second);
 }
}

void GTagML_Parse_State::special_parse_mode_acc(QString text)
{
 tile_acc(text);
}

void GTagML_Parse_State::html_tag_body_leave(QString prefix)
{

 if(get_light_xml())
 {
  // get_light_xml()->tag_body_leave(prefix);
  parse_context_.flags.inside_html_tag_body = false;
  return;
 }

 markup_position_.tag_body_leave();
 parse_context_.flags.inside_html_tag_body = false;
 if(prefix == "/")
  tag_command_leave();
}

