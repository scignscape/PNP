
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "expression-generator.h"

#include "statement-generator.h"

#include "kernel/document/phr-graph-document.h"

#include "kernel/graph/phr-graph.h"
#include "kernel/graph/phr-graph-node.h"
#include "kernel/graph/phr-graph-connection.h"

#include "kernel/phr-graph-root.h"

#include "token/phr-graph-token.h"

#include "token/phr-graph-token.h"
#include "token/phr-graph-fground-entry.h"
#include "token/phr-graph-block-info.h"
#include "token/phr-graph-type-declaration.h"
#include "token/phr-graph-cocyclic-type.h"
#include "token/phr-graph-signature.h"

#include "multigraph-token.h"

#include "textio.h"
USING_KANS(TextIO)

#include "rzns.h"
USING_RZNS(PhrGraphCore)


Expression_Generator::Expression_Generator()
 :  qy_(PHR_Graph_Query::instance()), statement_generator_(nullptr)
{

}

#define in_Gf PHR_Graph_Frame::instance(), //"graph"),

void Expression_Generator::generate_cocyclic_type_definition(QTextStream& qts,
  PHR_Graph_Cocyclic_Type& coy)
{
 qts << "\nenter_cocyclic_type $ " << coy.type_name() << " ;.\n";

 for(caon_ptr<PHR_Graph_Node> n : coy.precycle_fields())
 {
  generate_type_field_declaration(qts, *n, "pr");
 }
 for(caon_ptr<PHR_Graph_Node> n : coy.cocycle_fields())
 {
  generate_type_field_declaration(qts, *n, "co");
 }

 qts << "\nleave_cocyclic_type $ " << coy.type_name() << " ;.\n";
 generate_empty_line(qts);
}

void Expression_Generator::generate_from_node(QTextStream& qts,
 const PHR_Graph_Node& node, int unw)
{
 check_generate_type_declaration(qts, node);

 caon_ptr<PHR_Graph_Connection> cion;
 QString channel_name;
 if(caon_ptr<PHR_Graph_Node> n = qy_.Channel_Entry[cion](in_Gf &node))
 {
  CAON_PTR_DEBUG(PHR_Graph_Node ,n)
  if(cion)
    channel_name = cion->channel_name();
  if(caon_ptr<PHR_Graph_Token> tokn = node.phr_graph_token())
    generate_from_fn_node(qts, *tokn, channel_name, *n, unw);
 }
 else if(n = qy_.Channel_Entry_Block[cion](in_Gf &node))
 {
  CAON_PTR_DEBUG(PHR_Graph_Node ,n)
  caon_ptr<PHR_Graph_Block_Info> bin;
  caon_ptr<PHR_Graph_Statement_Info> sin;
  if(cion)
  {
   channel_name = cion->channel_name();
   bin = cion->phr_node()->block_info();
   if(cion->phr_node(1))
     sin = cion->phr_node(1)->statement_info();
  }
  if(caon_ptr<PHR_Graph_Token> tokn = node.phr_graph_token())
    generate_from_fn_node(qts, *tokn, channel_name, *n, unw,
    nullptr, SB_Info(sin, bin));
 }
 else if(n = qy_.Channel_FGround_Coentry[cion](in_Gf &node))
 {
  CAON_PTR_DEBUG(PHR_Graph_Node ,n)
  if(cion)
  {
   if(caon_ptr<PHR_Graph_FGround_Entry> fen = cion->phr_node()->fground_entry())
   {
    if(caon_ptr<PHR_Graph_Token> tokn = node.phr_graph_token())
    {
     CAON_PTR_DEBUG(PHR_Graph_Token ,tokn)
     channel_name = cion->channel_name();
     generate_from_fn_node(qts, *tokn, channel_name, *n, unw, fen.raw_pointer());
    }
   }
  }
 }
 else
 {
  // // assume void call ...
  if(caon_ptr<PHR_Graph_Token> tokn = node.phr_graph_token())
  {
   CAON_PTR_DEBUG(PHR_Graph_Token ,tokn)

   generate_line(qts, "push_carrier_stack", "fground");
   generate_line(qts, "hold_type_by_name", "fbase");
   generate_line(qts, "push_carrier_symbol", tokn->raw_text());
  }
 }

 caon_ptr<PHR_Graph_Connection> xcion;
 QString xchannel_name;
 caon_ptr<PHR_Graph_Node> xn = &node;
 if(xn = qy_.Channel_XEntry[cion](in_Gf xn))
 {
  CAON_PTR_DEBUG(PHR_Graph_Node ,xn)
  if(cion)
    xchannel_name = cion->channel_name();
  generate_xchannel(qts, xchannel_name, *xn, unw);
 }
}

