
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "phr-graph-dominion.h"

#include "graph/phr-graph-node.h"

#define DOMINION_NODE_NAMESPACE RZ::PhrGraphCore
#define DOMINION_TYPE DOMINION_RETRIEVE_TYPE_CODE
#include "dominion/types.h"
#undef DOMINION_TYPE
#undef DOMINION_NODE_NAMESPACE
