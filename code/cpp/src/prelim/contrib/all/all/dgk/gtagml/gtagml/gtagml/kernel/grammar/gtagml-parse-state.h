
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef GTAGML_PARSE_STATE__H
#define GTAGML_PARSE_STATE__H

#include "kernel/graph/gtagml-markup-position.h"
#include "kernel/grammar/gtagml-parse-context.h"

#include "kernel/gtagml-dominion.h"

#include "kernel/grammar/gtagml-parsing-mode.h"

#include "global-types.h"

#include "accessors.h"

#include "gtagml-streams.h"


#include <QXmlStreamWriter>
#include <QBuffer>

#include "textio.h"


#include "kans.h"


KANS_(GTagML)

class GTagML_Parser;
class GTagML_Tag_Command;
class GTagML_Attribute_Tile;
class GTagML_Paralex_Tile;
class GTagML_Raw_Tile;
class GTagML_Document_Info;
class GTagML_Annotation_Tile;

class GTagML_Document_Light_Xml;


class GTagML_Parse_State
{
 flags_(4)
  bool math_mode:1;
  bool skip_command_node_insert:1;
  bool active_annotation_entry:1;
  bool active_annotation_tile:1;
  bool active_attribute_sequence:1;
  bool latex_only:1;
  bool xml_only:1;
  bool sentences_only:1;
  bool sentences_latex_filter:1;
  bool just_ended_sentence:1;
  bool await_paragraph_start:1;
  bool await_sentence_start:1;
  bool use_latex_sdi_all_markers:1;
  bool use_latex_sdi_paragraph_markers:1;
  bool heading_acc:1;
  bool in_pa_1:1;
  bool in_ql:1;
 _flags

 enum class Acc_Mode {
  Main_Tile, Attribute, Math, Arg_Tile, Raw
 };

 Acc_Mode acc_mode_;

 enum Tag_Body_Follow_Mode {
  Tag_Body_Follow_Mode_Not_Recognized, Normal, Region, Empty
 };

 static Tag_Body_Follow_Mode parse_tag_body_follow(QString str)
 {
  static QMap<QString, Tag_Body_Follow_Mode> static_map {{
   {".",  Normal}, {",",  Region}, {";", Empty},
    // for gtags ...
    {">", Normal}
  }};
  return static_map.value(str, Tag_Body_Follow_Mode_Not_Recognized);
 }

 GTagML_Streams streams_;

 GTagML_Parser* parser_;

 GTagML_Parsing_Modes current_parsing_mode_;

 QStack<QPair<GTagML_Parsing_Modes, Acc_Mode>> prior_parsing_modes_;

 GTagML_Parse_Context parse_context_;

 GTagML_Markup_Position markup_position_;

 QString current_raw_format_;


 static inline int cutmax(int x)
 {
  if(x > 6)
   return 6;
  return x;
 }

 typedef GTagML_Node tNode;

 u4 tile_acc_length_adjustment_;
 QString tile_acc_;
 QTextStream tile_acc_qts_;

 QString string_literal_acc_;
 QTextStream string_literal_acc_qts_;

 QString current_html_attribute_;

//? QString khif_tile_;

 GTagML_Document_Info& document_info_;

 caon_ptr<GTagML_Tile> add_tile(QString tile_str);
 caon_ptr<GTagML_Attribute_Tile> add_attribute_tile(QString tile_str);
 caon_ptr<GTagML_Raw_Tile> add_raw_tile(QString tile_str);

 void add_string_literal_tile();

 GTagML_Document_Light_Xml* get_light_xml();

 QString quote_acc_;

 u1 held_semantic_mark_mode_;


 QString held_macro_string_;

 struct Nesting_Codes {

  static constexpr u1 Signal_Default = 255;
  static constexpr u1 Continue_At_End = 16;
  static constexpr u1 Continue_At_Start = 32;

 };

 QMap<u1, u1> nesting_codes_by_depth_;

 u1 sentence_nesting_depth_;

 QString sentences_sdi_;
 QTextStream sentences_sdi_stream_;

 QString sentences_text_;
 QTextStream sentences_text_stream_;

