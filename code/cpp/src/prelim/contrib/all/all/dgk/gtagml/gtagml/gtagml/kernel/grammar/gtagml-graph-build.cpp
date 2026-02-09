
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "gtagml-graph-build.h"

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

GTagML_Graph_Build::GTagML_Graph_Build(GTagML_Graph& g, GTagML_Document_Info& document_info)
 : Flags(0), markup_position_(g.root_node()), acc_mode_(Acc_Mode::Main_Tile), document_info_(document_info),
   current_parsing_mode_(GTagML_Parsing_Modes::GTagML), //?current_annotation_tile_(nullptr),
   tile_acc_length_adjustment_(0),
   tile_acc_qts_(&tile_acc_), string_literal_acc_qts_(&string_literal_acc_),
   current_raw_format_("latex"), held_semantic_mark_mode_(0)

   ,current_exs_group_number_(0)
   ,current_exs_number_(0)
   ,current_exs_offset_(0)

   ,current_slash_line_plus_count_(0)

   ,current_paragraph_type_(Paragraph_Types::N_A)
   ,current_paragraph_count_(0), current_paragraph_bridge_(0)
   ,latex_stream_(&latex_)
   ,primary_acc_stream_(&primary_acc_)
   ,sentences_sdi_stream_(&sentences_sdi_)
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
//   ,jats_stream_(&jats_)
{
 ref_labels_ = QStringList {
  "ArunaBansal",
  "ContractsforC++",
  "ElliBleekerEtAl",
  "ElliBleekerEtAlTEI",
  "ChavdarBotevEtAl",
  "EdwinCBrady",
  "MartinBraenne",
  "NCDataIntegration",
  "DonaldCComeauEtAl",
  "ReginaldCushingEtAl",
  "NickGarabedianEtAl",
  "HarijsGrinbergs",
  "PottayilHarisankerMenonEtAl",
  "BertramLudascherEtAl",
  "SeanMurphy",
  "LimorPeerEtAl",
  "SimplexNumericaManual",
  "VickyStevensEtAl",
  "YuyaUezato",
  "LucyLuWangEtAl",
  "AntonZhilin",
  };


// flags.use_latex_sdi_all_markers = true;
// flags.use_latex_sdi_paragraph_markers = true;
}


void GTagML_Graph_Build::init(caon_ptr<GTagML_Parser> parser)
{
 parser_ = parser;

 jats_buffer_.setBuffer(&jats_array_);
 jats_buffer_.open(QBuffer::WriteOnly);
 xml_writer_.setDevice(&jats_buffer_);

 xml_writer_.setAutoFormatting(true); // Optional: for human-readable XML
 xml_writer_.writeStartDocument();

 xml_writer_.writeComment("%XML-TEMPLATE%");

 xml_writer_.writeStartElement("article");
 xml_writer_.writeAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
 xml_writer_.writeAttribute("xml:lang", "en");
 xml_writer_.writeAttribute("dtd-version", "1.4");

 xml_writer_.writeComment("meta:here");

 xml_writer_.writeStartElement("body");

 latex_stream_ << "\n\n%PREAMBLE-TEMPLATE%\n\n\\begin{document}\n%BEGIN-TEMPLATE%";

 sentences_sdi_stream_ << "--- Global/start\n\n";
// xml_writer_.set
// xml_writer_ = QXmlStreamWriter(jats_);
// jats_stream_.setString(&jats_); // = QTextStream(&jats_);
// xml_writer_.setDevice(&jats_stream_);
}

void GTagML_Graph_Build::enter_special_section(QString text)
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

void GTagML_Graph_Build::enter_abstract()
{
 reset_primary();

 check_close_paragraph();

 current_paragraph_type_ = Paragraph_Types::Abstract;

 xml_writer_.writeCharacters("\n\n");
 xml_writer_.writeStartElement("doc-abstract");
 latex_stream_ << "\n\n\\twocolumn[\\begin{docAbstract}\n";

 sentences_sdi_stream_ << "\n\n--- Abstract/start\n";

 sentences_sdi_stream_ << "\n\n--- Sentence/start\nid: " << sentence_id_;
}


void GTagML_Graph_Build::insert_latex_template(QString path)
{
 QString contents = KA::TextIO::load_file(path);

 s4 ix = contents.indexOf("\n%%\n");
 s4 ix1 = contents.indexOf("\n%%%\n");

 if(ix != -1)
 {
  if(ix1 != -1)
  {
   QString bcontents = contents.mid(ix + 4, ix1 - ix - 4);
   QString econtents = contents.mid(ix1 + 4);
   contents.replace(ix + 4, contents.length() - ix - 4, "");
   latex_.replace("%PREAMBLE-TEMPLATE%", contents);
   latex_.replace("%BEGIN-TEMPLATE%", bcontents);
   latex_.replace("%END-TEMPLATE%", econtents);
  }
  else
  {
   QString bcontents = contents.mid(ix + 4);
   contents.replace(ix + 4, contents.length() - ix - 4, "");
   latex_.replace("%PREAMBLE-TEMPLATE%", contents);
   latex_.replace("%BEGIN-TEMPLATE%", bcontents);
  }
 }
 else if(ix1 != -1)
 {
  QString econtents = contents.mid(ix1 + 4);
  contents.replace(ix1 + 4, contents.length() - ix1 - 4, "");
  latex_.replace("%PREAMBLE-TEMPLATE%", contents);
  latex_.replace("%END-TEMPLATE%", econtents);
 }
}

void GTagML_Graph_Build::insert_xml_template(QString path)
{
 QString contents = KA::TextIO::load_file(path);
 jats_array_.replace("%XML-TEMPLATE%", contents.toLatin1());
}

void GTagML_Graph_Build::primary_acc(QString text)
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
     latex_stream_ << " \\> ";

   ++sentence_id_;
   sentences_sdi_stream_ << "\n\n--- Sentence/switch\nid: "
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
     latex_stream_ << "\\:";

   ++paragraph_id_;
   sentences_sdi_stream_ << "\n\n--- Paragraph/start\nid: " << paragraph_id_
     << "\nr#  "
     << document_info_.line_and_column_string(parser_->current_position())
     << "\n";

  }

  if(flags.await_sentence_start)
  {
   flags.await_sentence_start = false;
   if(flags.use_latex_sdi_all_markers)
     latex_stream_ << "\\+";

   ++sentence_id_;
   sentences_sdi_stream_ << "\n\n--- Sentence/start\nid: " << sentence_id_
     << "\nr#  "
     << document_info_.line_and_column_string(parser_->current_position());
  }
 }

 primary_acc_stream_ << text;
}

