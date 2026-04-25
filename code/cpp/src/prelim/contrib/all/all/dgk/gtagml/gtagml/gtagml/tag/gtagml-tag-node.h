
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef GTAGML_TAG_NODE__H
#define GTAGML_TAG_NODE__H

#include <QString>
#include <QPair>

#include <functional>

#include "accessors.h"
#include "flags.h"


#include "global-types.h"

#include "tsl/ordered_map.h"

using namespace tsl;


#include "kans.h"


KANS_(GTagML)

class GTagML_Subdocument;

class GTagML_TAG_Node
{
public:


private:

 ordered_map<QString, GTagML_Subdocument*> named_subdocuments_;

 QVector<GTagML_Subdocument*> subdocuments_;


public:

 GTagML_TAG_Node();

};

_KANS(GTagML)

#endif //  GTAGML_TAG_NODE__H
