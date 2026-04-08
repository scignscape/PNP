
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "chasm-run-router.h"


OTNS_(Chasm_TR)

template<>
struct Chasm_Run_Router::Is_Cast_Needed<Chasm_Run_Router::Cast_Needed>
{
 static const bool Value = true;
};



template<typename LHS_Type, typename RHS_Type>
struct Chasm_Run_Router::Get_Return_Type
{
 typedef RHS_Type Type;
};



_OTNS(Chasm_TR)


#endif // CHASM_RUN_ROUTER__H