void GTagML_Graph_Build::reset_primary()
{
 auto handle_sentences = [this]()
 {
  if(flags.sentences_latex_filter)
  {
   QString pa = primary_acc_;
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
    sentences_text_stream_ << primary_acc_;
 };

 if(flags.sentences_only)
 {
  handle_sentences();
 }

 else if(flags.latex_only)
 {
  latex_stream_ << primary_acc_;
  u4 pos = parser_->current_position();

  sentence_gaps_stream_ << "\n +" << line_and_column_string_tight(pos);

  pos += primary_acc_.size();

  sentence_gaps_stream_ << ":" << primary_acc_.size();
  sentence_gaps_stream_ << "=" << line_and_column_string_tight(pos);
 }

 else
 {
  latex_stream_ << primary_acc_;
  xml_writer_.writeCharacters(primary_acc_);
  handle_sentences();
 }

 primary_acc_.clear();
}


//void GTagML_Graph_Build::subsection_heading(QString stext, QString ltext, u1 level)
//{
//// reset_primary();
//// check_close_paragraph();

// xml_writer_.writeTextElement("s2", stext);
// latex_stream_ << "\n\n\\s|2|{" << ltext << "}\n";

// sentences_sdi_stream_ << "\n\n--- Section/start\n-l  2\n-t" << stext << "\n";

// set_paragraph_bridge();
//}


void GTagML_Graph_Build::section_heading(QString stext, QString ltext, u1 level)
{
 ++current_section_number_;

// reset_primary();
// check_close_paragraph();

 if(current_section_number_ > 1)
 {
  xml_writer_.writeCharacters("\n\n@=/sec=@\n\n");
 }

 xml_writer_.writeCharacters("\n\n");

 xml_writer_.writeTextElement("s1", stext);
 latex_stream_ << "\n\n\\s|" << level << "|{" << ltext << "}\n";

 ++section_ids_[level];

 sentences_sdi_stream_ << "\n\n--- Section/start\n-l  " << level << "\n-i  "
   << section_ids_[level] << "\n-t  " << stext << "\n";

 set_paragraph_bridge();
}

void GTagML_Graph_Build::blank_line_as_visible_space()
{
 reset_primary();

 latex_stream_ << "\n\n \\visbreak{}\n";
}

// <%- \ifnum\presetStretch=2\vspace*{9pt}\else\fi -%>


//void GTagML_Graph_Build::latex_only(QString text)
//{
// latex_stream_ << text;
//}


void GTagML_Graph_Build::enter_latex_only_to_space(QString match, QString space)
{
 primary_acc(space);

 enter_latex_only_to_space(match);

 //latex_stream_ << text;
}

void GTagML_Graph_Build::enter_latex_only_to_space(QString match)
{
 enter_latex_only(match);

 parse_context_.flags.latex_only_to_space = true;
}

void GTagML_Graph_Build::leave_latex_only_to_space(QString match)
{
 leave_latex_only(match);

 parse_context_.flags.latex_only_to_space = false;
}


void GTagML_Graph_Build::enter_latex_only(QString match)
{
 reset_primary();

 sentence_gaps_stream_ << "\n @l(" << match << ") +" << line_and_column_string_tight();

 parse_context_.flags.latex_only = true;
 flags.latex_only = true;
}

void GTagML_Graph_Build::leave_latex_only(QString match)
{
 reset_primary();

 sentence_gaps_stream_ << "\n @l(" << match << ") -" << line_and_column_string_tight();

 parse_context_.flags.latex_only = false;
 flags.latex_only = false;
}


void GTagML_Graph_Build::enter_sentences_only(QString open, QString pre_space)
{
 u1 count = open.size();
 if(pre_space.contains("\n"))
   primary_acc(" ");

 reset_primary();

 parse_context_.flags.sentences_only = true;
 flags.sentences_only = true;
}

void GTagML_Graph_Build::leave_sentences_only(QString close, QString post_space)
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

void GTagML_Graph_Build::force_switch_sentence()
{
 end_sentence("");
}

void GTagML_Graph_Build::ell_2_nonbreak()
{
 reset_primary();

 if(!flags.sentences_only)
   latex_stream_ << ".~";

 if(!flags.latex_only)
   sentences_text_stream_ << ". ";
}

void GTagML_Graph_Build::ell_count(u1 count, QString follow)
{
 reset_primary();

 static QStringList latex = {".\\@", "\\ellThree{2pt}{2pt}",
   "\\ellFour{2pt}{2pt}"};

 static QStringList sentences = {".", "...",
   "...."};

 if(!flags.sentences_only)
   latex_stream_ << latex.value(count - 2) << follow;

 if(!flags.latex_only)
   sentences_text_stream_ << sentences.value(count - 2) << follow;


// if(count == 3)
// {
//  if(!flags.sentences_only)
//    latex_stream_ << ;

//  if(!flags.latex_only)
//    sentences_text_stream_ << "...";
// }

// else if(count == 4)
// {
//  if(!flags.sentences_only)
//    latex_stream_ << "\\ellFour{2pt}{2pt}";

//  if(!flags.latex_only)
//    sentences_text_stream_ << "....";
// }

}

void GTagML_Graph_Build::noindent_marker()
{
 if(!flags.sentences_only)
   latex_stream_ << "\\noindent{}";
}


void GTagML_Graph_Build::footnote_marker(QString text)
{
 reset_primary();

 if(!flags.latex_only)
   sentences_text_stream_ << "\\" << text;

 if(!flags.sentences_only)
   latex_stream_ << "\\fnm{" << text << "}";
}


void GTagML_Graph_Build::end_sentence(QString punctuation,
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

 sentences_sdi_stream_ << "\n\n--- Sentence//end";

 sentences_sdi_stream_ << "\nid: " << sentence_id_;

 sentences_sdi_stream_ << "\nr#  " <<
   document_info_.line_and_column_string(parser_->current_position());

 if(!sentence_gaps_.isEmpty())
 {
  sentences_sdi_stream_ << "\ng. " << sentence_gaps_to_string();
  sentence_gaps_.clear();
 }

 sentences_sdi_stream_ << "\np: " << punctuation;

 u1 nc = sentence_nesting_depth_;

 if(nesting_code != Nesting_Codes::Signal_Default)
   nc |= nesting_code;

 if(nc != 1)
   sentences_sdi_stream_ << "\nN: " << nc;

 for(auto pr : supplements)
 {
  sentences_sdi_stream_ << "\n" << pr.first << ": "
    << pr.second;
 }

 sentences_sdi_stream_ << "\nt. " << sentences_text_to_string() << "\n";

 sentences_text_.clear();

 if(!flags.sentences_only)
 {
  xml_writer_.writeCharacters(punctuation);
  latex_stream_ << punctuation; // << "  ";
 }

  //? latex_stream_ << punctuation << "@ ";

// primary_acc_stream_ << punctuation << "  ";
}




void GTagML_Graph_Build::heading(u1 count, QString stext, QString ltext)
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
  latex_stream_ << "\n\n\\addendum{";
 }

}

void GTagML_Graph_Build::prepare_bibliography()
{
 xml_writer_.writeCharacters("\n\n");
 //<sec><title>Introduction</title> <ref

// xml_writer_.writeStartElement("sec");
// xml_writer_.writeTextElement("title", "Bibliography");

 xml_writer_.writeStartElement("back");
 xml_writer_.writeStartElement("ref-list");
 xml_writer_.writeTextElement("title", "Bibliography");
 xml_writer_.writeComment("bib:here");
 xml_writer_.writeEndElement();

 xml_writer_.writeEndElement();
// xml_writer_.writeEndElement();
}

void GTagML_Graph_Build::end_document()
{
 reset_primary();

//?? check_close_paragraph();

 latex_stream_ << "\n%END-TEMPLATE%\n";

 latex_stream_ << "\n\\end{document}";

//? xml_writer_.writeEndElement();
//? xml_writer_.writeComment("prepare end document");

 xml_writer_.writeCharacters("\n\n@=/sec=@\n");
 xml_writer_.writeComment("end section");

 xml_writer_.writeEndElement();
 xml_writer_.writeComment("ended body");

 xml_writer_.writeComment("inserting bibliography");
 prepare_bibliography();

 xml_writer_.writeEndElement();
 xml_writer_.writeEndDocument();
 xml_writer_.writeComment("ended document");

 parse_context_.flags.auto_paragraph_mode = false;
}

void GTagML_Graph_Build::set_paragraph_bridge()
{
 current_paragraph_bridge_ = current_paragraph_count_ + 1;
}

void GTagML_Graph_Build::leave_footnote(QString pretext, QString space)
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
  xml_writer_.writeEndElement();
  xml_writer_.writeComment("leave footnote");
  xml_writer_.writeCharacters("\n\n");
 }

 if(!flags.xml_only)
   latex_stream_ << latex_space << "}";

}

void GTagML_Graph_Build::enter_footnote(QString pretext, QString space)
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
  xml_writer_.writeCharacters("\n\n");
//  xml_writer_.writeStartElement("doc-footnote");
  xml_writer_.writeStartElement("fn");

  xml_writer_.writeAttribute("id", "fn-%1"_qt.arg(fn_count_));
  xml_writer_.writeCharacters("@fn-hold@");
 }

 if(!flags.xml_only)
   latex_stream_ << "\\docFootnote{" << latex_space;
}

