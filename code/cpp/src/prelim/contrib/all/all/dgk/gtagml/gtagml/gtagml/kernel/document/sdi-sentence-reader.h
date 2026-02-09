
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef SDI_SENTENCE_READER__H
#define SDI_SENTENCE_READER__H

#include "relae-graph/relae-node-ptr.h"

#include "accessors.h"

#include "global-types.h"

#include "flags.h"

#include "kans.h"

#include <QStringList>
#include <QVector>

#include "sentences/sdi-sentence.h"


KANS_(GTagML)


class GTagML_Parser;
class GTagML_Output_Blocks;

class Dummy_Class;

class SDI_Sentence_Reader
{
 QString sdi_sentences_file_;

 QVector<SDI_Sentence> sdi_sentences_;

 SDI_Sentence* current_sentence_;

 QString pipe_acc_;

 QString current_prelim_;

 QStringList current_prelim_split(QString field)
 {
  return current_prelim_.split("/") << field;
 }

 QString current_prelim_plus(QString field)
 {
  return current_prelim_ + "/" + field;
 }


 QString current_pipe_field_;

public:

 SDI_Sentence_Reader(QString sdi_sentences_file);

 void parse_sdi();

 void sdi_check(QString gt_contents, QString out_path);

 void read_Sentence_field(QString data, QStringList spl, QString field_style);
 void read_Paragraph_field(QString data, QStringList spl, QString field_style);

 void parse_blank_line();
 void parse_data_line(s2 pos, QString line, QString* simpptr = nullptr);
 void parse_mini_line(QString line);
 void parse_pipe_line(QString line);
 void parse_dot_line(QString line);
 void parse_prelim_line(QString line);
 void parse_colon_line(QString field, QString data);
 void parse_numbers_line(QString field, QString data);

 void read_pipe_field(QString text);
 void read_field(QString field, QString text, QString field_style);
 void read_field(QStringList spl, QString text, QString field_style);


 void parse_numbers_line(QString data, QVector<s4>& numbers);

 QVector<s4> parse_numbers(QString data);
 void parse_numbers(QString data, QVector<s4>& numbers);

};

_KANS(GTagML)


#endif