void Expression_Generator::generate_fground_entry(QTextStream& qts,
  PHR_Graph_FGround_Entry& fen,
  const PHR_Graph_Node& node, int unw)
{
 char fc = 0;
 QString cn = fen.channel_name();
 if(cn.startsWith('?'))
 {
  fc = '?';
  cn = cn.mid(1);
 }

 if(unw > 0)
   qts << "\npush_unwind_scope $ 1 "
   << cn << " ;.\n";

 generate_from_node(qts, node, unw);

 if(unw > 0)
 {
  qts << "push_carrier_stack $ " << cn << " ;.\n";
  generate_line(qts, "index_channel_group");
  generate_line(qts, "coalesce_channel_group");
  generate_comment_line(qts, "pop");
  generate_line(qts, "pop_unwind_scope");
  if(fc == '?')
    generate_line(qts, "temp_anchor_channel_group_by_need");
  else
    generate_line(qts, "temp_anchor_channel_group");
 }
 generate_comment_line(qts, "end fground entry");
}

void Expression_Generator::generate_line(QTextStream& qts, QString ln)
{
 qts << ln << " ;.\n";
}

void Expression_Generator::generate_line(QTextStream& qts, QString ln, QString arg)
{
 qts << ln << " $ " << arg << " ;.\n";
}

void Expression_Generator::generate_empty_line(QTextStream& qts, int n)
{
 qts << QString(n, '\n');
}

void Expression_Generator::generate_comment_line(QTextStream& qts,
  QString ln, int n)
{
 if(n > 0)
   generate_empty_line(qts, n);
 qts << " .; " << ln << " ;.\n";
}

void Expression_Generator::generate_from_fn_node(QTextStream& qts,
  PHR_Graph_Token& tok, QString channel_name,
  const PHR_Graph_Node& arg_node, int unw,
  PHR_Graph_FGround_Entry* fen, SB_Info sbi)
{
 if(unw > 0)
 {
  qts << "\n .; unwind_scope: " << unw << " ;.\n";
 }
 generate_comment_line(qts, "generate_from_fn_node", (unw > 0)?1:2);
 qts << "push_carrier_stack $ fground ;.\n";
 if(tok.type_name().isEmpty())
   tok.set_type_name("fbase");
 generate_carrier(qts, tok);
 generate_comment_line(qts, "args");
 if(fen)
   generate_arg_carriers(qts, channel_name, arg_node, unw, fen);
 else
   generate_arg_carriers(qts, channel_name, arg_node, unw, sbi);
}

void Expression_Generator::generate_arg_carriers(QTextStream& qts,
  QString channel_name, const PHR_Graph_Node& arg_node,
  int unw, PHR_Graph_FGround_Entry* fen)
{
 qts << "push_carrier_stack $ " << channel_name << " ;.\n";
 // assume depth 1 for now ...
 generate_fground_entry(qts, *fen, arg_node, unw + 1);
      qts << "hold_type_by_name $ " << fen->result_type_name() << " ;.\n";
      generate_line(qts, "push_carrier_expression");
 caon_ptr<PHR_Graph_Node> n = &arg_node;
 generate_arg_carriers_follow(qts, n, unw);
}

void Expression_Generator::generate_block(QTextStream& qts, PHR_Graph_Block_Info& bin,
  const PHR_Graph_Node& node, PHR_Graph_Statement_Info* sin)
{
 generate_comment_line(qts, "block ...", 2);
 generate_line(qts, "@fnp");
 statement_generator_->generate_from_node(qts, node, sin);
 generate_line(qts, "@fne");
 generate_comment_line(qts, "end block ...", 1);
 generate_empty_line(qts);
 generate_line(qts, "hold_type_by_name $ pcv");
 if(bin.fn_name().isEmpty())
   generate_line(qts, "push_carrier_anon_fn @ last_source_fn_name");
 else
   qts << "push_carrier_named_fn @ =" << bin.fn_name() << " ;.\n";

 generate_empty_line(qts);
}

void Expression_Generator::generate_xchannel(QTextStream& qts, QString channel_name,
 const PHR_Graph_Node& arg_node, int unw)
{
 generate_arg_carriers(qts, channel_name, arg_node, unw);
}

void Expression_Generator::generate_type_field_declaration(QTextStream& qts,
  const PHR_Graph_Node& node, QString modifier)
{
 if(caon_ptr<PHR_Graph_Type_Declaration> td = node.type_declaration())
 {
  CAON_PTR_DEBUG(PHR_Graph_Type_Declaration ,td)
  qts << "\ntype_field_decl $ " << modifier << ' ' <<
     td->symbol_name() << ' ' << td->type_name() << " ;.\n";
 }
}