void GTagML_Graph_Build::latex_command_via_semantic_annotation(QString concept, QString text)
{
 reset_primary();

 xml_writer_.writeTextElement("SA-%1"_qt.arg(concept), text);
 latex_stream_ << "\\SA" << concept << "{" << text << "}";
}


QString GTagML_Graph_Build::line_and_column_string_tight()
{
 return line_and_column_string_tight(parser_->current_position());
}

QString GTagML_Graph_Build::line_and_column_string()
{
 return line_and_column_string(parser_->current_position());
}


QString GTagML_Graph_Build::line_and_column_string_tight(u4 pos)
{
 return document_info_.line_and_column_string(pos, "/");
}

QString GTagML_Graph_Build::line_and_column_string(u4 pos)
{
 return document_info_.line_and_column_string(pos);
}

void GTagML_Graph_Build::enter_sentences_latex_filter(QString pretext)
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

void GTagML_Graph_Build::leave_sentences_latex_filter(QString pretext)
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

void GTagML_Graph_Build::enter_heading(u1 count1, u1 count2)
{
 reset_primary();
 check_close_paragraph();

 heading_counts_ = {count1, count2};

 if(heading_counts_ == QPair<u1, u1> {3, 2})
 {
  latex_stream_ << "\n\n";
  parse_context_.flags.auto_paragraph_mode = false;

  end_document();

  return;
 }


 flags.heading_acc = true;
 parse_context_.flags.heading_acc = true;
}


void GTagML_Graph_Build::leave_heading()
{
 heading(heading_counts_.first, heading_counts_.second,
   sentences_section_heading_, latex_section_heading_);
 sentences_section_heading_.clear();
 latex_section_heading_.clear();
 flags.heading_acc = false;
 parse_context_.flags.heading_acc = false;
}


void GTagML_Graph_Build::enter_justline(QString pretext)
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
    latex_stream_ << code;
 }

 parse_context_.flags.heading_acc = false;
 parse_context_.flags.justline = true;
}

void GTagML_Graph_Build::leave_justline(QString pretext, QString follow)
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
    latex_stream_ << code;
 }
}


void GTagML_Graph_Build::heading(u1 count1, u1 count2, QString stext, QString ltext)
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
   latex_stream_ << "\n\n\\notesdiv{" << ltext << "}\n";
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

void GTagML_Graph_Build::enter_auto_paragraph_mode()
{
 set_paragraph_bridge();
 parse_context_.flags.auto_paragraph_mode = true;
}

void GTagML_Graph_Build::close_paragraph()
{
 if(flags.in_pa_1)
 {
  flags.in_pa_1 = false;
 }
 else
 {
  xml_writer_.writeCharacters("\n");
  xml_writer_.writeEndElement();
  xml_writer_.writeComment("end of p1 paragraph");
  xml_writer_.writeCharacters("\n\n");
 }

 if(flags.just_ended_sentence)
 {
  if(flags.use_latex_sdi_all_markers)
    latex_stream_ << "\\<";

  sentences_sdi_stream_ << "\n--- Sentence/end \nid: "
    << sentence_id_ << "\nr#  "
    << line_and_column_string() << "\n";

  if(!sentence_gaps_.isEmpty())
  {
   sentences_sdi_stream_ << "g. " << sentence_gaps_to_string() << "\n";
   sentence_gaps_.clear();
  }
 }

 if(flags.use_latex_sdi_all_markers || flags.use_latex_sdi_paragraph_markers)
   latex_stream_ << "\\;";

 sentences_sdi_stream_ << "\n--- Paragraph/end \nid: " << paragraph_id_
   << "\ny: " << current_paragraph_type_to_string() << "\n";


 if(current_paragraph_type_ == Paragraph_Types::Abstract)
 {
  current_paragraph_type_ = Paragraph_Types::N_A;
  latex_stream_ << "\n\\end{docAbstract}] \n";
  set_paragraph_bridge();
 }
 else
 {
//  if(flags.use_latex_sdi_paragraph_markers)
//    latex_stream_ << "\n}\\;% end paragraph \n";
//  else
    latex_stream_ << "\n}% end paragraph \n"; // pLevel
    ; //latex_stream_ << "\n} % end paragraph";

 }

 flags.just_ended_sentence = false;
}

void GTagML_Graph_Build::check_close_paragraph()
{
 if(current_paragraph_bridge_)
   current_paragraph_bridge_ = 0;
 else
   close_paragraph();
}

void GTagML_Graph_Build::show_latex()
{
 qDebug() << "\n" << latex_ << "\n";
}

void GTagML_Graph_Build::auto_new_paragraph()
{
 auto_new_paragraph("p.1");
}

void GTagML_Graph_Build::auto_new_paragraph(QString cmd)
{ // p.1
 reset_primary();

 check_close_paragraph();

 current_slash_line_plus_count_ = 0;

 bool skip_flags = false;

 if(cmd == "p.1")
 {
  ++current_paragraph_count_;
  xml_writer_.writeCharacters("\n");
  xml_writer_.writeComment("enter " + cmd);
  xml_writer_.writeStartElement(cmd);
  xml_writer_.writeAttribute("id", QString::number(current_paragraph_count_).prepend("Para-"));
 }
 else if(cmd == "pa.1")
 {
  skip_flags = true;
  //   qDebug() << " \n\n   ! " << cmd;
  //   xml_writer_.writeComment("bib:here");
  flags.in_pa_1 = true;
 }

 latex_stream_ << "\n\\" << cmd << "{%\n";

// if(flags.use_latex_sdi_paragraph_markers)
//   latex_stream_ << "\\:";


//? latex_stream_ << "\n\n\\pLevelOne{";

//? latex_stream_ << "\n\n\\pLevelOne{";

 if(skip_flags)
   return;

 flags.await_paragraph_start = true;
 flags.await_sentence_start = true;
}

void GTagML_Graph_Build::pseudo_paragraph()
{
 reset_primary();

 xml_writer_.writeCharacters("\n");
 xml_writer_.writeEndElement();
 xml_writer_.writeComment("end of list paragraph");

 ++pseudo_paragraph_counts_.second;

 xml_writer_.writeStartElement("p");
// xml_writer_.writeAttribute("paragraph-container", "list");

 xml_writer_.writeAttribute("id", "P-list_%1-%2"_qt
   .arg(pseudo_paragraph_counts_.first).arg(pseudo_paragraph_counts_.second));

 //? xml_writer_.writeTextElement("<pp>", "");
 latex_stream_ << "\n\n\\pseudoIndent{} ";
}


void GTagML_Graph_Build::enter_subparagraph(QString text)
{
 reset_primary();

 current_item_count_ = 0;

 if(text == "exs")
 {
  latex_stream_ << "\n\n\\begin{exsGroup}\n";
  xml_writer_.writeStartElement("exs-group");
  parse_context_.flags.read_parens_as_label = true;
  parse_context_.flags.read_parens_as_ref = false;
 }

 else if(text == "desc")
 {
  latex_stream_ << "\n\n\\begin{docDescription}\n";
//  xml_writer_.writeStartElement("exs-group");
  parse_context_.flags.read_desc_label = true;
  parse_context_.flags.ignore_blank_lines = true;

  xml_writer_.writeCharacters("\n\n");

  xml_writer_.writeStartElement("list");
  xml_writer_.writeAttribute("list-type", "simple");
  xml_writer_.writeAttribute("list-content", "description");
  xml_writer_.writeCharacters("\n");


//  parse_context_.flags.read_parens_as_ref = false;
 }

 else if(text == "enumn")
 {
  latex_stream_ << "\n\n\\begin{docEnumerate}[1)]\n";
  xml_writer_.writeStartElement("enumn");
  parse_context_.flags.read_numbered_items = true;
  parse_context_.flags.ignore_blank_lines = true;
 }

 else if(text == "enums")
 {
//?  latex_stream_ << "\n\n\\begin{enums}\n";
  latex_stream_ << "\n\n\\begin{docEnumerate}\n";
  xml_writer_.writeStartElement("enums");
  parse_context_.flags.read_numbered_items = true;
  parse_context_.flags.ignore_blank_lines = true;
 }

 else if(text == "itemz")
 {
  latex_stream_ << "\n\n\\begin{itemz}\n";
  xml_writer_.writeStartElement("itemz");
  parse_context_.flags.read_bulleted_items = true;
  parse_context_.flags.ignore_blank_lines = true;
 }

 else if(text == "block")
 {
  latex_stream_ << "\n\n\\begin{blockQuote}\n";
  xml_writer_.writeStartElement("block-quote");
  parse_context_.flags.ignore_blank_lines = true;

  held_paragraph_types_.push(current_paragraph_type_);
  current_paragraph_type_ = Paragraph_Types::Block_Quote;
 }

 else if(text == "nblock")
 {
  latex_stream_ << "\n\n\\ndnblockQuote{%\n";
//  xml_writer_.writeStartElement("block-quote");
  parse_context_.flags.ignore_blank_lines = true;

  held_paragraph_types_.push(current_paragraph_type_);
  current_paragraph_type_ = Paragraph_Types::Endnote_Block_Quote;
 }


}

