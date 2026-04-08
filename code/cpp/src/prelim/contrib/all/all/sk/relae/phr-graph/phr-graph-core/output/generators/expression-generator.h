
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXPRESSION_GENERATOR__H
#define EXPRESSION_GENERATOR__H

#include "accessors.h"
#include "flags.h"

#include "phr-graph-core/kernel/query/phr-graph-query.h"
#include "phr-graph-core/kernel/frame/phr-graph-frame.h"

#include "relae-graph/relae-caon-ptr.h"

#include <QTextStream>
#include <QMap>

#include <functional>

#include "rzns.h"
RZNS_(PhrGraphCore)

class PHR_Graph_Node;
class PHR_Graph_Token;
class PHR_Graph_FGround_Entry;
class PHR_Graph_Statement_Info;
class PHR_Graph_Block_Info;
class PHR_Graph_Signature;

class Statement_Generator;
class PHR_Graph_Cocyclic_Type;

class Expression_Generator
{
 struct SB_Info
 {
  PHR_Graph_Statement_Info* sin;
  PHR_Graph_Block_Info* bin;
  SB_Info() : sin(nullptr), bin(nullptr){}
  SB_Info(caon_ptr<PHR_Graph_Statement_Info> s,
    caon_ptr<PHR_Graph_Block_Info> b)
    : sin(s.raw_pointer()), bin(b.raw_pointer()){}
 };

 const PHR_Graph_Query& qy_;

 Statement_Generator* statement_generator_;

public:

 Expression_Generator();

 ACCESSORS(Statement_Generator* ,statement_generator)

 void generate_line(QTextStream& qts, QString ln);
 void generate_line(QTextStream& qts, QString ln, QString arg);
 void generate_comment_line(QTextStream& qts, QString ln, int n = 0);
 void generate_empty_line(QTextStream& qts, int n = 1);

 void generate_cocyclic_type_definition(QTextStream& qts,
   PHR_Graph_Cocyclic_Type& coy);

 void generate_block_signature(QTextStream& qts,
   PHR_Graph_Signature& sig);

 void generate_minimal_signature(QTextStream& qts,
   PHR_Graph_Signature& sig);

 void generate_alt_entry(QTextStream& qts);
 void generate_alt_close(QTextStream& qts);



 void check_generate_type_declaration(QTextStream& qts, const PHR_Graph_Node& node);
 void generate_type_declaration(QTextStream& qts,
   const PHR_Graph_Node& node);

 void generate_type_field_declaration(QTextStream& qts,
   const PHR_Graph_Node& node, QString modifier);

 void generate_arg_carriers_follow(QTextStream& qts,
   caon_ptr<PHR_Graph_Node> n, int unw);

 void generate_carrier(QTextStream& qts,
   PHR_Graph_Token& tokn);

 void generate_from_node(QTextStream& qts,
  const PHR_Graph_Node& node, int unw = 0);

 void generate_fground_entry(QTextStream& qts, PHR_Graph_FGround_Entry& fen,
  const PHR_Graph_Node& node, int unw);

 void generate_block(QTextStream& qts, PHR_Graph_Block_Info& bin,
   const PHR_Graph_Node& node, PHR_Graph_Statement_Info* sin = nullptr);

 void generate_from_fn_node(QTextStream& qts,
   PHR_Graph_Token& tok, QString channel_name,
   const PHR_Graph_Node& arg_node, int unw,
   PHR_Graph_FGround_Entry* fen = nullptr,
   SB_Info sbi = SB_Info());

 void generate_xchannel(QTextStream& qts, QString channel_name,
  const PHR_Graph_Node& arg_node, int unw);

 void generate_arg_carriers(QTextStream& qts,
   QString channel_name,
   const PHR_Graph_Node& arg_node, int unw,
   PHR_Graph_FGround_Entry* fen); //, SB_Info sbi = SB_Info());

 void generate_arg_carriers(QTextStream& qts,
  QString channel_name,
  const PHR_Graph_Node& arg_node, int unw, SB_Info sbi = SB_Info());

 void generate_carrier_with_symbol(QTextStream& qts,
  PHR_Graph_Token& tok);

 void generate_carrier_with_raw_value(QTextStream& qts,
  PHR_Graph_Token& tok);

};

_RZNS(PhrGraphCore)

#endif //EXPRESSION_GENERATOR__H
