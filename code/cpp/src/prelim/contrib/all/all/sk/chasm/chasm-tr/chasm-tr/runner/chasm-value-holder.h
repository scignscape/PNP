
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef CHASM_VALUE_HOLDER__H
#define CHASM_VALUE_HOLDER__H

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

class Chasm_Runtime_Bridge;

class Chasm_Value_Holder
{
 Chasm_Type_Object* type_object_;

 n8 value_;

public:


 ACCESSORS(Chasm_Type_Object* ,type_object)
 ACCESSORS(n8 ,value)

 Chasm_Value_Holder(Chasm_Type_Object* type_object = nullptr, n8 value = 0);

};

_OTNS(Chasm_TR)

#endif // CHASM_VALUE_HOLDER__H