void GTagML_Graph_Build::check_blank_line()
{
 if(current_paragraph_type_ == Paragraph_Types::Block_Quote)
 {
  reset_primary();
//?  latex_stream_ << "\n\\parbreak.2{}\n";
  latex_stream_ << "\n\\parbreak{}\n";
 }

 else if(current_paragraph_type_ == Paragraph_Types::Endnote_Block_Quote)
 {
  reset_primary();
//?  latex_stream_ << "\n\\parbreak.2{}\n";
  latex_stream_ << "\n\\parbreak{}\n";
 }

 else if(parse_context_.flags.read_bulleted_items)
 {
  reset_primary();
  //?qDebug() << "\n\n" << latex_ << "\n\n";
  latex_stream_ << "\n";
 }
}

void GTagML_Graph_Build::single_slash_line_plus()
{
 single_slash_line();

 static QString letters = "abcdefghijklmnopqrstuvwxyz";

// latex_stream_ << "\n\\nip";

 latex_stream_ << "\n\\nip\\clrmMid{"
   << letters[current_slash_line_plus_count_] << "}%";

 ++current_slash_line_plus_count_;

}

void GTagML_Graph_Build::single_slash_line()
{
 reset_primary();

 xml_writer_.writeEndElement();

 if(parse_context_.flags.read_desc_label)
 {
//  xml_writer_.writeEndElement();
  xml_writer_.writeComment("end of list paragraph");
  xml_writer_.writeCharacters("\n");
  xml_writer_.writeCharacters("@=/list-item=@");
  xml_writer_.writeCharacters("\n");

  xml_writer_.writeEndElement();
  xml_writer_.writeComment("end of description");


  xml_writer_.writeCharacters("\n\n");

  latex_stream_ << "\n\\end{docDescription}\n";
  parse_context_.flags.read_desc_label = false;
  parse_context_.flags.ignore_blank_lines = false;
 }

 else if(parse_context_.flags.read_parens_as_label)
 {
  latex_stream_ << "\n\\end{exsGroup}\n";
  parse_context_.flags.read_parens_as_label = false;
  parse_context_.flags.read_parens_as_ref = true;
 }

 else if(parse_context_.flags.read_numbered_items)
 {
  latex_stream_ << "\n\\end{docEnumerate}\n";
//?  latex_stream_ << "\n\\end{enums}\n";
  parse_context_.flags.read_numbered_items = false;
  parse_context_.flags.ignore_blank_lines = false;
 }

 else if(parse_context_.flags.read_bulleted_items)
 {
  latex_stream_ << "\n\\end{itemz}\n";
  parse_context_.flags.read_bulleted_items = false;
  parse_context_.flags.ignore_blank_lines = false;
 }


 else if(current_paragraph_type_ == Paragraph_Types::Block_Quote)
 {
  latex_stream_ << "\\end{blockQuote}\n";
  parse_context_.flags.ignore_blank_lines = false;
  current_paragraph_type_ = held_paragraph_types_.pop();
 }

 else if(current_paragraph_type_ == Paragraph_Types::Endnote_Block_Quote)
 {
  latex_stream_ << "}\\newpage{}\n";
  parse_context_.flags.ignore_blank_lines = false;
  current_paragraph_type_ = held_paragraph_types_.pop();
 }

}



void GTagML_Graph_Build::paren_ref_global(u2 number, QString text)
{
 reset_primary();

 QString r = QString::number(number);

 latex_stream_ << "\\exsRef(" << r << ")" ;
 xml_writer_.writeTextElement("-exsRef", "r");
}


void GTagML_Graph_Build::paren_ref(u2 number, QString text)
{
 reset_primary();

 QString r = QString::number(number + current_exs_offset_);

 latex_stream_ << "\\exsRef(" << r << ")" ;
 xml_writer_.writeTextElement("-exsRef", "r");
}


void GTagML_Graph_Build::latex_command_auto_closed(QString command_name, QString arg)
{
 reset_primary();

 if(arg.isEmpty())
 {
  latex_stream_ << "\\" << command_name << "{}";

   // //  xml_writer_.writeCharacters("!%1%!"_qt.arg(command_name));
 }
 else
 {
  latex_stream_ << "\\" << command_name << "{" << arg << "}";
   // //  xml_writer_.writeCharacters("!%1:%2%!"_qt.arg(command_name).arg(arg));
 }
}

void GTagML_Graph_Build::citation(QString full_match, QString label, QString locator)
{
 u2 ref_index = ref_labels_.indexOf(label);

 reset_primary();

 sentences_text_stream_ << full_match;

 QStringList qsl = locator.split("==");
 locator = qsl.takeFirst();

 if(!qsl.isEmpty())
 {
  xml_writer_.writeTextElement("attrib", qsl.join(", "));
 }

 xml_writer_.writeCharacters("[");
 xml_writer_.writeStartElement("xref");
 xml_writer_.writeAttribute("ref-type", "bibr");
//? xml_writer_.writeAttribute("rid", label);

 xml_writer_.writeAttribute("rid", "B%1"_qt.arg(ref_index + 1));

 xml_writer_.writeCharacters("%1"_qt.arg(ref_index + 1));

 if(locator.isEmpty())
 {
  latex_stream_ << "\\citeLabel{" << label << "}";
//?  xml_writer_.writeCharacters("!%1%!"_qt.arg(command_name));
 }
 else if(locator.startsWith(":"))
 {
  QStringList qsl = locator.mid(1).simplified().split("-");
  if(qsl.size() == 1)
  {
   latex_stream_ << "\\citePage{" << label << "}"
     << "(" << qsl.first() << ")";

//   xml_writer_.writeAttribute("page", qsl.first());
   xml_writer_.writeCharacters(", p. %1"_qt.arg(qsl.first()));
  }
  else
  {
   latex_stream_ << "\\citePages{" << label << "}"
     << "(" << qsl.join(", ") << ")";

   xml_writer_.writeCharacters(", pp. %1"_qt.arg(qsl.join("&#x2013;")));
  }
 }

 else if(locator.startsWith(";"))
 {
  latex_stream_ << "\\citeLocator{" << label << "}"
    << "(" << locator.mid(1) << ")";

  QString loc = locator.mid(1);
  loc.replace("pp.", "pp. ");
  loc.replace("--", "@=#x2013;");
//  xml_writer_.writeAttribute("locator", locator.mid(1));
  xml_writer_.writeCharacters(loc.prepend(", "));
 }

 xml_writer_.writeEndElement();
 xml_writer_.writeCharacters("]");
}

void GTagML_Graph_Build::bulleted_item(QString symbol, QString supp)
{
 reset_primary(); //? qDebug() << "\n\n" << latex_ << "\n\n";

 latex_stream_ << "\n\\symItem{} ";
 xml_writer_.writeTextElement("sym-item", "");
}


