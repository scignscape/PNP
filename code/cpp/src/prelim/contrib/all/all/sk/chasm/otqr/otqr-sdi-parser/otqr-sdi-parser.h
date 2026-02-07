
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef OTQR_SDI_PARSER__H
#define OTQR_SDI_PARSER__H


#include "textio.h"

USING_KANS(TextIO)

#include "otns.h"

#include "sdi-sentence-reader.h"

OTNS_(SDI)

class OTQR_SDI_Parser
{
 QString file_path_;

 SDI_Sentence_Reader reader_;

public:

 ACCESSORS__RGET(SDI_Sentence_Reader ,reader)

 OTQR_SDI_Parser(QString file_path);

 void parse();
 void process_line(QString line);
};


_OTNS(SDI)


#endif //  OTQR_SDI_PARSER__H