void Expression_Generator::generate_type_declaration(QTextStream& qts,
  const PHR_Graph_Node& node)
{
 if(caon_ptr<PHR_Graph_Type_Declaration> td = node.type_declaration())
 {
  CAON_PTR_DEBUG(PHR_Graph_Type_Declaration ,td)
  qts << "\ntype_decl $ " <<
     td->symbol_name() << ' ' << td->type_name() << " ;.\n";
 }
}

void Expression_Generator::check_generate_type_declaration(QTextStream& qts,
  const PHR_Graph_Node& node)
{
 if(caon_ptr<PHR_Graph_Node> tdn = qy_.Type_Declaration(in_Gf &node))
 {
  generate_type_declaration(qts, *tdn);
 }
}

void Expression_Generator::generate_minimal_signature(QTextStream& qts,
  PHR_Graph_Signature& sig)
{
 QList<MG_Token>& mgts = sig.tokens();
 QString bt;
 for(MG_Token mgt : mgts)
 {
  switch (mgt.kind)
  {
  case MG_Token_Kinds::Sig_Channel:
    qts << "push_carrier_stack $ " << mgt.raw_text << " ;.\n";
   break;
  case MG_Token_Kinds::Sig_Symbol:
    qts << "hold_type_by_name $ auto ;.\n";
    qts << "push_carrier_symbol $ " << mgt.raw_text << " ;.\n";
   break;
  case MG_Token_Kinds::Sig_Type:
   if(bt.isEmpty())
     qts << "push_carrier_type_holder $ " << mgt.raw_text << " ;.\n";
   else
   {
    qts << "hold_type_by_name $ " << mgt.raw_text << " ;.\n";
    qts << "push_carrier_symbol $ " << bt << " ;.\n";
    bt.clear();
   }
   break;
  case MG_Token_Kinds::Sig_Symbol_Before_Type:
    bt = mgt.raw_text;
   break;
  default:
   break;
  }
 }
 qts << "coalesce_channel_group ;.\n";

}

void Expression_Generator::generate_alt_entry(QTextStream& qts)
{
 qts << "load_alt_program_stack ;.\n";
}

void Expression_Generator::generate_alt_close(QTextStream& qts)
{
 qts << "reload_program_stack ;.\n";
}

void Expression_Generator::generate_block_signature(QTextStream& qts,
  PHR_Graph_Signature& sig)
{
 qts << "\n .; block signature ... ;.\n";
 generate_alt_entry(qts);

 generate_minimal_signature(qts, sig);
 qts << "finalize_block_signature @ last_source_fn_name ;.\n";
 generate_alt_close(qts);
 qts << "\n .; end block signature ... ;.\n\n";
}

void Expression_Generator::generate_arg_carriers(QTextStream& qts,
  QString channel_name, const PHR_Graph_Node& arg_node,
  int unw, SB_Info sbi)
{
 qts << "push_carrier_stack $ " << channel_name << " ;.\n";

 if(sbi.bin)
 {
  // // first arg is nested block ...
  qts << "anticipate_nested_block $ " << channel_name << " ;.\n";

  generate_block(qts, *sbi.bin, arg_node, sbi.sin);

  if(PHR_Graph_Signature* sig = sbi.bin->signature())
  {
   generate_block_signature(qts, *sig);
  }

 }
 else
 {
  // // no nested entry ...
  if(caon_ptr<PHR_Graph_Token> tokn = arg_node.phr_graph_token())
  {
   CAON_PTR_DEBUG(PHR_Graph_Token ,tokn)
   generate_carrier(qts, *tokn);
  }
 }

 caon_ptr<PHR_Graph_Node> n = &arg_node;
 generate_arg_carriers_follow(qts, n, unw);
}

