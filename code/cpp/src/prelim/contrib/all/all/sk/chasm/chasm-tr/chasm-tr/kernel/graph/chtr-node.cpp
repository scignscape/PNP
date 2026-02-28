
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "chtr-node.h"

USING_OTNS(Chasm_TR)


void ChTR_Node::debug_connections() const
{
 targets_iterator_type it(targets_);
 while(it.hasNext())
 {
  it.next();
  Frame_type* fkey = it.key();
  in_frame_targets_iterator_type fit = it.value();
  while(fit.hasNext())
  {
   fit.next();
   CAON_EVALUATE_DEBUG(ChTR_Connectors ,key ,fit.key())
   CAON_EVALUATE_DEBUG(ChTR_Node ,value ,fit.value())
   CAON_DEBUG_NOOP
  }
 }
}




