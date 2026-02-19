
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PHR_GRAPH_SIGNATURE__H
#define PHR_GRAPH_SIGNATURE__H

#include "accessors.h"
#include "flags.h"


#include <QString>

#include <functional>

#include "multigraph-token.h"
#include "relae-graph/relae-caon-ptr.h"

#include "rzns.h"


RZNS_(PhrGraphCore)

class PHR_Graph_Node;

class PHR_Graph_Signature
{
 caon_ptr<PHR_Graph_Node> fground_node_;

 QList<MG_Token> tokens_;

public:

 ACCESSORS(caon_ptr<PHR_Graph_Node> ,fground_node)
 ACCESSORS__RGET(QList<MG_Token> ,tokens)

 PHR_Graph_Signature(caon_ptr<PHR_Graph_Node> fground_node);

 void add_tokens(QList<MG_Token>& mgts);

};

_RZNS(PhrGraphCore)

#endif //PHR_GRAPH_SIGNATURE__H
