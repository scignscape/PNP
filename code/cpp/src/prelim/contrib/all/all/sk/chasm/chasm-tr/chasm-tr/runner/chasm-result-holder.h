
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef CHASM_RESULT_HOLDER__H
#define CHASM_RESULT_HOLDER__H

#include <QString>

#include <QStack>
#include <QVariant>

#include <QDebug>

#include "chasm-lib/chasm/chasm-carrier.h"


#include "accessors.h"
#include "flags.h"

#include "global-types.h"
#include "otns.h"


OTNS_(Chasm_TR)


class Chasm_Type_Object;
class Chasm_Type_System;

class Chasm_Runtime_Bridge;

class Chasm_Result_Holder
{
 Chasm_Type_System* type_system_;
 Chasm_Type_Object* type_object_;

 n8 value_;

public:

// enum class Sign_Status { N_A, NN, NS, NU,
//   SN, SS, SU,  UN, US, UU
//   // s = signed and positive (or zero);
//   // n = signed and negative; u = unsigned
// };

 ACCESSORS__GET(Chasm_Type_System* ,type_system)
 ACCESSORS(Chasm_Type_Object* ,type_object)
 ACCESSORS(n8 ,value)


 Chasm_Result_Holder(Chasm_Type_System* type_system, Chasm_Type_Object* type_object = nullptr, n8 value = 0);

 Chasm_Result_Holder& type_u1();
 Chasm_Result_Holder& type_s1();
 template<typename MASK_Type>
 Chasm_Result_Holder& type_uors1(MASK_Type mask)
 {
  if(mask) return type_s1(); return type_u1();
 }

 Chasm_Result_Holder& type_u2();
 Chasm_Result_Holder& type_s2();
 template<typename MASK_Type>
 Chasm_Result_Holder& type_uors2(MASK_Type mask)
 {
  if(mask) return type_s2(); return type_u2();
 }

 Chasm_Result_Holder& type_u4();
 Chasm_Result_Holder& type_s4();
 template<typename MASK_Type>
 Chasm_Result_Holder& type_uors4(MASK_Type mask)
 {
  if(mask) return type_s4(); return type_u4();
 }

 Chasm_Result_Holder& type_u8();
 Chasm_Result_Holder& type_s8();
 template<typename MASK_Type>
 Chasm_Result_Holder& type_uors8(MASK_Type mask)
 {
  if(mask) return type_s8(); return type_u8();
 }

};

_OTNS(Chasm_TR)

#endif // CHASM_RESULT_HOLDER__H
