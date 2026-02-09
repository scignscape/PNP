
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PHR_GRAPH_TYPE_DECLARATION__H
#define PHR_GRAPH_TYPE_DECLARATION__H

#include "accessors.h"
#include "flags.h"

#include <QString>

#include <functional>


#include "relae-graph/relae-caon-ptr.h"

#include "rzns.h"


RZNS_(PhrGraphCore)


class PHR_Graph_Type_Declaration
{
 QString symbol_name_;
 QString type_name_;

public:

 ACCESSORS(QString ,symbol_name)
 ACCESSORS(QString ,type_name)

 PHR_Graph_Type_Declaration(QString symbol_name, QString type_name);

 PHR_Graph_Type_Declaration();
};

_RZNS(PhrGraphCore)

#endif //PHR_GRAPH_TYPE_DECLARATION__H
