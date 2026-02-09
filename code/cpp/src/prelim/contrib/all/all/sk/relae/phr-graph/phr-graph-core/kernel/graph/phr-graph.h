
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PHR_GRAPH__H
#define PHR_GRAPH__H


#include "relae-graph/relae-node-ptr.h"

#include "phr-graph-core/kernel/phr-graph-dominion.h"

#include <QTextStream>

#include "rzns.h"

RZNS_(PhrGraphCore)


class PHR_Graph : public node_graph<PHR_Graph_Dominion>
{
 public:
  PHR_Graph(caon_ptr<PHR_Graph_Node> root_node = nullptr);

 void report(QTextStream& qts);
 void report_from_node(QTextStream& qts,
  const PHR_Graph_Node& node, int indent = 0);

};

_RZNS(PhrGraphCore)



#endif // PHR_GRAPH__H
