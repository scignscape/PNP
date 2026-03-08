
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "chtr-chvm-line.h"



USING_OTNS(Chasm_TR)


ChTR_CHVM_Line::ChTR_CHVM_Line(s4 line_id, QString text)
  : line_id_(line_id), move_offset_(0), text_(text)
{
}

ChTR_CHVM_Line* ChTR_CHVM_Line::clone(s4 offset)
{
 ChTR_CHVM_Line* result = new ChTR_CHVM_Line(line_id_, text_);
 result->move_offset_ = offset;
 result->preambles_ = preambles_;
 return result;
}

void ChTR_CHVM_Line::pre(QStringList p)
{
 preambles_.append(p);
}

void ChTR_CHVM_Line::pre_blank()
{
 pre({"\n"});
}


QString ChTR_CHVM_Line::get_text()
{
 if(preambles_.isEmpty())
   return text_;

 return preambles_.join("") + text_;
}
