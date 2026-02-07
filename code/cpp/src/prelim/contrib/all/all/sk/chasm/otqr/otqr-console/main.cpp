

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

#include "tsl/ordered_map.h"


USING_OTNS(SDI)
USING_OTNS(Chasm_TR)


static SDI_Sentence_Reader* sdi;


void find(void* arg)
{
 typedef QVector<QPair<QString, QVector<QVariant>>> w_qvariant;

 w_qvariant* ptr = (w_qvariant*) arg;

 if(ptr->first().first.isEmpty())
 {
  ptr->first().first = ":text";
 }

 tsl::ordered_map<QString, QVector<QVariantList>> vmap;
 tsl::ordered_map<QString, QVector<QStringList>> smap;

 auto vmap_to_smap = [&smap](const QPair<QString, QVector<QVariant>>& pr)
 {
  smap[pr.first].resize(pr.second.size());

  std::transform(pr.second.begin(), pr.second.end(), smap[pr.first].begin(), [](QVariant qv)
  {
   return qv.toStringList();
  });
 };

 for(auto pr : *ptr)
 {
  vmap_to_smap(pr);
 }

 sdi->handle_find(smap, vmap);


// vmap_to_smap(pr);


 // = pr.second.to;

// for(auto pr : *ptr)
// {
//  QString key = pr.first;
//  qDebug() << "key = " << key;
// }
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

 cpt->register_s0(find, @>1009);

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

 sdi = osp.reader();

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
