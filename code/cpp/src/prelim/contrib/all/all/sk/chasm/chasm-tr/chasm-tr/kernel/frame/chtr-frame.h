
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef CHTR_FRAME__H
#define CHTR_FRAME__H


#include "relae-graph/relae-node-ptr.h"

#include "kernel/chtr-dominion.h"

#include "otns.h"

OTNS_(Chasm_TR)

class ChTR_Relae_Frame : public node_frame<ChTR_Dominion>
{
 QString label_;

 ChTR_Relae_Frame(QString label = {});

public:

 static ChTR_Relae_Frame& instance(QString label);

};


_OTNS(Chasm_TR)

#endif