void GTagML_Graph_Build::enums_item(u2 number, QString text, QString follow)
{
 reset_primary();

 if(follow == ")")
  //? latex_stream_ << "\n\\enumsItem{}[)] ";
   latex_stream_ << "\n\\item[" << text << ")] ";

 else
   latex_stream_ << "\n\\enumsItem{} ";

// xml_writer_.writeTextElement("enums-item", "");

 xml_writer_.writeComment("enums-item");
}

void GTagML_Graph_Build::desc_item_with_multiline_label(QString text)
{
 reset_primary();

 latex_stream_ << "\n\n\\descItemBlank{} \\descMLabel{" << text << "}";
//? xml_writer_.writeTextElement("exs-item", "");

 ++current_item_count_;

 if(current_item_count_ == 1)
 {
  xml_writer_.writeCharacters("\n");
 }
 else
 {
  xml_writer_.writeCharacters("\n");
  xml_writer_.writeEndElement();
  xml_writer_.writeComment("end of list paragraph");
  xml_writer_.writeCharacters("\n\n");
 }

 ++pseudo_paragraph_counts_.first;
 pseudo_paragraph_counts_.second = 0;

 if(pseudo_paragraph_counts_.first > 1)
   xml_writer_.writeCharacters("@=/list-item=@");

 xml_writer_.writeTextElement("list-item", text);

 ++pseudo_paragraph_counts_.second;

 xml_writer_.writeStartElement("p");
// xml_writer_.writeAttribute("paragraph-container", "list");
 xml_writer_.writeAttribute("id", "P-list_%1-%2"_qt
   .arg(pseudo_paragraph_counts_.first).arg(pseudo_paragraph_counts_.second));


 end_sentence();
}

void GTagML_Graph_Build::desc_item(QString text)
{
 reset_primary();

 ++current_item_count_;

 ++pseudo_paragraph_counts_.first;
 pseudo_paragraph_counts_.second = 0;

 if(current_item_count_ == 1)
 {
  xml_writer_.writeCharacters("\n");
 }
 else
 {
  xml_writer_.writeCharacters("\n");
  xml_writer_.writeEndElement();
  xml_writer_.writeComment("end of list paragraph");
  xml_writer_.writeCharacters("\n\n");

  xml_writer_.writeCharacters("@=/list-item=@");
 }

 // if(pseudo_paragraph_counts_.first > 1)
 xml_writer_.writeTextElement("list-item", text);

 xml_writer_.writeStartElement("p");


 ++pseudo_paragraph_counts_.second;

// xml_writer_.writeStartElement("p");
// xml_writer_.writeAttribute("paragraph-container", "list");

 xml_writer_.writeAttribute("id", "P-list_%1-%2"_qt
   .arg(pseudo_paragraph_counts_.first).arg(pseudo_paragraph_counts_.second));

// xml_writer_.writeAttribute("paragraph-container", "list");

 latex_stream_ << "\n\n\\descItem{" << text << "}";
//? xml_writer_.writeTextElement("exs-item", "");

 end_sentence();
}


void GTagML_Graph_Build::exs_item(u2 number, QString text)
{
 reset_primary();

 if(number == 1)
 {
  current_exs_offset_ = current_exs_number_;
  ++current_exs_group_number_;
 }

 ++current_exs_number_;

 latex_stream_ << "\n\\exsItem{} ";
 xml_writer_.writeTextElement("exs-item", "");

 end_sentence();
}

void GTagML_Graph_Build::enter_block_float_mode()
{
 reset_primary();
 latex_stream_ << "\\ndntext{";
 parse_context_.flags.block_float_mode = true;

//? parse_context_.flags.heading_acc = false;
}

void GTagML_Graph_Build::leave_block_float_mode()
{
 reset_primary();

 parse_context_.flags.block_float_mode = false;

 latex_stream_ << "}";
}


void GTagML_Graph_Build::enter_italics_mode()
{
 reset_primary();

 xml_writer_.writeStartElement("i");

 latex_stream_ << "\\textit{";

 parse_context_.flags.italics_mode = true;
}

void GTagML_Graph_Build::leave_italics_mode()
{
 reset_primary();

 parse_context_.flags.italics_mode = false;

 xml_writer_.writeEndElement();
 latex_stream_ << "}";

}

void GTagML_Graph_Build::hyperlink_2(QString text, QString link)
{
 reset_primary();

 xml_writer_.writeTextElement("hlink2", text + link);
 latex_stream_ << "\\hlinkTwo{" << text << "}{" + link + "}";
}

void GTagML_Graph_Build::hyperlink_1(QString text)
{
 reset_primary();

 xml_writer_.writeTextElement("hlink1", text);
 latex_stream_ << "\\hlinkOne{" << text << "}";
}

void GTagML_Graph_Build::emph_symbolic(QString text)
{
 reset_primary();

 xml_writer_.writeTextElement("eS", text);
 latex_stream_ << "\\eS{" << text << "}";
}

void GTagML_Graph_Build::emph_acronym(QString text)
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

 xml_writer_.writeTextElement(eA + version, text);
 latex_stream_ << "\\eA" << version << "{" << text << "}";
}

void GTagML_Graph_Build::enter_double_quote_mode(QString pre)
{
 reset_primary();

 parse_context_.flags.double_quote_mode = true;

 if(pre == "d")
   xml_writer_.writeStartElement("disp-quote");
 else if(pre == "!")
   xml_writer_.writeStartElement("ql");
 else
   xml_writer_.writeStartElement("q");

 if(pre == "!")
 {
  latex_stream_ << "\\ql{";
  flags.in_ql = true;
 }
 else
   latex_stream_ << "\\q{";
}

void GTagML_Graph_Build::leave_double_quote_mode()
{
 reset_primary();

 parse_context_.flags.double_quote_mode = false;

 if(flags.in_ql)
 {
  // //  anything?
  flags.in_ql = false;
 }

 xml_writer_.writeEndElement();
 latex_stream_ << "}";
}

void GTagML_Graph_Build::enter_single_quote_mode()
{
 reset_primary();
 parse_context_.flags.single_quote_mode = true;

 latex_stream_ << "\\sq{";
}

void GTagML_Graph_Build::leave_single_quote_mode()
{
 reset_primary();

 parse_context_.flags.single_quote_mode = false;
 latex_stream_ << "}";
}

void GTagML_Graph_Build::enter_single_quote_mode_doubled()
{
 reset_primary();
 parse_context_.flags.single_quote_mode_doubled = true;

 latex_stream_ << "\\sqq{";
}

void GTagML_Graph_Build::leave_single_quote_mode_doubled()
{
 reset_primary();
 parse_context_.flags.single_quote_mode_doubled = false;

 latex_stream_ << "}";
}

void GTagML_Graph_Build::enter_single_quote_mode_trebled()
{
 parse_context_.flags.single_quote_mode_trebled = true;

}

void GTagML_Graph_Build::leave_single_quote_mode_trebled()
{
 parse_context_.flags.single_quote_mode_trebled = false;

}

void GTagML_Graph_Build::enter_acronym_mode(u1 size)
{
 if(size == 2)
 {
  enter_sample_mode();
  return;
 }

 reset_primary();

 parse_context_.flags.acronym_mode = true;

 held_macro_string_ = "eA";
// xml_writer_.writeStartElement("eA");
// latex_stream_ << "\\eA{";
}


void GTagML_Graph_Build::leave_acronym_mode()
{
 QString version;

 if(primary_acc_ == primary_acc_.toLower())
   version = "AllLower";
 else if(primary_acc_ != primary_acc_.toUpper())
 {
  // //  what to do
  if(primary_acc_.endsWith("s"))
  {
   QString t = primary_acc_;
   t.chop(1);
   if(t == t.toUpper())
     goto skip_this;
  }
  version = "SomeLower";
 }

skip_this:

 if(flags.in_ql)
   xml_writer_.writeStartElement("iq_" + held_macro_string_ + version);
 else
   xml_writer_.writeStartElement(held_macro_string_ + version);

 latex_stream_ << "\\" + held_macro_string_ << version << "{";

 reset_primary();

 parse_context_.flags.acronym_mode = false;

 xml_writer_.writeEndElement();
 latex_stream_ << "}";
}

