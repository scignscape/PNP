

//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#include "chtr-expression-object.h"

#include "chtr-lexical-scope.h"

#include "chtr-source-token.h"


USING_OTNS(Chasm_TR)


ChTR_Expression_Object::ChTR_Expression_Object(ChTR_CHVM_Generator_Triple gtrip)
  :  ChTR_CHVM_Generator_Triple_Holder(gtrip)
//  :  gen_("--eo--")
//  :  channel_package_(nullptr)
{
 id_ = get_next_id();
}
