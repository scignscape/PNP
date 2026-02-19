
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "sdi-sentence.h".h"


// #include "gtagml-document-light-xml.h"

#include <QFile>
#include <QTextStream>

#include "kans.h"

USING_KANS(GTagML)


SDI_Sentence::SDI_Sentence(u4 id)
  :  id_(id)
{
}


void SDI_Sentence::read_sentence_range(QStringList read_dispatch, QVector<s4> numbers)
{

}


void SDI_Sentence::read_sentence_text(QStringList read_dispatch)
{
 if(read_dispatch.takeFirst() == "_end")
   set_sentence_text(read_dispatch.last());
}

void SDI_Sentence::read_sentence_gaps(QStringList read_dispatch)
{
 if(read_dispatch.takeFirst() == "_end")
   read_sentence_gaps(read_dispatch.last());
}

void SDI_Sentence::read_sentence_gaps(QString lines)
{

}


