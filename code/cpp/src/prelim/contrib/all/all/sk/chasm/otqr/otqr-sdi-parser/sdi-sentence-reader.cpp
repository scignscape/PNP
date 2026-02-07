
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "sdi-sentence-reader.h"

#include "textio.h"

#include <QFile>
#include <QTextStream>

#include "kans.h"

USING_OTNS(SDI)



SDI_Sentence_Reader::SDI_Sentence_Reader()
  :  current_sentence_(nullptr)
{

}


void align_columns(QString& lines, QString pre, u2 width, u4 sentence_id)
{
 pre.prepend("\n");

 lines = lines.trimmed();

 QString result;

 u4 old_pos = 0;
 s4 pos = pre.size();
 u2 count = 0;

 auto append_last = [&]()
 {
  if(count == 0)
    result += pre.mid(1) + lines.mid(old_pos);
  else
    result += pre + lines.mid(old_pos);
 };

 auto append = [&]()
 {
  if(count == 0)
    result += pre.mid(1) + lines.mid(old_pos, pos - 1 - old_pos);
  else
    result += pre + lines.mid(old_pos, pos - 1 - old_pos);
 };

 for(;; ++count)
 {
  pos += width;
  if(pos >= lines.size())
  {
   append_last();
   break;
  }

  while(!lines[pos].isSpace())
  {
   if(pos + 1 == lines.size())
   {
    append_last();
    goto break_outer;
   }
   ++pos;
  }

  while(lines[pos].isSpace())
  {
   if(pos + 1 == lines.size())
   {
    append_last();
    goto break_outer;
   }
   ++pos;
  }
  append();
  old_pos = pos;
 }
break_outer:
 lines = result;
}

