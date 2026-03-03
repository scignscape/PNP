
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "chasm-runtime-bridge.h"

#include "chvm-lexical-scope.h"

#include "chasm-lib/chasm/chasm-runtime.h"
#include "chasm-lib/chasm/chasm-call-package.h"

#include "chasm-lib/chasm/chasm-carrier.h"
#include "chasm-lib/chasm/chasm-channel.h"

#include "chasm-procedure-table/chasm-procedure-table.h"

#include "chasm-lib/chasm/types/chasm-type-object.h"

#include "writers/chvm-logger-writer.h"

#include "csm-ghost-scope.h"

#include <QString>
#include <QVariant>
#include <QByteArray>

#include <QUrl>

#include <QPoint>

USING_OTNS(Chasm_TR)


Chasm_Runtime_Bridge::Chasm_Runtime_Bridge(Chasm_Runtime* csr)
  :  csr_(csr), current_call_package_(nullptr),
     current_value_literal_position_(nullptr),
     current_type_object_(nullptr), current_carrier_deque_(nullptr),
     current_loaded_raw_value_(0), current_ghost_scope_(nullptr),
     current_lexical_scope_(nullptr),
     proctable_(nullptr),
     max_interned_symbol_(0),
     current_ql_vector_(nullptr),
     runner_(&csr->type_system())
{
 current_carrier_deque_target_ = "lambda";

 const QVector<Chasm_Type_Object*>& pto = *csr_->pretype_type_objects();

 type_object_ref_ = pto[0];
 type_object_u1_ = pto[1];
 type_object_u2_ = pto[2];
 type_object_QString_ = pto[3];
 type_object_u4_ = pto[4];
 type_object_QByteArray_ = pto[5];
 type_object_r8_ = pto[6];
 type_object_QVariant_ = pto[7];
 type_object_n8_ = pto[8];
 type_object_ptr_ = pto[9];

 channel_handoff_keys_ = QStringList {"retv", "ctor", "error"};
}

void Chasm_Runtime_Bridge::init_new_ghost_scope()
{
 if(current_ghost_scope_)
   active_ghost_scopes_.push(current_ghost_scope_);
 current_ghost_scope_ = new CSM_Ghost_Scope;
}

void Chasm_Runtime_Bridge::clear_current_ghost_scope()
{
 if(current_ghost_scope_)
   current_ghost_scope_->clear_all();
}

void Chasm_Runtime_Bridge::run_proc_eval()
{
 QString hp = held_procnames_.top();

 if(hp.isEmpty())
 {
  // taken from carrier ...
 }

 run_eval(hp);
}

void Chasm_Runtime_Bridge::load_proc_name(QString name)
{
 held_procnames_.push(name);
 current_call_package_->add_string_carrier(name);
}


