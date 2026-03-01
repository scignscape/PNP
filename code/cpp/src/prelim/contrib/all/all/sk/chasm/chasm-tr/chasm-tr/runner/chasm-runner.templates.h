
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "chasm-runner.h"

#include "chasm-run-router.h"

#include "chasm-result-holder.h"

#include "relae-graph/relae-caon-ptr.h"

#include "graph-run/call/asg-call-v-v.h"


USING_OTNS(Chasm_TR)

template<>
void Chasm_Runner::run<ASG_Proc_Family::ASG_Graph_Call_VV>(Chasm_Result_Holder& rh, Chasm_Run_Router::Known_Procedure_Codes proc_code,
 Chasm_Value_Holder& v1, Chasm_Value_Holder& v2)
{
 typedef Chasm_Run_Router::Cast_Schedule<ASG_Proc_Family::ASG_Graph_Call_VV,
   Chasm_Run_Router::Known_Procedure_Codes::Add2, ASG_Type_Family::Internal> Cast_Shedule_type;

// Cast_Shedule_type::Runner<Cast_Shedule_type, int, ASG_Type_Family::Internal>::run(rh,
//   caon_ptr<Chasm_Value_Holder>(&v1), caon_ptr<Chasm_Value_Holder>(&v2));

 Cast_Shedule_type::Runner<Cast_Shedule_type, Chasm_Run_Router::No_Cast_Needed, ASG_Type_Family::Internal>::run(rh,
   v1, v2);

}


template<>
void Chasm_Runner::run<ASG_Proc_Family::ASG_Graph_Call_CC>(Chasm_Result_Holder& rh,
  Chasm_Run_Router::Known_Procedure_Codes proc_code,
  caon_ptr<ChTR_Node> n1, caon_ptr<ChTR_Node> n2)
{
 typedef Chasm_Run_Router::Cast_Schedule<ASG_Proc_Family::ASG_Graph_Call_CC,
   Chasm_Run_Router::Known_Procedure_Codes::Write_Operand_LHS,
   ASG_Type_Family::Internal> Cast_Shedule_type;

// Cast_Shedule_type::Runner<Cast_Shedule_type, int, ASG_Type_Family::Internal> x;
// x.test();

 Cast_Shedule_type::Runner<Cast_Shedule_type, Chasm_Run_Router::LHS_Cast_Marker, ASG_Type_Family::Internal>::run(rh,
   *n1, *n2);

//  Chasm_Run_Router::Cast_Schedule<
//    ASG_Graph_Call_CC,
//    Known_Procedure_Codes::,
//    ASG_Type_Family::Internal,
//    Chasm_Run_Router::Cast_Needed, Chasm_Run_Router::Cast_Needed>::
//    Runner<Chasm_Run_Router::Cast_Null_Marker, Chasm_Run_Router::LHS_Cast_Marker,
//     ASG_Type_Family::Internal>::run(rh,
//     n1, n1);


}


