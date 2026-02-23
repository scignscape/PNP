

//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#include "textio.h"

USING_KANS(TextIO)


#include "chasm-tr-parser/chtr-document.h"
#include "chasm-tr/chvm/chvm-code-generator.h"

USING_OTNS(Chasm_TR)


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
 run_tr(ROOT_FOLDER "/../dev/chtr/t1/t3.cr");
 return 0;
}
