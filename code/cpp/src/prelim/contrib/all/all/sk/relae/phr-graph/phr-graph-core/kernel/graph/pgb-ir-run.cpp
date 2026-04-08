
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "pgb-ir-run.h"

#include "pgb-ir-build.h"

#include "phr-graph-build.h"

#include "token/phr-graph-signature.h"

#include "textio.h"

USING_KANS(TextIO)

#include "multigraph-token.h"

#include "rzns.h"

USING_RZNS(PhrGraphCore)

PGB_IR_Run::PGB_IR_Run(PHR_Graph_Build& graph_build)
 : graph_build_(graph_build), current_signature_block_node_(nullptr),
   last_signature_block_node_(nullptr)
{

}

void PGB_IR_Run::run_from_file(QString file)
{
 QString lines;
 load_file(file, lines);

 run_lines(lines);
}

void PGB_IR_Run::run_lines(QString& lines)
{
 int pos = 0;
 int end = lines.length();

 while(pos < end)
 {
  int sp = lines.indexOf("(pgb::", pos);
  if(sp == -1)
    break;
  int np = lines.indexOf(')', sp + 6);
  if(np == -1)
    break;
  QString l = lines.mid(sp + 6, np - sp - 6).simplified();
  run_line(l);
  pos = np + 1;
 }
}


PGB_IR_Run::PGB_Methods PGB_IR_Run::parse_pgb_method(QString key)
{
#define TEMP_MACRO(x) {#x, PGB_Methods::x},

 static QMap<QString, PGB_Methods> static_map {{

  TEMP_MACRO(make_root_node)
  TEMP_MACRO(make_token_node)
  TEMP_MACRO(add_type_declaration)
  TEMP_MACRO(add_indexed_type_declaration)
  TEMP_MACRO(make_fsym_ground_node)
  TEMP_MACRO(add_block_entry_node)
  TEMP_MACRO(add_channel_token)
  TEMP_MACRO(add_channel_continue_token)
  TEMP_MACRO(add_channel_entry_token)
  TEMP_MACRO(add_channel_xentry_token)
  TEMP_MACRO(add_block_entry_token)
  TEMP_MACRO(add_statement_sequence_token)
  TEMP_MACRO(copy_value)
  TEMP_MACRO(cond_copy)
  TEMP_MACRO(cond_block_entry_or_statement_sequence)
  TEMP_MACRO(add_statement_sequence_node)
  TEMP_MACRO(make_statement_info_node)
  TEMP_MACRO(add_fsym_ground_node)
  TEMP_MACRO(add_channel_fground_cross_node)
  TEMP_MACRO(add_channel_fground_coentry_node)
  TEMP_MACRO(make_block_info_node)
  TEMP_MACRO(add_channel_continue_block_node)
  TEMP_MACRO(make_signatuChasmRZ_Node)
  TEMP_MACRO(signature)
  TEMP_MACRO(enter_anon_signature)
  TEMP_MACRO(leave_anon_signature)
  TEMP_MACRO(add_channel_entry_block_node)
  TEMP_MACRO(add_channel_cross_block_node)
  TEMP_MACRO(add_channel_sequence_block_node)
  TEMP_MACRO(push_expression_entry)
  TEMP_MACRO(pop_expression_entry)
  TEMP_MACRO(push_block_entry)
  TEMP_MACRO(pop_block_entry)
 }};

#undef TEMP_MACRO

 return static_map.value(key.toLower(), PGB_Methods::N_A);
}


caon_ptr<PHR_Graph_Node>* PGB_IR_Run::get_target(const QMultiMap<MG_Token_Kinds, QPair<MG_Token, int>>& mgtm)
{
 QList<MG_Token> mgts = PGB_IR_Build::mgts_by_kind_group(mgtm, MG_Token_Kind_Groups::Target);
 MG_Token& mgt = mgts[0];
 if(mgt.kind == MG_Token_Kinds::Ledger_Target)
   return &ledger_[mgt.raw_text];
 return get_known_target(mgt.raw_text);
}

