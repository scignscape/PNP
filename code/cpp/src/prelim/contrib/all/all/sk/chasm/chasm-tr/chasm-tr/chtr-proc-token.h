
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#ifndef CHTR_PROC_TOKEN__H
#define CHTR_PROC_TOKEN__H


#include "accessors.h"

#include "otns.h"

#include "flags.h"

#include <QFile>
#include <QVector>

//?#include "tsl/ordered_map.h"

#include "global-types.h"

#include "relae-graph/relae-caon-ptr.h"

OTNS_(Chasm_TR)

class ChTR_Node;

class ChTR_Proc_Token
{
 QString text_;
 u2 infix_rank_;

 caon_ptr<ChTR_Node> operator_node_;

public:


 ChTR_Proc_Token(QString text = {}, u2 infix_rank = 0);

 ACCESSORS(QString ,text)
 ACCESSORS(u2 ,infix_rank)
 ACCESSORS(caon_ptr<ChTR_Node> ,operator_node)


};

_OTNS(Chasm_TR)


#endif // CHTR_PROC_TOKEN__H




