
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PHR_GRAPH_QUERY__H
#define PHR_GRAPH_QUERY__H

#include "phr-graph-core/kernel/phr-graph-dominion.h"

#include "relae-graph/relae-node-ptr.h"

#include "rzns.h"

RZNS_(PhrGraphCore)


class PHR_Graph_Query : public node_query<PHR_Graph_Dominion>
{
 PHR_Graph_Query();

 public:
  #define DOMINION_CONNECTOR(name, label) \
   PHR_Graph_Connectors name;
  #include "phr-graph-core/kernel/dominion/connectors.h"
  #undef DOMINION_CONNECTOR

 static const PHR_Graph_Query& instance();
};

_RZNS(PhrGraphCore)

#endif //PHR_GRAPH_QUERY__H
