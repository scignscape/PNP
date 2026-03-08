
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "chtr-graph-build.h"

#include "kernel/frame/chtr-frame.h"

#include "kernel/graph/chtr-graph.h"

#include "kernel/query/chtr-query.h"


#include "chtr-source-file.h"

#include <QMap>

#include "global-types.h"

#include "chasm-tr/chtr-channel-object.h"
#include "chasm-tr/chtr-channel-package.h"
#include "chasm-tr/chtr-code-statement.h"

#include "chasm-tr/chtr-expression-object.h"

#include "chasm-tr/types/chtr-type-object.h"

#include "chasm-tr/chtr-statement-body.h"

#include "chtr-node-factory.h"

#include "chasm-tr/chtr-source-token.h"
#include "chasm-tr/chtr-proc-token.h"

#include "chasm-tr/writers/chvm-logger-writer.h"

#include "relae-graph/relae-caon-ptr.h"
#include "relae-graph/relae-node-ptr.h"

#include "chtr-document.h"

#include "textio.h"
USING_KANS(TextIO)


//?#include "chasm-tr/kernel/dominion/types.h"


USING_OTNS(Chasm_TR)

#define in_If If,
#define in_Sf Sf,

#define from_If &If,
#define from_Sf &Sf,


ChTR_Graph_Build::ChTR_Graph_Build(ChTR_Document* d, ChTR_Parser& p, ChTR_Graph& g)
 : Flags(0)
   ,document_(d)
   ,graph_(g)
   ,parser_(p)
   ,If(ChTR_Relae_Frame::instance("infix-frame"))
   ,Sf(ChTR_Relae_Frame::instance("semantic-frame"))
   ,Qy(ChTR_Relae_Query::instance())
   ,node_factory_(ChTR_Node_Factory::instance())
   ,source_file_index_(0)
   ,held_line_number_(0)
   ,current_context_code_(0)
   ,alt_gen_(nullptr)
   ,current_source_type_(nullptr)
   ,current_channel_package_(nullptr)
   ,current_channel_object_(nullptr)
   ,current_code_statement_(nullptr)
   ,current_statement_level_node_(nullptr)
   ,current_line_number_(0)
   ,current_channel_state_(Channel_States::N_A)
   ,current_expression_state_(Expression_States::N_A)
   ,string_lines_count_(0)
   ,current_nesting_depth_(0)
   ,current_parse_node_(nullptr)
   ,current_left_operand_node_(nullptr)
   ,current_right_operand_node_(nullptr)
   ,topmost_infix_operator_node_(nullptr)
   ,current_infix_operator_node_(nullptr)
   ,current_statement_proc_node_(nullptr)
   ,current_statement_body_node_(nullptr)
   ,insertion_index_(0)
{
 current_lexical_scope_ = &file_lexical_scope_;

 current_source_file_ = new ChTR_Source_File;

 runner_.set_type_system(&chasm_type_system_);

 infix_ranks_["+"] = 1;
 infix_ranks_["//"] = 2;

 // acc << "\n"; cut();

}

ChTR_CHVM_Generator& ChTR_Graph_Build::gen()
{
 if(alt_gen_)
   return *alt_gen_;

 return base_gen_;
}


QString ChTR_Graph_Build::chvm_code()
{
 QString result;
 gen().chvm_code(result);
 return result;
}


void ChTR_Graph_Build::parse_line_number(QString text)
{
 current_line_number_ = text.mid(1).trimmed().toUInt();
}

void ChTR_Graph_Build::sharp_cut()
{
 gen().sharp_cut();
}

void ChTR_Graph_Build::cut()
{
 gen().cut();
}

