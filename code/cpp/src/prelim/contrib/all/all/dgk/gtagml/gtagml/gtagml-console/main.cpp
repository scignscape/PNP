
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


void process_gtagml_file(QString path, GTagML_Project_Info* gpi, GTagML_Folder* fld)
{
 qDebug() << "Processing file: " << path;

 GTagML_Document* gdoc = new GTagML_Document;

 if(fld)
   gdoc->set_man_folder_path(fld->man_path());

 gdoc->set_project_info(gpi);

 gdoc->load_and_parse(path);

 gdoc->insert_latex_template(path + "-template.tex");
 gdoc->insert_xml_template(path + "-template.xml");

 gdoc->save_jats(path + ".jats.xml", path + ".jats-bib.txt");
 gdoc->save_latex(path + ".tex");
 gdoc->save_sentences(path + ".sentences.sdi");

 gdoc->save_pregraph(path + ".pre.gtvm");

 qDebug() << "path = " << path + ".jats";

 return; // 0;


}

void _main(QString file, QString folder, QString manfolder)
{
 qDebug() << "Folder: " << folder << ", File: " << file;
 qDebug() << "ManFolder: " << manfolder;

 if(manfolder.isEmpty())
 {
  if(file.isEmpty())
  {
   GTagML_Folder fld(folder);
   fld.convert_all_files( &process_gtagml_file );
  }
  else
  {
   GTagML_Project_Info gpi(folder);

   process_gtagml_file(file, &gpi, nullptr);
  }
 }
 else
 {
  GTagML_Folder fld(folder, file, manfolder);
  fld.convert_all_files( &process_gtagml_file );
 }

}


int main(int argc, char *argv[])
{
 QString folder;
 QString file;
 QString manfolder;


 file = ROOT_FOLDER "/../gt/ScignTorq/ScignTorq.gt";
 folder = ROOT_FOLDER "/../gt/ScignTorq/src";

 _main(file, folder, manfolder);

 return 0;
}





#ifdef HIDE
// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"

using namespace clang::tooling;
using namespace llvm;

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");

int main(int argc, const char **argv)
{
  auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
  if (!ExpectedParser) {
    // Fail gracefully for unsupported options.
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  CommonOptionsParser& OptionsParser = ExpectedParser.get();
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());
  return Tool.run(newFrontendActionFactory<clang::SyntaxOnlyAction>().get());
}
#endif
