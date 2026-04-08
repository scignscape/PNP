

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

