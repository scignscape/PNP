
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "gtagml-streams.h"

#include "gtagml-parse-state.h"


USING_KANS(GTagML)

GTagML_Streams::GTagML_Streams(GTagML_Parse_State* parse_state)
  :  parse_state_(parse_state), latex_stream_(&latex_)

{

}

void GTagML_Streams::init()
{
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

 latex_stream_ << "\n\n%PREAMBLE-TEMPLATE%\n\n%BEGIN-TEMPLATE%";

 sentences_sdi_stream_ << "--- Global/start\n\n";
// xml_writer_.set
// xml_writer_ = QXmlStreamWriter(jats_);
// jats_stream_.setString(&jats_); // = QTextStream(&jats_);
// xml_writer_.setDevice(&jats_stream_);
}

void GTagML_Streams::enter_abstract()
{
 xml_writer_.writeCharacters("\n\n");
 xml_writer_.writeStartElement("doc-abstract");
 latex_stream_ << "\n\n\\twocolumn[\\begin{docAbstract}\n";

 sentences_sdi_stream_ << "\n\n--- Abstract/start\n";

 sentences_sdi_stream_ << "\n\n--- Sentence/start\nid: " <<
   parse_state_->sentence_id();
}

void GTagML_Streams::prepare_jats(QString& text, QString bib_path)
{
 QString bibtext = KA::TextIO::load_file(bib_path);

 u1 count = 0;
// for(QString ref_label : ref_labels_)
// {
//  ++count;
//  bibtext.replace(ref_label, "B%1"_qt.arg(count));
// }

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

void GTagML_Streams::insert_xml_template(QString path, QString* result)
{
 QString contents = KA::TextIO::load_file(path);

 if(result)
 {
  *result = jats_array_;
  result->replace("%XML-TEMPLATE%", contents.toLatin1());
 }
 else
   jats_array_.replace("%XML-TEMPLATE%", contents.toLatin1());
}


void GTagML_Streams::insert_latex_template(QString path, QString* result)
{
 QString contents = KA::TextIO::load_file(path);

 s4 ix = contents.indexOf("\n%%\n");
 s4 ix1 = contents.indexOf("\n%%%\n");

 QString lat = latex_;

 bool have_pt = lat.contains("%PREAMBLE-TEMPLATE%");
 bool have_bt = lat.contains("%BEGIN-TEMPLATE%");

 QString econtents;

 if(ix1 != -1)
 {
  econtents = contents.mid(ix1 + 5);
  contents = contents.left(ix1);
 }

 if(ix != -1)
 {
  QString bcontents = contents.mid(ix + 4);

  //  contents.replace(ix + 4, contents.length() - ix - 4, "");
  contents = contents.left(ix);

  if(have_pt)
  {
   if(have_bt)
   {
    lat.replace("%PREAMBLE-TEMPLATE%", contents);
    lat.replace("%BEGIN-TEMPLATE%", bcontents);
   }
   else
   {
    lat.replace("%PREAMBLE-TEMPLATE%", contents + "\n\n" + bcontents + "\n\n");
   }
  }
  else if(have_bt)
  {
   lat.replace("%BEGIN-TEMPLATE%", bcontents);
   lat.prepend(contents + "\n\n");
  }
  else
  {
   lat.prepend(contents + "\n\n" + bcontents + "\n\n");
  }
 }

 if(lat.contains("%END-TEMPLATE%"))
   lat.replace("%END-TEMPLATE%", "\n\n" + econtents);
 else if(!econtents.isEmpty())
   lat.append("\n\n" + econtents);

 if(result)
   *result = lat;
 else
   latex_ = lat;
}
