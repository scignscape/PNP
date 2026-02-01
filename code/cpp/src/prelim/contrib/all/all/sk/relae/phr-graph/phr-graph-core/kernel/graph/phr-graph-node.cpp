
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "phr-graph-node.h"

#include "token/phr-graph-token.h"

#include "rzns.h"

USING_RZNS(PhrGraphCore)

void PHR_Graph_Node::each_connection(std::function<void(PHR_Graph_Frame* fr, const PHR_Graph_Connectors& connector,
 const PHR_Graph_Node&, const PHR_Graph_Connection* connection)> fn) const
{
 targets_iterator_type it(targets_);
 while(it.hasNext())
 {
  it.next();

  in_frame_targets_iterator_type itf(it.value());

  while(itf.hasNext())
  {
   itf.next();
   const PHR_Graph_Connectors& connector = *itf.key();
   const PHR_Graph_Node& target = *itf.value();
   fn(it.key(), connector, target, nullptr);
  }
 }

 annotated_targets_iterator_type ait(annotated_targets_);
 while(ait.hasNext())
 {
  ait.next();

  in_frame_annotated_targets_iterator_type aitf(ait.value());

  while(aitf.hasNext())
  {
   aitf.next();

   const PHR_Graph_Connectors& connector = *aitf.key();
   const PHR_Graph_Connection* connection = aitf.value().first.raw_pointer();
   const PHR_Graph_Node& target = *aitf.value().second;
   fn(ait.key(), connector, target, connection);
  }
 }

}


void PHR_Graph_Node::debug_connections() const
{
 targets_iterator_type it(targets_);
 while(it.hasNext())
 {
  it.next();

  in_frame_targets_iterator_type itf(it.value());

  while(itf.hasNext())
  {
   itf.next();
   CAON_EVALUATE_DEBUG(PHR_Graph_Connectors ,key ,itf.key())
   CAON_EVALUATE_DEBUG(PHR_Graph_Node ,value ,itf.value())
   CAON_DEBUG_NOOP
  }
 }

 annotated_targets_iterator_type ait(annotated_targets_);
 while(ait.hasNext())
 {
  ait.next();

  in_frame_annotated_targets_iterator_type aitf(ait.value());

  while(aitf.hasNext())
  {
   aitf.next();

   CAON_EVALUATE_DEBUG(PHR_Graph_Connectors ,key ,aitf.key())
   CAON_EVALUATE_DEBUG(PHR_Graph_Node ,value ,aitf.value().second)
   CAON_DEBUG_NOOP
  }
 }


}

void PHR_Graph_Node::add_hyponode(caon_ptr<PHR_Graph_Node> n)
{
 hyponodes_.push_back(n);
}


void PHR_Graph_Node::swap_relation(PHR_Graph_Frame* fr, const PHR_Graph_Connectors& connector,
 caon_ptr<PHR_Graph_Node> n1, caon_ptr<PHR_Graph_Node> n2)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,n1)
 CAON_PTR_DEBUG(PHR_Graph_Node ,n2)


 #ifdef NO_CAON
   PHR_Graph_Connectors* pc = const_cast<PHR_Graph_Connectors*>( &connector );
   targets_.remove(pc, n1);
   targets_.insert(pc, n2);
 #else
  targets_[fr].remove(&connector, n1);
  targets_[fr].insert(&connector, n2);
 #endif //NO_CAON

}


void PHR_Graph_Node::delete_relation(PHR_Graph_Frame* fr, const PHR_Graph_Connectors& connector,
 caon_ptr<PHR_Graph_Node> n1)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,n1)
  #ifdef NO_CAON
    PHR_Graph_Connectors* pc = const_cast<PHR_Graph_Connectors*>( &connector );
    targets_.remove(pc, n1);
  #else
   targets_[fr].remove(&connector, n1);
  #endif //NO_CAON
}

