

//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#include "chtr-proc-token.h"


USING_OTNS(Chasm_TR)


ChTR_Proc_Token::ChTR_Proc_Token(u4 line_number, QString text, u2 infix_rank)
  :  line_number_(line_number), text_(text), infix_rank_(infix_rank), operator_node_(nullptr), infix_nesting_level_(0)
{

}



