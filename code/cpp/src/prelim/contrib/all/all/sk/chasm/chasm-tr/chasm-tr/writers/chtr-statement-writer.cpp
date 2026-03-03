

//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#include "chtr-statement-writer.h"

#include "chtr-source-token.h"

#include "chtr-lexical-scope.h"


USING_OTNS(Chasm_TR)


ChTR_Statement_Writer::ChTR_Statement_Writer()
  :  lexical_scope_(nullptr)
//  :  channel_package_(nullptr)
{

}



void ChTR_Statement_Writer::write_symbol_token(ChTR_Source_Token& cst,
  ChTR_Lexical_Scope& cls, ChTR_CHVM_Generator& gen)
{
 QString token = cst.text();

 write_symbol_token(token, cls, gen);
}


void ChTR_Statement_Writer::write_symbol_token(ChTR_Source_Token& cst,
  ChTR_Lexical_Scope& cls)
{
 QString token = cst.text();

 write_symbol_token(token, cls);
}

void ChTR_Statement_Writer::write_symbol_token(QString token,
  ChTR_Lexical_Scope& cls)
{
 write_symbol_token(token, cls, gen_);
}


void ChTR_Statement_Writer::write_symbol_token(QString token,
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


