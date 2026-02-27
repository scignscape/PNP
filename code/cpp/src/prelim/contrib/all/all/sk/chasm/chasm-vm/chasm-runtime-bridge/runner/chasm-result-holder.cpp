
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "chasm-result-holder.h"

#include "chasm-lib/chasm/types/chasm-type-system.h"


USING_OTNS(Chasm_TR)


#include "textio.h"
USING_KANS(TextIO)

#include <QString>
#include <QVariant>
#include <QByteArray>

#include <QUrl>

#include <QPoint>

Chasm_Result_Holder::Chasm_Result_Holder(Chasm_Type_System* type_system, Chasm_Type_Object* type_object, n8 value)
  :  type_system_(type_system), type_object_(type_object), value_(value)
{

}


Chasm_Result_Holder& Chasm_Result_Holder::type_u1()
{
 set_type_object(type_system_->the_u1_type());
 return *this;
}

Chasm_Result_Holder& Chasm_Result_Holder::type_s1()
{
 set_type_object(type_system_->the_s1_type());
 return *this;
}

Chasm_Result_Holder& Chasm_Result_Holder::type_u2()
{
 set_type_object(type_system_->the_u2_type());
 return *this;
}

Chasm_Result_Holder& Chasm_Result_Holder::type_s2()
{
 set_type_object(type_system_->the_s2_type());
 return *this;
}

Chasm_Result_Holder& Chasm_Result_Holder::type_u4()
{
 set_type_object(type_system_->the_u4_type());
 return *this;
}

Chasm_Result_Holder& Chasm_Result_Holder::type_s4()
{
 set_type_object(type_system_->the_s4_type());
 return *this;
}

Chasm_Result_Holder& Chasm_Result_Holder::type_u8()
{
 set_type_object(type_system_->the_u8_type());
 return *this;
}

Chasm_Result_Holder& Chasm_Result_Holder::type_s8()
{
 set_type_object(type_system_->the_s8_type());
 return *this;
}