void ChTR_Graph_Build::read_graph_build_program(QString lines)
{
 s4 pos = 0;
 s4 end = lines.length();

 while(pos < end)
 {
  s4 np = lines.indexOf(";.\n", pos);
  if(np == -1)
    np = lines.indexOf("\n.\n", pos);
  if(np == -1)
    break;
  QString l = lines.mid(pos, np - pos).trimmed();
  if(l.startsWith(".;"))
  {
   pos = np + 3;
   continue;
  }
  if(l.startsWith("# "))
  {
   line_ops_.push_back({l, fn_u{.fn1 = &ChTR_Graph_Build::parse_line_number}});
   //parse_line_number(l);
   pos = np + 3;
   continue;
  }

//  bool reread = false;
  int mp = l.indexOf(" $");
//  if(mp == -1)
//  {
//   mp = l.indexOf(" @");
//   reread = true;
//  }
  if(mp != -1)
  {
   QString l1 = l.mid(0, mp).trimmed();
   QString l2 = l.mid(mp + 2, np - mp - 2).trimmed();
//   if(reread)
//     reread_substitute(l2);
   read_line(l1, l2);
  }
  else
  {
   read_line(l.trimmed());
  }
  pos = np + 3;
 }

 run_lines();
}

void ChTR_Graph_Build::run_lines()
{

// const QVector<QPair<QString*, fn_u>>& lines = it.value();
 for(auto& pr: line_ops_)
 {
  if(pr.first.isEmpty())
    (this->*(pr.second.fn0))();
  else
    (this->*(pr.second.fn1))(pr.first);
 }
}

void ChTR_Graph_Build::source_file(QString file_path)
{
 ++source_file_index_;

 gen()
   .blank()
   << "@sf " << file_path; cut();
 gen()
   .blank()
   << "init-source-file-lexical-scope"; cut();

 gen()
   .blank()
   << "source-file-index $ " << source_file_index_; cut();

 gen().blank();
}


void ChTR_Graph_Build::scoped_symbol_decl(QString symbol)
{
 ChTR_Source_Token* token = new ChTR_Source_Token(symbol);
 current_parse_node_ = node_factory_.make_new_node(token);
 current_expression_state_ = Expression_States::Held_Declare_Point_Token;
}

void ChTR_Graph_Build::type_expression_token(QString token)
{
 ChTR_Type_Object* cto = type_system_.find_type(token);

 if(!cto)
 {
  // //  error?
  return;
 }

 caon_ptr<ChTR_Node> type_node = node_factory_.make_new_node(cto);

 switch (current_expression_state_)
 {
 case Expression_States::Held_Declare_Point_Token:
  {
   caon_ptr<ChTR_Source_Token> source_token = current_parse_node_->source_token();
   QString text = source_token->text();

   current_lexical_scope_->add_symbol(text, cto);

   gen()
     .blank()
     .preamble_comment("statement-level declaration");
   //preamble()

//   gen()_.blank();

   if(cto->flags.built_in)
     gen() << "load-type-" << cto->name();
   else
     gen() << "load-type-object $ " << token;
   cut();

   gen() << "declare-lexical-typed-symbol $ " << text; cut();

   current_parse_node_ << Sf/Qy.Symbol_to_Type_Object >> type_node;

  }
  break;
 default:
  break;
 }
}

void ChTR_Graph_Build::scoped_symbol_pin(QString symbol)
{
 gen()
   .blank()
   .preamble_comment("statement-level pin")
   << "single-init-pin $ " << symbol; cut();
}


void ChTR_Graph_Build::expression_proc_name(QString token)
{
 proc_name(token);
}


void ChTR_Graph_Build::expression_depth(QString token)
{
 current_nesting_depth_ = token.toUInt();
}


void ChTR_Graph_Build::statement_proc_name(QString token)
{
 ChTR_Proc_Token* ptoken = new ChTR_Proc_Token(token);

 if(current_statement_proc_node_)
   statement_node_stack_.push({current_statement_proc_node_, current_statement_body_node_});

 current_statement_proc_node_ = node_factory_.make_new_node(ptoken);
 current_statement_proc_node_->set_hint("spt:" + token);

 current_statement_body_node_ = nullptr;

 current_channel_state_ = Channel_States::Implicit_Lambda;
}


