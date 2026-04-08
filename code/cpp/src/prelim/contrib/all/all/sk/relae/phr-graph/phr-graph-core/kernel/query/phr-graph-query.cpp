
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "phr-graph-query.h"

#include "rzns.h"


USING_RZNS(PhrGraphCore)


PHR_Graph_Query::PHR_Graph_Query()
 : node_query<PHR_Graph_Dominion>()
  #define DOMINION_CONNECTOR(name, label) \
    ,name(PHR_Graph_Connectors(PHR_Graph_Connectors_Case_Labels::name, label))
  #include "kernel/dominion/connectors.h"
  #undef DOMINION_CONNECTOR
{
 int order = 0;
 #define DOMINION_CONNECTOR(name, label) \
  name.order = order; \
  ++order;
 #include "kernel/dominion/connectors.h"
 #undef DOMINION_CONNECTOR
}

const PHR_Graph_Query& PHR_Graph_Query::instance()
{
 static PHR_Graph_Query* the_instance = nullptr;
 if(!the_instance)
  the_instance = new PHR_Graph_Query;
 return *the_instance;
}