void Chasm_Runtime_Bridge::run_eval(QString proc_name)
{
 {
  auto it = proctable_->procedure_name_resolutions().find(proc_name);
  if(it != proctable_->procedure_name_resolutions().end())
    proc_name = *it;
 }
 auto it = proctable_->registered_procedures().find(proc_name);
 if(it == proctable_->registered_procedures().end())
 {
  Chasm_Run_Router::Known_Procedure_Codes kpc = runner_.get_proc_code(proc_name);
  if(kpc == Chasm_Run_Router::Known_Procedure_Codes::N_A)
    return; // error ...
  Chasm_Value_Holder lhs, rhs;
  Chasm_Carrier& cc1 = current_call_package_->channel("lambda")->carrier_ref(0);
  Chasm_Carrier& cc2 = current_call_package_->channel("lambda")->carrier_ref(1);

  lhs.set_value(cc1.raw_value());
  rhs.set_value(cc2.raw_value());

  lhs.set_type_object(cc1.type_object());
  rhs.set_type_object(cc2.type_object());

  ChVM_Logger_Writer clw(&csr_->type_system());
  Chasm_Result_Holder rh(&clw);

  runner_.run_core_proc(proc_name, rh, lhs, rhs);

  Chasm_Carrier rcar;
  rcar.set_hint("retv:" + proc_name);
  rcar.set_type_flag(rh.type_object()->get_pretype_code());
  rcar.set_type_object(rh.type_object());
  rcar.set_value(rh.value());
  current_call_package_->add_carrier("retv", rcar);
  qDebug() << rcar.raw_value();
 }
 else
 {
  const QPair<CFC_Pair, _minimal_fn_type>& pr = *it;
  if(pr.first.first.convention == 0)
  {
   if(pr.first.first.return_code == 0)
   {
    if(proc_name.contains("@>"))
    {
     Chasm_Carrier& cc = current_call_package_->channel("lambda")->first_carrier_ref();
     QVector<QPair<QString, QVector<QVariant>>>* vptr =
       current_call_package_->channel("qlambda")->coalesce_to_query(cc);

     vptr->append({":_orig", {QVariant(string_lines_acc_)}});
    }
    csr_->evaluate(current_call_package_, pr.first, pr.second.s0);
   }
   else
   {
    // //  normal run through channel package
    Chasm_Carrier rcar;
    rcar.set_hint("retv:" + proc_name);
    rcar.set_type_flag(pr.first.first.return_code);
    csr_->evaluate(current_call_package_, pr.first, pr.second.s0r1, &rcar);
    current_call_package_->add_carrier("retv", rcar);
    qDebug() << rcar.raw_value();
   }
  }
  else if(pr.first.first.convention == 1)
  {
   csr_->evaluate(current_call_package_, pr.first, pr.second.s1);
  }
 }
}

void Chasm_Runtime_Bridge::resolve_handoffs(QMap<QString, QString> channels)
{
 for(QString k : channel_handoff_keys_)
 {
  auto it = channels.find(k);
  if(it == channels.end())
    continue;

  Chasm_Channel* ch = current_call_package_->channel(k);
  for(Chasm_Carrier& cc : ch->carriers())
  {
   held_handoff_carriers_[*it].push_back(cc);
  }
 }
}


void Chasm_Runtime_Bridge::load_ql_key(QString token)
{
 current_ql_vector_->push_back({token, {}});
}


void Chasm_Runtime_Bridge::load_ql_token(QString token)
{
 auto& pr = current_ql_vector_->back();

 QStringList* qsl = new QStringList({token});
 Chasm_Carrier cc = csr_->gen_carrier<void*>().take_value(&qsl);
//? cc.set_value(qv);
 pr.second.push_back(cc);
}

void Chasm_Runtime_Bridge::append_ql_token(QString token)
{
 auto& pr = current_ql_vector_->back();

 Chasm_Carrier cc = pr.second.last();
 QStringList& qsl = cc.value_as<QStringList>();
 qsl.push_back(token);
}

void Chasm_Runtime_Bridge::track_string_line(QString line)
{
 // //  3-digit number to start indicates how many spaces up front
 string_lines_acc_.push_back(line.mid(5));
}

void Chasm_Runtime_Bridge::string_lines_to_follow()
{
 string_lines_acc_.clear();
}


void Chasm_Runtime_Bridge::gen_voidp_carrier()
{
 Chasm_Carrier cc = csr_->gen_carrier<void*>();
 check_claims(cc);
 current_carrier_deque_->push_back(cc);
}

void Chasm_Runtime_Bridge::ql_key_empty()
{
 current_ql_vector_->push_back({{}, {}});
}

void Chasm_Runtime_Bridge::new_qlambda()
{
 if(current_ql_vector_)
   delete current_ql_vector_;

 current_ql_vector_ = new QVector<QPair<QString, QVector<Chasm_Carrier>>>;
}

void Chasm_Runtime_Bridge::insert_ql_vector_ptr()
{
 Chasm_Carrier cc = csr_->gen_carrier<void*>().take_value(&current_ql_vector_);
 check_claims(cc);
 current_carrier_deque_->push_back(cc);
}


void Chasm_Runtime_Bridge::pop_proc_name()
{
 held_procnames_.pop();
}