void ChTR_Graph_Build::proc_name(QString token)
{
 gen()
   .dissolve({"add-new-channel $ proc"})
   << "load-proc-name $ " << token;

 cut();
 gen().blank();

 current_channel_state_ = Channel_States::Implicit_Lambda;
}

void ChTR_Graph_Build::query_proc_name(QString token)
{
 gen()
   .dissolve({"new-qlambda", "add-new-channel $ proc"})
   << "load-proc-name $ " << token;

 cut();
 gen().blank();

 current_channel_state_ = Channel_States::Implicit_QLambda;
}

void ChTR_Graph_Build::ql_tokens_init(QString last_instruction)
{
 QStringList qsl = {"add-new-channel $ lambda", "gen-voidp-carrier",
   "add-carriers", "reset-carrier-deque",
   "add-new-channel $ qlambda", "insert-ql-vector-ptr"};

 if(!last_instruction.isEmpty())
   qsl.push_back(last_instruction);

 gen().dissolve(qsl.toVector()).blank();
}

void ChTR_Graph_Build::ql_keyword_token(QString token)
{
 switch(current_channel_state_)
 {
 case Channel_States::Implicit_QLambda:
   ql_tokens_init();
   current_channel_state_ = Channel_States::Explicit_QLambda;
   // //  fall through
 case Channel_States::Explicit_QLambda:
  {
   gen() << "load-ql-key $ " << token; cut();
  }
 }

}


void ChTR_Graph_Build::query_lambda_token_expecting_another(QString token)
{
 query_lambda_token(token);
 flags.query_lambda_token_expected_another = true;
}

void ChTR_Graph_Build::query_lambda_token(QString token)
{
 switch(current_channel_state_)
 {
 case Channel_States::Implicit_QLambda:
   ql_tokens_init("ql-key-empty");
   current_channel_state_ = Channel_States::Explicit_QLambda;
   // //  fall through
 case Channel_States::Explicit_QLambda:
  {
   if(flags.query_lambda_token_expected_another)
   {
    gen() << "append-ql-token $ " << token;
    flags.query_lambda_token_expected_another = false;
   }
   else
     gen() << "load-ql-token $ " << token;
   cut();
  }
 }
}

void ChTR_Graph_Build::string_lines_to_follow()
{
 gen() << "string-lines-to-follow"; cut();
 string_lines_count_ = 0;
}

void ChTR_Graph_Build::track_string_line(QString line)
{
 ++string_lines_count_;

 if(string_lines_count_ == 1)
 {
  gen().blank();
 }

 gen() << "track-string-line $ " << line; cut();
}


ChTR_CHVM_Generator_Triple ChTR_Graph_Build::make_insertion_triple()
{
 QString ins = make_insertion_code(gen().size());
 QString outer = gen().current_insertion_code();
 gen().dissolve({"@ins %1 (%2)"_qt.arg(ins)});

 return {&gen(), outer, ins};
}


ChTR_CHVM_Generator_Triple ChTR_Graph_Build::passive_insertion_triple()
{
 return {&gen(), gen().current_insertion_code(), gen().current_insertion_code()};
}

