
#ifndef GTAGML_FRAME__H
#define GTAGML_FRAME__H

#include "relae-graph/relae-node-ptr.h"

#include "kernel/gtagml-dominion.h"

#include "kans.h"

KANS_(GTagML)


class GTagML_Frame : public node_frame<GTagML_Dominion>
{
 GTagML_Frame();

public:

 static GTagML_Frame& instance();
 static GTagML_Frame* instance_ptr() {return &instance(); }
 static const GTagML_Frame& const_instance() { return const_cast<const GTagML_Frame&>(instance()); }
};

_KANS(GTagML)

#endif
