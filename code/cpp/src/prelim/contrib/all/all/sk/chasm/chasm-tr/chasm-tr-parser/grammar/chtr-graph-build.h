
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef ChTR_GRAPH_BUILD__H
#define ChTR_GRAPH_BUILD__H

#include "flags.h"

#include "accessors.h"

#include "global-types.h"

#include "grammar/chtr-parse-context.h"

#include "kernel/graph/chtr-graph.h"

#include "types/chtr-type-system.h"

#include "codegen/chtr-chvm-generator.h"

#include "chasm-tr/runner/chasm-runner.h"
#include "chasm-tr/chtr-lexical-scope.h"

#include "chasm-lib/chasm/types/chasm-type-system.h"

#include <QStack>


#include "otns.h"
OTNS_(Chasm_TR)

class ChTR_Graph;
class ChTR_Node;
class ChTR_Node_Factory;


class ChTR_Document;
class ChTR_Parser;
class ChTR_Graph;
class ChTR_Relae_Frame;
class ChTR_Relae_Query;


class ChTR_Source_Type;
class ChTR_Source_File;

class ChTR_Channel_Package;
class ChTR_Channel_Object;
class ChTR_Code_Statement;

class ChTR_Lexical_Scope;


class ChTR_Graph_Build
{

public:

 flags_(2)
  bool discard_acc:1;
  bool split_acc:1;
  bool array_field:1;
  bool numeric_acc:1;
  bool replace_acc:1;
  bool wildcard_acc:1;
  bool define_replacement:1;
  bool query_lambda_token_expected_another;
  //bool expecting_field:1;
 _flags

 typedef ChTR_Graph::hypernode_type hypernode_type;
 typedef ChTR_Graph::numeric_index_type numeric_index_type;

private:

 //QString

 ChTR_CHVM_Generator base_gen_;
 ChTR_CHVM_Generator* alt_gen_;


 ChTR_CHVM_Generator& gen();

// void acc(QString text);


 ChTR_Parse_Context parse_context_;


 ChTR_Document* document_;
 ChTR_Parser& parser_;
 ChTR_Graph& graph_;

 ChTR_Relae_Frame& If;
 ChTR_Relae_Frame& Sf;
 const ChTR_Relae_Query& Qy;

 ChTR_Node_Factory& node_factory_;


 QVector<hypernode_type*> top_level_hypernodes_;

 u2 held_line_number_;
 u1 current_context_code_;

 ChTR_Source_Type* current_source_type_;

 ChTR_Source_File* current_source_file_;

 ChTR_Channel_Package* current_channel_package_;
 ChTR_Channel_Object* current_channel_object_;
 ChTR_Code_Statement* current_code_statement_;

 ChTR_Node* current_statement_level_node_;

 ChTR_Type_System type_system_;
 Chasm_Type_System chasm_type_system_;


 u4 current_line_number_;

 QString current_channel_name_;

 u4 source_file_index_;

 void cut();
 void sharp_cut();

// QTextStream acc;


 typedef union {void(ChTR_Graph_Build::*fn0)();
   void(ChTR_Graph_Build::*fn1)(QString);} fn_u;

 QVector<QPair<QString, fn_u>> line_ops_;

 enum class Expression_States {

  N_A, Held_Declare_Point_Token, Held_Anchor_Token, Expression_Return

 };
 Expression_States current_expression_state_;

 enum class Channel_States : s2 {

  N_A, Implicit_Lambda = 1, Explicit_Lambda = 2, Named_Channel_Entered = 4, Named_Channel_Exited = 8,
  Implicit_QLambda = 16, Explicit_QLambda = 32
  //Expression_Return = 256
 };

 Channel_States current_channel_state_;

 ChTR_Lexical_Scope* current_lexical_scope_;
 ChTR_Lexical_Scope file_lexical_scope_;


 caon_ptr<ChTR_Node> current_parse_node_;
 caon_ptr<ChTR_Node> current_left_operand_node_;
 caon_ptr<ChTR_Node> current_right_operand_node_;
 caon_ptr<ChTR_Node> topmost_infix_operator_node_;
 caon_ptr<ChTR_Node> current_infix_operator_node_;

 QMap<QString, u2> infix_ranks_;

 void ql_tokens_init(QString last_instruction = {});

 u2 string_lines_count_;

 u2 current_nesting_depth_;

 Chasm_Runner runner_;

 QStack<QPair<caon_ptr<ChTR_Node>, caon_ptr<ChTR_Node>>> statement_node_stack_;

 caon_ptr<ChTR_Node> current_statement_proc_node_;
 caon_ptr<ChTR_Node> current_statement_body_node_;

 QStack<QString> insertion_codes_;

 u2 insertion_index_;

 QString make_insertion_code(u2 pos)
 {
  ++insertion_index_;
  return "@>>%1:%2<<@"_qt.arg(insertion_index_).arg(pos);
 }

 ChTR_CHVM_Generator_Triple make_insertion_triple();
 ChTR_CHVM_Generator_Triple passive_insertion_triple();

 void write_infix_expression(caon_ptr<ChTR_Node> operator_node, ChVM_Logger_Writer& clw);


public:

 ChTR_Graph_Build(ChTR_Document* d, ChTR_Parser& p, ChTR_Graph& g);

 ACCESSORS__RGET(ChTR_Parse_Context ,parse_context)
 ACCESSORS__RGET(QVector<hypernode_type*> ,top_level_hypernodes)

 void load_pregraph(QString file_path);

 void parse_line_number(QString text);

 void init();

 void track_string_line(QString line);
 void string_lines_to_follow();

 void read_line(QString fn, QString arg);
 void read_line(QString fn);

 void run_lines();

 void read_graph_build_program(QString lines);

 QString chvm_code();

 void source_file(QString file_path);

 void check_resolve_statement();

 void expression_to_statement();
 void expression_to_expression();

 void write_handoff_rtl();
 void write_handoff_rts();
 void resolve_expression();
 void resolve_statement();
 void enter_expression();
 void enter_statement();

 void check_resolve_infix_tree();
 void write_infix_expression(caon_ptr<ChTR_Node> operator_node);


 void enter_infix_mode();
 void leave_infix_mode();

 void expression_proc_name(QString token);
 void expression_depth(QString token);

 void symbol_token_operand_node(QString symbol);
 void infix_proc_name_node(QString token);

 void scoped_symbol_decl(QString symbol);
 void type_expression_token(QString token);
 void scoped_symbol_pin(QString symbol);
 void proc_name(QString token);
 void query_proc_name(QString token);

 void statement_proc_name(QString token);

 void symbol_token(QString token);
 void query_lambda_token(QString token);
 void query_lambda_token_expecting_another(QString token);

 void ql_keyword_token(QString token);

 void pin_value_literal(QString token);

 void source_file_end();

 void enter_statement_body();

 void prepare_carrier_declaration(QString symbol);

 void enter_channel_body();
 void leave_channel_body();

 void read_channel_string(QString channel_string);
 void read_carrier_string(QString carrier_string);



};

_OTNS(Chasm_TR)

#endif

