
#include "form-weaver.h"

#include <QFileInfo>

#include <QDir>

#include <QRegularExpression>

#include "textio.h"

USING_KANS(TextIO)

Form_Weaver::Form_Weaver(QString base_folder)
  :  base_folder_(base_folder)
{

}

void Form_Weaver::default_folders()
{
 input_folder_ = base_folder_ + "/src";
 target_folder_ = base_folder_ + "/gen";
 latex_folder_ = base_folder_ + "/..";
}

void Form_Weaver::form_to_latex(QString inpath, QString outpath, QMap<QString, QString> data)
{
 QString text = load_file(inpath);

 QRegularExpression rx("\\\\%-([^%]+)-\\\\%");

 u1 count = 0;

 while(++count)
 {
  QRegularExpressionMatch match = rx.match(text);

  if(match.hasMatch())
  {
   QString cap = match.captured(1);

   QString repl = data.value(cap, "???");

   text.replace(match.capturedStart(), match.capturedEnd() - match.capturedStart(),
     repl);
//   text.replace(match.capturedStart(), match.capturedEnd() - match.capturedStart(),
//     "\\input{fields/gen/%1/%1.gen}"_qt.arg(repl));
  }
  else
    break;
 }

// save_file(outpath, text);

}

void Form_Weaver::gen_latex(QString inpath)
{
 QString path = latex_folder_ + "/" + inpath;

 QFileInfo qfi(path);
// QString bn = qfi.completeBaseName();
// QString sfx = qfi.suffix();

 QString outpath = qfi.absolutePath() + "/" + qfi.completeBaseName() + ".gen." + qfi.suffix();

 qDebug() << outpath;

 QString text = load_file(path);

 QRegularExpression rx("\\\\%-([\\d.]+)-\\\\%");

 u1 count = 0;

 while(++count)
 {
  QRegularExpressionMatch match = rx.match(text);

  if(match.hasMatch())
  {
   QString repl = match.captured(1);
   repl.replace(".", "_");

   text.replace(match.capturedStart(), match.capturedEnd() - match.capturedStart(),
     "\\input{fields/gen/%1/%1.gen}"_qt.arg(repl));
  }
  else
    break;
 }

 save_file(outpath, text);

 // target_folder_ = qfi.absoluteDir();

}

void Form_Weaver::weave_to_latex(QString inpath)
{
 QDir qd(input_folder_);
 qd.cd(inpath);
 QString path = qd.absoluteFilePath(inpath + ".src.txt");

 QString text = load_file(path);

 qDebug() << text;


// QFileInfo qfi(input_folder_);

// target_folder_ = qfi.absoluteDir();

// qd.absoluteFilePath(inpath + ".tex");



}
