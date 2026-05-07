
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "gtagml-grammar.h"

#include "kernel/grammar/gtagml-parse-context.h"

#include "kernel/grammar/gtagml-parse-state.h"

#include "kernel/grammar/gtagml-parser.h"

#include "relae-graph/relae-parser.templates.h"

#include "global-types.h"

USING_KANS(GTagML)

GTagML_Grammar::GTagML_Grammar()
{
}

void GTagML_Grammar::init(GTagML_Parser& p, GTagML_Graph& g, GTagML_Parse_State& parse_state)
{
 pre_rule( "script-word", "(?:[^{}()\\[\\]\\s`;,:]|(?:\\w::?\\w))+" );
 pre_rule( "valid-tag-command-name", "[\\w@](?:[^{}()\\[\\]<>/\\s`;,.:]|(?:\\w+::?\\w))*" );
 pre_rule( "space-to-end-of-line", "[__\\t]* \\n" );
 pre_rule( "end-of-line", "[__\\t\\S]* \\n" );
 pre_rule( "single-space", "[__\\t]" );
// pre_rule( "single-dot", "[.]" );

 pre_rule( "blank-line-content", "[__\\t]* \\n" );

 pre_rule( "blank-line", " \\n [__\\t]* " );

 pre_rule( "blank-lines", " \\s*?\\n\\s* " );

 pre_rule( "tag-command-wrap-mode-indicator", ": :? \\.?" );

 Context gtagml_context = add_context("gtaml-context");
 //Context* raw_context = new Context( add_context("raw-context") );
 Context raw_context = add_context("raw-context");
 Context html_context = add_context("html-context");

 Context comment_context = add_context("comment-context");

 Context gtagml_or_html_context = add_context("gtaml-or-html",
   {gtagml_context, html_context});


 track_context({&gtagml_context, &raw_context,
   &gtagml_or_html_context, &html_context, &comment_context});

 switch(parse_state.current_parsing_mode())
 {
 case GTagML_Parsing_Modes::HTML:
  activate(gtagml_or_html_context);
  break;

 case GTagML_Parsing_Modes::Raw:
  activate(raw_context);
  break;

 case GTagML_Parsing_Modes::GTagML:
  activate(gtagml_context);
  break;
 }


 GTagML_Parse_Context& parse_context = parse_state.parse_context();

// size_t test;


 add_rule( gtagml_context, "left-right-mid-processing-instruction",
   " \\( (?<left> <*) (?<left-dash> -+) (?<instruction> [^-]*) (?<right-dash> -+) (?<right> >*) \\) "
   ,[&] //raw_context, &parse_state, this, &p]
 {
  QString lrcode = QString::number(p.matched("left").size())
    + QString::number(p.matched("left-dash").size())
    + QString::number(p.matched("right-dash").size())
    + QString::number(p.matched("right").size());

  QString instruction = p.matched("instruction");
  parse_state.parse_processing_instruction(instruction, lrcode);
 });


 add_rule( gtagml_context, "tag-command-enter",
   " (?<outer> [[{(] )  (?<pre> [^\\s\\[\\]`]*) ` (?<main> [^\\s\\[\\]`,;.]+)"
   " (?*supl* (?: ` (/- [^\\s\\[\\]`,;.]+ -/) )* ) "
   " (?<post> [,;.]+ )"
   ,[&] //raw_context, &parse_state, this, &p]
 {
  QString outer = p.matched("outer");
  QString pre = p.matched("pre");
  QString main = p.matched("main");

  QStringVector supl = p.rematched("supl");

  QString post = p.matched("post");

  parse_state.outer_tag_command_entry(outer, pre, main, supl, post);

 });















 add_rule( gtagml_context, "enter-multi-line-comment",
   " ; ;+ (?<tail> [~-]{2,}) "
   ,[comment_context, this, &p] //raw_context, &parse_state, this, &p]
 {
  QString tail = p.matched("tail");
  activate_with_depth_mark(comment_context, tail.length());
 });

 add_rule( gtagml_context, "single-line-comment",
   " ;{2,} [~=-] [^\\n]* \\n "
   ,[] //raw_context, &parse_state, this, &p]
 {
 });


 add_rule( gtagml_context, "latex-command-via-annotation",
   " @\\[(?<concept> \\w+):(?<text> [^\\]]+)\\] "
   ,[&]
 {
  QString concept = p.matched("concept");
  QString text = p.matched("text");

  parse_state.latex_command_via_semantic_annotation(concept, text);
 });


 add_rule( comment_context, "leave-multi-line-comment",
   " (?<tail> [~-]{2,}) ; ;+  "
   ,[gtagml_context, comment_context, this, &p] //raw_context, &parse_state, this, &p]
 {
  QString tail = p.matched("tail");
  check_activate_with_depth_mark(gtagml_context, comment_context, tail.length());
 });

 add_rule( flags_all_(parse_context ,emph_italics_mode),
   gtagml_context, "leave-emph-italics-mode",
   " /\\\\ (?=/) "
   ,[&]
 {
  parse_state.leave_emph_italics_mode();
 });

 add_rule( flags_all_(parse_context ,emph_italics_mode),
   gtagml_context, "leave-emph-italics-mode",
   " / (?!=/) "
   ,[&]
 {
  parse_state.leave_emph_italics_mode();
 });

 add_rule( gtagml_context, "enter-emph-italics-mode",
   " & (?<mid> =?) / "
   ,[&]
 {
  parse_state.enter_emph_italics_mode(p.matched("mid"));
 });


 add_rule( gtagml_context, "noindent-marker",
   " (?<! \\S) --- (?= \\S) "
   ,[&]
 {
  parse_state.noindent_marker();
 });


 add_rule( gtagml_context, "ell-count-restrict-space",
   " [.]{3} (?= \\s) "
   ,[&]
 {
  parse_state.ell_count(p.match_text().size(), "\\");
 });

 add_rule( gtagml_context, "ell-2-nonbreak",
   " [.]{2} (?= [^\\s.]) "
   ,[&]
 {
  parse_state.ell_2_nonbreak();
 });

 add_rule( gtagml_context, "ell-count",
   " [.]{2,} "
   ,[&]
 {
  parse_state.ell_count(p.match_text().size());
 });


 add_rule( gtagml_context, "single-slash-and-consume-blank-lines",
  " \n (?<first> /\\+) \\s* (?=\\n) "
  ,[&]
 {
  parse_state.single_slash_line_plus();
 });

 add_rule( gtagml_context, "single-slash-line",
  " \n (?<first> /) .single-space.* (?=\\n) "
  ,[&]
 {
  parse_state.single_slash_line();
 });

// add_rule( gtagml_context, "single-slash-line",
//  " \n (?<first> /) .single-space.* (?=\\n) "
//  ,[&]
// {
//  parse_state.single_slash_line();
// });

 add_rule( gtagml_context, "enter-latex-only-to-space--leave-space",
  " .single-space.* -%>> (?<sp> .single-space.*)  "
  ,[&]
 {
  parse_state.enter_latex_only_to_space(p.matched("sp"));
 });


 add_rule( gtagml_context, "enter-latex-only-to-space",
  " .single-space.* (?<m> -%>) .single-space.*  "
  ,[&]
 {
  parse_state.enter_latex_only_to_space(p.matched("m"));
 });



 add_rule( flags_all_(parse_context ,latex_only_to_space),
  gtagml_context, "leave-latex-only-to-space--consume-space",
  " (?<m> _) (?= \\s) "
  ,[&]
 {
  parse_state.leave_latex_only_to_space(p.matched("m"));
 });


 add_rule( flags_all_(parse_context ,latex_only_to_space),
  gtagml_context, "leave-latex-only-to-space",
  " .single-space.+ (?=\\S) "
  ,[&]
 {
  parse_state.leave_latex_only_to_space("");
 });

 add_rule( flags_all_(parse_context ,latex_only_to_space),
  gtagml_context, "leave-latex-only-to-space-at-newline",
  " .single-space.* (?=\\n) "
  ,[&]
 {
  parse_state.leave_latex_only_to_space("");
 });


// add_rule( gtagml_context, "immediate-latex-only",
//  " -%>> (?<text>\\S+) "
//  ,[&]
// {
//  QString m = p.matched("text");
//  parse_state.latex_only(m);
// });




 add_rule( gtagml_context, "enter-latex-only--leave-space",
  " (?<m> <<-%) \\s* "
  ,[&]
 {
  parse_state.enter_latex_only(p.matched("m"));
 });

 add_rule( gtagml_context, "enter-latex-only",
  " \\s+ (?<m> <-%) \\s* "
  ,[&]
 {
  parse_state.enter_latex_only(p.matched("m"));
 });

 add_rule( flags_all_(parse_context ,latex_only),
   gtagml_context, "leave-latex-only--leave-space",
   " \\s* (?<m> %->>) "
   ,[&]
 {
  parse_state.leave_latex_only(p.matched("m"));
 });


 add_rule( flags_all_(parse_context ,latex_only),
   gtagml_context, "leave-latex-only",
   " \\s* (?<m> %->) \\s* "
   ,[&]
 {
  parse_state.leave_latex_only(p.matched("m"));
 });




 add_rule( gtagml_context, "footnote-marker",
   " \\\\ (?<number> \\d+) (?= [\\s)%]) "
   ,[&]
 {
  parse_state.footnote_marker(p.matched("number"));
 });


 add_rule( gtagml_context, "force-switch-sentence",
  " => \\s "
  ,[&]
 {
  parse_state.force_switch_sentence();
 });

 add_rule( gtagml_context, "enter-sentences-latex-filter",
  " (?<pretext> [~-]+) > (?= [\\\\$]) "
  ,[&]
 {
  parse_state.enter_sentences_latex_filter(p.matched("pretext"));
 });

 add_rule( gtagml_context, "leave-sentences-latex-filter",
  " (?<= [}$]) < (?<pretext> [~-]+) "
  ,[&]
 {
  parse_state.leave_sentences_latex_filter(p.matched("pretext"));
 });


 add_rule( gtagml_context, "enter-sentences-only",
  " (?<pre-space> (?: .single-space.* \\n .single-space.*) | "
  "  (?: .single-space.+ ) ) "
  " (?<open> <{0,2}) \\{ (?![[%<>-]) "
  ,[&]
 {
  parse_state.enter_sentences_only(p.matched("open"), p.matched("pre-space"));
 });

 add_rule( flags_all_(parse_context ,sentences_only),
   gtagml_context, "leave-sentences-only",
   " \\} (?<close> >{0,2}) "
   " (?<post-space> .single-space.* \\n? .single-space.*) "
   ,[&]
 {
  parse_state.leave_sentences_only(p.matched("close"), p.matched("post-space"));
 });

 add_rule( flags_all_(parse_context ,read_parens_as_label),
   gtagml_context, "exs-blank-line",
   " (?: \\n \\s*){2,} "
   ,[&]
 {
  parse_state.blank_line_as_visible_space();
 });

 add_rule( flags_all_(parse_context ,read_parens_as_ref),
   gtagml_context, "paren-ref",
   " \\( (?<number> \\d+ ) (?<text> \\S*) \\) "
   ,[&]
 {
  parse_state.paren_ref(p.matched("number").toShort(), p.matched("text"));
 });

 add_rule( flags_all_(parse_context ,read_parens_as_ref),
   gtagml_context, "paren-ref-global",
   " \\(! (?<number> \\d+ ) (?<text> \\S*) \\) "
   ,[&]
 {
  parse_state.paren_ref_global(p.matched("number").toShort(), p.matched("text"));
 });


 add_rule( gtagml_context, "enter-justline",
   "  \\{% (?<pretext> -+ ) "
   ,[&]
 {
  parse_state.enter_justline(p.matched("pretext"));
 });


 add_rule( flags_all_(parse_context ,justline),
   gtagml_context, "leave-justline",
   " (?<pretext> \\s* [|-]+) % (?<follow> \\s* = \\d [\\d.]* )? \\s* \\}  "
   ,[&]
 {
  parse_state.leave_justline(p.matched("pretext"), p.matched("follow"));
 });


 add_rule( flags_all_(parse_context ,heading_acc),
   gtagml_context, "leave-heading",
   " (?= \\n) "
   ,[&]
 {
  parse_state.leave_heading();
 });

 add_rule( gtagml_context, "slashes",
  " .space-to-end-of-line.+ (?<first> /+) .single-space.+ (?<second> /*) "
  " .single-space.* " // (?<text> [^\\n]*) "
  ,[&]
 {
  QString first = p.matched("first");
  QString second = p.matched("second");
  // QString text = p.matched("text");

  parse_state.enter_heading(first.size(), second.size());

 });

// single-dot

 add_rule( gtagml_context, "end-sentence",
   " (?<punctuation> [!?.]) (? (?=\\s{2,}) | (?= \\n) )"
//   " (?<punctuation> [!?:.]) (\\s{2,} | \\n)"
   ,[&]
 {
  parse_state.end_sentence(p.matched("punctuation"));
 });

 add_rule( gtagml_context, "manual-end-sentence",
   "\\\\[.]/"
   ,[&]
 {
  parse_state.end_sentence();
 });

// -.>

 add_rule( gtagml_context, "pseud-paragraph",
   " - \\. > "
   ,[&]
 {
  parse_state.pseudo_paragraph();
 });

 add_rule( gtagml_context, "special-section",
   " .blank-lines. %\\. \\s* (?<text> \\S+) \\s+"
   ,[&]
 {
  parse_state.enter_special_section(p.matched("text"));
 });

 add_rule( gtagml_context, "subparagraph",
   " .blank-lines.  %\\/ \\s* (?<text> \\S+) (?:\\s* = (?<sup>\\S+))? \\s+"
   ,[&]
 {
  parse_state.enter_subparagraph(p.matched("text"), p.matched("sup"));
 });

 add_rule( gtagml_context, "enter-auto-paragraph-mode",
   " />> "
   ,[&]
 {
  parse_state.enter_auto_paragraph_mode();
 });

 add_rule( flags_all_(parse_context ,read_parens_as_label),
   gtagml_context, "exs-item",
   " \\( (?<number> \\d+) (?<text> \\S*) \\) "
   ,[&]
 {
  u2 number = p.matched("number").toShort();
  QString text = p.matched("text");
  parse_state.exs_item(number, text);
 });

 add_rule( flags_all_(parse_context ,read_desc_label),
   gtagml_context, "desc-item",
   " \\[\\| (?<text> [^|]+) \\|\\] "
   ,[&]
 {
  QString text = p.matched("text");
  parse_state.desc_item(text);
 });

 add_rule( flags_all_(parse_context ,read_desc_label),
   gtagml_context, "desc-item-with-multiline-label",
   " \\[; (?<text> [^;]+) ;\\] "
   ,[&]
 {
  QString text = p.matched("text");
  parse_state.desc_item_with_multiline_label(text);
 });

 add_rule( flags_all_(parse_context ,ignore_blank_lines),
   gtagml_context, "consume-blank-line",
//?   " (?<=\\n) .blank-line-content. "
   " .single-space.* \\n .blank-line-content. "
   ,[&]
 {
  parse_state.check_blank_line();
//  parse_state.show_latex();
 });

 add_rule( flags_all_(parse_context ,read_numbered_items),
   gtagml_context, "enums-item",
   " (?<= \\n) (?<number> \\d+) (?<text> \\S*) (?<follow> [).]) "
   ,[&]
 {
  u2 number = p.matched("number").toShort();
  QString text = p.matched("text");
  QString follow = p.matched("follow");

  parse_state.enums_item(number, text, follow);
 });

 add_rule( flags_all_(parse_context ,read_bulleted_items),
   gtagml_context, "bulleted-item",
   " (?<= \\s) (?<symbol> \\*+) (?<supp> [^*/\\s]*) (?=\\s) "
   ,[&]
 {
  QString symbol = p.matched("symbol");
  QString supp = p.matched("supp");
  parse_state.bulleted_item(symbol, supp);
 });


 add_rule( flags_all_(parse_context ,auto_paragraph_mode),
   gtagml_context, "auto-new-paragraph",
   " \\s* \\n .single-space.* \\n"
   ,[&]
 {
  parse_state.auto_new_paragraph();
 });


 add_rule( gtagml_context, "enter-italics-mode",
   " \\*/ "
   ,[&]
 {
  parse_state.enter_italics_mode();
 });

 add_rule( flags_all_(parse_context ,italics_mode),
   gtagml_context, "leave-italics-mode",
   " /\\* "
   ,[&]
 {
  parse_state.leave_italics_mode();
 });


 add_rule( gtagml_context, "enter-block-float-mode",
   " \\s* \\[-> "
   ,[&]
 {
  parse_state.enter_block_float_mode();
 });

 add_rule( flags_all_(parse_context ,block_float_mode),
   gtagml_context, "leave-block-float-mode",
   " \\s* ->\\] "
   ,[&]
 {
  parse_state.leave_block_float_mode();
 });


 add_rule( gtagml_context, "latex-command-auto-closed",
   " ` (?<cmd-name> \\w+) (?: < (?<arg> [^>]+) > )?; "
   ,[&]
 {
  parse_state.latex_command_auto_closed(p.matched("cmd-name"),
    p.matched("arg"));
 });

 add_rule( gtagml_context, "citation",
   " \\[/ (?<label> [^:;*/]+) (?: (?<locator> [^/]*) )? /\\] "
   ,[&]
 {
  parse_state.citation(p.match_text(), p.matched("label"), p.matched("locator"));
 });

 add_rule( gtagml_context, "enter-footnote",
   " \\{< (?<pretext> [~_-]*) (?<space> \\s+)  "
   ,[&]
 {
  parse_state.enter_footnote(p.matched("pretext"), p.matched("space"));
 });

 add_rule( gtagml_context, "leave-footnote",
   " (?<space> \\s+) (?<pretext> [~_-]*) >\\} "
   ,[&]
 {
  parse_state.leave_footnote(p.matched("pretext"), p.matched("space"));
 });

 add_rule( flags_all_(parse_context ,acronym_mode),
   gtagml_context, "leave-acronym-mode",
   " / "
   ,[&]
 {
  parse_state.leave_acronym_mode();
 });

 add_rule( flags_all_(parse_context ,short_macro_mode),
   gtagml_context, "leave-short-maco-mode",
   " / "
   ,[&]
 {
  parse_state.leave_short_macro_mode();
 });

 add_rule( flags_all_(parse_context ,emph_sample_mode),
   gtagml_context, "leave-sample-mode",
   " / ; (?! ;) "
   ,[&]
 {
  parse_state.leave_sample_mode();
 });

 add_rule( flags_all_(parse_context ,emph_sample_mode),
   gtagml_context, "leave-sample-mode",
   " / (?! \\w) "
   ,[&]
 {
  parse_state.leave_sample_mode();
 });

 add_rule( flags_all_(parse_context ,emph_sample_mode),
   gtagml_context, "leave-sample-mode",
   " / (?! \\w) "
   ,[&]
 {
  parse_state.leave_sample_mode();
 });

 add_rule( flags_all_(parse_context ,emph_highlight_mode),
   gtagml_context, "leave-highlight-mode",
   " / "
   ,[&]
 {
  parse_state.leave_highlight_mode();
 });

 add_rule( gtagml_context, "enter-acronym-mode",
   " (?<pre> ;+) / (?=\\w) "
   ,[&]
 {
  QString pre = p.matched("pre");
  parse_state.enter_acronym_mode(pre.size());
 });

 add_rule( gtagml_context, "enter-short-macro-mode",
   " (?<pre> ,+) / (?=\\w) "
   ,[&]
 {
  parse_state.enter_short_macro_mode(p.matched("pre").size());
 });

 add_rule( gtagml_context, "short-macro",
   " (?<=\\s) (?<pre> ,+) (?<text> \\S+) "
   ,[&]
 {
  QString text = p.matched("text");
  parse_state.short_macro(text, p.matched("pre").size());
 });

 add_rule( gtagml_context, "short-acronym",
   " (?<=\\s) (?<pre> ;+) (?<text> \\S+) "
   ,[&]
 {
  QString text = p.matched("text");
  parse_state.short_acronym(text, p.matched("pre").size());
 });


 add_rule( gtagml_context, "emph-symbolic",
   " @/ (?<text> [^;,:/\\s-]+) "
   ,[&]
 {
  QString text = p.matched("text");
  parse_state.emph_symbolic(text);
 });



 add_rule( gtagml_context, "hyperlink-1",
   " \\[< (?<text> (?: [^>] | >[^\\]])+ ) \\s+ & \\s+ "
   " (?<link> (?: [^>] | >[^\\]])+ ) >\\] "
   ,[&]
 {
  QString text = p.matched("text");
  QString link = p.matched("link");
  parse_state.hyperlink_2(text.simplified(), link.simplified());
 });



 add_rule( gtagml_context, "hyperlink-1",
//           " \\[ < (?<text> (?: [^>] | >[*\\]])+ ) > \\] "
   " \\[ < (?<text> (?: [^>] | > [^\\]])+ ) > \\] "
   ,[&]
 {
  QString text = p.matched("text");
  parse_state.hyperlink_1(text.simplified());
 });


 add_rule( flags_all_(parse_context ,double_quote_mode),
   gtagml_context, "leave-double-quote-mode",
   " /\" "
   ,[&]
 {
  parse_state.leave_double_quote_mode();
 });

 add_rule( gtagml_context, "enter-double-quote-mode",
   " \"(?<pre> [\\w!] *)/ "
   ,[&]
 {
  parse_state.enter_double_quote_mode(p.matched("pre"));
 });



 add_rule( flags_all_(parse_context ,single_quote_mode),
   gtagml_context, "leave-single-quote-mode",
   " /' "
   ,[&]
 {
  parse_state.leave_single_quote_mode();
 });

 add_rule( gtagml_context, "enter-single-quote-mode",
   " '/ "
   ,[&]
 {
  parse_state.enter_single_quote_mode();
 });



 add_rule( flags_all_(parse_context ,single_quote_mode_doubled),
   gtagml_context, "leave-single-quote-mode-doubled",
   " /'' "
   ,[&]
 {
  parse_state.leave_single_quote_mode_doubled();
 });

 add_rule( gtagml_context, "enter-single-quote-mode-doubled",
   " ''(?<pre> \\w*)/ "
   ,[&]
 {
  parse_state.enter_single_quote_mode_doubled();
 });



 add_rule( flags_all_(parse_context ,single_quote_mode_trebled),
   gtagml_context, "leave-single-quote-mode-trebled",
   " /''' "
   ,[&]
 {
  parse_state.leave_single_quote_mode_trebled();
 });

 add_rule( gtagml_context, "enter-single-quote-mode-trebled",
   " '''(?<pre> \\w*)/ "
   ,[&]
 {
  parse_state.enter_single_quote_mode_trebled();
 });

 add_rule( gtagml_context, "special-character-sequence",
   " (?: %-+ ) | (?: \\^: ) | (?: %[%,$&#]) "
   ,[&]
 {
  QString m = p.match_text();
  parse_state.special_character_sequence(m);
 });




// add_rule( gtagml_context, "slashes",
//  " (?<first> /+)  .single-space.+  (?<second> /*) "
//  ,[&]
// {
//  QString first = p.matched("first");
//  QString second = p.matched("second");
//  QString text = p.matched("text");

//  parse_state.heading(first.size(), second.size(), text);
// });



 add_rule( gtagml_context, "primary-acc-newline",
  " .single-space.+ \\n "
           ,[&]
 {
  parse_state.primary_acc("\n");
 });

 add_rule( gtagml_context, "primary-acc",
  " . "
           ,[&]
 {
  parse_state.primary_acc(p.match_text());
 });


// flag_(2, italics_mode)
// flag_(3, double_quote_mode)
// flag_(4, single_quote_mode)
// flag_(5, single_quote_mode_doubled)
// flag_(6, single_quote_mode_trebled)

// flag_(7, acronym_mode)
// flag_(8, emph_italics_mode)



#ifdef HIDE

 add_rule( gtagml_context, "enter-special-parse-mode",
   " \\{  "
   " (?<spm> .valid-tag-command-name. ) "
   " >> "
   ,[raw_context, &parse_state, this, &p]
 {
  QString spm = p.matched("spm");
  parse_state.enter_special_parse_mode(spm);
  if(parse_state.current_parsing_mode() == GTagML_Parsing_Modes::Raw)
    activate(raw_context);
 });

 add_rule( raw_context, "leave-special-parse-mode",
   " << (?<spm> .valid-tag-command-name. )? \\}  "
   ,[gtagml_context, &parse_state, this, &p]
 {
  QString spm = p.matched("spm");
  if(spm.isEmpty())
    spm = "raw";
  parse_state.leave_special_parse_mode(spm);
  if(parse_state.current_parsing_mode() == GTagML_Parsing_Modes::GTagML)
    activate(gtagml_context);
 });

 add_rule( raw_context, "spm-long-acc",
   " [^<]+  "
   ,[&]
 {
  parse_state.special_parse_mode_acc(p.match_text());
 });

 add_rule( raw_context, "spm-short-acc",
   " < "
   ,[&]
 {
  parse_state.special_parse_mode_acc(p.match_text());
 });


 add_rule( gtagml_context, "gtag-command-entry-inline",
   " \\[  "
   " (?<tag-command> .valid-tag-command-name. ) "
   " (?<tag-body-follow> [,>;.] ) "
   ,[&]
 {
  QString tag_command = p.matched("tag-command");
  QString tag_body_follow = p.matched("tag-body-follow");
 // QString argument = p.matched("argument");
  parse_state.gtag_command_entry_inline(tag_command, tag_body_follow);
  //parse_state.tag_body_leave();
 });

 add_rule( gtagml_context, "block-gtag-command-leave",
  " < (?<tag-command> .valid-tag-command-name. ) \\]  "
           ,[&]
 {
  parse_state.inline_tag_command_leave();
 });

 add_rule( flags_all_(parse_context ,inside_attribute_sequence),
   gtagml_context, "attribute-sequence-leave",
   " \\s+ => \\s+ "
   //" [/-]? "
   //" > "
   //" [,;.]? "
   ,[&]
 {
  //QString m = p.match_text();
  parse_state.attribute_sequence_leave();
 });

 add_rule( flags_all_(parse_context ,inside_attribute_sequence),
   gtagml_context, "mark-attribute-tile",
   " \\s+ @ \\s+ "
   ,[&]
 {
  parse_state.mark_attribute_tile();
 });



 // //  these should be for parse_state the equivalent
  //    of ->> (etc.) then `::some_cmd;
 add_rule( flags_all_(parse_context ,inside_multi_generic),
   gtagml_context,
   "cmd-multi-arg-transition",
   " \\s+ (?<fiat-or-wmi> (?: (?: :: ) | [.] | = ) ) (?<main> -{1,2}>{1,2}) "
   " \\s+ (?<cmd> .valid-tag-command-name. ) "
    ,[&]
 {
  QString cmd = p.matched("cmd");

  QString m = p.matched("main");
  QString fiat_or_wmi = p.matched("fiat-or-wmi");

  if(fiat_or_wmi == ".")
  {
   parse_state.multi_arg_transition({}, fiat_or_wmi, {}, "->");
   parse_state.tile_acc(cmd);
  }
  else
  {
   parse_state.multi_arg_transition({}, {}, {}, m);

   if(fiat_or_wmi == "=")
     parse_state.tag_command_entry_inline("::", {}, "==", cmd, ";", {});
   else
     parse_state.tag_command_entry_inline(fiat_or_wmi, {}, {}, cmd, ";", {});
  }
 });


 add_rule( flags_all_(parse_context ,inside_multi_generic),
   gtagml_context,
   "multi-arg-transition-to-main-tile",
   " \\s+ => \\s+ "
   ,[&]
 {
  parse_state.multi_arg_transition_to_main_tile();
 });

 add_rule( flags_all_(parse_context ,inside_multi_generic),
   gtagml_context,
   "multi-arg-transition",
   " \\s+ (?<wmi> .tag-command-wrap-mode-indicator.? ) "
   " (?<fiat> [=,]?)  "
   " (?<main> -{1,2}>{1,2} ) \\s+ "
   ,[&]
 {
  QString wmi = p.matched("wmi");
  QString fiat = p.matched("fiat");
  QString m = p.matched("main");
  parse_state.multi_arg_transition(wmi, {}, fiat, m);
 });

 add_rule( flags_all_(parse_context ,inside_multi_parent_semis),
   gtagml_context, "tag-command-leave-multi",
   "  \\s+ ;[.]*; "
   ,[&]
 {
  parse_state.tag_command_leave_multi({});
 //?  parse_context.flags.inside_multi_parent_semis = false;
 //? parse_context.flags.inside_multi_parent = false;
 });



 add_rule( flags_all_(parse_context ,inside_multi_parent),
   gtagml_context, "tag-command-leave-multi",
   "  ` (?<tag-command> .valid-tag-command-name.? ) ` "
   ,[&]
 {
  QString tag_command = p.matched("tag-command");
  parse_state.tag_command_leave_multi(tag_command);
//?  parse_context.flags.inside_multi_parent = false;
 });


 add_rule( gtagml_context, "cmd-tag-command-entry-multi",
   " ` (?<wmi> .tag-command-wrap-mode-indicator.? ) "
   " (?<tag-command> .valid-tag-command-name. ) "
   " (?<tag-body-follow> [,.]?) "
   " \\s+ (?<fiat-or-wmi> (?: (?: :: ) | [.] | = ) ) "
   " (?<first-arg-marker> -{1,2} >{1,2} ) \\s+ (?<cmd> .valid-tag-command-name. )"
   ,[&]
 {
  QString wmi = p.matched("wmi");

  QString tag_command = p.matched("tag-command");
  QString tag_body_follow = p.matched("tag-body-follow");

  QString fiat_or_wmi = p.matched("fiat-or-wmi");
  QString first_arg_marker = p.matched("first-arg-marker");

  QString cmd = p.matched("cmd");

  if(fiat_or_wmi == ".")
  {
   parse_state.tag_command_entry_multi(wmi, fiat_or_wmi, tag_command,
     tag_body_follow, {}, {}, first_arg_marker);
   parse_state.tile_acc(cmd);
  }
  else
  {
   parse_state.tag_command_entry_multi(wmi, {}, tag_command,
     tag_body_follow, {}, {}, first_arg_marker);

   if(fiat_or_wmi == "=")
     parse_state.tag_command_entry_inline("::",  {}, fiat_or_wmi, cmd, ";", {});
   else
     parse_state.tag_command_entry_inline(fiat_or_wmi, {}, {}, cmd, ";", {});
  }
 });


 add_rule( gtagml_context, "tag-command-entry-multi",
  " ` (?<wmi> .tag-command-wrap-mode-indicator.? ) " 
  " (?<tag-command> .valid-tag-command-name. ) "
  " (?<tag-body-follow> [,.]?) \\s+  "
  " (?<fwmi> .tag-command-wrap-mode-indicator.? ) "
  " (?<first-arg-marker> @ | (?: -{1,2} >{1,2} ) ) \\s+ "
           ,[&]
 {
  QString wmi = p.matched("wmi");
  QString fwmi = p.matched("fwmi");
  QString tag_command = p.matched("tag-command");
  QString tag_body_follow = p.matched("tag-body-follow");
  QString first_arg_marker = p.matched("first-arg-marker");
  parse_state.tag_command_entry_multi(wmi, {}, tag_command,
    tag_body_follow, {}, fwmi, first_arg_marker);
    //parse_state.tag_body_leave();
 });



 add_rule( gtagml_context, "tag-command-entry-with-layer",
   " ` " //?(?<wmi> .tag-command-wrap-mode-indicator.? ) "
   " (?<tag-command> .valid-tag-command-name. ) / "
   " (?: .single-space.+ )  (?<layer-marker> -{1,2} >{1,2} ) "
   ,[&]
 {
  QString tag_command = p.matched("tag-command");
  QString layer_marker = p.matched("layer-marker");
  parse_state.tag_command_entry_with_layer(tag_command, layer_marker);
  //parse_state.tag_body_leave();
 });


 add_rule( gtagml_context, "tag-command-entry-inline",
   " ` (?<wmi> .tag-command-wrap-mode-indicator.? ) "
   " (?<tag-command> .valid-tag-command-name. ) "
   " (?: < (?<argument> [^>]+ ) >)?  (?<tag-body-follow> [,;.] ) "
   ,[&]
 {
  QString wmi = p.matched("wmi");
  QString tag_command = p.matched("tag-command");
  QString tag_body_follow = p.matched("tag-body-follow");
  QString argument = p.matched("argument");
  parse_state.tag_command_entry_inline(wmi,  {}, {}, tag_command, tag_body_follow, argument);
  //parse_state.tag_body_leave();
 });

 add_rule( gtagml_context, "alt-tag-command-entry-inline",
   " ` (?<wmi> .tag-command-wrap-mode-indicator.? ) "
   " (?<tag-command> .valid-tag-command-name. ) "
   " <> (?<argument> [^;]* ) ;"
   ,[&]
 {
  QString wmi = p.matched("wmi");
  QString tag_command = p.matched("tag-command");
  QString argument = p.matched("argument");

  argument.replace('{', "! ");
  argument.replace('[', "? ");
  argument.replace('}', " ");
  argument.replace(']', " ");

  parse_state.tag_command_entry_inline(wmi, {}, {}, tag_command, ";", argument);
  //parse_state.tag_body_leave();
 });

#endif

#ifdef HIDE_before
 add_rule( html_context, "html-tag-command-leave",
  " </ (?<tag-command> .valid-tag-command-name. ) "
  " > "
  ,[&]
 {
  QString tag_command = p.matched("tag-command");
  parse_state.check_html_tag_command_leave(tag_command, p.match_text());
 });

 add_rule( gtagml_context, "tag-command-leave",
  " < (?<load-connector> .valid-tag-command-name. )? "
  "  < (?<tag-command> .valid-tag-command-name. ) "
  " /? "  // this really needs to be set to check for matching env's
  " > "
           ,[&]
 {
  QString load_connector = p.matched("load-connector");
  QString tag_command = p.matched("tag-command");
  if(load_connector.isEmpty())
   parse_state.check_tag_command_leave(tag_command, p.match_text());
  else
   parse_state.tag_command_leave(load_connector, tag_command);
 });
#endif //def HIDE_before


#ifdef HIDE
 add_rule( gtagml_context, "tag-block-command-leave",
  "  ` (?<tag-command> .valid-tag-command-name. ) ` "
  ,[&]
 {
  QString tag_command = p.matched("tag-command");
  parse_state.check_tag_command_leave(tag_command, p.match_text());
 });

 add_rule( gtagml_context, "inline-tag-command-leave",
  " ` (?: / | (?= \\s))  "
           ,[&]
 {
  parse_state.inline_tag_command_leave();
 });


#ifdef HIDE
 add_rule( gtagml_context, "generic-tag-command-leave",
  " </>  "
           ,[&]
 {
  parse_state.tag_command_leave();
 });
#endif //def HIDE

//?
// add_rule( gtagml_context, "special-character-sequence",
//  " (?: %-- ) | (?: %[<>$'] ) | (&-\\S>\\w) | (&#\\w+;) | (&:\\w+;) "
//           ,[&]
// {
//  QString m = p.match_text();
//  parse_state.special_character_sequence(m);
// });

  add_rule( gtagml_context, "semantic-mark",
   " (?: ` (?<pre> [`']?) (?: "
   "  \\( (?<sem1> [^)]+ ) \\) ) "
   "  | (?: ` { (?<sem2> [^}]+ ) } ) "
   "  | (?: ` \\[ (?<sem3> [^]]+ ) \\] ) "
   "  | (?: ` < (?<sem4> [^>]+ ) \\> ) "
   " ) " ,[&]
  {
   int which = 1;
   QString sem = p.matched("sem1");
   if(sem.isEmpty())
   {
    sem = p.matched("sem2");
    ++which;
   }
   if(sem.isEmpty())
   {
    sem = p.matched("sem3");
    ++which;
   }
   if(sem.isEmpty())
   {
    sem = p.matched("sem4");
    ++which;
   }
   if(sem.isEmpty())
   {
    which = 0;
   }

   QString m = p.match_text();

   parse_state.semantic_mark(m, sem, which);


  });

  add_rule( gtagml_context, "deleted-visual-line",
    "\\n* \\s* \\|[.]\\| \\s* ",
    [&]
  {
   QString m = p.match_text();
   parse_state.tile_acc("\n");
  });

  add_rule( gtagml_context, "deleted-visual-line-then-sentence-start",
    "\\n* \\s* \\|-\\| \\s* ",
    [&]
  {
   QString m = p.match_text();
   parse_state.tile_acc("\n`\(@)");
  });


  add_rule( gtagml_context, "declare-sentence-end-special-character-sequence",
    " (?<se> [,;:-]) (?<sp> \\s*) \\| (?<cue> [=]) \\|",
    [&]
  {

   QString se = p.matched("se");
   QString cue = p.matched("cue");

   QString esc1 = cue;
   QString m1 = QString("`\\[%1]").arg(cue);

   u1 which = 3;

//   if(se == ",")
//   {
//    QString sp = p.matched("sp");

//    QString m = "`\\<~>";
//    QString esc = "~";
//    u1 which0 = 4;
//    parse_state.tile_acc(se);
//    parse_state.special_character_sequence(m, esc, which0);
//    //?parse_state.tile_acc(sp);
//    parse_state.special_character_sequence(m1, esc1, which);
//    return;
//   }

   QString m = QString("`\\[%1]").arg(se);
   QString esc = se;


   parse_state.special_character_sequence(m, esc, which);
   parse_state.special_character_sequence(m1, esc1, which);

  });

  add_rule( gtagml_context, "declare-sentence-end",
    " (?<se> [,?!:-] | [.]) (?<mid> [\\])]*)"
    " \\s* \\|(?<end>[+*])\\| \\s* ",
    [&]
  {
   QString se = p.matched("se");
   QString mid = p.matched("mid");
   QString end = p.matched("end");
   QString m, m1, esc, esc1;
   u1 which = 3; // = "`\\[.]" QString esc = "." u1 which = 3

   if(end == "+")
   {
    m1 = "`\\[;]";
    esc1 = ";";
   }
   else
   {
    m1 = QString("`\\[%1]").arg(end);
    esc1 = end;
   }


   if( (se == ":") || (se == ",") )
   {
    m = QString("`\\[%1]").arg(se);
    esc = se;
   }
//   else if(se == ",")
//   {
//    m = "`\\[,]";
//    m1 = "`\\[;]";
//    esc = ",";
//    esc1 = ";";
//   }

//   else
//   {
//    m1 = "`\\[;]";
//    esc1 = ";";
//   }

//   else if(se == ".")
//   {
////    m = "`\\[.]";
//    m1 = "`\\[;]";
////    esc = ".";
//    esc1 = ";";
//   }
//   else if(se == "?")
//   {
//    m = "`\\[?]";
//    m1 = "`\\[;]";
//    esc = "?";
//    esc1 = ";";
//   }
//   else if(se == "!")
//   {
//    m = "`\\[!]";
//    m1 = "`\\[;]";
//    esc = "?";
//    esc1 = ";";
//   }

//   else
//     return;
//   // else others?

   if(m.isEmpty())
     parse_state.tile_acc(se);
   else
     parse_state.special_character_sequence(m, esc, which);

   if(!mid.isEmpty())
     parse_state.tile_acc(mid);

   parse_state.special_character_sequence(m1, esc1, which);

  });

  add_rule( gtagml_context, "special-special-character-sequence",
    " \\s* \\| (?<cue> [=_@]) \\| \\s* ",
    [&]
  {
   QString cue = p.matched("cue");
   QString esc = cue;
   QString m = QString("`\\[%1]").arg(cue);
   u1 which = 3;
   parse_state.special_character_sequence(m, esc, which);
  });

  add_rule( gtagml_context, "special-character-sequence",
   " (?: %-- ) | (?: %_) | (?: ->- ) | (?: %\\.,{2,3} ) "
   "  | (?: ` \\\\ \\( (?<bq-esc1> [^)]+ ) \\) ) "
   "  | (?: ` \\\\ { (?<bq-esc2> [^}]+ ) } ) "
   "  | (?: ` \\\\ \\[ (?<bq-esc3> [^]]+ ) \\] ) "
   "  | (?: ` \\\\ < (?<bq-esc4> [^>]+ ) \\> ) "
   "  | (?: \\| (?<pipe-esc> [^|]+) \\| )"
 
          //? "| (?: %[<>$'] ) | (&-\\S>\\w) | (&#\\w+;) | (&:\\w+;) "
            ,[&]
  {
   u1 which = 1;
   QString esc = p.matched("bq-esc1");
   if(esc.isEmpty())
   {
    esc = p.matched("bq-esc2");
    ++which;
   }
   if(esc.isEmpty())
   {
    esc = p.matched("bq-esc3");
    ++which;
   }
   if(esc.isEmpty())
   {
    esc = p.matched("bq-esc4");
    ++which;
   }
   if(esc.isEmpty())
   {
    esc = p.matched("pipe-esc");
    ++which;
   }
   if(esc.isEmpty())
   {
    which = 0;
   }

   QString m = p.match_text();

   parse_state.special_character_sequence(m, esc, which);
  });


 // // do we always want this?
 add_rule( gtagml_context, "tile-acc-newline",
  " .single-space.+ \\n "
           ,[&]
 {
  parse_state.tile_acc("\n");
 });

 add_rule( gtagml_context, "tile-acc",
  " . "
           ,[&]
 {
  parse_state.tile_acc(p.match_text());
 });

#endif //def HIDE


}
