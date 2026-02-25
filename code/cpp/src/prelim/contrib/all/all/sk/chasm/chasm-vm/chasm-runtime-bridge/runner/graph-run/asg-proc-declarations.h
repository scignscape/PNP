
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASG_PROC_DECLARATIONS__H
#define ASG_PROC_DECLARATIONS__H

//#include "rzns.h"

#include "otns.h"

#define ASG_CALL_IMPLEMENT template<> struct ASG_Proc_Run

#ifdef HIDE
#include "functions/graph-call-s.h"
#include "functions/graph-call-c-c.h"
#include "functions/graph-call-c-t.h"
#include "functions/graph-call-c.h"
#include "functions/graph-call-t-c.h"
#include "functions/graph-call-t-v.h"
#include "functions/graph-call-c-v.h"
#include "functions/graph-call-t.h"
#include "functions/graph-call-tc.h"
#include "functions/graph-call-tv.h"
#include "functions/graph-call-v-t.h"
#include "functions/graph-call-v-v.h"

#endif

#endif //RZ_ASG_CORE_FUNCTION_DECLARATIONS__H