 QString sentence_gaps_;
 QTextStream sentence_gaps_stream_;

 QString sentences_section_heading_;
 QTextStream sentences_section_heading_stream_;

 QString latex_section_heading_;
 QTextStream latex_section_heading_stream_;

 QPair<u1, u1> heading_counts_;

 QString line_and_column_string_tight(u4 pos);
 QString line_and_column_string(u4 pos);

 QString line_and_column_string_tight();
 QString line_and_column_string();

 QString lines_to_string(QString text)
 {
  QString result = text;

  if(!result.startsWith('\n'))
    result.prepend('\n');

  result.replace('\n', "\n|  ");

  s4 ix = result.lastIndexOf("\n|  ");
  result[ix + 1] = '.';

  return result;
 }

 QString sentence_gaps_to_string()
 {
  return lines_to_string(sentence_gaps_);
 }

 QString sentences_text_to_string()
 {
  return lines_to_string(sentences_text_);
 }

 QMap<u1, u2> section_ids_;
 u4 sentence_id_;
 u4 paragraph_id_;

 u4 footnote_id_;



 QMap<u1, u2> current_section_counts_;

 u2 current_paragraph_count_;
 u2 current_paragraph_bridge_;

 u2 current_exs_group_number_;
 u2 current_exs_number_;
 u2 current_exs_offset_;

 u2 current_slash_line_plus_count_;

 QPair<u2, u2> pseudo_paragraph_counts_;

 QStringList ref_labels_;

 u2 fn_count_;

 void set_paragraph_bridge();

 enum class Paragraph_Types {
  N_A, Abstract, P0, P1, Block_Quote,
  Endnote_Block_Quote,
  Paragraph_Addendum
 };

 Paragraph_Types current_paragraph_type_;
 QStack<Paragraph_Types> held_paragraph_types_;

 u2 current_section_number_;
 u2 current_item_count_;

// QMap<QString, u2> paragraph_cmd_counts_;

public:


 GTagML_Parse_State(GTagML_Graph& g, GTagML_Document_Info& document_info);

 ACCESSORS__RGET(GTagML_Parse_Context ,parse_context)
 ACCESSORS(GTagML_Parsing_Modes ,current_parsing_mode)

 ACCESSORS(u4 ,sentence_id)

 void init(GTagML_Parser* parser);

 QString current_paragraph_type_to_string()
 {
  switch (current_paragraph_type_)
  {
  case Paragraph_Types::Abstract:
    return "Abstract";
  case Paragraph_Types::Block_Quote:
    return "bq";
  case Paragraph_Types::Endnote_Block_Quote:
    return "nbq";
  case Paragraph_Types::N_A:
    return "N/A";
  case Paragraph_Types::P0:
    return "P0";
  case Paragraph_Types::P1:
    return "P1";

  default: return "?";
  }

 }


 void end_sentence(QString punctuation,
   u1 nesting_code = Nesting_Codes::Signal_Default,
   QVector<QPair<QString, QString>> supplements = {});

 void end_sentence(QString punctuation,
   QVector<QPair<QString, QString>> supplements)
 {
  end_sentence(punctuation, Nesting_Codes::Signal_Default, supplements);
 }

 void end_sentence(QVector<QPair<QString, QString>> supplements)
 {
  end_sentence("", Nesting_Codes::Signal_Default, supplements);
 }

 void end_sentence(u1 nesting_code,
   QVector<QPair<QString, QString>> supplements = {})
 {
  end_sentence("", nesting_code, supplements);
 }

 void end_sentence()
 {
  end_sentence("", Nesting_Codes::Signal_Default, {});
 }

 void pseudo_paragraph();

 void prepare_bibliography();

 void noindent_marker();

 void footnote_marker(QString text);

 void ell_count(u1 count, QString follow = "");
 void ell_2_nonbreak();

 void desc_item(QString text);
 void desc_item_with_multiline_label(QString text);

 void latex_command_via_semantic_annotation(QString concept, QString text);

 void hyperlink_1(QString text);
 void hyperlink_2(QString text, QString link);

 void exs_item(u2 number, QString text);
 void paren_ref(u2 number, QString text);
 void paren_ref_global(u2 number, QString text);

