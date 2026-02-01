
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "phr-graph.h"

#include "phr-graph-node.h"

#include "kernel/phr-graph-root.h"

#include "token/phr-graph-token.h"

#include "kernel/graph/phr-graph-connection.h"

#include "rzns.h"

USING_RZNS(PhrGraphCore)

PHR_Graph::PHR_Graph(caon_ptr<PHR_Graph_Node> root_node)
 : node_graph<PHR_Graph_Dominion>(root_node)
{

}

void PHR_Graph::report(QTextStream& qts)
{
 report_from_node(qts, *root_node_);
}

void PHR_Graph::report_from_node(QTextStream& qts,
 const PHR_Graph_Node& node, int indent)
{
 QString padding(indent, ' ');
 qts << "\n" << padding;

 qts << "<<node/" << node.label() << ">>";

 node.each_connection([this, node, &qts, &padding, &indent]
  (PHR_Graph_Frame* fr, const PHR_Graph_Connectors& connector, const PHR_Graph_Node& target, const PHR_Graph_Connection* connection)
 {
  // //   For debugging...
  QString label = node.label();

  qts << "\n\n" << padding << "For connection: " << connector.label() << "\n"
      << padding << "==== ";

  if(connection)
  {
   qts << "\n\n" << padding << "Annotated: \n";
   qts << padding << "---- ";

  }

  report_from_node(qts, target, indent + 1);
  qts << "\n" << padding << "....";
 });
}
