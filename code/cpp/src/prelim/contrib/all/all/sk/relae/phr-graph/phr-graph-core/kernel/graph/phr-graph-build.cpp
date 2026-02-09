
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "phr-graph-build.h"

#include "phr-graph.h"

#include "kernel/phr-graph-root.h"

#include "token/phr-graph-token.h"

#include "token/phr-graph-block-info.h"

#include "token/phr-graph-cocyclic-type.h"

#include "kernel/graph/phr-graph-connection.h"

#include "phr-graph-core/kernel/phr-graph-root.h"

#include "kernel/document/phr-graph-document.h"
#include "kernel/graph/phr-graph-node.h"

#include "phr-graph-core/kernel/frame/phr-graph-frame.h"
#include "phr-graph-core/kernel/query/phr-graph-query.h"

#include "phr-graph-core/token/phr-graph-statement-info.h"
#include "phr-graph-core/token/phr-graph-fground-entry.h"

#include "phr-graph-core/token/phr-graph-type-declaration.h"

#include "phr-graph-core/token/phr-graph-signature.h"

#include "pgb-ir-run.h"

#include "multigraph-token.h"

#include <QDebug>

#include "rzns.h"

USING_RZNS(PhrGraphCore)

PHR_Graph_Build::PHR_Graph_Build(PHR_Graph& graph)
 : graph_(graph), current_node_(nullptr),
   last_statement_entry_node_(nullptr),
   last_block_pre_entry_node_(nullptr),
   last_expression_entry_node_(nullptr),
   held_type_declaration_node_(nullptr),
   last_block_entry_node_(nullptr),
   prior_block_entry_node_(nullptr),
   prior_expression_entry_node_(nullptr),
   current_cocyclic_type_(nullptr),
   Gf(PHR_Graph_Frame::instance()), //"graph")),
   qy_(PHR_Graph_Query::instance())
{

}

#define in_Gf Gf,


void PHR_Graph_Build::load_from_pgb_file(QString file)
{
 PGB_IR_Run pgbr(*this);
 pgbr.run_from_file(file);
}

caon_ptr<PHR_Graph_Node> PHR_Graph_Build::make_root_node()
{
 caon_ptr<PHR_Graph_Document> doc = new PHR_Graph_Document(file_);

 caon_ptr<PHR_Graph_Root> rt = new PHR_Graph_Root(doc.raw_pointer());
 caon_ptr<PHR_Graph_Node> result = new PHR_Graph_Node(rt);

 graph_.set_root_node(result);

 result->set_label("<root>");

 return result;

}

caon_ptr<PHR_Graph_Node> PHR_Graph_Build::add_type_declaration(
   MG_Token& amgt, MG_Token& tmgt)
{
 QString sym = amgt.raw_text;
 QString ty = tmgt.raw_text;

 caon_ptr<PHR_Graph_Type_Declaration> td = new
   PHR_Graph_Type_Declaration(sym, ty);
 caon_ptr<PHR_Graph_Node> result = new PHR_Graph_Node(td);
 result->set_label(QString("<type-decl:%1%2>").arg(sym).arg(ty));

 held_type_declaration_node_ = result;

 return result;

}


