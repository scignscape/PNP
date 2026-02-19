
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "phr-graph-cocyclic-type.h"

#include "kernel/graph/phr-graph-node.h"


#include "rzns.h"

USING_RZNS(PhrGraphCore)

PHR_Graph_Cocyclic_Type::PHR_Graph_Cocyclic_Type(QString type_name)
 :  type_name_(type_name), node_(nullptr)
{
}

void PHR_Graph_Cocyclic_Type::add_precycle_field(quint16 index, caon_ptr<PHR_Graph_Node> n)
{
 if(precycle_fields_.size() < index)
   precycle_fields_.resize(index);
 precycle_fields_[index - 1] = n;
}

void PHR_Graph_Cocyclic_Type::add_cocycle_field(quint16 index, caon_ptr<PHR_Graph_Node> n)
{
 if(cocycle_fields_.size() < index)
   cocycle_fields_.resize(index);
 cocycle_fields_[index - 1] = n;
}


caon_ptr<PHR_Graph_Type_Declaration> PHR_Graph_Cocyclic_Type::get_precycle_field(quint16 index)
{
 if(caon_ptr<PHR_Graph_Node> n = precycle_fields_.value(index))
 {
  return n->type_declaration();
 }
 return nullptr;
}

caon_ptr<PHR_Graph_Type_Declaration> PHR_Graph_Cocyclic_Type::get_cocycle_field(quint16 index)
{
 if(caon_ptr<PHR_Graph_Node> n = cocycle_fields_.value(index))
 {
  return n->type_declaration();
 }
 return nullptr;
}

