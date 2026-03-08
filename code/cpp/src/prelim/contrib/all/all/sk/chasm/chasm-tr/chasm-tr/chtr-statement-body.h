
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#ifndef CHTR_STATEMENT_BODY__H
#define CHTR_STATEMENT_BODY__H


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


class ChTR_Statement_Body
{
 ChTR_CHVM_Generator* gen_;

public:


 ChTR_Statement_Body(ChTR_CHVM_Generator* gen);

 ACCESSORS__GET(ChTR_CHVM_Generator* ,gen)


// ACCESSORS(QString ,anchor)
// ACCESSORS(ChTR_Channel_Package* ,channel_package)


};

_OTNS(Chasm_TR)


#endif // CHTR_STATEMENT_BODY__H