void GTagML_Graph_Build::enter_sample_mode()
{
 reset_primary();

 parse_context_.flags.emph_sample_mode = true;

 xml_writer_.writeStartElement("eS");
 latex_stream_ << "\\eS{";
}


void GTagML_Graph_Build::leave_sample_mode()
{
 reset_primary();

 parse_context_.flags.emph_sample_mode = false;

 xml_writer_.writeEndElement();
 latex_stream_ << "}";
}


void GTagML_Graph_Build::short_macro(QString text, u1 size)
{
 if(size == 2)
 {
  enter_highlight_mode();
  primary_acc_ = text;
  leave_highlight_mode();
  return;
 }
 enter_short_macro_mode(1);
 primary_acc_ = text;
 leave_short_macro_mode();
}

void GTagML_Graph_Build::short_acronym(QString text, u1 size)
{
 if(size == 2)
 {
  enter_sample_mode();
  primary_acc_ = text;
  leave_sample_mode();
  return;
 }

 enter_acronym_mode(1);
 primary_acc_ = text;
 leave_acronym_mode();
}

void GTagML_Graph_Build::short_emph_sample(QString text)
{
 enter_sample_mode();
 primary_acc_ = text;
 leave_sample_mode();
}

void GTagML_Graph_Build::enter_highlight_mode()
{
 reset_primary();

 parse_context_.flags.emph_highlight_mode = true;

 xml_writer_.writeStartElement("eH");
 latex_stream_ << "\\eH{";
}

void GTagML_Graph_Build::leave_highlight_mode()
{
 reset_primary();

 parse_context_.flags.emph_highlight_mode = false;

 xml_writer_.writeEndElement();
 latex_stream_ << "}";
}

void GTagML_Graph_Build::enter_short_macro_mode(u1 size)
{
 if(size == 2)
 {
  enter_highlight_mode();
  return;
 }

 reset_primary();

 parse_context_.flags.short_macro_mode = true;

 xml_writer_.writeStartElement("sM");
 latex_stream_ << "\\";
}


void GTagML_Graph_Build::leave_short_macro_mode()
{
 QString latex = primary_acc_;
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

 xml_writer_.writeCDATA(primary_acc_);
 xml_writer_.writeEndElement();

 primary_acc_.clear();

 latex_stream_ << latex << "{}";
}


void GTagML_Graph_Build::enter_emph_italics_mode(QString mid)
{
 reset_primary();

 parse_context_.flags.emph_italics_mode = true;

 if(mid == "=")
 {
  xml_writer_.writeStartElement("eIbox");
  latex_stream_ << "\\eIbox{";
 }
 else
 {
  xml_writer_.writeStartElement("eI");
  latex_stream_ << "\\eI{";
 }
}

void GTagML_Graph_Build::leave_emph_italics_mode()
{
 reset_primary();

 parse_context_.flags.emph_italics_mode = false;

 xml_writer_.writeEndElement();
 latex_stream_ << "}";
}


void GTagML_Graph_Build::special_character_sequence(QString text)
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
    latex_stream_ << latex;

  else if(flags.sentences_only)
    sentences_text_stream_ << sentences;

  else
  {
   latex_stream_ << latex;
   sentences_text_stream_ << sentences;
   xml_writer_.writeCharacters(xml);
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

void GTagML_Graph_Build::prepare_jats(QString& text, QString bib_path)
{
 QString bibtext = KA::TextIO::load_file(bib_path);

 u1 count = 0;
 for(QString ref_label : ref_labels_)
 {
  ++count;
  bibtext.replace(ref_label, "B%1"_qt.arg(count));
 }

 bibtext.replace("B3TEI", "B4");



 QString meta = KA::TextIO::load_file(bib_path.replace("bib", "meta"));

 text.replace("<!--bib:here-->", bibtext);
 text.replace("<!--meta:here-->", meta);

 text.replace("footnote \\lhRef{poppler-adj}{\\thetrackFNpoppler}",
   "footnote&#xa0;<xref ref-type=\"fn\" rid=\"fn-3\"></xref>");
 text.replace("{\\sth}", " ");

 text.replace("\\resizebox{51pt}{!}{capabilities}", "capabilities");

 text.replace("CDATA[l", "CDATA[");
 text.replace("CDATA[Len", "CDATA[l");
 text.replace("CDATA[ctrlleft", "CDATA[ctrl+click");
 text.replace("CDATA[ThreeD", "CDATA[3D");

 text.replace(" ....", " &#x2026;.");
 text.replace("....", ".&#x2026;");

 //?text.replace("<document>", "<document xmlns:xlink=\"http://www.w3.org/1999/xlink\">");
 text.replace("<body>", "");

 QString da = R"(
<fn fn-type="supplementary-material"><p>
SOURCES: PDF <uri>https://scignscape.github.io/PNP/documents/A-perspective-from-compiler-theory.pdf</uri>
         Demo code <uri>https://github.com/scignscape/PNP</uri>
         JATS <uri>https://scignscape.github.io/PNP/documents/A-perspective-from-compiler-theory.jats.xml</uri>
         .gt file (parses into C++ objects): <uri>https://scignscape.github.io/PNP/documents/A-perspective-from-compiler-theory.gt</uri>
</fn>

  )";


 text.replace("<doc-abstract>", "<abstract><p>");
 text.replace("</doc-abstract>", da + "</p></abstract></article-meta></front>\n\n<body>");

 text.replace("@fn-hold@", "<p>");
 text.replace("</fn>", "</p></fn>");


// text.replace("<ref-list>", "<back><ref-list>");
// text.replace("</ref-list>", "</ref-list></back>");

 text.replace("\\Visavis{}", "Vis-a-vis");
 text.replace("{\\sth}", " ");
 text.replace("\\-", "");

 text.replace("visavis", "vis-a-vis");

 text.replace("fOFg", "f-of-g");
 text.replace("slashA", "\\A");

 text.replace("\\\\", "");

 text.replace("<p.1", "<p");
// text.replace("<p.1>", "<p l=\"1\">");
 text.replace("</p.1>", "</p> <!--/p.1-->");

 text.replace("<i>", "<italic>");
 text.replace("</i>", "</italic>");


 text.replace("<s1>", "<sec><title>");
 text.replace("</s1>", "</title>\n\n");

 text.replace("<eASomeLower>", "<styled-content use=\"emph-acronym_some-lc\" style=\"color:rgb(2, 37, 48)\">");
//                               "lc-status=\"some-lower\" >");

 text.replace("</eASomeLower>", "</styled-content>");


 text.replace("<iq_eASomeLower>", "<styled-content use=\"emph-acronym_in-ql_some-lc\" style=\"color:rgb(154, 14, 19)\">");
 text.replace("</iq_eASomeLower>", "</styled-content>");

 text.replace("<eAAllLower>", "<styled-content use=\"emph-acronym_all-lc\" style=\"color:rgb(2, 37, 48)\">");
//                               "lc-status=\"all-lower\" >");

 text.replace("</eAAllLower>", "</styled-content>");


 text.replace("<iq_eAAllLower>", "<styled-content use=\"emph-acronym_in-ql_all-lc\" style=\"color:rgb(154, 14, 19)\">");
 text.replace("</iq_eAAllLower>", "</styled-content>");


 text.replace("<eA>", "<styled-content use=\"emph-acronym\" style=\"color:rgb(2, 37, 48)\"><abbrev>");
 text.replace("</eA>", "</abbrev></styled-content>");

 text.replace("<iq_eA>", "<styled-content use=\"emph-acronym_in-ql\" style=\"color:rgb(154, 14, 19)\"><abbrev>");
 text.replace("</iq_eA>", "</abbrev></styled-content>");


 text.replace("<eS>", "<styled-content use=\"emph-symbol\" style=\"color:rgb(93, 129, 194)\">");
 text.replace("</eS>", "</styled-content>");

 text.replace("<eH>", "<styled-content use=\"emph-highlight\" style=\"color:rgb(168, 30, 35)\">");
 text.replace("</eH>", "</styled-content>");

 text.replace("<eI>", "<styled-content use=\"ital-literal\">");
 text.replace("</eI>", "</styled-content>");

 text.replace("<eIbox>", "<styled-content use=\"ital-literal-boxed\">");
 text.replace("</eIbox>", "</styled-content>");

 text.replace("<sM>", "<styled-content use=\"emph-macro\">");
 text.replace("</sM>", "</styled-content>");

 text.replace("<SA-nmbr>", "<styled-content use=\"SA-nmbr\">");
 text.replace("</SA-nmbr>", "</styled-content>");

 text.replace("<SA-year>", "<styled-content use=\"SA-year\">");
 text.replace("</SA-year>", "</styled-content>");

 text.replace("<SA-df>", "<styled-content use=\"SA-df\">");
 text.replace("</SA-df>", "</styled-content>");

 text.replace("<hlink1>", "<styled-content use=\"hlink1\">");
 text.replace("</hlink1>", "</styled-content>");

 text.replace("<q>", "<styled-content use=\"double-quote\">&#x201c;");
 text.replace("</q>", "&#x201d;</styled-content>");

 text.replace("<ql>", "<styled-content use=\"long-double-quote\" style=\"color:rgb(48, 14, 40)\">&#x201c;");
 text.replace("</ql>", "&#x201d;</styled-content>");

 text.replace("styled-content use", "styled-content style-type");

 text.replace("<attrib>", "<!-- source: ");
 text.replace("</attrib>", " -->");

 text.replace("<list-item>", "<list-item><title>");
 text.replace("</list-item>", "</title>");

 text.replace("<enums><!--enums-item-->", "<list><list-item><p>");
 text.replace("<!--enums-item-->", "</p></list-item><list-item><p>");
 text.replace("</enums>", "</p></list-item></list>");

 text.replace("\\makebox{object/value}", "object/value");

 text.replace("\\lhRef{section-one-one}{\\soneone}", "1.1");

 text.replace("@=/", "</");
 text.replace("=@", ">");

 text.replace("@=", "&");

 text.replace("/ ", "/");

}


