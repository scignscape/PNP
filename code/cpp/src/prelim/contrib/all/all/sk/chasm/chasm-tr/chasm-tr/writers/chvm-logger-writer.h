
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#ifndef CHVM_LOGGER_WRITER__H
#define CHVM_LOGGER_WRITER__H


#include "accessors.h"

#include "otns.h"

#include "flags.h"

#include <QFile>
#include <QVector>

//?#include "tsl/ordered_map.h"

#include "global-types.h"

#include "codegen/chtr-chvm-generator.h"

OTNS_(Chasm_TR)

class ChTR_Channel_Package;

class ChTR_Lexical_Scope;
class Chasm_Type_System;

class Chasm_Runner;

class ChVM_Logger_Writer
{
 ChTR_CHVM_Generator* gen_;
 QString ins_code_;

 Chasm_Type_System* type_system_;
 ChTR_Lexical_Scope* lexical_scope_;

// Chasm_Runner* runner_;

public:


// ChVM_Logger_Writer(QString gen_subroutine_name, Chasm_Type_System* type_system = nullptr);
 ChVM_Logger_Writer(//?QString ins_code,
                    ChTR_CHVM_Generator* gen, Chasm_Type_System* type_system = nullptr);

 ChVM_Logger_Writer(Chasm_Type_System* type_system = nullptr);

 ChTR_CHVM_Generator& gen()
 {
//  gen_->set_active_insertion_code(ins_code_);
  return *gen_;
 }

 ACCESSORS(ChTR_Lexical_Scope* ,lexical_scope)
 ACCESSORS(Chasm_Type_System* ,type_system)
// ACCESSORS(Chasm_Runner* ,runner)


 static void write_symbol_token(ChTR_Source_Token& cst,
   ChTR_Lexical_Scope& cls, ChTR_CHVM_Generator& gen);

 static void write_symbol_token(QString token,
   ChTR_Lexical_Scope& cls, ChTR_CHVM_Generator& gen);

 ChVM_Logger_Writer& merge(ChTR_CHVM_Generator& new_lines);

 ChVM_Logger_Writer& merge(ChVM_Logger_Writer& new_lines)
 {
  return *this; //merge(new_lines.gen());
 }

 void write_symbol_token(ChTR_Source_Token& cst,
   ChTR_Lexical_Scope& cls);

 void write_symbol_token(QString token,
   ChTR_Lexical_Scope& cls);

 void write_symbol_token(ChTR_Source_Token& cst)
 {
  write_symbol_token(cst, *lexical_scope_);
 }

 void write_symbol_token(QString token)
 {
  write_symbol_token(token, *lexical_scope_);
 }


// ACCESSORS(QString ,anchor)
// ACCESSORS(ChTR_Channel_Package* ,channel_package)


};

_OTNS(Chasm_TR)


#endif // CHVM_LOGGER_WRITER__H




