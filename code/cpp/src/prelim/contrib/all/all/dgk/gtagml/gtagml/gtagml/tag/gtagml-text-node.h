
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef GTAGML_TEXT_NODE__H
#define GTAGML_TEXT_NODE__H

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

class GTagML_Text_Node
{
public:


private:

 QString contents_;


public:

 GTagML_Text_Node(QString contents);

};

_KANS(GTagML)

#endif //  GTAGML_TEXT_NODE__H
