
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "statement-generator.h"

#include "expression-generator.h"

#include "kernel/document/phr-graph-document.h"

#include "kernel/graph/phr-graph.h"
#include "kernel/graph/phr-graph-node.h"
#include "kernel/graph/phr-graph-connection.h"

#include "kernel/phr-graph-root.h"

#include "token/phr-graph-token.h"

#include "token/phr-graph-statement-info.h"
#include "token/phr-graph-type-declaration.h"

#include "token/phr-graph-signature.h"
#include "multigraph-token.h"

#include "textio.h"
USING_KANS(TextIO)

#include "rzns.h"
USING_RZNS(PhrGraphCore)


Statement_Generator::Statement_Generator(Expression_Generator& expression_generator)
 :  qy_(PHR_Graph_Query::instance()),
    expression_generator_(expression_generator)
{
 expression_generator_.set_statement_generator(this);
}

#define in_Gf PHR_Graph_Frame::instance(), //"graph"),

void Statement_Generator::generate_anchor_without_channel_group(QTextStream& qts,
 const PHR_Graph_Node& node, PHR_Graph_Statement_Info& sin)
{
 expression_generator_.check_generate_type_declaration(qts, node);

 if(caon_ptr<PHR_Graph_Token> tok = node.phr_graph_token())
 {
  qts << "\npush_carrier_stack $ " << sin.channel_name() << " ;.\n";
  expression_generator_.generate_carrier(qts, *tok);
  qts << "anchor_without_channel_group $ " <<
    sin.anchor_name() << ' ' << sin.channel_name() << " ;.\n";
 }
 qts << "reset_program_stack ;.\n";
}

void Statement_Generator::generate_from_node(QTextStream& qts,
 const PHR_Graph_Node& node, PHR_Graph_Statement_Info* sin)
{
 if(sin && (
   (sin->channel_name() == "parse-literal") ||
   (sin->channel_name() == "type-default") ) )
 {
  generate_anchor_without_channel_group(qts, node, *sin);
 }
 else if(caon_ptr<PHR_Graph_Node> sn = qy_.SignatuChasmRZ_Node(in_Gf &node))
 {
  if(caon_ptr<PHR_Graph_Token> tok = node.phr_graph_token())
  {
   CAON_PTR_DEBUG(PHR_Graph_Token ,tok)
   if(caon_ptr<PHR_Graph_Signature> sig = sn->phr_graph_signature())
   {
    generate_signature(qts, tok->raw_text(), *sig);
   }
  }
 }
 else if(caon_ptr<PHR_Graph_Cocyclic_Type> coy = node.cocyclic_type())
 {
  expression_generator_.generate_cocyclic_type_definition(qts, *coy);
 }
 else
 {
  expression_generator_.generate_from_node(qts, node);
  generate_close(qts, sin);
 }

 node.debug_connections();

 caon_ptr<PHR_Graph_Connection> cion;
 if(caon_ptr<PHR_Graph_Node> sen = qy_.Statement_Sequence[cion](in_Gf &node))
 {
  PHR_Graph_Statement_Info* sin1 = nullptr;
  if(cion)
    sin1 = cion->phr_node()->statement_info().raw_pointer();
  generate_from_node(qts, *sen, sin1);
 }
}

void Statement_Generator::generate_minimal_close(QTextStream& qts)
{
 qts << "delete_temps ;.\n"
  "delete_retired ;.\n"
  "clear_temps ;.\n"
  "reset_program_stack ;.\n"
  " .; end of statement ;.\n";
}

void Statement_Generator::generate_close(QTextStream& qts, PHR_Graph_Statement_Info* sin)
{
 qts << "coalesce_channel_group ;.\n";
 if(sin)
  qts << "copy_anchor_channel_group $ " <<
  sin->anchor_name() << ' ' << sin->channel_name() << " ;.\n";
 qts <<
  "evaluate_channel_group ;.\n"
  "delete_temps ;.\n"
  "delete_retired ;.\n"
  "clear_temps ;.\n"
  "reset_program_stack ;.\n"
  " .; end of statement ;.\n"
        ;
}

void Statement_Generator::generate_signature(QTextStream& qts,
  QString fn, PHR_Graph_Signature& sig)
{
 qts << "\n .; signature ... ;.\n";
 expression_generator_.generate_minimal_signature(qts, sig);
 qts << "finalize_signature $ " << fn << " ;.\n";
 generate_minimal_close(qts);
 qts << "\n .; end signature ... ;.\n\n";
}
