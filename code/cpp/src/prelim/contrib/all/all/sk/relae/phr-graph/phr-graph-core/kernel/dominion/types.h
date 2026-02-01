
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#define DOMINION_NODE_TYPE PHR_Graph_Node

//#ifndef DOMINION_HIDE_NO_NAMESPACE
//// // No namespace
//#include "relae-graph/dominion-macros.h"
//DOMINION_TYPE(dbl, double, Double)
//DOMINION_TYPE(integer, int, Int)
//DOMINION_TYPE(i8, char, I8)
//DOMINION_TYPE(qstring, QString, QStr)
//#endif
//

#include "relae-graph/dominion-macros.h"

#define DOMINION_OUTER_NAMESPACE RZ

#define DOMINION_INNER_NAMESPACE PhrGraphCore
#include "relae-graph/dominion-macros.h"
DOMINION_TYPE(phr_graph_root, PHR_Graph_Root, PHR_Graph_Root)
DOMINION_TYPE(phr_graph_token, PHR_Graph_Token, PHR_Graph_Token)
DOMINION_TYPE(fground_entry, PHR_Graph_FGround_Entry, PHR_Graph_FGround_Entry)
DOMINION_TYPE(statement_info, PHR_Graph_Statement_Info, PHR_Graph_Statement_Info)
DOMINION_TYPE(block_info, PHR_Graph_Block_Info, PHR_Graph_Block_Info)
DOMINION_TYPE(type_declaration, PHR_Graph_Type_Declaration, PHR_Graph_Type_Declaration)
DOMINION_TYPE(phr_graph_signature, PHR_Graph_Signature, PHR_Graph_Signature)
DOMINION_TYPE(cocyclic_type, PHR_Graph_Cocyclic_Type, PHR_Graph_Cocyclic_Type)
#undef DOMINION_INNER_NAMESPACE

#undef DOMINION_OUTER_NAMESPACE
#undef DOMINION_NODE_TYPE
