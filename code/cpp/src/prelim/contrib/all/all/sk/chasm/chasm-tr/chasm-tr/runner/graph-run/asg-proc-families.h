
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASG_PROC_FAMILIES__H
#define ASG_PROC_FAMILIES__H

//#include "rzns.h"

#include "otns.h"

#include "global-types.h"

OTNS_(Chasm_TR)


class Chasm_Result_Holder;

enum ASG_Proc_Family
{
 ASG_Function_Family_Not_Set, ASG_Graph_Call_S, ASG_Graph_Call_T, ASG_Graph_Call_TC, ASG_Graph_Call_Tc,
 ASG_Graph_Call_Tv, ASG_Graph_Call_TV, ASG_Graph_Call_VT, ASG_Graph_Call_VV,
   ASG_Graph_Call_C, ASG_Graph_Call_CT, ASG_Graph_Call_CV,
   ASG_Graph_Call_CC, ASG_Left_Macro, ASG_Right_Macro
};

enum class ASG_Type_Family {
 N_A = 0, Internal, Class
};

#define ASG_GCALL_VV(name, fam) ASG_Graph_Call_VV, \
 ASG_Proc_Family_<ASG_Graph_Call_VV>::name, ASG_Type_Family::fam

#define ASG_GCALL_CC(name, fam) ASG_Graph_Call_CC, \
 ASG_Proc_Family_<ASG_Graph_Call_CC>::name, ASG_Type_Family::fam


template<ASG_Proc_Family apf, u2 i, ASG_Type_Family TFam>
struct ASG_Proc_Run
{
  template<typename T1, typename T2>
  static void run(Chasm_Result_Holder& rh, T1& t1, T2& t2)
  {
   qDebug() << "ff " << apf;
   qDebug() << "i " << i;
   qDebug() << "TFam " << (u2) TFam;
  }

};

// //   search for RZ_ASG_Function_Family


_OTNS(Chasm_TR)


#endif //ASG_ASG_CORE_FUNCTION_DECLARATIONS__H
