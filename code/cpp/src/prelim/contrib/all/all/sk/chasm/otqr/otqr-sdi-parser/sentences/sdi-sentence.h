
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef SDI_SENTENCE__H
#define SDI_SENTENCE__H


#include "accessors.h"

#include "global-types.h"

#include "flags.h"

#include "otns.h"

#include <QStringList>

OTNS_(SDI)


class GTagML_Parser;
class GTagML_Output_Blocks;


class SDI_Sentence
{
 u4 id_;
 QString sentence_text_;


public:

 SDI_Sentence(u4 id);

 ACCESSORS(QString ,sentence_text)
 ACCESSORS(u4 ,id)

 u2 search_for(QString search_text);

 void read_sentence_text(QStringList read_dispatch);
 void read_sentence_gaps(QStringList read_dispatch);

 void read_sentence_end_id(QStringList read_dispatch);

 void read_sentence_range(QStringList read_dispatch, QVector<s4> numbers);

 void read_sentence_gaps(QString lines);
};

_OTNS(SDI)


#endif
