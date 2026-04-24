
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
