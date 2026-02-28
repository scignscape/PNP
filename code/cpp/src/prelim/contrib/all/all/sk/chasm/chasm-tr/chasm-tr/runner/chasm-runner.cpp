
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "chasm-runner.h"

#include "chasm-result-holder.h"

#include "chasm-runner.templates.h"

USING_OTNS(Chasm_TR)


#include "textio.h"
USING_KANS(TextIO)

#include <QString>
#include <QVariant>
#include <QByteArray>

#include <QUrl>

#include <QPoint>



Chasm_Runner::Chasm_Runner(Chasm_Type_System* type_system)
  :  type_system_(type_system)
{
 init();
}

void Chasm_Runner::init()
{
 known_procedures_ << "add2" << "mult2" << "div2" << "ratio2";
 known_procedure_map_["+"] = "add2";
 known_procedure_map_["*"] = "mult2";
 known_procedure_map_["//"] = "div2";
 known_procedure_map_["/"] = "ratio2";
}

Chasm_Run_Router::Known_Procedure_Codes Chasm_Runner::get_proc_code(QString proc)
{
 proc = known_procedure_map_.value(proc, proc);
 s2 result = known_procedures_.indexOf(proc);
 return (Chasm_Run_Router::Known_Procedure_Codes) (result + 1);
}


void Chasm_Runner::run_core_proc(QString proc_name, Chasm_Result_Holder& rh,
  caon_ptr<ChTR_Node> lhs, caon_ptr<ChTR_Node> rhs)
{

}

void Chasm_Runner::run_core_proc(QString proc_name, Chasm_Result_Holder& rh, Chasm_Value_Holder& lhs, Chasm_Value_Holder& rhs)
{
 //Chasm_Run_Router::Cast_Schedule::Runner

 Chasm_Run_Router::Known_Procedure_Codes kpc = get_proc_code(proc_name);

 run<ASG_Proc_Family::ASG_Graph_Call_VV>(rh, kpc, lhs, rhs);



// switch(kpc)
// {

// case Chasm_Run_Router::Known_Procedure_Codes::Add2:
//  Chasm_Runner::add2::init(lhs, rhs);
//  break;

// default: break;
// }
}