void ChTR_Graph_Build::symbol_token(QString token)
{
 ChTR_Statement_Body* csb;

 if(current_statement_body_node_)
 {
  // //  now we know the statement body is more than just one token
  if(caon_ptr<ChTR_Source_Token> cst = current_statement_body_node_->source_token())
  {
   ChTR_CHVM_Generator_Triple gtrip = make_insertion_triple();
   csb = new ChTR_Statement_Body(gtrip);
   gen().push_insertion_code(gtrip.inner_insertion_code);

   //alt_gen_ = &csb->gen();

   if(current_channel_state_ == Channel_States::Implicit_Lambda)
   {
    gen().dissolve({"add-new-channel $ lambda"}).blank();

    current_channel_state_ = Channel_States::Explicit_Lambda;
   }

   // //  which gen?
   ChVM_Logger_Writer::write_symbol_token(*cst, *current_lexical_scope_, gen());
   ChVM_Logger_Writer::write_symbol_token(token, *current_lexical_scope_, gen());
   // //   mark retired for cst, current_statement_body_node_

   current_statement_body_node_ = node_factory_.make_new_node(csb);
   //ChTR_Statement_Body::write_symbol_token(*st, *current_lexical_scope_, gen());
  }

  else if(caon_ptr<ChTR_Statement_Body> ccsb = current_statement_body_node_->statement_body())
  {
   csb = ccsb.raw_pointer();

   if(current_channel_state_ == Channel_States::Implicit_Lambda)
   {
    gen().dissolve({"add-new-channel $ lambda"}).blank();
    current_channel_state_ = Channel_States::Explicit_Lambda;
   }
   ChVM_Logger_Writer::write_symbol_token(token, *current_lexical_scope_, gen());
  }

  else if(caon_ptr<ChTR_Expression_Object> ceo = current_statement_body_node_->expression_object())
  {
   if(gen().id() != ceo->gen().id())
   {
    throw "Unexpected gen mismatch";
   }

   if(current_channel_state_ == Channel_States::Implicit_Lambda)
   {
    gen().dissolve({"add-new-channel $ lambda"}).blank();
    current_channel_state_ = Channel_States::Explicit_Lambda;
   }
   ChVM_Logger_Writer::write_symbol_token(token, *current_lexical_scope_, gen());
  }

  else
  {
   // //  error?
   csb = nullptr;
  }
 }
 else
 {
  caon_ptr<ChTR_Source_Token> cst = new ChTR_Source_Token(token);
  current_statement_body_node_ = node_factory_.make_new_node(cst);

 }

// switch(current_channel_state_)
// {
// case Channel_States::Implicit_Lambda:
//   gen().dissolve({"add-new-channel $ lambda"}).blank();
//   current_channel_state_ = Channel_States::Explicit_Lambda;
//   // //  fall through
// case Channel_States::Explicit_Lambda:
//  {
//   ChTR_Statement_Body::write_symbol_token()
//  }

// switch(current_channel_state_)
// {
// case Channel_States::Implicit_Lambda:
////   gen().dissolve({"add-new-channel $ lambda"}).blank();
//   current_channel_state_ = Channel_States::Explicit_Lambda;
//  break;
//   // //  fall through
// case Channel_States::Explicit_Lambda:
//  break;

// default:
//  break;
// }

}

void ChTR_Graph_Build::pin_value_literal(QString token)
{
 gen()
   .blank()
   << "load-value-literal $ " << token;
   cut();
 gen() << "resolve-pins"; cut();
}

void ChTR_Graph_Build::check_resolve_statement()
{
// switch(current_channel_state_)
// {
// case Channel_States::Implicit_Lambda:
// case Channel_States::Explicit_Lambda:

//  gen()
//    .blank()
//    .dissolve({"add-carriers", "run-proc-eval"})
//    .blank()
//    .dissolve({"reset-carrier-deque", "clear-current-ghost-scope"})
//    .blank();
// }
}


void ChTR_Graph_Build::source_file_end()
{
 check_resolve_statement();
 gen().blank().dissolve({"@sfe"});
}

void ChTR_Graph_Build::write_handoff_rtl()
{
 gen().blank().dissolve({"resolve-handoffs $ retv lambda"});
}

void ChTR_Graph_Build::write_handoff_rts()
{

}

void ChTR_Graph_Build::resolve_expression()
{
 switch(current_channel_state_)
 {
 case Channel_States::Implicit_Lambda:
 case Channel_States::Explicit_Lambda:

  gen()
    .blank()
    .dissolve({"add-carriers", "run-proc-eval"})
    .blank()
    .dissolve({"reset-carrier-deque", "clear-current-ghost-scope"})
    .blank();
 }

}

void ChTR_Graph_Build::expression_to_statement()
{
 current_expression_state_ = Expression_States::Expression_Return;

 gen()
   .blank()
   .dissolve({"pop-proc-name", "pull-call-package"});
//   .blank()
//   .dissolve("run-proc-eval");
}

