
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "sdi-sentence-reader.h"

#include "textio.h"

#include <QFile>
#include <QTextStream>

#include "kans.h"

USING_KANS(GTagML)



SDI_Sentence_Reader::SDI_Sentence_Reader(QString sdi_sentences_file)
  :  sdi_sentences_file_(sdi_sentences_file),
     current_sentence_(nullptr)
{

}

void SDI_Sentence_Reader::parse_blank_line()
{

}

void SDI_Sentence_Reader::parse_mini_line(QString line)
{

}

void SDI_Sentence_Reader::parse_prelim_line(QString line)
{
 current_prelim_ = line.mid(3).simplified().replace("//", "/_");
}


QVector<s4> SDI_Sentence_Reader::parse_numbers(QString data)
{
 QVector<s4> result;

 parse_numbers(data, result);

 return result;
}

void SDI_Sentence_Reader::parse_numbers(QString data, QVector<s4>& numbers)
{
 QStringList qsl = data.simplified().split(" ");

 numbers.resize(qsl.size());

 std::transform(qsl.begin(), qsl.end(), numbers.begin(), [](const QString& n) -> s4
 {
  return n.toInt();
 });
}


void SDI_Sentence_Reader::parse_numbers_line(QString data, QVector<s4>& numbers)
{
 parse_numbers(data, numbers);
}


void SDI_Sentence_Reader::parse_numbers_line(QString field, QString data)
{
// QVector<s4> numbers;
// parse_numbers_line(data, numbers);
// qDebug() << "n: " << numbers;

 read_field(field, data, "#");
}


void SDI_Sentence_Reader::parse_colon_line(QString field, QString data)
{
 if(field == "id")
 {
  if(current_prelim_ == "Sentence/start")
  {
   sdi_sentences_.push_back(SDI_Sentence(data.simplified().toInt()));
   current_sentence_ = &sdi_sentences_.last();
   return;
  }
 }

 read_field(field, data, ".");

}

void SDI_Sentence_Reader::parse_data_line(s2 pos, QString line, QString* simpptr)
{
 switch (line[pos].toLatin1())
 {
 case ' ':
   parse_data_line(pos - 1, line, simpptr); break;
 case ':': parse_colon_line(line.left(pos), line.mid(pos + 2)); break;
 case '#': parse_numbers_line(line.left(pos), line.mid(pos + 2)); break;
 case '.':
  if(simpptr)
    current_pipe_field_ = line.left(pos);
  break;
 default:
  break;
 }

}

void SDI_Sentence_Reader::parse_dot_line(QString line)
{
 parse_pipe_line(line);
 read_pipe_field(pipe_acc_);
 pipe_acc_.clear();
}

void SDI_Sentence_Reader::read_Paragraph_field(QString data, QStringList spl, QString field_style)
{

}

void SDI_Sentence_Reader::read_field(QString field, QString text, QString field_style)
{
 QStringList spl = current_prelim_split(field);
 read_field(spl, text, field_style);
}


void SDI_Sentence_Reader::read_pipe_field(QString text)
{
// qDebug() << "current = " << current_pipe_field_
//   << "\n = " << text;

 QStringList spl = current_prelim_split(current_pipe_field_);

 read_field(spl, text, ".");

 //QString cpp = current_prelim_plus(current_pipe_field_);
}

void SDI_Sentence_Reader::read_field(QStringList spl, QString text, QString field_style)
{
 typedef void (SDI_Sentence_Reader::*fn_type)(QString, QStringList, QString);

 static QMap<QString, fn_type> static_map {{
   {"Sentence", &SDI_Sentence_Reader::read_Sentence_field},
   {"Paragraph", &SDI_Sentence_Reader::read_Paragraph_field}
   }};


 QString key = spl.takeFirst();

 auto it = static_map.find(key);

 if(it != static_map.end())
 {
  (this->*(it.value()))(text, spl, field_style);
 }

// if(klass == "Sentence")
//   read_Sentence_field(text, spl, ".");
}

void SDI_Sentence_Reader::read_Sentence_field(QString data, QStringList spl, QString field_style)
{
 typedef void (SDI_Sentence::*fn_type_qsl)(QStringList);
 typedef void (SDI_Sentence::*fn_type_numbers)(QStringList, QVector<s4>);

 union fn_union {
   fn_type_qsl _dot;
   fn_type_numbers _hash;
 };

 QString key = spl.first() + field_style + spl.at(1);

// fn_union read_sentence_text = { ._qsl = &SDI_Sentence::read_sentence_text};
// fn_union read_sentence_gaps = { ._qsl = &SDI_Sentence::read_sentence_text};
// fn_union read_sentence_range = { ._nums = &SDI_Sentence::read_sentence_range};

 static QMap<QString, fn_union> static_map {{
   {"_end.t", { ._dot = &SDI_Sentence::read_sentence_text} },
   {"_end.g", { ._dot = &SDI_Sentence::read_sentence_gaps} },
   {"start#r", { ._hash =  &SDI_Sentence::read_sentence_range} }
   }};

 auto it = static_map.find(key);

 if(it != static_map.end())
 {
  switch (field_style[0].toLatin1())
  {
  case '.':  (current_sentence_->*(it.value()._dot))(spl << data); break;
  case '#':  (current_sentence_->*(it.value()._hash))(spl, parse_numbers(data)); break;
  default: break;
  }
 }
}



void SDI_Sentence_Reader::parse_sdi()
{
 QString text = KA::TextIO::load_file(sdi_sentences_file_);

 QStringList lines = text.split("\n");

 for(QString line : lines)
 {
  if(line.isEmpty())
  {
   parse_blank_line();
   continue;
  }

  if(line.startsWith("---"))
  {
   parse_prelim_line(line);
   continue;
  }

  if(line.startsWith('|'))
  {
   parse_pipe_line(line);
   continue;
  }

  if(line.startsWith(". "))
  {
   parse_dot_line(line);
   continue;
  }

  QString* simpptr = nullptr;
  QString simp = line.simplified();

  if(simp.length() < 3)
  {
   if(simp.endsWith('.'))
     simpptr = &simp;
   else
   {
    parse_mini_line(simp);
    continue;
   }
  }

  parse_data_line(2, line, simpptr);
 }

}

void SDI_Sentence_Reader::parse_pipe_line(QString line)
{
 pipe_acc_ += line.mid(3);
}


void SDI_Sentence_Reader::sdi_check(QString gt_contents, QString out_path)
{
 parse_sdi();
 KA::TextIO::save_file(out_path, gt_contents);
}



