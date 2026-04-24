
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef GTAGML_PARSE_CONTEXT__H
#define GTAGML_PARSE_CONTEXT__H

#include "relae-graph/relae-parser.h"

#include "flags.h"

#include "kans.h"
KANS_(GTagML)


class GTagML_Graph;
class GTagML_Node;


class GTagML_Parse_Context
{
 typedef GTagML_Node tNode;

public:
 flags_(4)
  flag_(1, auto_paragraph_mode)
  flag_(2, italics_mode)
  flag_(3, double_quote_mode)
  flag_(4, single_quote_mode)

  flag_(5, emph_sample_mode)
  flag_(6, short_macro_mode)

  flag_(7, emph_highlight_mode)
  flag_(8, acronym_mode)
  flag_(9, emph_italics_mode)

//?  flag_(9, code_display_mode)

  flag_(10, underline_mode)

  //?flag_(11, strikethrough_mode)

  flag_(11, read_desc_label)

  flag_(12, read_parens_as_label)
  flag_(13, read_parens_as_ref)

  flag_(14, read_numbered_items)
  flag_(15, read_bulleted_items)
  flag_(16, ignore_blank_lines)

  flag_(17, latex_only)
  flag_(18, latex_only_to_space)

  flag_(19, sentences_only)

  flag_(20, heading_acc)
  flag_(21, justline)

  flag_(22, block_float_mode)

  flag_(23, single_quote_mode_doubled)

  flag_(24, single_quote_mode_trebled)


//  flag_(14, inside_tag_body)
//  flag_(14, inside_html_tag_body)

//  flag_(22, inside_html_script_tag)

  flag_(25, inside_tag_command_entry)


  flag_(26, inside_multi_parent_semis)
  flag_(27, inside_multi_generic)
  flag_(28, inside_attribute_sequence)

  flag_(29, inside_html_tag_body)


  flag_(30, inside_tag_body)


  flag_(31, inside_multi_parent)
  flag_(32, inside_inline_multi_parent)
//  flag_(33, inside_multiline_comment)


 _flags_

public:

 GTagML_Parse_Context();

};

_KANS(GTagML)

#endif