void GTagML_Graph_Build::enter_multiline_comment(QString semis, QString tildes)
{
 markup_position_.enter_multiline_comment(cutmax(semis.length()), cutmax(tildes.length()));
//? parse_context_.flags.inside_multiline_comment = true;
}

void GTagML_Graph_Build::check_leave_multiline_comment(QString semis, QString tildes)
{
 if(markup_position_.check_leave_multiline_comment(cutmax(tildes.length()),
  cutmax(tildes.length())));
//?  parse_context_.flags.inside_multiline_comment = false;
}

void GTagML_Graph_Build::tile_acc(QString str)
{
 tile_acc_qts_ << str;
}

//void GTagML_Graph_Build::spm_acc(QString str)
//{
// spm_acc_qts_ << str;
//}


void GTagML_Graph_Build::tag_command_annotation(QString annotation)
{
 //?
 // current_annotation_tile_ = new GTagML_Annotation_Tile("");
 // current_annotation_tile_->set_connector(annotation);
 // caon_ptr<tNode> node = make_new_node(current_annotation_tile_);
 // markup_position_.tag_command_annotation(node);
}

void GTagML_Graph_Build::annotation_entry(QString flag, QString text, QString rel, QString tile, QString follow)
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

void GTagML_Graph_Build::check_tile_acc(Acc_Mode new_mode)
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