void Expression_Generator::generate_arg_carriers_follow(QTextStream& qts,
  caon_ptr<PHR_Graph_Node> n, int unw)
{
 caon_ptr<PHR_Graph_Node> n1 = nullptr;
 while(n)
 {
  CAON_PTR_DEBUG(PHR_Graph_Node ,n)
  n->debug_connections();
  caon_ptr<PHR_Graph_Connection> cion1;
  if(n1 = qy_.Channel_Sequence(in_Gf n))
  {
   if(caon_ptr<PHR_Graph_Token> tok = n1->phr_graph_token())
   {
    generate_carrier(qts, *tok);
   }
  }
  else if(n1 = qy_.Channel_FGround_Entry[cion1](in_Gf n))
  {
   if(caon_ptr<PHR_Graph_FGround_Entry> fen = cion1->phr_node()->fground_entry())
   {
    // // already handled ...?
    CAON_PTR_DEBUG(PHR_Graph_Node ,n1)
    generate_fground_entry(qts, *fen, *n1, unw + 1);
    qts << "hold_type_by_name $ " << fen->result_type_name() << " ;.\n";
    generate_line(qts, "push_carrier_expression");
   }
  }


  else if(n1 = qy_.Channel_Continue(in_Gf n))
  {
   if(caon_ptr<PHR_Graph_Token> tok = n1->phr_graph_token())
   {
    CAON_PTR_DEBUG(PHR_Graph_Token ,tok)
    generate_carrier(qts, *tok);
   }
  }
  else if(n1 = qy_.Channel_FGround_Coentry[cion1](in_Gf n))
  {
   CAON_PTR_DEBUG(PHR_Graph_Node ,n1)
   n = qy_.Channel_Continue(in_Gf n1);
   continue;
  }
  else if(n1 = qy_.Channel_FGround_Cross[cion1](in_Gf n))
  {
   CAON_PTR_DEBUG(PHR_Graph_Connection ,cion1)
   CAON_PTR_DEBUG(PHR_Graph_Node ,n1)
   if(caon_ptr<PHR_Graph_FGround_Entry> fen = cion1->phr_node()->fground_entry())
   {
    generate_fground_entry(qts, *fen, *n1, unw + 1);
    qts << "hold_type_by_name $ " << fen->result_type_name() << " ;.\n";
    generate_line(qts, "push_carrier_expression");
   }
  }
  else if(n1 = qy_.Channel_Continue_Block[cion1](in_Gf n))
  {
   if(caon_ptr<PHR_Graph_Block_Info> bin = cion1->phr_node()->block_info())
   {
    if(cion1->phr_node(1))
    {
     if(caon_ptr<PHR_Graph_Statement_Info> sin = cion1->phr_node(1)->statement_info())
     {
      generate_block(qts, *bin, *n1, sin.raw_pointer());
     }
    }
    else
    {
     generate_block(qts, *bin, *n1);
    }
   }
  }
  else if(n1 = qy_.Channel_Cross_Block[cion1](in_Gf n))
  {
   if(caon_ptr<PHR_Graph_Block_Info> bin = cion1->phr_node()->block_info())
   {
    if(cion1->phr_node(1))
    {
     if(caon_ptr<PHR_Graph_Statement_Info> sin = cion1->phr_node(1)->statement_info())
     {
      generate_block(qts, *bin, *n1, sin.raw_pointer());
     }
    }
    else
    {
     generate_block(qts, *bin, *n1);
    }
   }
  }
  else if(n1 = qy_.Channel_Sequence_Block[cion1](in_Gf n))
  {
   if(caon_ptr<PHR_Graph_Block_Info> bin = cion1->phr_node()->block_info())
   {
    if(cion1->phr_node(1))
    {
     if(caon_ptr<PHR_Graph_Statement_Info> sin = cion1->phr_node(1)->statement_info())
     {
      generate_block(qts, *bin, *n1, sin.raw_pointer());
     }
    }
    else
    {
     generate_block(qts, *bin, *n1);
    }
   }
  }
  // // no coentry right? ...
  n = n1;
 }
}

void Expression_Generator::generate_carrier(QTextStream& qts,
  PHR_Graph_Token& tokn)
{
 if(tokn.flags.gen_raw_value)
   generate_carrier_with_raw_value(qts, tokn);
 else
   generate_carrier_with_symbol(qts, tokn);
}

void Expression_Generator::generate_carrier_with_symbol(QTextStream& qts,
 PHR_Graph_Token& tok)
{
 QString ty = tok.type_name();
 if(!ty.isEmpty())
   qts << "hold_type_by_name $ " << ty << " ;.\n";
 qts << "push_carrier_symbol $ " << tok.raw_text() << " ;.\n";
}

void Expression_Generator::generate_carrier_with_raw_value(QTextStream& qts,
 PHR_Graph_Token& tok)
{
 QString ty = tok.type_name();

 //?
 if(ty.isEmpty())
 {
  if(tok.flags.is_string_literal)
    ty = "str";
  else
    ty = "u4";
 }

 if(!ty.isEmpty())
   qts << "hold_type_by_name $ " << ty << " ;.\n";

 if(tok.flags.is_string_literal)
   qts << "push_carrier_raw_value $ " << tok.raw_text() << " ;.\n";
 else
   qts << "push_carrier_raw_value $ " << tok.raw_text() << " ;.\n";
}

