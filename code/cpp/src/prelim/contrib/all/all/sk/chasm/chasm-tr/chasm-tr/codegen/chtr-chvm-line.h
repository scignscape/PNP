
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef ChTR_CHVM_LINE__H
#define ChTR_CHVM_LINE__H

#include "global-types.h"

#include "otns.h"
OTNS_(Chasm_TR)


class ChTR_CHVM_Line
{
 friend class ChTR_CHVM_Generator;

 s4 line_id_;
 s4 move_offset_;

 QStringList preambles_;

 QString text_;

public:

 ChTR_CHVM_Line(s4 line_id, QString text);

 ChTR_CHVM_Line* clone(s4 offset);

 void pre(QStringList p);
 void pre_blank();

 QString get_text();

};


_OTNS(Chasm_TR)

#endif //  ChTR_CHVM_LINE