caon_ptr<PHR_Graph_Node> PHR_Graph_Build::add_indexed_type_declaration(
  MG_Token& imgt, MG_Token& amgt, MG_Token& tmgt, caon_ptr<PHR_Graph_Node> sn)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,sn)

 QString index = imgt.raw_text;
 QString sym = amgt.raw_text;
 QString ty = tmgt.raw_text;

 QString uty_name;
 QString decl_mode;

 if(index.contains('!'))
 {
  QStringList qsl = index.split('!');
  uty_name = qsl[0];
  index = qsl[1];
 }

 if(index.contains(':'))
 {
  QStringList qsl = index.split(':');
  decl_mode = qsl[0];
  index = qsl[1];
 }

 if(!uty_name.isEmpty())
 {
  current_cocyclic_type_ = new PHR_Graph_Cocyclic_Type(uty_name);
  caon_ptr<PHR_Graph_Node> cn = new PHR_Graph_Node(current_cocyclic_type_);
  current_cocyclic_type_->set_node(cn);
  add_statement_sequence_node(sn, cn);
 }

 caon_ptr<PHR_Graph_Type_Declaration> td = new
   PHR_Graph_Type_Declaration(sym, ty);

 caon_ptr<PHR_Graph_Node> result = new PHR_Graph_Node(td);
 result->set_label(QString("<type-decl:%1%2>").arg(sym).arg(ty));

 if(decl_mode == "Pr")
   current_cocyclic_type_->add_precycle_field(index.toInt(), result);
 else if(decl_mode == "Co")
   current_cocyclic_type_->add_cocycle_field(index.toInt(), result);

 // //  expect this always to be defined ...
 if(current_cocyclic_type_)
   return current_cocyclic_type_->node();

 // //  i.e., we shouldn't be here ...
 return result;
}

void PHR_Graph_Build::PHR_Graph_Build::finalize_block_signature(
  caon_ptr<PHR_Graph_Node> bin_node)
{
 if(caon_ptr<PHR_Graph_Block_Info> bin = bin_node->block_info())
 {
  CAON_PTR_DEBUG(PHR_Graph_Block_Info ,bin)
  if(caon_ptr<PHR_Graph_Node> sn = qy_.SignatuChasmRZ_Node(in_Gf bin_node))
  {
   if(caon_ptr<PHR_Graph_Signature> sig = sn->phr_graph_signature())
   {
    bin->set_signature(sig.raw_pointer());
   }
  }
  CAON_DEBUG_NOOP
 }
}

caon_ptr<PHR_Graph_Node> PHR_Graph_Build::make_signatuChasmRZ_Node(
  caon_ptr<PHR_Graph_Node> token_node)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,token_node)

 caon_ptr<PHR_Graph_Signature> sig = new PHR_Graph_Signature(token_node);
 caon_ptr<PHR_Graph_Node> result = new PHR_Graph_Node(sig);

 token_node << Gf/qy_.SignatuChasmRZ_Node >> result;

 return result;
}

caon_ptr<PHR_Graph_Node> PHR_Graph_Build::make_token_node(MG_Token& mgt)
{
 bool rv = ((mgt.kind == MG_Token_Kinds::Raw_Value)
   || (mgt.kind == MG_Token_Kinds::Arg_Raw_Value)
   || (mgt.kind == MG_Token_Kinds::String_Literal)
   );

 bool sl = false;
 QString rt = mgt.raw_text;
 if(rv && rt.startsWith('"'))
 {
  sl = true;
  rt = rt.mid(1);
  if(rt.endsWith('"'))
    rt.chop(1);
 }

 caon_ptr<PHR_Graph_Token> tok = new PHR_Graph_Token(rt);
 if(rv)
   tok->flags.gen_raw_value = true;
 if(sl)
   tok->flags.is_string_literal = true;

 caon_ptr<PHR_Graph_Node> result = new PHR_Graph_Node(tok);
 result->set_label(rt);
 CAON_PTR_DEBUG(PHR_Graph_Node ,result)

 return result;
}

caon_ptr<PHR_Graph_Node> PHR_Graph_Build::add_channel_token(caon_ptr<PHR_Graph_Node> source,
  MG_Token& mgt)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,source)

 caon_ptr<PHR_Graph_Node> result = make_token_node(mgt);

 source << Gf/qy_.Channel_Sequence >> result;

 return result;
}


caon_ptr<PHR_Graph_Node> PHR_Graph_Build::add_channel_continue_token(caon_ptr<PHR_Graph_Node> source,
  MG_Token& mgt)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,source)

 caon_ptr<PHR_Graph_Node> result = make_token_node(mgt);

 source << Gf/qy_.Channel_Continue >> result;

 return result;
}


caon_ptr<PHR_Graph_Node> PHR_Graph_Build::add_channel_entry_token(caon_ptr<PHR_Graph_Node> source,
  QString channel, MG_Token& mgt)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,source)

 caon_ptr<PHR_Graph_Node> result = make_token_node(mgt);

 caon_ptr<PHR_Graph_Connection> cion = new PHR_Graph_Connection(channel);
 source << Gf/qy_.Channel_Entry(cion) >> result;

 return result;
}

