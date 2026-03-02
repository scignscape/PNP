
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef CHASM_RUNNER__H
#define CHASM_RUNNER__H

#include <QString>

#include <QStack>
#include <QVariant>

#include <QDebug>

#include "chasm-lib/chasm/chasm-carrier.h"


#include "accessors.h"
#include "flags.h"

#include "global-types.h"
#include "otns.h"

#include "chasm-value-holder.h"
#include "chasm-run-router.h"

OTNS_(Chasm_TR)


class Chasm_Runtime;

class Chasm_Runtime_Bridge;

class Chasm_Value_Holder;
class Chasm_Result_Holder;
class Chasm_Type_System;

class ChTR_Node;


class Chasm_Runner
{

// enum class Known_Procedure_Codes {
//  N_A = 0, Add2 = 1, Mult2 = 2, Div2 = 3, Ratio2 = 4,
//  Write_Operand_LHS = 5, Write_Operand_RHS = 6, Write_Statement = 7
// };

 enum class Known_Procedure_Families {
  N_A = 0, Double_VV, Single_V, Double_T, Single_T
 };

 Chasm_Type_System* type_system_;

 QStringList known_procedures_;
 QMap<QString, QString> known_procedure_map_;


 void init();

 template<ASG_Proc_Family>
 static void run(Chasm_Result_Holder& rh, Chasm_Run_Router::Known_Procedure_Codes proc_code,
  Chasm_Value_Holder& v1, Chasm_Value_Holder& v2);

 template<ASG_Proc_Family>
 static void run(Chasm_Result_Holder& rh, Chasm_Run_Router::Known_Procedure_Codes proc_code,
   caon_ptr<ChTR_Node> n1, caon_ptr<ChTR_Node> n2);


public:

 Chasm_Runner(Chasm_Type_System* type_system = nullptr);

 ACCESSORS(Chasm_Type_System* ,type_system)

 Chasm_Run_Router::Known_Procedure_Codes get_proc_code(QString proc);

 void run_core_proc(QString proc_name, Chasm_Result_Holder& rh, Chasm_Value_Holder& lhs, Chasm_Value_Holder& rhs);

 void run_core_proc(QString proc_name, Chasm_Result_Holder& rh,
   caon_ptr<ChTR_Node> lhs, caon_ptr<ChTR_Node> rhs);

 void run_core_proc(QString proc_name, Chasm_Result_Holder& rh,
   ChTR_Node& lhs, caon_ptr<ChTR_Node> rhs);

};

_OTNS(Chasm_TR)

//#include "fns/chasm-runner--add2.h"

#endif // CHASM_RUNNER__H
