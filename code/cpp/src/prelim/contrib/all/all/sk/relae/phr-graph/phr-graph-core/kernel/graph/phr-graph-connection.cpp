
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "phr-graph-connection.h"


#include "rzns.h"
USING_RZNS(PhrGraphCore)

PHR_Graph_Connection::PHR_Graph_Connection(caon_ptr<PHR_Graph_Node> phr_node)
 : phr_nodes_({phr_node})
{

}

PHR_Graph_Connection::PHR_Graph_Connection(QString channel_name,
  caon_ptr<PHR_Graph_Node> phr_node)
 : channel_name_(channel_name)
{
 if(phr_node)
   phr_nodes_ = {phr_node};
}

caon_ptr<PHR_Graph_Node> PHR_Graph_Connection::phr_node(int i)
{
 return phr_nodes_.value(i, nullptr);
}

void PHR_Graph_Connection::add_node(caon_ptr<PHR_Graph_Node> n)
{
 phr_nodes_.push_back(n);
}