caon_ptr<PHR_Graph_Node> PHR_Graph_Build::add_channel_xentry_token(caon_ptr<PHR_Graph_Node> source,
  QString channel, MG_Token& mgt)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,source)

 caon_ptr<PHR_Graph_Node> result = make_token_node(mgt);

 caon_ptr<PHR_Graph_Connection> cion = new PHR_Graph_Connection(channel);
 source << Gf/qy_.Channel_XEntry(cion) >> result;

 return result;
}

caon_ptr<PHR_Graph_Node> PHR_Graph_Build::make_block_info_node()
{
 caon_ptr<PHR_Graph_Block_Info> bin = new
   PHR_Graph_Block_Info();
 caon_ptr<PHR_Graph_Node> result = new PHR_Graph_Node(bin);
 result->set_label(QString("<block-info>"));
 return result;
}

caon_ptr<PHR_Graph_Node> PHR_Graph_Build::make_statement_info_node(QString anchor_name,
   QString channel_name, QString anchor_kind)
{
 caon_ptr<PHR_Graph_Statement_Info> sin = new
   PHR_Graph_Statement_Info(anchor_name, channel_name, anchor_kind);
 caon_ptr<PHR_Graph_Node> result = new PHR_Graph_Node(sin);
 result->set_label(QString("<statement-info:%1%2>").arg(anchor_name).arg(anchor_kind));
 return result;
}

caon_ptr<PHR_Graph_Node> PHR_Graph_Build::add_statement_sequence_token(caon_ptr<PHR_Graph_Node> source,
  MG_Token& mgt, caon_ptr<PHR_Graph_Node> statement_info_node)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,source)
 CAON_PTR_DEBUG(PHR_Graph_Node ,statement_info_node)

 caon_ptr<PHR_Graph_Connection> cion = new PHR_Graph_Connection(statement_info_node);

 caon_ptr<PHR_Graph_Node> result = make_token_node(mgt);

 CAON_PTR_DEBUG(PHR_Graph_Connection ,cion)

 source << Gf/qy_.Statement_Sequence(cion) >> result;

 if(held_type_declaration_node_)
 {
  result << Gf/qy_.Type_Declaration >> held_type_declaration_node_;
  held_type_declaration_node_ = nullptr;
 }

 return result;
}

caon_ptr<PHR_Graph_Node> PHR_Graph_Build::add_block_entry_token(caon_ptr<PHR_Graph_Node> source,
  MG_Token& mgt, caon_ptr<PHR_Graph_Node> statement_info_node)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,source)
 CAON_PTR_DEBUG(PHR_Graph_Node ,statement_info_node)

 caon_ptr<PHR_Graph_Block_Info> bin = new PHR_Graph_Block_Info;
 caon_ptr<PHR_Graph_Node> nbin = new PHR_Graph_Node(bin);
 caon_ptr<PHR_Graph_Connection> cion = new PHR_Graph_Connection(nbin);
 cion->add_node(statement_info_node);

 caon_ptr<PHR_Graph_Node> result = make_token_node(mgt);

 CAON_PTR_DEBUG(PHR_Graph_Connection ,cion)

 source << Gf/qy_.Block_Entry(cion) >> result;

 if(held_type_declaration_node_)
 {
  result << Gf/qy_.Type_Declaration >> held_type_declaration_node_;
  held_type_declaration_node_ = nullptr;
 }

 return result;
}

caon_ptr<PHR_Graph_Node> PHR_Graph_Build::make_fsym_ground_node(QString chn, QString ty)
{
 caon_ptr<PHR_Graph_FGround_Entry> fen = new PHR_Graph_FGround_Entry(chn, ty);
 caon_ptr<PHR_Graph_Node> result = new PHR_Graph_Node(fen);
 result->set_label("<channel-fground-entry>");
 return result;
}

