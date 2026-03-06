
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "chtr-node-factory.h"

#include "kernel/graph/chtr-node.h"

USING_OTNS(Chasm_TR)

ChTR_Node_Factory::ChTR_Node_Factory()
  :  call_entry_count_(0), block_entry_count_(0), tuple_entry_count_(0)
{
}


caon_ptr<ChTR_Node> ChTR_Node_Factory::make_new_node(caon_ptr<ChTR_Source_Token> token) const
{
 caon_ptr<ChTR_Node> result = new ChTR_Node(token);
 RELAE_SET_NODE_LABEL(result, token->string_summary());
 return result;
}

caon_ptr<ChTR_Node> ChTR_Node_Factory::make_new_node(caon_ptr<ChTR_Statement_Body> csb) const
{
 caon_ptr<ChTR_Node> result = new ChTR_Node(csb);
 RELAE_SET_NODE_LABEL(result, "csb");
 return result;
}

caon_ptr<ChTR_Node> ChTR_Node_Factory::make_new_node(caon_ptr<ChTR_Proc_Token> token) const
{
 caon_ptr<ChTR_Node> result = new ChTR_Node(token);
 RELAE_SET_NODE_LABEL(result, token->string_summary());
 return result;
}

caon_ptr<ChTR_Node> ChTR_Node_Factory::make_new_node(caon_ptr<ChTR_Type_Object> cto) const
{
 caon_ptr<ChTR_Node> result = new ChTR_Node(cto);
 RELAE_SET_NODE_LABEL(result, cto->name());
 return result;
}

caon_ptr<ChTR_Node> ChTR_Node_Factory::make_new_node(caon_ptr<ChTR_Expression_Object> ceo) const
{
 caon_ptr<ChTR_Node> result = new ChTR_Node(ceo);
 RELAE_SET_NODE_LABEL(result, "expr:%1"_qt.arg(ceo->id()));
 return result;
}