caon_ptr<PHR_Graph_Node>* PGB_IR_Run::get_known_target(QString tr)
{
 if(tr == "current_node")
   return &graph_build_.current_node_;
 if(tr == "last_statement_entry_node")
   return &graph_build_.last_statement_entry_node_;
 if(tr == "last_block_pre_entry_node")
   return &graph_build_.last_block_pre_entry_node_;
 if(tr == "last_expression_entry_node")
   return &graph_build_.last_expression_entry_node_;
 if(tr == "last_block_entry_node")
   return &graph_build_.last_block_entry_node_;
 if(tr == "prior_block_entry_node")
   return &graph_build_.prior_block_entry_node_;
 if(tr == "prior_expression_entry_node")
   return &graph_build_.prior_expression_entry_node_;
 return nullptr;
}

caon_ptr<PHR_Graph_Node> PGB_IR_Run::get_arg(const QMultiMap<MG_Token_Kinds, QPair<MG_Token, int>>& mgtm)
{
 QList<MG_Token> mgts = PGB_IR_Build::mgts_by_kind_group(mgtm, MG_Token_Kind_Groups::Arg_Target);
 MG_Token& mgt = mgts[0];
 if(mgt.kind == MG_Token_Kinds::Arg_Ledger_Target)
   return ledger_[mgt.raw_text];
 return unpoint(get_known_target(mgt.raw_text));
}

QPair<caon_ptr<PHR_Graph_Node>, caon_ptr<PHR_Graph_Node>>
  PGB_IR_Run::get_args(const QMultiMap<MG_Token_Kinds,
  QPair<MG_Token, int>>& mgtm, caon_ptr<PHR_Graph_Node>* extra)
{
 caon_ptr<PHR_Graph_Node> r1, r2;
 QList<MG_Token> mgts = PGB_IR_Build::mgts_by_kind_group(mgtm, MG_Token_Kind_Groups::Arg_Target);
 MG_Token& mgt0 = mgts[0];
 if(mgt0.kind == MG_Token_Kinds::Arg_Ledger_Target)
   r1 = ledger_[mgt0.raw_text];
 else
   r1 = unpoint(get_known_target(mgt0.raw_text));
 MG_Token& mgt1 = mgts[1];
 if(mgt1.kind == MG_Token_Kinds::Arg_Ledger_Target)
   r2 = ledger_[mgt1.raw_text];
 else
   r2 = unpoint(get_known_target(mgt1.raw_text));

 if(extra && mgts.size() > 2)
 {
  MG_Token& mgt2 = mgts[2];
  if(mgt2.kind == MG_Token_Kinds::Arg_Ledger_Target)
    *extra = ledger_[mgt2.raw_text];
  else
    *extra = unpoint(get_known_target(mgt2.raw_text));
 }

 return {r1, r2};
}

QList<MG_Token> PGB_IR_Run::get_signature_tokens(const QMultiMap<MG_Token_Kinds,
  QPair<MG_Token, int>>& mgtm)
{
 return PGB_IR_Build::mgts_by_kind_group(mgtm, MG_Token_Kind_Groups::Sig);
}

QList<MG_Token> PGB_IR_Run::get_generic_tokens(const QMultiMap<MG_Token_Kinds,
  QPair<MG_Token, int>>& mgtm)
{
 return PGB_IR_Build::mgts_by_kind_group(mgtm, MG_Token_Kind_Groups::Generic);
}

MG_Token PGB_IR_Run::get_arg_token(const QMultiMap<MG_Token_Kinds, QPair<MG_Token, int>>& mgtm)
{
 QList<MG_Token> mgts = PGB_IR_Build::mgts_by_kind_group(mgtm, MG_Token_Kind_Groups::Arg);
 if(mgts.isEmpty())
   mgts = PGB_IR_Build::mgts_by_kind_group(mgtm, MG_Token_Kind_Groups::String_Literal);
 return mgts[0];
}

