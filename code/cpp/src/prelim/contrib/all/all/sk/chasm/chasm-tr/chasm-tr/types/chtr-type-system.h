
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#ifndef CHTR_TYPE_SYSTEM__H
#define CHTR_TYPE_SYSTEM__H


#include "accessors.h"

#include "otns.h"

#include "flags.h"

#include <QFile>
#include <QVector>

//?#include "tsl/ordered_map.h"

#include "global-types.h"

#include <QVector>

OTNS_(Chasm_TR)


class ChTR_Type_Object;


class ChTR_Type_System
{
 QMap<QString, ChTR_Type_Object*> types_by_name_;

public:


 ChTR_Type_System();

 ChTR_Type_Object* find_type(QString name);


};

_OTNS(Chasm_TR)


#endif // CHTR_TYPE_SYSTEM__H




