

//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#include "textio.h"

USING_KANS(TextIO)


#include "chasm-tr-parser/chtr-document.h"
#include "chasm-tr/chvm/chvm-code-generator.h"


#include "chasm-lib/chasm/chasm-runtime.h"

#include "chasm-vm/chasm-vm.h"
#include "chasm-runtime-bridge/chasm-runtime-bridge.h"
#include "chasm-procedure-table/chasm-procedure-table.h"


#include "otqr-sdi-parser/otqr-sdi-parser.h"

USING_OTNS(SDI)
USING_OTNS(Chasm_TR)



void test(void* arg)
{
 QString* t = (QString*) arg;
 qDebug() << "t = " << arg;
 qDebug() << "arg = " << *t;
}


void prn(u1 arg)
{
 qDebug() << "arg = " << arg;
}

Chasm_VM* setup_chvm()
{
 Chasm_Runtime* csr = new Chasm_Runtime;
 Chasm_Runtime_Bridge* crb = new Chasm_Runtime_Bridge(csr);
 Chasm_Procedure_Table* cpt = new Chasm_Procedure_Table(csr);
 crb->set_proctable(cpt);

// cpt.register_s0(testqvar, @300762);
 cpt->register_s0(prn, @1001);

 cpt->register_s0(test, @>1009);

// cpt.register_s0(prn2, @20044);

// cpt.register_procedure_s0("+",
//   (_minimal_fn_s0_type) &add, "@20444");

 Chasm_VM* result = new Chasm_VM(crb);

 return result;
}

void run_chvm(Chasm_VM* vm, QString chvm_path)
{
 vm->load_program(chvm_path);
 vm->run_current_source_proc_name();
}



int main(int argc, char *argv[])
{

 OTQR_SDI_Parser osp(ROOT_FOLDER "/../dev/chtr/sdi/sentences.sdi");

 osp.parse();


 ChTR_Document chrd(ROOT_FOLDER "/../dev/chtr/otqr/t1.ot");

 chrd.parse();

 QString pre_path = chrd.save_pregraph("..pre");

 chrd.load_pregraph(pre_path);

 QString chvm_path = chrd.save_chvm("..chvm");

 Chasm_VM* vm = setup_chvm();

 run_chvm(vm, chvm_path);

 //  qDebug() << "You can now run " << chvm_path;

 return 0;
}