QString PGB_IR_Run::get_string_arg(const QMultiMap<MG_Token_Kinds, QPair<MG_Token, int>>& mgtm)
{
 return mgtm.value(MG_Token_Kinds::Arg_String_Literal).first.raw_text;
}

void PGB_IR_Run::run_line(QString fn, QMultiMap<MG_Token_Kinds, QPair<MG_Token, int>>& mgtm)
{
 PGB_Methods md = parse_pgb_method(fn);

 switch (md)
 {
 case PGB_Methods::make_root_node:
  {
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   if(tr)
     *tr = graph_build_.make_root_node();
  };
  break;

 case PGB_Methods::add_type_declaration:
  {
   QList<MG_Token> mgts = PGB_IR_Build::mgts_by_kind_group(mgtm, MG_Token_Kind_Groups::Arg);
   if(mgts.size() < 2)
     break;
   graph_build_.add_type_declaration(mgts[0], mgts[1]);
  };
  break;

 case PGB_Methods::add_indexed_type_declaration:
  {
   QList<MG_Token> mgts = PGB_IR_Build::mgts_by_kind_group(mgtm, MG_Token_Kind_Groups::Arg);
   if(mgts.size() < 2)
     break;
   QList<MG_Token> gmgts = PGB_IR_Build::mgts_by_kind_group(mgtm, MG_Token_Kind_Groups::Generic);
   if(mgts.size() == 0)
     break;
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   caon_ptr<PHR_Graph_Node> sn = get_arg(mgtm);

   if(tr)
     *tr = graph_build_.add_indexed_type_declaration(gmgts[0], mgts[0], mgts[1], sn);
   else
     graph_build_.add_indexed_type_declaration(gmgts[0], mgts[0], mgts[1], sn);
  };
  break;

 case PGB_Methods::enter_anon_signature:
  {
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   caon_ptr<PHR_Graph_Node> n = get_arg(mgtm);
   if(tr)
     *tr = graph_build_.make_signatuChasmRZ_Node(n);
   else
     graph_build_.make_signatuChasmRZ_Node(n);
   current_signature_block_node_ = n;
  }
  break;

 case PGB_Methods::leave_anon_signature:
  {
   caon_ptr<PHR_Graph_Node> n = get_arg(mgtm);

   graph_build_.finalize_block_signature(n);

   last_signature_block_node_ = nullptr;
   current_signature_block_node_ = nullptr;
  }
  break;

 case PGB_Methods::signature:
  {
   caon_ptr<PHR_Graph_Node> n = get_arg(mgtm);
   if(caon_ptr<PHR_Graph_Signature> s = n->phr_graph_signature())
   {
    QList<MG_Token> mgts = get_signature_tokens(mgtm);
    s->add_tokens(mgts);
   }
  }
  break;

 case PGB_Methods::make_signatuChasmRZ_Node:
  {
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   caon_ptr<PHR_Graph_Node> n = get_arg(mgtm);

   if(tr)
     *tr = graph_build_.make_signatuChasmRZ_Node(n);
   else
     graph_build_.make_signatuChasmRZ_Node(n);
  }
  break;

 case PGB_Methods::make_token_node:
  {
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   MG_Token tok = get_arg_token(mgtm);
   if(tr)
     *tr = graph_build_.make_token_node(tok);
   else
     graph_build_.make_token_node(tok);
  }
  break;

 case PGB_Methods::make_fsym_ground_node:
  {
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   QList<MG_Token> gts = get_generic_tokens(mgtm);
   if(gts.size() < 2)
     break;
   if(tr)
     *tr = graph_build_.make_fsym_ground_node(gts[0].raw_text, gts[1].raw_text);
   else
     graph_build_.make_fsym_ground_node(gts[0].raw_text, gts[1].raw_text);
  };
  break;
 case PGB_Methods::make_statement_info_node:
  {
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   QList<MG_Token> mgts = get_generic_tokens(mgtm);
   MG_Token tok = get_arg_token(mgtm);
   if(tr)
     *tr = graph_build_.make_statement_info_node(tok.raw_text,
     mgts[0].raw_text, mgts[1].raw_text);
   else
     graph_build_.make_statement_info_node(tok.raw_text,
     mgts[0].raw_text, mgts[1].raw_text);
  }
  break;
 case PGB_Methods::add_block_entry_node:
  {
   caon_ptr<PHR_Graph_Node> extra = nullptr;
   auto pr = get_args(mgtm, &extra);
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   graph_build_.add_block_entry_node(pr.first, pr.second, extra);
   if(tr)
     *tr = pr.second;
  }
  break;
 case PGB_Methods::add_fsym_ground_node:
  {
   caon_ptr<PHR_Graph_Node> extra = nullptr;
   auto pr = get_args(mgtm, &extra);
   QList<MG_Token> gts = get_generic_tokens(mgtm);
   if(gts.isEmpty())
     graph_build_.add_fsym_ground_node(pr.first, pr.second);
   else
     graph_build_.add_fsym_ground_node(pr.first, pr.second,
     gts.first().raw_text, extra);
  }
  break;
 case PGB_Methods::add_channel_fground_cross_node:
  {
   caon_ptr<PHR_Graph_Node> extra = nullptr;
   auto pr = get_args(mgtm, &extra);
   QList<MG_Token> gts = get_generic_tokens(mgtm);
   if(gts.isEmpty())
     graph_build_.add_channel_fground_cross_node(pr.first, pr.second);
   else
     graph_build_.add_channel_fground_cross_node(pr.first, pr.second,
     gts.first().raw_text, extra);
  }
  break;
 case PGB_Methods::add_channel_fground_coentry_node:
  {
   caon_ptr<PHR_Graph_Node> extra = nullptr;
   auto pr = get_args(mgtm, &extra);
   QList<MG_Token> gts = get_generic_tokens(mgtm);
   if(gts.isEmpty())
     graph_build_.add_channel_fground_coentry_node(pr.first, pr.second);
   else
     graph_build_.add_channel_fground_coentry_node(pr.first, pr.second,
     gts.first().raw_text, extra);
  }
  break;
 case PGB_Methods::add_statement_sequence_node:
  {
   caon_ptr<PHR_Graph_Node> extra = nullptr;
   auto pr = get_args(mgtm, &extra);
   graph_build_.add_statement_sequence_node(pr.first, pr.second, extra);
  }
  break;
 case PGB_Methods::copy_value:
  {
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   caon_ptr<PHR_Graph_Node> n = get_arg(mgtm);
   if(tr)
     *tr = n;
  }
  break;
 case PGB_Methods::cond_copy:
  {
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   caon_ptr<PHR_Graph_Node> n = get_arg(mgtm);
   if(tr && !*tr)
     *tr = n;
  }
  break;
 case PGB_Methods::cond_block_entry_or_statement_sequence:
  {
   caon_ptr<PHR_Graph_Node> extra = nullptr;
   auto pr = get_args(mgtm, &extra);
   if(pr.first)
     graph_build_.add_statement_sequence_node(pr.first, extra);
   else
     graph_build_.add_block_entry_node(pr.second, extra);
  }
  break;
 case PGB_Methods::add_channel_token:
  {
   caon_ptr<PHR_Graph_Node> n = get_arg(mgtm);
   MG_Token mgt = get_arg_token(mgtm);
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   if(tr)
     *tr = graph_build_.add_channel_token(n, mgt);
   else
     graph_build_.add_channel_token(n, mgt);
  }
  break;
 case PGB_Methods::add_channel_continue_token:
  {
   caon_ptr<PHR_Graph_Node> n = get_arg(mgtm);
   MG_Token mgt = get_arg_token(mgtm);
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   if(tr)
     *tr = graph_build_.add_channel_continue_token(n, mgt);
   else
     graph_build_.add_channel_continue_token(n, mgt);
  }
  break;
 case PGB_Methods::add_statement_sequence_token:
  {
   auto pr = get_args(mgtm);
   MG_Token mgt = get_arg_token(mgtm);
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   if(tr)
     *tr = graph_build_.add_statement_sequence_token(pr.first, mgt, pr.second);
   else
     graph_build_.add_statement_sequence_token(pr.first, mgt, pr.second);
  }
  break;
 case PGB_Methods::add_block_entry_token:
  {
   auto pr = get_args(mgtm);
   MG_Token mgt = get_arg_token(mgtm);
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   if(tr)
     *tr = graph_build_.add_block_entry_token(pr.first, mgt, pr.second);
   else
     graph_build_.add_block_entry_token(pr.first, mgt, pr.second);
  }
  break;
 case PGB_Methods::add_channel_entry_token:
  {
   caon_ptr<PHR_Graph_Node> n = get_arg(mgtm);
   QString cn = get_string_arg(mgtm);
   MG_Token mgt = get_arg_token(mgtm);
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   if(tr)
     *tr = graph_build_.add_channel_entry_token(n, cn, mgt);
   else
     graph_build_.add_channel_entry_token(n, cn, mgt);
  }
  break;

 case PGB_Methods::add_channel_xentry_token:
  {
   caon_ptr<PHR_Graph_Node> n = get_arg(mgtm);
   QString cn = get_string_arg(mgtm);
   MG_Token mgt = get_arg_token(mgtm);
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   if(tr)
     *tr = graph_build_.add_channel_xentry_token(n, cn, mgt);
   else
     graph_build_.add_channel_xentry_token(n, cn, mgt);
  }
  break;

 case PGB_Methods::make_block_info_node:
  {
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   if(tr)
     *tr = graph_build_.make_block_info_node();
   else
     graph_build_.make_block_info_node();
  }
  break;

 case PGB_Methods::add_channel_continue_block_node:
  {
   caon_ptr<PHR_Graph_Node> extra = nullptr;
   auto pr = get_args(mgtm, &extra);
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   graph_build_.add_channel_continue_block_node(pr.first,
     pr.second, extra);
   if(tr)
     *tr = pr.second;
  }
  break;

 case PGB_Methods::add_channel_sequence_block_node:
  {
   caon_ptr<PHR_Graph_Node> extra = nullptr;
   auto pr = get_args(mgtm, &extra);
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   graph_build_.add_channel_sequence_block_node(pr.first,
     pr.second, extra);
   if(tr)
     *tr = pr.second;
  }
  break;

 case PGB_Methods::add_channel_cross_block_node:
  {
   caon_ptr<PHR_Graph_Node> extra = nullptr;
   auto pr = get_args(mgtm, &extra);
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   graph_build_.add_channel_cross_block_node(pr.first,
     pr.second, extra);
   if(tr)
     *tr = pr.second;
  }
  break;

 case PGB_Methods::add_channel_entry_block_node:
  {
   caon_ptr<PHR_Graph_Node> extra = nullptr;
   QString chn = get_string_arg(mgtm);
   auto pr = get_args(mgtm, &extra);
   caon_ptr<PHR_Graph_Node>* tr = get_target(mgtm);
   graph_build_.add_channel_entry_block_node(pr.first, chn,
     pr.second, extra);
   if(tr)
     *tr = pr.second;
  }
  break;

 case PGB_Methods::push_expression_entry:
  graph_build_.push_expression_entry();
  break;

 case PGB_Methods::pop_expression_entry:
  graph_build_.pop_expression_entry();
  break;

 case PGB_Methods::push_block_entry:
  graph_build_.push_block_entry();
  break;

 case PGB_Methods::pop_block_entry:
  graph_build_.pop_block_entry();
  break;

 default:
  break;
 }
}


void PGB_IR_Run::run_line(QString line)
{
 QMultiMap<MG_Token_Kinds, QPair<MG_Token, int>> mgtm;
 QString fn = PGB_IR_Build::parse_line(line, mgtm);
 run_line(fn, mgtm);
}

