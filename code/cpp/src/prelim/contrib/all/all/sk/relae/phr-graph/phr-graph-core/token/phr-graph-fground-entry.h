
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PHR_GRAPH_FGROUND_ENTRY__H
#define PHR_GRAPH_FGROUND_ENTRY__H

#include "accessors.h"
#include "flags.h"

#include <QString>

#include <functional>


#include "relae-graph/relae-caon-ptr.h"

#include "rzns.h"


RZNS_(PhrGraphCore)


class PHR_Graph_FGround_Entry
{
 QString channel_name_;
 QString result_type_name_;
 int depth_;

public:

 ACCESSORS(QString ,channel_name)
 ACCESSORS(QString ,result_type_name)

 PHR_Graph_FGround_Entry(QString channel_name, QString result_type_name);

};

_RZNS(PhrGraphCore)

#endif //PHR_GRAPH_FGROUND_ENTRY