void Chasm_Runtime_Bridge::pull_call_package()
{
 current_call_package_ = csr_->pull_call_package();

 QString str = current_call_package_->channel("proc")->first_carrier().string_ptr_value();
 qDebug() << str;

 QMapIterator<QString, QVector<Chasm_Carrier>> it(held_handoff_carriers_);
 while(it.hasNext())
 {
  it.next();

  QString name = it.key();

  Chasm_Channel* ch = current_call_package_->check_channel(name);

  if(name == current_carrier_deque_target_)
  {
   current_call_package_->set_build_channel(ch);

   for(const Chasm_Carrier& cc : it.value())
   {
    current_carrier_deque_->push_back(cc);
   }
  }
  else
  {
   for(const Chasm_Carrier& cc : it.value())
   {
    ch->add_carrier(cc);
   }
  }

 }

 // //  any cleanup?
 //it.value().clear();

 held_handoff_carriers_.clear();
}


void Chasm_Runtime_Bridge::pop_call_package()
{
 csr_->pop_call_package();
// QString str = current_call_package_->channel("proc")->first_carrier().string_ptr_value();
// qDebug() << str;


}

void Chasm_Runtime_Bridge::resolve_handoffs(QString channels)
{
 QStringList qsl = channels.simplified().split(" ");

 QMap<QString, QString> handoff;

 while(!qsl.isEmpty())
 {
  QString k = qsl.takeFirst();
  handoff[k] = qsl.takeFirst();
 }

 resolve_handoffs(handoff);
}

void Chasm_Runtime_Bridge::gen_retvalue_channel_u4()
{
 current_call_package_->add_new_channel("retv");
 Chasm_Carrier rcc = csr_->gen_carrier(4);
 current_call_package_->add_carrier(rcc);
}

void Chasm_Runtime_Bridge::gen_return_channels()
{
 current_call_package_->add_new_channel("retv");

 // //  possible exception, etc.
}


void Chasm_Runtime_Bridge::statement_line_number(QString value)
{
 csr_->set_current_statement_line_number(value.toUInt());
}

void Chasm_Runtime_Bridge::source_file_index(QString value)
{
 csr_->set_current_source_file_index(value.toUInt());
}

void Chasm_Runtime_Bridge::reset_carrier_deque()
{
 if(carrier_stacks_.isEmpty())
   current_carrier_deque_->clear();

 else
 {
  // //  need to handle carriers too
  delete current_carrier_deque_;

  current_carrier_deque_ = carrier_stacks_.pop();
 }

}

void Chasm_Runtime_Bridge::add_carriers_statement_context()
{
 current_call_package_->add_carriers(*current_carrier_deque_);
}

void Chasm_Runtime_Bridge::add_carriers()
{
 current_call_package_->add_carriers(*current_carrier_deque_);
}

void Chasm_Runtime_Bridge::push_carrier_deque()
{
 if(current_carrier_deque_)
   carrier_stacks_.push(current_carrier_deque_);
 current_carrier_deque_ = new std::deque<Chasm_Carrier>;
}

void Chasm_Runtime_Bridge::check_claims(const Chasm_Carrier& cc)
{
 check_ghost(cc);
}

void Chasm_Runtime_Bridge::check_ghost(const Chasm_Carrier& cc)
{
 if(current_ghost_scope_)
   current_ghost_scope_->add_carrier(cc);
}


void Chasm_Runtime_Bridge::load_symbol_u_(QString literal, u1 radix)
{
 current_loaded_raw_value_ = literal.toULongLong(nullptr, radix);
 if(!current_type_object_)
   infer_unsigned_type();

}

void Chasm_Runtime_Bridge::load_symbol_s_(QString literal, u1 radix)
{
 current_loaded_raw_value_ = literal.toInt(nullptr, radix);
 if(!current_type_object_)
   infer_signed_type();
}


// // unsigned, base 10
void Chasm_Runtime_Bridge::load_symbol_u10(QString literal)
{
 load_symbol_u_(literal, 10);
}

