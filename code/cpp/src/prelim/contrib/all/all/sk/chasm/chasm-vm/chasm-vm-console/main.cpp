
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include <QDebug>

#include "textio.h"
USING_KANS(TextIO)


#include "chasm-lib/chasm/chasm-runtime.h"

#include "chasm-vm/chasm-vm.h"
#include "chasm-runtime-bridge/chasm-runtime-bridge.h"
#include "chasm-procedure-table/chasm-procedure-table.h"

#include "chasm-tr-parser/chtr-document.h"
#include "chasm-tr/chvm/chvm-code-generator.h"


USING_OTNS(Chasm_TR)

void testqvar(QVariant arg1, r8 arg2, u2 arg3)
{
 qDebug() << "arg1 = " << arg1;
 qDebug() << "arg2 = " << arg2;
 qDebug() << "arg3 = " << arg3;
}

void prn(u1 arg)
{
 qDebug() << "arg = " << arg;
}

void prn2(u1 arg1, u1 arg2)
{
 qDebug() << "arg1 = " << arg1;
 qDebug() << "arg2 = " << arg2;
}

u4 add(u4 arg1, u4 arg2)
{
 qDebug() << "arg1 = " << arg1;
 qDebug() << "arg2 = " << arg2;
 qDebug() << "arg1 + arg2 = " << arg1 + arg2;
 return arg1 + arg2;
}

u4 mult(u4 arg1, u4 arg2)
{
 qDebug() << "arg1 = " << arg1;
 qDebug() << "arg2 = " << arg2;
 qDebug() << "arg1 * arg2 = " << arg1 * arg2;
 return arg1 * arg2;
}


void run_tr(QString file_path)
{
 ChTR_Document chrd(file_path);

 chrd.parse();

 QString pre_path = chrd.save_pregraph("..pre");

 chrd.load_pregraph(pre_path);

 QString chvm_path = chrd.save_chvm("..chvm");

 qDebug() << "You can now run " << chvm_path;
}


int main(int argc, char *argv[])
{
 Chasm_Runtime csr;
 Chasm_Runtime_Bridge crb(&csr);
 Chasm_Procedure_Table cpt(&csr);
 crb.set_proctable(&cpt);

 cpt.register_s0(testqvar, @300762);
 cpt.register_s0(prn, @1001);
 cpt.register_s0(prn2, @20044);

 cpt.register_s0(add, @20444);
 cpt.register_s0(mult, @20444);

 cpt.register_procedure_s0("+",
   (_minimal_fn_s0_type) &add, "@20444");

 QString script_path = DEMO_CVM_FOLDER "/t1/t3.cr";

 run_tr(script_path);

 Chasm_VM csvm(&crb);

 csvm.load_program(script_path + ".chvm");
 csvm.run_current_source_proc_name();

 return 0;
}
