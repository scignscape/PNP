
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PHR_GRAPH_BUILD__H
#define PHR_GRAPH_BUILD__H


#include "relae-graph/relae-node-ptr.h"

#include "phr-graph-core/kernel/phr-graph-dominion.h"

#include "accessors.h"

#include <QTextStream>
#include <QStack>

#include "rzns.h"

struct MG_Token;

RZNS_(PhrGraphCore)

class PHR_Graph;
class PHR_Graph_Frame;
class PHR_Graph_Query;
class PHR_Graph_Cocyclic_Type;

class PHR_Graph_Build
{
 friend class PGB_IR_Run;

 PHR_Graph& graph_;
 QString file_;
 caon_ptr<PHR_Graph_Node> current_node_;
 caon_ptr<PHR_Graph_Node> last_statement_entry_node_;
 caon_ptr<PHR_Graph_Node> last_block_pre_entry_node_;
 caon_ptr<PHR_Graph_Node> last_expression_entry_node_;
 caon_ptr<PHR_Graph_Node> last_block_entry_node_;
 caon_ptr<PHR_Graph_Node> prior_block_entry_node_;
 caon_ptr<PHR_Graph_Node> prior_expression_entry_node_;

 QStack<caon_ptr<PHR_Graph_Node>> expression_entry_node_stack_;
 QStack<QPair<caon_ptr<PHR_Graph_Node>, caon_ptr<PHR_Graph_Node>>> block_entry_node_stack_;

 caon_ptr<PHR_Graph_Node> held_type_declaration_node_;
 caon_ptr<PHR_Graph_Cocyclic_Type> current_cocyclic_type_;


 PHR_Graph_Frame& Gf;
 const PHR_Graph_Query& qy_;

public:

 PHR_Graph_Build(PHR_Graph& graph);

 ACCESSORS(QString ,file)
 ACCESSORS(caon_ptr<PHR_Graph_Node> ,current_node)

 caon_ptr<PHR_Graph_Node> make_root_node();

 caon_ptr<PHR_Graph_Node> make_statement_info_node(QString anchor_name,
    QString channel_name, QString anchor_kind);

 caon_ptr<PHR_Graph_Node> add_block_entry_node(
   caon_ptr<PHR_Graph_Node> source,
   caon_ptr<PHR_Graph_Node> target, caon_ptr<PHR_Graph_Node> sin = nullptr);


 void add_channel_sequence_block_node(
   caon_ptr<PHR_Graph_Node> source,
   caon_ptr<PHR_Graph_Node> target, caon_ptr<PHR_Graph_Node> bin = nullptr);


 void add_channel_cross_block_node(
   caon_ptr<PHR_Graph_Node> source,
   caon_ptr<PHR_Graph_Node> target, caon_ptr<PHR_Graph_Node> bin = nullptr);


 void add_channel_continue_block_node(
   caon_ptr<PHR_Graph_Node> source,
   caon_ptr<PHR_Graph_Node> target, caon_ptr<PHR_Graph_Node> bin = nullptr);

 void add_channel_entry_block_node(
   caon_ptr<PHR_Graph_Node> source, QString chn,
   caon_ptr<PHR_Graph_Node> target, caon_ptr<PHR_Graph_Node> bin = nullptr);

 caon_ptr<PHR_Graph_Node> make_fsym_ground_node(QString chn, QString ty);

 void finalize_block_signature(
   caon_ptr<PHR_Graph_Node> bin_node);


 caon_ptr<PHR_Graph_Node> add_fsym_ground_node(
   caon_ptr<PHR_Graph_Node> source,
   caon_ptr<PHR_Graph_Node> target,
   QString chn = QString(),
   caon_ptr<PHR_Graph_Node> cfen = nullptr);

 caon_ptr<PHR_Graph_Node> add_channel_fground_cross_node(
   caon_ptr<PHR_Graph_Node> source,
   caon_ptr<PHR_Graph_Node> target,
   QString chn = QString(),
   caon_ptr<PHR_Graph_Node> cfen = nullptr);

 caon_ptr<PHR_Graph_Node> add_channel_fground_coentry_node(
   caon_ptr<PHR_Graph_Node> source,
   caon_ptr<PHR_Graph_Node> target,
   QString chn = QString(),
   caon_ptr<PHR_Graph_Node> cfen = nullptr);


 void add_statement_sequence_node(
   caon_ptr<PHR_Graph_Node> source,
   caon_ptr<PHR_Graph_Node> target, caon_ptr<PHR_Graph_Node> sin = nullptr);

 void push_expression_entry();
 void pop_expression_entry();

 void push_block_entry();
 void pop_block_entry();

 caon_ptr<PHR_Graph_Node> make_token_node(MG_Token& mgt);
 caon_ptr<PHR_Graph_Node> make_token_node(MG_Token&& mgt)
 {
  return make_token_node(mgt);
 }

 caon_ptr<PHR_Graph_Node> make_signatuChasmRZ_Node(
   caon_ptr<PHR_Graph_Node> token_node);


 caon_ptr<PHR_Graph_Node> add_type_declaration(
   MG_Token& amgt, MG_Token& tmgt);

 caon_ptr<PHR_Graph_Node> add_indexed_type_declaration(
   MG_Token& imgt, MG_Token& amgt, MG_Token& tmgt, caon_ptr<PHR_Graph_Node> sn);

 caon_ptr<PHR_Graph_Node> add_channel_token(caon_ptr<PHR_Graph_Node> source,
   MG_Token& mgt);

 caon_ptr<PHR_Graph_Node> add_channel_continue_token(caon_ptr<PHR_Graph_Node> source,
   MG_Token& mgt);

 caon_ptr<PHR_Graph_Node> add_channel_entry_token(caon_ptr<PHR_Graph_Node> source,
   QString channel, MG_Token& mgt);

 caon_ptr<PHR_Graph_Node> add_channel_xentry_token(caon_ptr<PHR_Graph_Node> source,
   QString channel, MG_Token& mgt);

 caon_ptr<PHR_Graph_Node> add_block_entry_token(caon_ptr<PHR_Graph_Node> source,
   MG_Token& mgt, caon_ptr<PHR_Graph_Node> statement_info_node);

 caon_ptr<PHR_Graph_Node> add_statement_sequence_token(caon_ptr<PHR_Graph_Node> source,
   MG_Token& mgt, caon_ptr<PHR_Graph_Node> statement_info_node);

 caon_ptr<PHR_Graph_Node> make_block_info_node();

 void load_from_pgb_file(QString file);


};

_RZNS(PhrGraphCore)



#endif // PHR_GRAPH_BUILD__H
