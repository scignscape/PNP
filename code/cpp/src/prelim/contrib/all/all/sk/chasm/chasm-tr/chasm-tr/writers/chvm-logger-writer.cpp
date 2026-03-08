

//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#include "chvm-logger-writer.h"

#include "chtr-source-token.h"

#include "chtr-lexical-scope.h"


USING_OTNS(Chasm_TR)


ChVM_Logger_Writer::ChVM_Logger_Writer(Chasm_Type_System* type_system)
 :
   lexical_scope_(nullptr), type_system_(type_system)
{
 gtrip_.gen = new ChTR_CHVM_Generator;
}


ChVM_Logger_Writer::ChVM_Logger_Writer(ChTR_CHVM_Generator_Triple gtrip, Chasm_Type_System* type_system)
  : ChTR_CHVM_Generator_Triple_Holder(gtrip),
    lexical_scope_(nullptr), type_system_(type_system)
//  :  channel_package_(nullptr)
{

}



ChVM_Logger_Writer& ChVM_Logger_Writer::merge(ChTR_CHVM_Generator& new_lines)
{
//? gen_.absorb(new_lines);
 return *this;
}


void ChVM_Logger_Writer::write_symbol_token(ChTR_Source_Token& cst,
  ChTR_Lexical_Scope& cls, ChTR_CHVM_Generator& gen)
{
 QString token = cst.text();

 write_symbol_token(token, cls, gen);
}


void ChVM_Logger_Writer::write_symbol_token(ChTR_Source_Token& cst,
  ChTR_Lexical_Scope& cls)
{
 QString token = cst.text();

 write_symbol_token(token, cls);
}

void ChVM_Logger_Writer::write_symbol_token(QString token,
  ChTR_Lexical_Scope& cls)
{
 write_symbol_token(token, cls, gen());
}


void ChVM_Logger_Writer::write_symbol_token(QString token,
  ChTR_Lexical_Scope& cls, ChTR_CHVM_Generator& gen)
{
 QString symbol_name = cls.get_symbol_name(token);

 if(symbol_name.isEmpty())
 {
  bool negative = false;
  bool positive = false;
  bool fp = false;
  if(token[0] == '-')
  {
   negative = true;
   token = token.mid(1);
  }
  if(token[0] == '+')
  {
   positive = true;
   token = token.mid(1);
  }
  if(token[0] == '.')
  {
   fp = true;
   token = token.mid(1);
  }

  if(token[0].isDigit())
  {
   QString base;
   QString signed_or_not;
   QString generic_or_literal;
   QString int_or_float;
   if(token[0] == '0')
   {
    if(token.size() == 1)
    {
     generic_or_literal = "generic";
    }
    else if(token[1].isDigit())
      base = "_0";
    else
      base = token.mid(0, 2).prepend("_");
   }

   if(negative)
     signed_or_not = "signed-negative";
   else if(positive)
     signed_or_not = "signed-positive";
   else
     signed_or_not = "unsigned";

   if(generic_or_literal.isEmpty())
     generic_or_literal = "literal";
   if(fp)
     int_or_float = "float";
   else
     int_or_float = "int";
   gen << "load-" << signed_or_not << "-" << generic_or_literal
       << "-" << int_or_float << " $ " << token; gen.cut();
  }
  return;
 }

 gen << "load-carrier-symbol-lxs $ " << symbol_name; gen.cut();
}


