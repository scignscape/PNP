
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "chasm-runner--procs2.h"

#include "chasm-lib/chasm/types/chasm-type-system.h"
#include "chasm-lib/chasm/types/chasm-type-object.h"

#include "../chasm-result-holder.h"

#include <functional>

OTNS_(Chasm_TR)

void div2(Chasm_Result_Holder& rh, u1 type_codes, n8 lhs, n8 rhs)
{
 //Chasm_Type_System* cts = rh.type_system();
 //bool signed_result =

 u1 signed_mask = Chasm_Type_Object::signed_unmap(type_codes);

 std::function<s8()> _div_4[4] = {
  [=](){return (s8) ( (u4) lhs / (u4) rhs );},  //  signed_mask = 0
  [=](){return (s8) ( (u4) lhs / (s4) rhs );},  //  signed_mask = 1
  [=](){return (s8) ( (s4) lhs / (u4) rhs );},  //  signed_mask = 2
  [=](){return (s8) ( (s4) lhs / (s4) rhs );}
 }; //  signed_mask = 3

 auto div_4 = [=]() { return _div_4[signed_mask](); };

 switch(type_codes)
 {
 case 11: rh.type_uors1(signed_mask).set_value((u1) div_4()); break;

 case 12:
 case 21:
 case 22: rh.type_uors2(signed_mask).set_value((u2) div_4()); break;

// case 12:
// case 21:
// case 22: rh.type_uors2(signed_mask).set_value((u2) add_4()); break;

 default: break;


 }

}

_OTNS(Chasm_TR)