void ChTR_Graph_Build::expression_to_expression()
{
 gen()
   .blank()
   .dissolve({"pop-proc-name", "pull-call-package"});
}


void ChTR_Graph_Build::write_infix_expression(caon_ptr<ChTR_Node> operator_node, ChVM_Logger_Writer& clw)
{
 CAON_PTR_DEBUG(ChTR_Node ,operator_node)
 operator_node->debug_connections();


 caon_ptr<ChTR_Node> loperand_node = Qy.Infix_Left_Operand(in_If operator_node);
 caon_ptr<ChTR_Node> roperand_node = Qy.Infix_Right_Operand(in_If operator_node);

 CAON_PTR_DEBUG(ChTR_Node ,loperand_node)
 CAON_PTR_DEBUG(ChTR_Node ,roperand_node)

 if(caon_ptr<ChTR_Source_Token> ltoken = loperand_node->source_token())
 {
  CAON_PTR_DEBUG(ChTR_Source_Token ,ltoken)
  CAON_DEBUG_NOOP
 }

 if(caon_ptr<ChTR_Source_Token> rtoken = roperand_node->source_token())
 {
  CAON_PTR_DEBUG(ChTR_Source_Token ,rtoken)
  CAON_DEBUG_NOOP
 }

 Chasm_Result_Holder rh(&clw);
 runner_.run_core_proc("write-operand-lhs", rh, operator_node, loperand_node);
 runner_.run_core_proc("write-operand-rhs", rh, operator_node, roperand_node);

 if(caon_ptr<ChTR_Node> next_node = rh.value_as_node())
 {
  CAON_PTR_DEBUG(ChTR_Node ,next_node)

  write_infix_expression(next_node, clw);
 }

}


void ChTR_Graph_Build::write_infix_expression(caon_ptr<ChTR_Node> operator_node)
{
 ChVM_Logger_Writer clw(passive_insertion_triple(), &chasm_type_system_);

 clw.set_lexical_scope(current_lexical_scope_);

 write_infix_expression(operator_node, clw);
// clw.set_runner(&runner_);


// Chasm_Result_Holder right_rh(&clw);
// runner_.run_core_proc("write-operand-rhs", right_rh, operator_node, roperand_node);

// clw.merge(*left_rh.lwriter());

 //?alt_gen_ = nullptr;

// QString ic = insertion_codes_.pop();

//? clw.gen() << "lines-inserted-at $ " << ic;
 //?clw.gen().cut_to_front();

// gen().absorb(ic, clw.gen());
}



void ChTR_Graph_Build::check_resolve_infix_tree()
{
 if(!topmost_infix_operator_node_)
   return;

 CAON_PTR_DEBUG(ChTR_Node ,topmost_infix_operator_node_)

 caon_ptr<ChTR_Proc_Token> proc_token = topmost_infix_operator_node_->proc_token();

 CAON_PTR_DEBUG(ChTR_Proc_Token ,proc_token)

 write_infix_expression(topmost_infix_operator_node_);



}

void ChTR_Graph_Build::resolve_statement()
{
 ChVM_Logger_Writer clw(make_insertion_triple(), &chasm_type_system_);

 clw.set_lexical_scope(current_lexical_scope_);

 Chasm_Result_Holder rh(&clw);


// ChTR_Statement_Body* csb = new ChTR_Statement_Body;
// caon_ptr<ChTR_Node> n = node_factory_.make_new_node(csb);

 runner_.run_core_proc("write-statement", rh, current_statement_proc_node_, current_statement_body_node_);
 //?runner_.run_core_proc("write-statement", rh, current_statement_proc_node_, n);

// alt_gen_ = nullptr;
// base_gen_.absorb(clw.gen());

 switch(current_channel_state_)
 {
 case Channel_States::Implicit_Lambda:
 case Channel_States::Explicit_Lambda:
 case Channel_States::Implicit_QLambda:  // //   anything different with these?
 case Channel_States::Explicit_QLambda:   //     ...
  if(current_expression_state_ != Expression_States::Expression_Return)
   gen()
     .blank()
     .dissolve({"add-carriers", "run-proc-eval"});
  else
   gen()
     .blank()
     .dissolve({"add-carriers-statement-context", "run-proc-eval"});
  gen()
    .blank()
    .dissolve({"reset-carrier-deque", "clear-current-ghost-scope"})
    .blank();
 }

}


