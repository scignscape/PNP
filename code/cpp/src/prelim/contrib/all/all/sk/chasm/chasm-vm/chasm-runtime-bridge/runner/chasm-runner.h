
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

class Chasm_Runner
{
 struct add2
 {
  template<typename LHS_Type, typename RHS_Type, typename RETURN_Type>
  static RETURN_Type proceed(LHS_Type lhs, RHS_Type rhs);

  static void init(Chasm_Value_Holder& lhs, Chasm_Value_Holder& rhs);
 };


 enum class Known_Procedure_Codes {
  N_A = 0, Add2 = 1, Mult2 = 2, Div2 = 3, Ratio2 = 4
 };

 enum class Known_Procedure_Families {
  N_A = 0, Double_VV, Single_V, Double_T, Single_T
 };


 QStringList known_procedures_;

 Chasm_Run_Router::Known_Procedure_Codes get_proc_code(QString proc);

 void init();

public:

 Chasm_Runner();


 void run_core_proc(QString proc_name, Chasm_Value_Holder& lhs, Chasm_Value_Holder& rhs);


};

_OTNS(Chasm_TR)

//#include "fns/chasm-runner--add2.h"

#endif // CHASM_RUNNER__H
