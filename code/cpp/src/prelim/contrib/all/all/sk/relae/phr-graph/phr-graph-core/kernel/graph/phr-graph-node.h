
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PHR_GRAPH_NODE__H
#define PHR_GRAPH_NODE__H

#include "relae-graph/relae-node-ptr.h"

#include "phr-graph-core/kernel/phr-graph-dominion.h"

#include "phr-graph-core/kernel/frame/phr-graph-frame.h"

#include "accessors.h"

#include <functional>

#include <QVector>

#include "rzns.h"


RZNS_(PhrGraphCore)

class PHR_Graph_Token;
class PHR_Graph_Statement_Info;
class PHR_Graph_FGround_Entry;
class PHR_Graph_Block_Info;
class PHR_Graph_Type_Declaration;
class PHR_Graph_Signature;
class PHR_Graph_Cocyclic_Type;

class PHR_Graph_Node : public node_ptr<PHR_Graph_Dominion>
{

 QVector<caon_ptr<PHR_Graph_Node>> hyponodes_;
 caon_ptr<PHR_Graph_Node> parent_;

public:

 #define DOMINION_TYPE DOMINION_NODE_CONSTRUCTOR
 //??
#include "kernel/dominion/types.h"
 #undef DOMINION_TYPE

// PHR_Graph_Node(caon_ptr<RZ::PhrGraphCore::PHR_Graph_Token> v)
//  : PHR_Graph_Node::node_ptr_base_type(v) {}

 ACCESSORS__RGET(QVector<caon_ptr<PHR_Graph_Node>> ,hyponodes)
 ACCESSORS(caon_ptr<PHR_Graph_Node> ,parent)

 void debug_connections() const;

 void each_connection(std::function<void(PHR_Graph_Frame* fr, const PHR_Graph_Connectors& connector,
  const PHR_Graph_Node&, const PHR_Graph_Connection* connection)> fn)  const;

 void swap_relation(PHR_Graph_Frame* fr, const PHR_Graph_Connectors& connector,
  caon_ptr<PHR_Graph_Node> n1, caon_ptr<PHR_Graph_Node> n2);

 void delete_relation(PHR_Graph_Frame* fr, const PHR_Graph_Connectors& connector,
  caon_ptr<PHR_Graph_Node> n1);

 void add_hyponode(caon_ptr<PHR_Graph_Node> n);

};

_RZNS(PhrGraphCore)


#endif //PHR_GRAPH_NODE__H
