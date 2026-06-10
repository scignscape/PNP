
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef GTAGML_STREAMS__H
#define GTAGML_STREAMS__H


#include <QString>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QBuffer>

#include "global-types.h"

#include "accessors.h"

#include "textio.h"
USING_KANS(TextIO)


#include "kans.h"


KANS_(GTagML)


class GTagML_Parser;
class GTagML_Parse_State;


class GTagML_Streams
{
 GTagML_Parse_State* parse_state_;

 QXmlStreamWriter xml_writer_;
 QString jats_;
 QTextStream jats_stream_;
 QBuffer jats_buffer_;
 QByteArray jats_array_;

 QString latex_;
 QTextStream latex_stream_;

 QString sentences_sdi_;
 QTextStream sentences_sdi_stream_;

 QString primary_acc_;
 QTextStream primary_acc_stream_;


 GTagML_Parser* parser_;

 public:

 GTagML_Streams(GTagML_Parse_State* parse_state);

 ACCESSORS(QString ,jats)

 ACCESSORS__RGET(QTextStream ,primary_acc_stream)
 ACCESSORS__RGET(QTextStream ,latex_stream)
 ACCESSORS__RGET(QTextStream ,jats_stream)
 ACCESSORS__RGET(QTextStream ,sentences_sdi_stream)
 ACCESSORS__RGET(QXmlStreamWriter ,xml_writer)
 ACCESSORS__RGET(QString ,primary_acc)

 void latex(QString text);
 void primary(QString text);

 void insert_latex_template(QString path, QString* result = nullptr);
 void insert_xml_template(QString path, QString* result = nullptr);


 void new_text_node(u2 ply_id, QString text);
 void init();

 void prepare_jats(QString& text, QString bib_path);

 void save_jats(QString path, QString bib_path)
 {
  KA::TextIO::save_file(path + ".test", jats_array_);// jats_array_.replace("@=", "&"));

  QString text = QString::fromLatin1(jats_array_);
  prepare_jats(text,  bib_path);
  //jats_ = QString::fromLatin1(jat)
  KA::TextIO::save_file(path, text);// jats_array_.replace("@=", "&"));
 }

 void save_latex(QString path)
 {
  //jats_ = QString::fromLatin1(jat)
  KA::TextIO::save_file(path, latex_);
 }

 void save_sentences(QString path)
 {
  KA::TextIO::save_file(path, sentences_sdi_);
 }

 void enter_abstract();

};


_KANS(GTagML)


#endif //  GTAGML_STREAMS__H
