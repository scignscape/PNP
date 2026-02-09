
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PHR_GRAPH_FRAME__H
#define PHR_GRAPH_FRAME__H


#include "relae-graph/relae-node-ptr.h"

#include "phr-graph-core/kernel/phr-graph-dominion.h"

#include "rzns.h"

RZNS_(PhrGraphCore)

class PHR_Graph_Frame : public node_frame<PHR_Graph_Dominion>
{
 PHR_Graph_Frame();

 public:

 static PHR_Graph_Frame& instance();

};

_RZNS(PhrGraphCore)

#endif //RZ_PHR_Graph_FRAME__H
