
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#ifndef CHTR_EXPRESSION_OBJECT__H
#define CHTR_EXPRESSION_OBJECT__H


#include "accessors.h"

#include "otns.h"

#include "flags.h"

#include <QFile>
#include <QVector>

//?#include "tsl/ordered_map.h"

#include "global-types.h"

#include "codegen/chtr-chvm-generator.h"

OTNS_(Chasm_TR)


class ChTR_Source_Token;
class ChTR_Lexical_Scope;


class ChTR_Expression_Object
{
 u2 id_;

 ChTR_CHVM_Generator_Triple gtrip_;

 static u2 get_next_id()
 {
  static u2 result = 0;
  return ++result;
 }

public:


 ChTR_Expression_Object(ChTR_CHVM_Generator_Triple gtrip);

 QString ins_outer()
 {
  return gtrip_.outer_insertion_code;
 }

 QString ins_inner()
 {
  return gtrip_.inner_insertion_code;
 }

 ChTR_CHVM_Generator& gen()
 {
  return *gtrip_.gen;
 }

 ACCESSORS__GET(u2 ,id)


// ACCESSORS(QString ,anchor)
// ACCESSORS(ChTR_Channel_Package* ,channel_package)


};

_OTNS(Chasm_TR)


#endif // CHTR_EXPRESSION_OBJECT__H




