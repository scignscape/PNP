
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PHR_GRAPH_ROOT__H
#define PHR_GRAPH_ROOT__H


#include "relae-graph/relae-node-ptr.h"

#include "phr-graph-dominion.h"

#include "rzns.h"

RZNS_(PhrGraphCore)

class PHR_Graph_Document;

class PHR_Graph_Root
{
 PHR_Graph_Document* document_;

public:

 PHR_Graph_Root(PHR_Graph_Document* document);

 QString document_path();
};

_RZNS(PhrGraphCore)


#endif //PHR_GRAPH_ROOT__H
