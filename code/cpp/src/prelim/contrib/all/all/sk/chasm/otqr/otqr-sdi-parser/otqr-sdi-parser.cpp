

//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#include "otqr-sdi-parser.h"

USING_OTNS(SDI)


OTQR_SDI_Parser::OTQR_SDI_Parser(QString file_path)
  : file_path_(file_path)
{
}

void OTQR_SDI_Parser::process_line(QString line)
{
 // //  qDebug() << line;
 reader_.parse_line(line);
}

void OTQR_SDI_Parser::parse()
{
 QFile infile(file_path_);
 if (!infile.open(QIODevice::ReadOnly | QIODevice::Text))
   return;

 QTextStream instream(&infile);

 while(!instream.atEnd())
 {
  QString line = instream.readLine();
  process_line(line);
 }

 reader_.set_file_path(file_path_);
}

