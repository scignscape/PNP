
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#ifndef CHTR_TYPE_OBJECT__H
#define CHTR_TYPE_OBJECT__H


#include "accessors.h"

#include "otns.h"

#include "flags.h"

#include <QFile>
#include <QVector>

//?#include "tsl/ordered_map.h"

#include "global-types.h"

#include <QVector>

OTNS_(Chasm_TR)


class ChTR_Carrier;


class ChTR_Type_Object
{
public:

 flags_(1)
  bool built_in:1;
  bool pointer:1;
  bool reference:1;
 _flags

 QString name_;

public:


 ChTR_Type_Object(QString name = {}, bool built_in = false);

 ACCESSORS(QString ,name)


};

_OTNS(Chasm_TR)


#endif // CHTR_TYPE_OBJECT__H




