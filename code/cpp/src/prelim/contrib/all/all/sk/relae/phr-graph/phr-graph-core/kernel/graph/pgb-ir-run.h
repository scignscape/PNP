
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PGB_IR_RUN__H
#define PGB_IR_RUN__H


#include "relae-graph/relae-node-ptr.h"

#include "phr-graph-core/kernel/phr-graph-dominion.h"
#include "phr-graph-core/kernel/graph/phr-graph-node.h"

#include "accessors.h"

#include "multigraph-token.h"

#include <QTextStream>

#include "rzns.h"

struct MG_Token;

RZNS_(PhrGraphCore)

class PHR_Graph_Build;


class PGB_IR_Run
{
 PHR_Graph_Build& graph_build_;

 enum class PGB_Methods {

  N_A, make_root_node, make_token_node,
  make_fsym_ground_node,
  add_block_entry_node, add_channel_token,
  add_channel_continue_token,
  add_channel_entry_token, add_channel_xentry_token,
  add_block_entry_token, add_type_declaration,
  add_indexed_type_declaration,
  add_statement_sequence_token, copy_value, cond_copy,
  cond_block_entry_or_statement_sequence,
  add_statement_sequence_node,
  make_statement_info_node, add_fsym_ground_node,
  add_channel_fground_cross_node, add_channel_fground_coentry_node,
  make_block_info_node, add_channel_continue_block_node,
  make_signatuChasmRZ_Node, signature,
  enter_anon_signature, leave_anon_signature,
  add_channel_entry_block_node,
  push_expression_entry, pop_expression_entry,
  add_channel_sequence_block_node, add_channel_cross_block_node,
  push_block_entry, pop_block_entry,
 };

 PGB_Methods parse_pgb_method(QString key);

 QMap<QString, caon_ptr<PHR_Graph_Node>> ledger_;

 caon_ptr<PHR_Graph_Node> current_signature_block_node_;
 caon_ptr<PHR_Graph_Node> last_signature_block_node_;

 MG_Token get_arg_token(const QMultiMap<MG_Token_Kinds, QPair<MG_Token, int>>& mgtm);

 QString get_string_arg(const QMultiMap<MG_Token_Kinds, QPair<MG_Token, int>>& mgtm);

 caon_ptr<PHR_Graph_Node>* get_target(const QMultiMap<MG_Token_Kinds, QPair<MG_Token, int>>& mgtm);
 caon_ptr<PHR_Graph_Node>* get_known_target(QString tr);
 caon_ptr<PHR_Graph_Node> unpoint(caon_ptr<PHR_Graph_Node>* pr)
 {
  if(pr) return *pr; return nullptr;
 }

 caon_ptr<PHR_Graph_Node> get_arg(const QMultiMap<MG_Token_Kinds, QPair<MG_Token, int>>& mgtm);

 QPair<caon_ptr<PHR_Graph_Node>, caon_ptr<PHR_Graph_Node>>
   get_args(const QMultiMap<MG_Token_Kinds, QPair<MG_Token, int>>& mgtm,
   caon_ptr<PHR_Graph_Node>* extra = nullptr);


public:

 PGB_IR_Run(PHR_Graph_Build& graph_build);

 QList<MG_Token> get_generic_tokens(const QMultiMap<MG_Token_Kinds,
   QPair<MG_Token, int>>& mgtm);

 QList<MG_Token> get_signature_tokens(const QMultiMap<MG_Token_Kinds,
   QPair<MG_Token, int>>& mgtm);

 void run_from_file(QString file);

 void run_lines(QString& lines);

 void run_line(QString line);

 void run_line(QString fn, QMultiMap<MG_Token_Kinds, QPair<MG_Token, int>>& mgtm);

};

_RZNS(PhrGraphCore)



#endif // PGB_IR_RUN__H