void ChTR_Graph_Build::symbol_token_operand_node(QString symbol)
{
 ChTR_Source_Token* stoken = new ChTR_Source_Token(symbol);
 caon_ptr<ChTR_Node> node = node_factory_.make_new_node(stoken);

 node->set_hint("st:" + symbol);

 if(current_infix_operator_node_)
 {
  CAON_PTR_DEBUG(ChTR_Node ,current_infix_operator_node_)

  current_infix_operator_node_ << If/Qy.Infix_Right_Operand >> node;

  current_infix_operator_node_->debug_connections();

  node << If/Qy.Infix_From_Right_Operand >> current_infix_operator_node_;
  current_right_operand_node_ = node;
 }
 else
 {
  current_left_operand_node_ = node;
 }

// else if(held_operand_node_)
// {
//  //?
// }
// else
// {
//  held_operand_node_ = node;
// }

}

void ChTR_Graph_Build::infix_proc_name_node(QString token)
{
 ChTR_Proc_Token* ptoken = new ChTR_Proc_Token(token, infix_ranks_[token]);
 caon_ptr<ChTR_Node> node = node_factory_.make_new_node(ptoken);

 node->set_hint("pt:" + token);


 if(current_right_operand_node_)
 {
  if(caon_ptr<ChTR_Source_Token> stoken = current_right_operand_node_->source_token())
  {
   CAON_PTR_DEBUG(ChTR_Source_Token ,stoken)
   caon_ptr<ChTR_Node> proc_node = Qy.Infix_From_Right_Operand(in_If current_right_operand_node_);
   CAON_PTR_DEBUG(ChTR_Node ,proc_node)
   if(caon_ptr<ChTR_Proc_Token> oper = proc_node->proc_token())
   {
    CAON_PTR_DEBUG(ChTR_Proc_Token ,oper)
    if(ptoken->infix_rank() > oper->infix_rank())
    {
     proc_node->debug_connections();

     // //   the new operator claims the operand
     proc_node->detach(from_If Qy.Infix_Right_Operand, current_right_operand_node_);
     proc_node->debug_connections();

     current_right_operand_node_->detach(from_If Qy.Infix_From_Right_Operand, proc_node);
     proc_node << If/Qy.Infix_Right_Operand >> node;
     proc_node->debug_connections();


     node << If/Qy.Infix_From_Right_Operand >> proc_node;
     node << If/Qy.Infix_Left_Operand >> current_right_operand_node_;
     current_right_operand_node_ << If/Qy.Infix_Left_Operand >> node;
     current_left_operand_node_ = current_right_operand_node_;
     current_right_operand_node_ = node;

     node->debug_connections();

    }
   }
  }
  else if(caon_ptr<ChTR_Expression_Entry> expr = current_right_operand_node_->expression_entry())
  {

  }
 }
 else
 {
  node << If/Qy.Infix_Left_Operand >> current_left_operand_node_;
  current_left_operand_node_ << If/Qy.Infix_From_Left_Operand >> node;
  topmost_infix_operator_node_ = node;
 }
 current_infix_operator_node_ = node;

}

void ChTR_Graph_Build::enter_infix_mode()
{
// s4 sz = gen().size();
// QString code = make_insertion_code(sz);
// gen() << code; sharp_cut();

// insertion_codes_.push(code);

 //enter_expression();
}

void ChTR_Graph_Build::leave_infix_mode()
{
 check_resolve_infix_tree();
 //enter_expression();
}

