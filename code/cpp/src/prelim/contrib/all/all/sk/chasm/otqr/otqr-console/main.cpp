

//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#include "textio.h"

USING_KANS(TextIO)


#include "chasm-tr-parser/chtr-document.h"
#include "chasm-tr/chvm/chvm-code-generator.h"

USING_OTNS(Chasm_TR)


#include "otqr-sdi-parser/otqr-sdi-parser.h"

USING_OTNS(SDI)


int main(int argc, char *argv[])
{

 OTQR_SDI_Parser osp(ROOT_FOLDER "/../dev/chtr/sdi/paper-conf.gt.sentences.sdi");

 osp.parse();

 return 0;
}

int main1(int argc, char *argv[])
{

 ChTR_Document chrd(ROOT_FOLDER "/../dev/chtr/otqr/t1.ot");

 chrd.parse();

 QString pre_path = chrd.save_pregraph("..pre");

 chrd.load_pregraph(pre_path);

 QString chvm_path = chrd.save_chvm("..chvm");

 qDebug() << "You can now run " << chvm_path;

 return 0;
}