// // unsigned, base 2
void Chasm_Runtime_Bridge::load_symbol_u2(QString literal)
{
 load_symbol_u_(literal, 2);
}


// // unsigned, base 8
void Chasm_Runtime_Bridge::load_symbol_u8(QString literal)
{
 load_symbol_u_(literal, 8);
}

// // unsigned, base 16
void Chasm_Runtime_Bridge::load_symbol_u16(QString literal)
{
 load_symbol_u_(literal, 16);
}

// // unsigned, base 32
void Chasm_Runtime_Bridge::load_symbol_u32(QString literal)
{
 load_symbol_u_(literal, 32);
}


// // signed, base 10
void Chasm_Runtime_Bridge::load_symbol_s10(QString literal)
{
 load_symbol_s_(literal, 10);
}

// // signed, base 2
void Chasm_Runtime_Bridge::load_symbol_s2(QString literal)
{
 load_symbol_s_(literal, 2);
}


// // signed, base 8
void Chasm_Runtime_Bridge::load_symbol_s8(QString literal)
{
 load_symbol_s_(literal, 8);
}

// // signed, base 16
void Chasm_Runtime_Bridge::load_symbol_s16(QString literal)
{
 load_symbol_s_(literal, 16);
}

// // unsigned, base 32
void Chasm_Runtime_Bridge::load_symbol_s32(QString literal)
{
 load_symbol_s_(literal, 32);
}


void Chasm_Runtime_Bridge::gen_carrier_lsr()
{
 gen_carrier_tvr({}); //?
}

void Chasm_Runtime_Bridge::gen_carrier_tvr(QString rep)
{
 Chasm_Typed_Value_Representation tvr({current_type_object_, current_loaded_raw_value_, rep});
 Chasm_Carrier cc = csr_->gen_carrier(tvr);
 check_claims(cc);
 current_carrier_deque_->push_back(cc);
}


void Chasm_Runtime_Bridge::load_string_literal(QString literal)
{
 load_type_QString();
 Chasm_Carrier cc = csr_->gen_carrier<QString>(&literal);
 check_claims(cc);
 current_carrier_deque_->push_back(cc);
}


void Chasm_Runtime_Bridge::reset_loaded_raw_value()
{
 current_loaded_raw_value_ = 0;
}

void Chasm_Runtime_Bridge::reset_type_object()
{
 current_type_object_ = nullptr;
}

Chasm_Carrier Chasm_Runtime_Bridge::last_carrier()
{
 return current_carrier_deque_->back();
}

void Chasm_Runtime_Bridge::gen_carrier(Chasm_Type_Object* cto)
{
 Chasm_Carrier cc = csr_->gen_carrier_by_type_object(cto);
 check_claims(cc);
 current_carrier_deque_->push_back(cc);
}

void Chasm_Runtime_Bridge::gen_carrier_with_raw_value()
{
 Chasm_Carrier cc = csr_->gen_carrier_by_type_object(current_type_object_);
 cc.set_raw_value(current_loaded_raw_value_);
 check_claims(cc);
 current_carrier_deque_->push_back(cc);
}


void Chasm_Runtime_Bridge::gen_carrier()
{
 gen_carrier(current_type_object_);
}

void Chasm_Runtime_Bridge::gen_carrier(QString symbol, Chasm_Type_Object* cto)
{
 Chasm_Carrier cc = csr_->gen_carrier_by_type_object(cto);

 cc.set_hint(symbol);

// cc.set_fcode(current_source_file_index_);
// cc.set_lcode(current_statement_line_number_);

 cc.set_ccode(interned(symbol));

 n8 val = current_lexical_scope_->retrieve_value(symbol);
 cc.set_raw_value(val);

 check_claims(cc);
 current_carrier_deque_->push_back(cc);
}

void Chasm_Runtime_Bridge::gen_carrier(void* pv)
{
 Chasm_Carrier cc = csr_->gen_carrier_by_type_object(current_type_object_, pv);
 check_claims(cc);
 current_carrier_deque_->push_back(cc);
}