void ChTR_Graph_Build::enter_expression()
{
 ChTR_Statement_Body* csb;

 if(current_statement_body_node_)
 {
  // //  now we know the statement body is more than just one token
  if(caon_ptr<ChTR_Source_Token> cst = current_statement_body_node_->source_token())
  {
   ChTR_CHVM_Generator_Triple gtrip = make_insertion_triple();
   csb = new ChTR_Statement_Body(gtrip);
   gen().push_insertion_code(gtrip.inner_insertion_code);
   //?alt_gen_ = &csb->gen();

   if(current_channel_state_ == Channel_States::Implicit_Lambda)
   {
    gen().dissolve({"add-new-channel $ lambda"}).blank();

    current_channel_state_ = Channel_States::Explicit_Lambda;
   }

   // //  which gen?
   ChVM_Logger_Writer::write_symbol_token(*cst, *current_lexical_scope_, gen());
   // //   mark retired for cst, current_statement_body_node_
   current_statement_body_node_ = node_factory_.make_new_node(csb);
  }
 }
 else
 {
  ChTR_CHVM_Generator_Triple gtrip = make_insertion_triple();
  ChTR_Expression_Object* ceo = new ChTR_Expression_Object(gtrip);
  gen().push_insertion_code(gtrip.inner_insertion_code);

  //?alt_gen_ = &ceo->gen();
  current_statement_body_node_ = node_factory_.make_new_node(ceo);
 }

 gen()
   .blank()
   .preamble_comment("expression")
   << "statement-line-number $ " << current_line_number_; cut();

// .dissolve({"init-new-ghost-scope", "push-carrier-deque"})

 gen()
  .dissolve({"push-carrier-deque"})
  .blank()
  .dissolve({"new-call-package", "gen-return-channels"})
  .blank();
}

void ChTR_Graph_Build::enter_statement()
{
 gen()
   .blank()
   .preamble_comment("statement")
   << "statement-line-number $ " << current_line_number_; cut();

 gen()
  .dissolve({"init-new-ghost-scope", "push-carrier-deque"})
  .blank()
  .dissolve({"new-call-package"})
  .blank();

}



void ChTR_Graph_Build::read_line(QString fn, QString arg)
{
 static QMap<QString, void(ChTR_Graph_Build::*)(QString)> static_map {{
   { ".source-file", &ChTR_Graph_Build::source_file },
   { ".scoped-symbol-decl", &ChTR_Graph_Build::scoped_symbol_decl },
   { ".type-expression-token", &ChTR_Graph_Build::type_expression_token },
   { ".scoped-symbol-pin", &ChTR_Graph_Build::scoped_symbol_pin },
   { ".pin-value-literal", &ChTR_Graph_Build::pin_value_literal },
   { ".proc-name", &ChTR_Graph_Build::proc_name },
   { ".statement-proc-name", &ChTR_Graph_Build::statement_proc_name },
   { ".symbol-token", &ChTR_Graph_Build::symbol_token },

   { ".n/infix-proc-name", &ChTR_Graph_Build::infix_proc_name_node },
   { ".n/symbol-token-operand", &ChTR_Graph_Build::symbol_token_operand_node },

   { ".expression-proc-name", &ChTR_Graph_Build::expression_proc_name },
   { ".expression-depth", &ChTR_Graph_Build::expression_depth },

   { ".query-proc-name", &ChTR_Graph_Build::query_proc_name },
   { ".query-lambda-token", &ChTR_Graph_Build::query_lambda_token },
   { ".query-lambda-token-expecting-another", &ChTR_Graph_Build::query_lambda_token_expecting_another },
   { ".ql-keyword-token", &ChTR_Graph_Build::ql_keyword_token },
//   { ".symbol-token", &ChTR_Graph_Build::symbol_token },

   { ".track-string-line", &ChTR_Graph_Build::track_string_line },

 }};

 auto it = static_map.find(fn);
 if(it != static_map.end())
 {
//  QString* a = new QString(arg);
  line_ops_.push_back({arg, fn_u{.fn1=it.value()}});
 }

}

