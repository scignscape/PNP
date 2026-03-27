
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include <QDebug>

#include <QFileInfo>

#include "gtagml/kernel/document/gtagml-document.h"
#include "get-cmdl.h"

#include "gtagml/output/gtagml-output-latex.h"
#include "gtagml/output/gtagml-output-blocks.h"
#include "gtagml/output/gtagml-output-sdi-infoset.h"


#include "gtagml/kernel/document/gtagml-folder.h"

#include "gtagml/kernel/document/gtagml-project-info.h"



#include "gh/gh-block-writer.h"

#include "sdi/gh-sdi-document.h"

#include "textio.h"


#include <QRegularExpression>

USING_KANS(GTagML)
USING_KANS(Util)
USING_KANS(SDI)
USING_KANS(TextIO)

#include "qh/qh-web-view-dialog.h"

USING_KANS(RdSC)

#include <QApplication>

#include "MRichTextEditor/mrichtextedit.h"

#include "demo-form-frame.h"

#include "form-weaver.h"

int main(int argc, char *argv[])
{
// Demo_Form_Frame::create_fields_folder(DEFAULT_FIELDS_FOLDER);

// Form_Weaver fw(DEFAULT_FIELDS_FOLDER);
// fw.default_folders();

// fw.gen_latex("ScignTorq.tex");

// fw.weave_to_latex("1_2");

// return 0;

 QApplication qapp(argc, argv);



// QString url = "https://www.google.com/maps/d/u/0/viewer?mid=144OL4Q89OhRc2JInBCT-TMrvVjZ5zG4&ll=39.281772899999986%2C-76.6505279&z=16";
// QH_Web_View_Dialog dlg("https://www.google.com/mymaps");

 QString url = QUrl::fromLocalFile(DEFAULT_HTML_FOLDER "/t1.htm").toString();

   //"https://www.google.com/maps/place/607+Ashburton+St,+Baltimore,+MD+21216/@39.2950837,-76.6612176,16z/data=!3m1!4b1!4m6!3m5!1s0x89c81b5f0923b6af:0x91f4d91b76d85c37!8m2!3d39.2950837!4d-76.6612176!16s%2Fg%2F11cpm1ftjm?entry=ttu&g_ep=EgoyMDI1MDQzMC4xIKXMDSoASAFQAw%3D%3D";


 QH_Web_View_Dialog dlg(url);

 dlg.show();

 return qapp.exec();
}


int main1(int argc, char *argv[])
{

 QString folder;
 QString file;
// QString manfolder;


 file = ROOT_FOLDER "/../conf/paper-conf.gt";
 folder = ROOT_FOLDER "/../conf/gt/src";

 qDebug() << file;

// _main(file, folder, manfolder);

 return 0;
}