 void enums_item(u2 number, QString text, QString follow);
 void bulleted_item(QString symbol, QString supp);

 void latex_command_auto_closed(QString command_name, QString arg);
 void citation(QString full_match, QString command_name, QString arg);


 void blank_line_as_visible_space();
 void check_blank_line();


 void single_slash_line();
 void single_slash_line_plus();

 void enter_subparagraph(QString text, QString sup = {});

 void enter_footnote(QString pretext, QString space);
 void leave_footnote(QString pretext, QString space);

 void enter_sentences_latex_filter(QString pretext);
 void leave_sentences_latex_filter(QString pretext);

 void force_switch_sentence();

 void enter_sentences_only(QString open, QString pre_space);
 void leave_sentences_only(QString close, QString post_space);

 void enter_latex_only(QString match);
 void leave_latex_only(QString match);

 void enter_latex_only_to_space(QString match, QString space);

 void enter_latex_only_to_space(QString match);
 void leave_latex_only_to_space(QString match);

 void latex_only(QString text);

 void enter_special_section(QString text);
 void enter_abstract();


 void primary_acc(QString text);
 void reset_primary();

 void close_paragraph();
 void check_close_paragraph();

 void enter_auto_paragraph_mode();

 void section_heading(QString stext, QString ltext, u1 level);
// void subsection_heading(QString stext, QString ltext);

 void enter_heading(u1 count1, u1 count2);

 void heading(u1 count, QString stext, QString ltext);
 void heading(u1 count1, u1 count2, QString stext, QString ltext);

 void leave_heading();

 void enter_justline(QString pretext);
 void leave_justline(QString pretext, QString follow);

 void end_document();
 void end_inner_document();

 void emph_symbolic(QString text);
 void emph_acronym(QString text);

 void auto_new_paragraph(QString cmd);
 void auto_new_paragraph();

 void enter_block_float_mode();
 void leave_block_float_mode();

 void short_macro(QString text, u1 size);
 void short_acronym(QString text, u1 size);
 void short_emph_sample(QString text);

 void enter_short_macro_mode(u1 size);
 void leave_short_macro_mode();

 void enter_highlight_mode();
 void leave_highlight_mode();

 void enter_sample_mode();
 void leave_sample_mode();

 void enter_italics_mode();
 void leave_italics_mode();
 void enter_double_quote_mode(QString pre);
 void leave_double_quote_mode();
 void enter_single_quote_mode();
 void leave_single_quote_mode();
 void enter_single_quote_mode_doubled();
 void leave_single_quote_mode_doubled();
 void enter_single_quote_mode_trebled();
 void leave_single_quote_mode_trebled();
 void enter_acronym_mode(u1 size);
 void leave_acronym_mode();
 void enter_emph_italics_mode(QString mid);
 void leave_emph_italics_mode();

 void special_character_sequence(QString text);
 void show_latex();

 void add_string_literal_tile(QString str);
 void enter_multiline_comment(QString semis, QString tildes);
 void check_leave_multiline_comment(QString semis, QString tildes);

 void tag_command_leave();
 void tag_command_leave_multi(QString tag_command);

 void inline_tag_command_leave();
 void block_gtag_command_leave();


 caon_ptr<GTagML_Tag_Command> tag_command_entry(QString wmi, QString prefix, QString tag_command, QString argument, QString parent_tag_type = QString());
 void tag_command_instruction_leave(caon_ptr<tNode> node);

 void attribute_sequence_leave();
 void tag_body_leave(QString match = QString());

 void call_leave();

 void tile_acc(QString str);
 void check_tile_acc(Acc_Mode new_mode = Acc_Mode::Main_Tile);

 void check_non_or_left_wrapped(QString wmi, caon_ptr<GTagML_Tag_Command> ntc);

 void html_tag_body_leave(QString prefix);

 void enter_special_parse_mode(QString spm);
 void leave_special_parse_mode(QString spm);
 void special_parse_mode_acc(QString spm);

//? void khif_tile_acc(QString m);

 caon_ptr<GTagML_Tag_Command> html_tag_command_entry(QString prefix, QString tag_command);