void SDI_Sentence_Reader::handle_find(tsl::ordered_map<QString, QVector<QStringList>>& str_data,
  tsl::ordered_map<QString, QVector<QVariantList>>& other_data)
{
 QString pre = "|  ";
 u2 report_width = 80;

 if(str_data.contains(":report-width"))
 {
  report_width = str_data[":report-width"].value(0, {{"0"}}).first().toUInt();
 }

 QVector<QStringList> text = str_data[":text"];
 QVector<QStringList> in = str_data[":in"];
 QVector<QStringList> save = str_data[":save"];

 QVector<QStringList> orig = str_data[":_orig"];

 QStringList file_paths;

 if(!save.isEmpty())
 {
  QFileInfo qfi(file_path_);
  QString folder = qfi.canonicalPath();
  QString fn = qfi.completeBaseName();

  if(save.size() > 1)
  {
   if(save.first().first() == "to")
     save.takeFirst();
  }
  for(QString s : save.first())
  {
   QString path;
   if(s.contains("%"))
   {
    path = folder + "/" + s.replace("%", fn);
    file_paths.push_back(path);
   }
   else
     file_paths.push_back(s);
  }
 }

 QString ins = in.first().first();

 if(ins != "$Sentences")
 {
  qDebug() << "Search area not recognized: " << ins;
  return;
 }

 QMap<SDI_Sentence*, QStringList> finds;

 for(QStringList search_texts: text)
 {
  QString search_text = search_texts.join(" ");
  for(SDI_Sentence& s : sdi_sentences_)
  {
   u2 count = s.search_for(search_text);
   if(count)
     finds[&s].push_back(search_text);
  }
 }

 QVector<QFile*> files;
// files.resize(file_paths.size());

 QVector<QTextStream*> qtss;

 for(QString fp : file_paths)
 {
  QFile* qf = new QFile(fp);
  qf->open(QIODevice::WriteOnly);
  QTextStream* qts = new QTextStream(qf);

  for(QString s: orig.first())
  {
   (*qts) << "\n" << s;
  }
  (*qts) << "\n===\n\n";

  qtss.push_back(qts);
  files.push_back(qf);
 }

 QMapIterator<SDI_Sentence*, QStringList> it(finds);
 while(it.hasNext())
 {
  it.next();
  SDI_Sentence* s = it.key();
  QString id_space = sdi_sentences_.size() > 999? (s->id() > 100? " ": s->id() > 10? "  " : "   ")
    :  sdi_sentences_.size() > 99? (s->id() > 100? "": s->id() > 10? " " : "  ")
    :  (s->id() > 10? "" : " ");

  for(QTextStream* qts : qtss)
  {
   QString lines = s->sentence_text();
   align_columns(lines, pre, report_width, s->id());
   (*qts) << "\n\n# " << s->id() << id_space << " = " << it.value().join("; ") << " -> \n" << lines;
  }
 }

 for(QTextStream* qts : qtss)
 {
  (*qts) << "\n\n";
 }

 for(QFile* qf : files)
 {
  qf->close();
  qDebug() << "Saved: " << qf->fileName();
  delete qf;
 }
 for(QTextStream* qts : qtss)
 {
  delete qts;
 }


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

void SDI_Sentence_Reader::init_sentence(QString data)
{
 sdi_sentences_.push_back(SDI_Sentence(data.simplified().toInt()));
 current_sentence_ = &sdi_sentences_.last();
}

void SDI_Sentence_Reader::parse_colon_line(QString field, QString data)
{
 if(field == "id")
 {
  if(current_prelim_ == "Sentence/start")
  {
   init_sentence(data);
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


void SDI_Sentence_Reader::read_Section_field(QString data, QStringList spl, QString field_style)
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
   {"Section", &SDI_Sentence_Reader::read_Section_field},
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


void SDI_Sentence_Reader::read_sentence_end_id(QStringList read_dispatch)
{

}


void SDI_Sentence_Reader::read_sentence_switch_id(QStringList read_dispatch)
{
 init_sentence(read_dispatch.last());
 current_prelim_ = "Sentence/switch";
}

void SDI_Sentence_Reader::read_Sentence_field(QString data, QStringList spl, QString field_style)
{
 typedef void (SDI_Sentence::*sfn_type_qsl)(QStringList);
 typedef void (SDI_Sentence::*sfn_type_numbers)(QStringList, QVector<s4>);

 typedef void (SDI_Sentence_Reader::*rfn_type_qsl)(QStringList);
 typedef void (SDI_Sentence_Reader::*rfn_type_numbers)(QStringList, QVector<s4>);

 union sfn_union {
   sfn_type_qsl _dot;
   sfn_type_numbers _hash;
 };

 union rfn_union {
   rfn_type_qsl _dot;
   rfn_type_numbers _hash;
 };

 QString key = spl.first() + field_style + spl.at(1);

// fn_union read_sentence_text = { ._qsl = &SDI_Sentence::read_sentence_text};
// fn_union read_sentence_gaps = { ._qsl = &SDI_Sentence::read_sentence_text};
// fn_union read_sentence_range = { ._nums = &SDI_Sentence::read_sentence_range};

 static QMap<QString, sfn_union> SDI_Sentence_map {{
   {"_end.t", { ._dot = &SDI_Sentence::read_sentence_text} },
   {"_end.g", { ._dot = &SDI_Sentence::read_sentence_gaps} },
//   {"start#r", { ._hash =  &SDI_Sentence::read_sentence_range} }
   {"_end#r", { ._hash =  &SDI_Sentence::read_sentence_range} }
   }};

 static QMap<QString, rfn_union> SDI_Sentence_Reader_map {{
   {"_end.id", { ._dot = &SDI_Sentence_Reader::read_sentence_end_id} },
   {"switch.id", { ._dot = &SDI_Sentence_Reader::read_sentence_switch_id} },
   //{"_end#r", { ._hash =  &SDI_Sentence::read_sentence_range} }
   }};


 auto it = SDI_Sentence_map.find(key);

 if(it == SDI_Sentence_map.end())
 {
  auto it1 = SDI_Sentence_Reader_map.find(key);
  if(it1 != SDI_Sentence_Reader_map.end())
  {
   switch (field_style[0].toLatin1())
   {
   case '.':  (this->*(it1.value()._dot))(spl << data); break;

//   case '.':  (current_sentence_->*(it.value()._dot))(spl << data); break;
//   case '#':  (current_sentence_->*(it.value()._hash))(spl, parse_numbers(data)); break;
   default: break;
   }
  }
 }
 else
 {
  switch (field_style[0].toLatin1())
  {
  case '.':  (current_sentence_->*(it.value()._dot))(spl << data); break;
  case '#':  (current_sentence_->*(it.value()._hash))(spl, parse_numbers(data)); break;
  default: break;
  }
 }
}



void SDI_Sentence_Reader::parse_line(QString line)
{
 if(line.isEmpty())
 {
  parse_blank_line();
  return;
 }

 if(line.startsWith("---"))
 {
  parse_prelim_line(line);
  return;
 }

 if(line.startsWith('|'))
 {
  parse_pipe_line(line);
  return;
 }

 if(line.startsWith(". "))
 {
  parse_dot_line(line);
  return;
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
   return;
  }
 }

 parse_data_line(2, line, simpptr);

}

void SDI_Sentence_Reader::parse_pipe_line(QString line)
{
 if(!pipe_acc_.isEmpty())
   pipe_acc_ += " ";

 pipe_acc_ += line.mid(3);
}


//void SDI_Sentence_Reader::sdi_check(QString gt_contents, QString out_path)
//{
// parse_sdi();
// KA::TextIO::save_file(out_path, gt_contents);
//}



