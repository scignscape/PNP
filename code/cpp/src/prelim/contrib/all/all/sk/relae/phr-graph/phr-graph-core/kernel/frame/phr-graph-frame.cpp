
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "phr-graph-frame.h"

#include "kernel/graph/phr-graph-node.h"

#include "rzns.h"

#include <QDebug>


USING_RZNS(PhrGraphCore)

PHR_Graph_Frame::PHR_Graph_Frame()
 : node_frame<PHR_Graph_Dominion>()
{


}

PHR_Graph_Frame& PHR_Graph_Frame::instance()
{
 static PHR_Graph_Frame* the_instance = nullptr;
 if(!the_instance)
  the_instance = new PHR_Graph_Frame;
 return *the_instance;
}