 void attach_whitespace(QString whitespace);
 void attach_left_whitespace();
 void attach_right_whitespace();

//? void end_khif_tile(QString connector_prefix, QString connectors);
//? void end_khif_tile();
//? void attach_khif_tile();

 void attach_predicate_vector(QString connector_prefix, QString connectors);

 void tag_command_leave(QString connector_to_load, QString tag_command);

 void check_tag_command_leave(QString tag_command, QString match_text);
 void check_html_tag_command_leave(QString tag_command, QString match_text);

 void html_tag_command_attribute_acc(QString str);

 void html_tag_command_attribute_entry(QString pre_space, QString attribute, QString s_or_d);

 void check_add_words();

 void enter_tag_command_with_predicate_vector(QString tag_command,
  QString connector_prefix, QString connectors);

 caon_ptr<GTagML_Tag_Command> html_tag_instruction(QString prefix,
  QString tag_command, QString argument);

 void tag_command_annotation(QString annotation);

 void annotation_entry(QString flag, QString text, QString rel, QString tile, QString follow);

 void check_multi_parent_reset();

 void multi_arg_transition_to_main_tile();

 void multi_arg_transition(QString wmi, QString inner_wmi, QString fiat,
   QString arg_marker, QString carried_arg_marker = {});

 caon_ptr<GTagML_Attribute_Tile> complete_html_tag_command_attribute();

 caon_ptr<GTagML_Attribute_Tile> make_new_attribute_tile(QString key, QString value);

 caon_ptr<GTagML_Raw_Tile> make_new_raw_tile(QString format, QString value);

 void tag_command_entry_with_layer(QString tag_command, QString layer_marker);

 void tag_command_entry_multi(QString wmi,
   QString inner_wmi, QString tag_command,
   QString tag_body_follow, QString fiat, QString first_arg_wmi, QString first_arg_marker);

 void tag_command_entry_inside_multi(QString wmi, QString inner_wmi,
   QString fiat,  QString tag_command, QString arg_marker,
   QString* carried_arg_marker = nullptr, QString argument = QString(), QString name = QString());

 void tag_command_entry_inline(QString wmi, QString inner_wmi,
   QString fiat, QString tag_command,
   QString tag_body_follow, QString argument = QString());

 void gtag_command_entry_inline(QString tag_command,
   QString tag_body_follow);



 void mark_attribute_tile();

 void special_character_sequence(QString match_text, QString esc, u1 mode);

 void semantic_mark(QString match_text, QString esc, u1 mode);

 void check_nonstandard_special_character_sequence(QString match_text, QString& esc,
   u1& mode, QString& sup_text);

   //QString bq_trig, QString bq_esc, QString pipe);

 caon_ptr<GTagML_Attribute_Tile> make_new_attribute_tile(QString tile);
 caon_ptr<GTagML_Tile> make_new_tile(QString tile);
 caon_ptr<GTagML_Tag_Command> make_new_tag_command(QString name, 
QString argument, QString parent_tag_type = QString());
 caon_ptr<GTagML_Paralex_Tile> make_new_paralex_tile(QString tile, 
   u1 kind, u1 w_or_a);

 caon_ptr<tNode> make_new_node(caon_ptr<GTagML_Attribute_Tile> tile);

 caon_ptr<tNode> make_new_node(caon_ptr<GTagML_Raw_Tile> tile);

 caon_ptr<tNode> make_new_node(caon_ptr<GTagML_Tile> tile);
 caon_ptr<tNode> make_new_node(caon_ptr<GTagML_Tag_Command> ntc);
 caon_ptr<tNode> make_new_node(caon_ptr<GTagML_Paralex_Tile> tile);

 caon_ptr<tNode> make_new_node(caon_ptr<GTagML_Annotation_Tile> tile);
 caon_ptr<tNode> make_new_node(caon_ptr<GTagML_Annotation_Tile> tile, QString label);





 void parse_processing_instruction(QString instruction, QString lrcode);

 void outer_tag_command_entry(QString outer, QString pre, QString main,
   QStringVector supl, QString post);


};

_KANS(GTagML)

#endif //  GTAGML_PARSE_STATE__H