caon_ptr<PHR_Graph_Node> PHR_Graph_Build::add_fsym_ground_node(
  caon_ptr<PHR_Graph_Node> source,
  caon_ptr<PHR_Graph_Node> target, QString chn,
  caon_ptr<PHR_Graph_Node> cfen)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,source)
 CAON_PTR_DEBUG(PHR_Graph_Node ,target)

 if(cfen)
 {
  caon_ptr<PHR_Graph_Connection> cion = new PHR_Graph_Connection(chn, cfen);
  source << Gf/qy_.Channel_FGround_Entry(cion) >> target;
 }
 else
   source << Gf/qy_.Channel_FGround_Entry >> target;
}

caon_ptr<PHR_Graph_Node> PHR_Graph_Build::add_channel_fground_cross_node(
  caon_ptr<PHR_Graph_Node> source,
  caon_ptr<PHR_Graph_Node> target, QString chn,
  caon_ptr<PHR_Graph_Node> cfen)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,source)
 CAON_PTR_DEBUG(PHR_Graph_Node ,target)

 if(cfen)
 {
  CAON_PTR_DEBUG(PHR_Graph_Node ,cfen)
  caon_ptr<PHR_Graph_Connection> cion = new PHR_Graph_Connection(chn, cfen);
  source << Gf/qy_.Channel_FGround_Cross(cion) >> target;
 }
 else
   source << Gf/qy_.Channel_FGround_Cross >> target;
}

caon_ptr<PHR_Graph_Node> PHR_Graph_Build::add_channel_fground_coentry_node(
  caon_ptr<PHR_Graph_Node> source,
  caon_ptr<PHR_Graph_Node> target, QString chn,
  caon_ptr<PHR_Graph_Node> cfen)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,source)
 CAON_PTR_DEBUG(PHR_Graph_Node ,target)

 if(cfen)
 {
  caon_ptr<PHR_Graph_Connection> cion = new PHR_Graph_Connection(chn, cfen);
  source << Gf/qy_.Channel_FGround_Coentry(cion) >> target;
 }
 else
   source << Gf/qy_.Channel_FGround_Coentry >> target;
}

void PHR_Graph_Build::add_channel_entry_block_node(
  caon_ptr<PHR_Graph_Node> source, QString chn,
  caon_ptr<PHR_Graph_Node> target, caon_ptr<PHR_Graph_Node> bin)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,source)
 CAON_PTR_DEBUG(PHR_Graph_Node ,target)

 if(bin)
 {
  CAON_PTR_DEBUG(PHR_Graph_Node ,bin)
  if(caon_ptr<PHR_Graph_Block_Info> bi = bin->block_info())
  {
   CAON_PTR_DEBUG(PHR_Graph_Block_Info ,bi)
   CAON_DEBUG_NOOP
  }
 }

 caon_ptr<PHR_Graph_Connection> cion = new PHR_Graph_Connection(chn, bin);

 source << Gf/qy_.Channel_Entry_Block(cion) >> target;
}

void PHR_Graph_Build::push_expression_entry()
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,last_block_entry_node_)
 expression_entry_node_stack_.push(last_expression_entry_node_);
}

void PHR_Graph_Build::pop_expression_entry()
{
 prior_expression_entry_node_ = last_expression_entry_node_;
 CAON_PTR_DEBUG(PHR_Graph_Node ,prior_expression_entry_node_)

 if(expression_entry_node_stack_.isEmpty())
   last_expression_entry_node_ = nullptr; // // err?
 else
 {
  last_expression_entry_node_ = expression_entry_node_stack_.pop();
  CAON_PTR_DEBUG(PHR_Graph_Node ,last_expression_entry_node_)
 }
}

void PHR_Graph_Build::push_block_entry()
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,last_block_entry_node_)
 CAON_PTR_DEBUG(PHR_Graph_Node ,last_statement_entry_node_)
 block_entry_node_stack_.push({last_block_entry_node_, last_statement_entry_node_});
}

