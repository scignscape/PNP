
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#ifndef CHTR_LEXICAL_SCOPE__H
#define CHTR_LEXICAL_SCOPE__H


#include "accessors.h"

#include "otns.h"

#include "flags.h"

#include <QFile>
#include <QVector>

//?#include "tsl/ordered_map.h"

#include "global-types.h"

#include "codegen/chtr-chvm-generator.h"

OTNS_(Chasm_TR)

class ChTR_Type_Object;

class ChTR_Lexical_Scope
{
 QMap<QString, ChTR_Type_Object*> known_symbols_;

public:

 ChTR_Lexical_Scope();

 void add_symbol(QString token, ChTR_Type_Object* cto)
 {
  known_symbols_[token] = cto;
 }

 QString get_symbol_name(QString token)
 {
  if(known_symbols_.contains(token))
    return token;

  return {};
 }
};



_OTNS(Chasm_TR)


#endif // CHTR_LEXICAL_SCOPE__H