void ChTR_Graph_Build::read_line(QString fn)
{
 static QMap<QString, void(ChTR_Graph_Build::*)()> static_map {{
   { ".source-file-end", &ChTR_Graph_Build::source_file_end },

   { ".write-handoff-rtl", &ChTR_Graph_Build::write_handoff_rtl },
   { ".write-handoff-rts", &ChTR_Graph_Build::write_handoff_rts },
   { ".enter-expression", &ChTR_Graph_Build::enter_expression },
   { ".enter-statement", &ChTR_Graph_Build::enter_statement },
   { ".resolve-expression", &ChTR_Graph_Build::resolve_expression },
   { ".resolve-statement", &ChTR_Graph_Build::resolve_statement },
   { ".expression-to-expression", &ChTR_Graph_Build::expression_to_expression },
   { ".expression-to-statement", &ChTR_Graph_Build::expression_to_statement },

   { ".string-lines-to-follow", &ChTR_Graph_Build::string_lines_to_follow },

   { ".enter-infix-mode", &ChTR_Graph_Build::enter_infix_mode },
   { ".leave-infix-mode", &ChTR_Graph_Build::leave_infix_mode },


 }};

 auto it = static_map.find(fn);
 if(it != static_map.end())
 {
//  QString* a = new QString(arg);
  line_ops_.push_back({{}, fn_u{.fn0=it.value()}});
 }

}


void ChTR_Graph_Build::load_pregraph(QString file_path)
{
 QString lines;
 load_file(file_path, lines);
 read_graph_build_program(lines);
}




//void ChTR_Graph_Build::acc(QString contents)
//{
// acc << contents;
//}




// // // //

void ChTR_Graph_Build::init()
{
 ChTR_Channel_Package::set_channel_object_encoder([](ChTR_Channel_Object* cco)
 {
  //caon_ptr<ChTR_Channel_Object> pcco(cco);
  ChTR_Node* node = new ChTR_Node(caon_ptr<ChTR_Channel_Object>(cco));
  return (void*) node;
 });

 ChTR_Channel_Package::set_channel_object_decoder([](void* pv)//  -> ChTR_Channel_Object*
 {
  //caon_ptr<ChTR_Channel_Object> pcco(cco);
  ChTR_Node* node = (ChTR_Node*) pv;
  return node->channel_object().raw_pointer();
 });

 ChTR_Source_File* root_file = new ChTR_Source_File(document_->local_path());

 ChTR_Node* root_node = new ChTR_Node(root_file);

 graph_.set_root_node(root_node);
 current_statement_level_node_ = root_node;

 //graph_.set
}


void ChTR_Graph_Build::read_channel_string(QString channel_string)
{
 ChTR_Channel_Object* cco = new ChTR_Channel_Object(channel_string);

 if(!current_channel_package_)
 {
  current_channel_package_ = new ChTR_Channel_Package;
 }

 if(parse_context_.flags.awaiting_statement_body)
 {
  parse_context_.flags.awaiting_statement_body = false;
  current_code_statement_->set_channel_package(current_channel_package_);
 }

 current_channel_package_->add_channel_object(cco);
 current_channel_object_ = cco;

 parse_context_.flags.active_channel = true;
}

void ChTR_Graph_Build::enter_statement_body()
{
 ChTR_Code_Statement* ccs = new ChTR_Code_Statement;

 current_code_statement_ = ccs;
 parse_context_.flags.awaiting_statement_body = true;

 if(current_statement_level_node_ == graph_.root_node())
 {
  ChTR_Node* n = new ChTR_Node(ccs);
  current_statement_level_node_ << Sf/Qy.Root_Sequence >> n;
 }
}

void ChTR_Graph_Build::enter_channel_body()
{
 parse_context_.flags.open_channel_body = true;
}

void ChTR_Graph_Build::leave_channel_body()
{
 parse_context_.flags.open_channel_body = false;
}

void ChTR_Graph_Build::read_carrier_string(QString carrier_string)
{
 current_channel_object_->add_carrier(carrier_string);
}
