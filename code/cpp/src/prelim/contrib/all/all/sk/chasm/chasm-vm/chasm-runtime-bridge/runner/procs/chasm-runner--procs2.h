
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef CHASM_RUNNER__PROCS2__H
#define CHASM_RUNNER__PROCS2__H



#include "global-types.h"
#include "otns.h"


OTNS_(Chasm_TR)

class Chasm_Result_Holder;

void add2(Chasm_Result_Holder& rh, u1 type_codes, n8 lhs, n8 rhs);
void div2(Chasm_Result_Holder& rh, u1 type_codes, n8 lhs, n8 rhs);

_OTNS(Chasm_TR)

#endif // CHASM_RUNNER__ADD_2__H