void Chasm_Runtime_Bridge::init_source_file_lexical_scope()
{
 current_lexical_scope_ = new CHVM_Lexical_Scope();
}


void Chasm_Runtime_Bridge::load_type_object(QString token)
{

}

void Chasm_Runtime_Bridge::load_carrier_symbol_lxs(QString symbol)
{
 Chasm_Type_Object* cto = current_lexical_scope_->type_object_for_symbol(symbol);


 gen_carrier(symbol, cto);
 current_type_object_ = nullptr;
}

void Chasm_Runtime_Bridge::load_value_literal(QString token)
{
 *current_value_literal_position_ = token;
}

void Chasm_Runtime_Bridge::load_unsigned_literal_int(QString token)
{
 load_symbol_u10(token);
 gen_carrier_with_raw_value();
 current_type_object_ = nullptr;
}



void Chasm_Runtime_Bridge::resolve_pins()
{
 for(QStringList& qsl : current_pins_)
 {
  resolve_value_literal(qsl);
 }
}

u4 Chasm_Runtime_Bridge::truncate_u(u4 value, u1 byte_span)
{
 switch (byte_span)
 {
 case 1: return (u4)(u1) value;
 case 2: return (u4)(u2) value;

 default: return value;
 }
}

s4 Chasm_Runtime_Bridge::truncate_s(s4 value, u1 byte_span)
{
 switch (byte_span)
 {
 case 1: return (s4)(u1) value;
 case 2: return (s4)(u2) value;

 default: return value;
 }
}

void Chasm_Runtime_Bridge::resolve_value_literal(QStringList& qsl)
{
 QString symbol = qsl.first();
 QString value = qsl.last();

 Chasm_Type_Object* cto = current_lexical_scope_->type_object_for_symbol(symbol);

 n8 nval = 0;

 switch (cto->built_in_status())
 {
 case Chasm_Type_Object::Built_In_Status::u_like:
  {
   u4 val = value.toUInt();
   nval = truncate_u(val, cto->byte_span());
  }
  break;
 default:
  break;
 }

 current_lexical_scope_->register_value(symbol, nval);
}

void Chasm_Runtime_Bridge::single_init_pin(QString symbol)
{
 current_pins_.push_back({symbol, ""});
 current_value_literal_position_ = &current_pins_.last()[1];
}

void Chasm_Runtime_Bridge::declare_lexical_typed_symbol(QString symbol)
{
 current_lexical_scope_->register_symbol(symbol, current_type_object_);
}

void Chasm_Runtime_Bridge::load_type_ref()
{
 current_type_object_ = type_object_ref_;
}

void Chasm_Runtime_Bridge::load_type_u1()
{
 current_type_object_ = type_object_u1_;
}

void Chasm_Runtime_Bridge::load_type_u2()
{
 current_type_object_ = type_object_u2_;
}

void Chasm_Runtime_Bridge::load_type_QString()
{
 current_type_object_ = type_object_QString_;
}

void Chasm_Runtime_Bridge::load_type_u4()
{
 current_type_object_ = type_object_u4_;
}

void Chasm_Runtime_Bridge::load_type_QByteArray()
{
 current_type_object_ = type_object_QByteArray_;
}

void Chasm_Runtime_Bridge::load_type_r8()
{
 current_type_object_ = type_object_r8_;
}

void Chasm_Runtime_Bridge::load_type_QVariant()
{
 current_type_object_ = type_object_QVariant_;
}

void Chasm_Runtime_Bridge::load_type_n8()
{
 current_type_object_ = type_object_n8_;
}

void Chasm_Runtime_Bridge::load_type_ptr()
{
 current_type_object_ = type_object_ptr_;
}


void Chasm_Runtime_Bridge::new_call_package() //Chasm_Call_Package*
{
 current_call_package_ = csr_->new_call_package();
}

void Chasm_Runtime_Bridge::add_new_channel(QString name)
{
 current_call_package_->add_new_channel(name);
}