void GTagML_Graph_Build::check_add_words()
{
 QStringList strings = tile_acc_.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
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


void GTagML_Graph_Build::mark_attribute_tile()
{
 check_tile_acc(Acc_Mode::Attribute);
}



void GTagML_Graph_Build::attach_left_whitespace()
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

void GTagML_Graph_Build::attach_right_whitespace()
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


void GTagML_Graph_Build::attach_whitespace(QString whitespace)
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
void GTagML_Graph_Build::end_khif_tile(QString connector_prefix, QString connectors)
{
 end_khif_tile();

 bool string_follow = (connector_prefix.startsWith('+'));
 bool subject_claim = (connector_prefix.endsWith('['));
 markup_position_.load_khif_connectors(connectors);

 parse_context_.flags.inside_khif_tile = true;

}

void GTagML_Graph_Build::attach_khif_tile()
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

void GTagML_Graph_Build::end_khif_tile()
{
 //? parse_context.flags.inside_khif_tile = false;
 attach_khif_tile();
 markup_position_.khif_tag_command_leave();
}
#endif //def HIDE


void GTagML_Graph_Build::enter_tag_command_with_predicate_vector(QString tag_command,
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


//void GTagML_Graph_Build::khif_tile_acc(QString m)
//{
// khif_tile_ += m;
//}

void GTagML_Graph_Build::attach_predicate_vector(QString connector_prefix, QString connectors)
{
// attach_khif_tile();
 bool string_follow = (connector_prefix.startsWith('+'));
 bool subject_claim = (connector_prefix.endsWith('['));
 markup_position_.load_khif_connectors(connectors);
}


caon_ptr<GTagML_Tag_Command> GTagML_Graph_Build::html_tag_instruction(QString prefix,
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



caon_ptr<GTagML_Tag_Command> GTagML_Graph_Build::html_tag_command_entry(QString prefix, QString tag_command)
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

void GTagML_Graph_Build::check_non_or_left_wrapped(QString wmi, caon_ptr<GTagML_Tag_Command> gtc)
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

caon_ptr<GTagML_Tag_Command> GTagML_Graph_Build::tag_command_entry(QString wmi, 
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


void GTagML_Graph_Build::html_tag_command_attribute_entry(QString pre_space,
 QString attribute, QString s_or_d)
{
 current_html_attribute_ = attribute;
//? if(s_or_d == "'")
//?  parse_context_.flags.inside_html_tag_attribute_single_quote = true;
//? else if(s_or_d == "\"")
//?  parse_context_.flags.inside_html_tag_attribute_double_quote = true;
}

void GTagML_Graph_Build::html_tag_command_attribute_acc(QString str)
{
 tile_acc_ += str;
}

caon_ptr<GTagML_Attribute_Tile> GTagML_Graph_Build::complete_html_tag_command_attribute()
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

GTagML_Document_Light_Xml* GTagML_Graph_Build::get_light_xml()
{
 return nullptr;
 //return document_info_.light_xml();
}

void GTagML_Graph_Build::multi_arg_transition_to_main_tile()
{
 multi_arg_transition({}, {}, {}, "-->", "=>");
  // // need to mark as main tile somehow ...
}

void GTagML_Graph_Build::multi_arg_transition(QString wmi, QString inner_wmi,
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

void GTagML_Graph_Build::tag_command_entry_with_layer(QString tag_command, QString layer_marker)
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

void GTagML_Graph_Build::tag_command_entry_inside_multi(QString wmi,
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

void GTagML_Graph_Build::tag_command_entry_multi(QString wmi,
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

void GTagML_Graph_Build::tag_command_entry_inline(QString wmi,
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

void GTagML_Graph_Build::gtag_command_entry_inline(QString tag_command,
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


caon_ptr<GTagML_Tag_Command> GTagML_Graph_Build::make_new_tag_command(QString name, QString argument, QString parent_tag_type)
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


caon_ptr<GTagML_Tile> GTagML_Graph_Build::add_tile(QString tile_str)
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

caon_ptr<GTagML_Raw_Tile> GTagML_Graph_Build::add_raw_tile(QString tile_str)
{
  // // current latex is the only raw format ...
 caon_ptr<GTagML_Raw_Tile> tile = make_new_raw_tile(current_raw_format_, tile_str);
 CAON_PTR_DEBUG(GTagML_Raw_Tile ,tile)
 caon_ptr<tNode> node = make_new_node(tile);
 markup_position_.add_raw_tile_node(node);
 return tile;
}

caon_ptr<GTagML_Attribute_Tile> GTagML_Graph_Build::add_attribute_tile(QString tile_str)
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


void GTagML_Graph_Build::add_string_literal_tile(QString str)
{
 //? caon_ptr<GTagML_Tile> tile = add_tile(str);
 //? tile->flags.is_string_literal = true;
}

void GTagML_Graph_Build::tag_command_leave(QString connector_to_load, QString tag_command)
{

}

void GTagML_Graph_Build::block_gtag_command_leave()
{
 if(caon_ptr<tNode> node = markup_position_.tag_command_leave())
 {
  CAON_PTR_DEBUG(tNode ,node)
  check_tile_acc();
  markup_position_.confirm_tag_command_leave(node);
  check_multi_parent_reset();
 }
}

void GTagML_Graph_Build::inline_tag_command_leave()
{
 if(caon_ptr<tNode> node = markup_position_.tag_command_leave())
 {
  CAON_PTR_DEBUG(tNode ,node)
  check_tile_acc();
  markup_position_.confirm_tag_command_leave(node);
  check_multi_parent_reset();
 }
}

void GTagML_Graph_Build::tag_command_leave_multi(QString tag_command)
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

void GTagML_Graph_Build::tag_command_leave()
{
 if(caon_ptr<tNode> node = markup_position_.tag_command_leave())
 {
  CAON_PTR_DEBUG(tNode ,node)
  check_tile_acc();
  markup_position_.confirm_tag_command_leave(node);
  check_multi_parent_reset();
 }
}

void GTagML_Graph_Build::tag_command_instruction_leave(caon_ptr<tNode> node)
{
 check_tile_acc();
 markup_position_.restore_current_node(node);
}

void GTagML_Graph_Build::check_html_tag_command_leave(QString tag_command, QString match_text)
{
 check_tile_acc();

 if(get_light_xml())
 {
  // get_light_xml()->tag_leave(tag_command);
  return;
 }

 markup_position_.rewind_tag_command_leave(tag_command);
}

void GTagML_Graph_Build::check_multi_parent_reset()
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

void GTagML_Graph_Build::check_tag_command_leave(QString tag_command, QString match_text)
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

//void GTagML_Graph_Build::add_string_literal_tile()
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

caon_ptr<GTagML_Attribute_Tile> GTagML_Graph_Build::make_new_attribute_tile(QString tile)
{
 QRegularExpression rx("(\\S+)\\s+(.+)", QRegularExpression::DotMatchesEverythingOption);
 QRegularExpressionMatch rxm = rx.match(tile);
 if(rxm.hasMatch())
 {
  return caon_ptr<GTagML_Attribute_Tile>(
   new GTagML_Attribute_Tile(rxm.captured(1), rxm.captured(2)) );
 }
}

caon_ptr<GTagML_Attribute_Tile> GTagML_Graph_Build::make_new_attribute_tile(QString key, QString value)
{
 return caon_ptr<GTagML_Attribute_Tile>(
   new GTagML_Attribute_Tile(key, value) );
}

caon_ptr<GTagML_Raw_Tile> GTagML_Graph_Build::make_new_raw_tile(QString format, QString value)
{
 return caon_ptr<GTagML_Raw_Tile>(
   new GTagML_Raw_Tile(format, value) );
}

caon_ptr<GTagML_Tile> GTagML_Graph_Build::make_new_tile(QString tile)
{
 return caon_ptr<GTagML_Tile>( new GTagML_Tile(tile) );
}


caon_ptr<GTagML_Paralex_Tile> GTagML_Graph_Build::make_new_paralex_tile(QString tile,
  u1 kind, u1 w_or_a)
{
 return caon_ptr<GTagML_Paralex_Tile>( new GTagML_Paralex_Tile(tile, 
   (GTagML_Paralex_Tile::Kind) kind, w_or_a) );
}


caon_ptr<GTagML_Graph_Build::tNode> GTagML_Graph_Build::make_new_node(caon_ptr<GTagML_Tile> tile)
{
 CAON_PTR_DEBUG(GTagML_Tile ,tile)
 caon_ptr<tNode> result = caon_ptr<tNode>( new tNode(tile) );
 #ifdef RELAE_LABEL_NODES
 result->set_label(tile->thumbnail());
 #endif
 return result;
}

caon_ptr<GTagML_Graph_Build::tNode> GTagML_Graph_Build::make_new_node(caon_ptr<GTagML_Raw_Tile> tile)
{
 CAON_PTR_DEBUG(GTagML_Raw_Tile ,tile)
 caon_ptr<tNode> result = caon_ptr<tNode>( new tNode(tile) );
 #ifdef RELAE_LABEL_NODES
 result->set_label(tile->thumbnail());
 #endif
 return result;
}

void GTagML_Graph_Build::check_nonstandard_special_character_sequence
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

void GTagML_Graph_Build::semantic_mark(QString match_text, QString sem, u1 mode)
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



void GTagML_Graph_Build::special_character_sequence(QString match_text, 
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

caon_ptr<GTagML_Graph_Build::tNode> GTagML_Graph_Build::make_new_node(caon_ptr<GTagML_Annotation_Tile> tile)
{
 caon_ptr<tNode> result = new tNode(tile);
 result->set_label(tile->thumbnail());
 return result;
}

caon_ptr<GTagML_Graph_Build::tNode> GTagML_Graph_Build::make_new_node(caon_ptr<GTagML_Annotation_Tile> tile, QString label)
{
 caon_ptr<tNode> result = new tNode(tile);
 result->set_label(label);
 return result;
}

caon_ptr<GTagML_Graph_Build::tNode> GTagML_Graph_Build::make_new_node(caon_ptr<GTagML_Attribute_Tile> tile)
{
 CAON_PTR_DEBUG(GTagML_Attribute_Tile ,tile)
 caon_ptr<tNode> result = caon_ptr<tNode>( new tNode(tile) );
 #ifdef RELAE_LABEL_NODES
 result->set_label(tile->thumbnail());
 #endif
 return result;
}

caon_ptr<GTagML_Graph_Build::tNode> GTagML_Graph_Build::make_new_node(caon_ptr<GTagML_Paralex_Tile> tile)
{
 CAON_PTR_DEBUG(GTagML_Paralex_Tile ,tile)
 caon_ptr<tNode> result = caon_ptr<tNode>( new tNode(tile) );
 #ifdef RELAE_LABEL_NODES
 result->set_label(tile->thumbnail());
 #endif
 return result;
}

caon_ptr<GTagML_Graph_Build::tNode> GTagML_Graph_Build::make_new_node(caon_ptr<GTagML_Tag_Command> gtc)
{
 CAON_PTR_DEBUG(GTagML_Tag_Command ,gtc)
 caon_ptr<tNode> result = caon_ptr<tNode>( new tNode(gtc) );
 #ifdef RELAE_LABEL_NODES
 result->set_label(gtc->name());
 #endif
 return result;
}

void GTagML_Graph_Build::attribute_sequence_leave()
{
 check_tile_acc();
 parse_context_.flags.inside_attribute_sequence = false;
 parse_context_.flags.inside_multi_generic = false;
 markup_position_.attribute_sequence_leave();
}

void GTagML_Graph_Build::tag_body_leave(QString match)
{
 check_tile_acc();
 markup_position_.tag_body_leave();
 parse_context_.flags.inside_tag_body = false;
 if(match == "/>")
  tag_command_leave();
}

void GTagML_Graph_Build::enter_special_parse_mode(QString spm)
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

void GTagML_Graph_Build::leave_special_parse_mode(QString spm)
{
 if(prior_parsing_modes_.isEmpty())
   current_parsing_mode_ = GTagML_Parsing_Modes::Parse_Error;
 else
 {
  current_parsing_mode_ = prior_parsing_modes_.top().first;
  check_tile_acc(prior_parsing_modes_.pop().second);
 }
}

void GTagML_Graph_Build::special_parse_mode_acc(QString text)
{
 tile_acc(text);
}

void GTagML_Graph_Build::html_tag_body_leave(QString prefix)
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

