
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#ifndef CHTR_EXPRESSION_ENTRY__H
#define CHTR_EXPRESSION_ENTRY__H


#include "accessors.h"

#include "otns.h"

#include "flags.h"

#include <QFile>
#include <QVector>

#include "global-types.h"

#include "relae-graph/relae-caon-ptr.h"

OTNS_(Chasm_TR)

class ChTR_Node;

class ChTR_Expression_Entry
{
 caon_ptr<ChTR_Node> proc_node_;
 u2 infix_rank_;

public:


 ChTR_Expression_Entry();

 ACCESSORS(caon_ptr<ChTR_Node> ,proc_node)
 ACCESSORS(u2 ,infix_rank)


};

_OTNS(Chasm_TR)


#endif // CHTR_EXPRESSION_ENTRY__H




