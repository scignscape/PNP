
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef CHTR_RELAE_QUERY__H
#define CHTR_RELAE_QUERY__H

#include "kernel/chtr-dominion.h"

#include "relae-graph/relae-node-ptr.h"

#include "otns.h"

OTNS_(Chasm_TR)

class ChTR_Relae_Query : public node_query<ChTR_Dominion>
{
 ChTR_Relae_Query();

public:
  #define DOMINION_CONNECTOR(name, label) \
   ChTR_Connectors name;
  #include "kernel/dominion/connectors.h"
  #undef DOMINION_CONNECTOR

 static const ChTR_Relae_Query& instance();
};

_OTNS(Chasm_TR)

#endif