void PHR_Graph_Build::pop_block_entry()
{
 prior_block_entry_node_ = last_block_entry_node_;
 CAON_PTR_DEBUG(PHR_Graph_Node ,prior_block_entry_node_)

 if(block_entry_node_stack_.isEmpty())
 {
  last_block_entry_node_ = nullptr; // // err?
 }
 else
 {
  auto pr = block_entry_node_stack_.pop();
  last_block_entry_node_ = pr.first;
  last_statement_entry_node_ = pr.second;
  CAON_PTR_DEBUG(PHR_Graph_Node ,last_block_entry_node_)
  CAON_PTR_DEBUG(PHR_Graph_Node ,last_statement_entry_node_)
  CAON_DEBUG_NOOP
 }
}

void PHR_Graph_Build::add_channel_sequence_block_node(
  caon_ptr<PHR_Graph_Node> source,
  caon_ptr<PHR_Graph_Node> target, caon_ptr<PHR_Graph_Node> bin)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,source)
 CAON_PTR_DEBUG(PHR_Graph_Node ,target)

 caon_ptr<PHR_Graph_Connection> cion = new PHR_Graph_Connection(bin);

 source << Gf/qy_.Channel_Sequence_Block(cion) >> target;
}


void PHR_Graph_Build::add_channel_cross_block_node(
  caon_ptr<PHR_Graph_Node> source,
  caon_ptr<PHR_Graph_Node> target, caon_ptr<PHR_Graph_Node> bin)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,source)
 CAON_PTR_DEBUG(PHR_Graph_Node ,target)

 caon_ptr<PHR_Graph_Connection> cion = new PHR_Graph_Connection(bin);

 source << Gf/qy_.Channel_Cross_Block(cion) >> target;
}

void PHR_Graph_Build::add_channel_continue_block_node(
  caon_ptr<PHR_Graph_Node> source,
  caon_ptr<PHR_Graph_Node> target, caon_ptr<PHR_Graph_Node> bin)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,source)
 CAON_PTR_DEBUG(PHR_Graph_Node ,target)

 caon_ptr<PHR_Graph_Connection> cion = new PHR_Graph_Connection(bin);

 source << Gf/qy_.Channel_Continue_Block(cion) >> target;
}

caon_ptr<PHR_Graph_Node> PHR_Graph_Build::add_block_entry_node(
  caon_ptr<PHR_Graph_Node> source,
  caon_ptr<PHR_Graph_Node> target, caon_ptr<PHR_Graph_Node> sin)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,source)
 CAON_PTR_DEBUG(PHR_Graph_Node ,target)

 caon_ptr<PHR_Graph_Block_Info> bin = new PHR_Graph_Block_Info;
 //caon_ptr<PHR_Graph_Statement_Info> sin = new PHR_Graph_Statement_Info;
 caon_ptr<PHR_Graph_Node> nbin = new PHR_Graph_Node(bin);
 caon_ptr<PHR_Graph_Connection> cion = new PHR_Graph_Connection(nbin);

 if(sin)
   cion->add_node(sin);

 source << Gf/qy_.Block_Entry(cion) >> target;

 if(held_type_declaration_node_)
 {
  target << Gf/qy_.Type_Declaration >> held_type_declaration_node_;
  held_type_declaration_node_ = nullptr;
 }

 return nbin;
}


void PHR_Graph_Build::add_statement_sequence_node(
  caon_ptr<PHR_Graph_Node> source,
  caon_ptr<PHR_Graph_Node> target, caon_ptr<PHR_Graph_Node> sin)
{
 CAON_PTR_DEBUG(PHR_Graph_Node ,source)
 CAON_PTR_DEBUG(PHR_Graph_Node ,target)

 if(sin)
 {
  caon_ptr<PHR_Graph_Connection> cion = new PHR_Graph_Connection(sin);
    source << Gf/qy_.Statement_Sequence(cion) >> target;
 }
 else
   source << Gf/qy_.Statement_Sequence >> target;

 if(held_type_declaration_node_)
 {
  target << Gf/qy_.Type_Declaration >> held_type_declaration_node_;
  held_type_declaration_node_ = nullptr;
 }
}
