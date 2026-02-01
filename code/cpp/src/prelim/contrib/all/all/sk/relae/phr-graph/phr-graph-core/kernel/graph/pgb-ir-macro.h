
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PGB_IR_MACRO__H
#define PGB_IR_MACRO__H


#include "pgb-ir-build.h"

#include "accessors.h"

#include "multigraph-token.h"

#include <QTextStream>

#include "rzns.h"

struct MG_Token;

RZNS_(PhrGraphCore)


class PGB_IR_Macro
{
 enum class Macros {

  N_A, add_empty_channel, add_signature_carrier_via_type_name,

 };

 Macros parse_macro(QString key);

 Macros macro_;
 QList<MG_Token> mgts_;

public:

 PGB_IR_Macro(QString mn, QList<MG_Token>& mgts);

 void write_expand(PGB_IR_Build& pgb, QList<PGB_IR_Build::Text_With_Purpose>& tps);

};

_RZNS(PhrGraphCore)



#endif // PGB_IR_MACRO__H
