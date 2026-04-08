
#include "demo-form-frame.h"

#include "MRichTextEditor/text-edit-frame.h"

#include "form-weaver.h"

#include <QApplication>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QButtonGroup>

#include <QScrollArea>
#include <QFileDialog>
#include <QTabWidget>
#include <QSplitter>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QDebug>

#include <QDateTime>

#include <QPlainTextEdit>
#include <QTextStream>

#include <QTableWidget>

#include <QMessageBox>

//#include <QNetworkAccessManager>
//#include <QNetworkRequest>
//#include <QNetworkReply>
#include <QUrlQuery>

#include <QHeaderView>
#include <QGraphicsTextItem>
#include <QListWidget>

#include <QDirIterator>

#include "styles.h"


Demo_Form_Frame::Demo_Form_Frame(Text_Edit_Frame* text_edit_frame,
  QString initial_form_file_path, QString base_projects_folder, QWidget* parent)
  :  QFrame(parent), text_edit_frame_(text_edit_frame),
     base_projects_folder_(base_projects_folder),
     initial_form_file_path_(initial_form_file_path)
{
 connect(text_edit_frame, &Text_Edit_Frame::empty_project_name, this,
   &Demo_Form_Frame::handle_empty_project_name);

 btn_select_project_folder_ = new QPushButton("Select", this);

 connect(btn_select_project_folder_, &QPushButton::clicked, this, &Demo_Form_Frame::handle_select_project_folder);

 btn_select_project_folder_->setStyleSheet(basic_button_style_sheet_());


 project_group_box_ = new QGroupBox("Project", this);
 project_layout_ = new QHBoxLayout(project_group_box_);

 cLE_project_name_ = new QLineEdit(this);
 cLE_project_folder_ = new QLineEdit(this);
 cLE_project_created_ = new QLineEdit(this);

// cLE_project_created_->set

 connect(cLE_project_name_, &QLineEdit::textEdited,
   [this](const QString& text)
 {
  current_project_name_ = text;
  text_edit_frame_->update_project_name(text);
  if(project_folder_edited_value_.isEmpty())
  {
   project_folder_value_ = base_projects_folder_ + "/" + text;
   cLE_project_folder_->setText(project_folder_value_);
  }
 });

 cLB_project_name_ = new QLabel("Name", this);
 cLB_project_folder_ = new QLabel("Folder", this);
 cLB_project_created_ = new QLabel("Created", this);
 cLB_project_created_->setMaximumWidth(40);
 cLB_project_created_->setMinimumWidth(40);

 cLE_project_created_->setMaximumWidth(80);
 cLE_project_created_->setMinimumWidth(80);

 project_layout_->addWidget(cLB_project_name_);
 project_layout_->addWidget(cLE_project_name_);
 project_layout_->addWidget(cLB_project_created_);
 project_layout_->addWidget(cLE_project_created_);

 project_layout_->addWidget(cLB_project_folder_);
 project_layout_->addWidget(cLE_project_folder_);
 project_layout_->addWidget(btn_select_project_folder_);


 author_title_group_box_ = new QGroupBox("Document", this);
 author_title_layout_ = new QFormLayout(author_title_group_box_);

 cLE_author_ = new QLineEdit(this);
 cLE_title_ = new QLineEdit(this);
 cLE_affiliation_ = new QLineEdit(this);

 cLB_author_ = new QLabel("Author", this);
 cLB_affiliation_ = new QLabel("Affiliation", this);

 author_layout_ = new QHBoxLayout;
 author_layout_->addWidget(cLB_author_);
 author_layout_->addWidget(cLE_author_);
 author_layout_->addWidget(cLB_affiliation_);
 author_layout_->addWidget(cLE_affiliation_);

 author_title_layout_->addRow(author_layout_);
 author_title_layout_->addRow("Title", cLE_title_);



 cLE_classification_ = new QLineEdit(this);
 cLE_classification_->setPlaceholderText("Defaults to \"FOR OFFICIAL USE ONLY\"");

 cLE_acquisition_plan_number_ = new QLineEdit(this);
 cLE_rev_ = new QLineEdit(this);


 cLE_program_title_ = new QLineEdit(this);
 cLE_acat_ = new QLineEdit(this);
 cLE_program_manager_ = new QLineEdit(this);
 cLE_code_ = new QLineEdit(this);
 cLE_questions_name_ = new QLineEdit(this);
 cLE_questions_code_ = new QLineEdit(this);
 cLE_questions_tel_ = new QLineEdit(this);
 cLE_questions_cutoff_date_ = new QLineEdit(this);

 cLBL_classification_ = new QLabel("Classification", this);
 cLBL_classification_->setMinimumWidth(cLBL_classification_->width() + 3);

 cLBL_acquisition_plan_number_ = new QLabel("ACQUISITION PLAN NUMBER", this);
 cLBL_acquisition_plan_number_->setMinimumWidth(200);


 cLBL_rev_ = new QLabel("REV", this);


 cLBL_program_title_ = new QLabel("PROGRAM TITLE", this);
 cLBL_act_ = new QLabel("ACAT", this);
 cLBL_program_manager_ = new QLabel("ACQUISITION PROGRAM MANAGER", this);

 cLBL_code_ = new QLabel("CODE", this);
 cLBL_questions_name_ = new QLabel("Contact Name", this);
 cLBL_questions_code_ = new QLabel("Contact Code", this);
 cLBL_questions_tel_ = new QLabel("Contact Telephone #", this);
 cLBL_questions_cutoff_date_ = new QLabel("Questions Cutoff Date", this);


// main_form_layout_ = new QVBoxLayout;

 //cLBL_acquisition_plan_number_;
 //cLBL_rev_;
 //cLBL_program_title_;
 //cLBL_act_;
 //cLBL_program_manager_;
 //cLBL_code_;
 //cLBL_questions_name_;
 //cLBL_questions_code_;
 //cLBL_questions_tel_;
 //cLBL_questions_cutoff_date_;

 char short_minimum_width = 35;
 char medium_maximum_width = 55;

 top_form_group_box_ = new QGroupBox("Acquisition Plan", this);
 top_form_layout_ = new QFormLayout(top_form_group_box_);

 top_form_layout_->addRow(cLBL_classification_, cLE_classification_);

 QHBoxLayout* l1 = new QHBoxLayout;
 l1->addWidget(cLBL_acquisition_plan_number_);
 cLBL_acquisition_plan_number_->setMinimumWidth(200);
 l1->addWidget(cLE_acquisition_plan_number_);
 l1->addSpacing(20);
 l1->addWidget(cLBL_rev_);
 cLBL_rev_->setMinimumWidth(short_minimum_width);
 l1->addWidget(cLE_rev_);
 cLE_rev_->setMaximumWidth(medium_maximum_width);
 top_form_layout_->addRow(l1);

 QHBoxLayout* l2 = new QHBoxLayout;
 l2->addWidget(cLBL_program_title_);
 cLBL_program_title_->setMinimumWidth(200);
 l2->addWidget(cLE_program_title_);
 l2->addSpacing(20);
 l2->addWidget(cLBL_act_);
 cLBL_act_->setMinimumWidth(short_minimum_width);
 l2->addWidget(cLE_acat_);
 cLE_acat_->setMaximumWidth(medium_maximum_width);
 top_form_layout_->addRow(l2);

 QHBoxLayout* l3 = new QHBoxLayout;
 l3->addWidget(cLBL_program_manager_);
 cLBL_program_manager_->setMinimumWidth(200);
 l3->addWidget(cLE_program_manager_);
 l3->addSpacing(20);
 l3->addWidget(cLBL_code_);
 cLBL_code_->setMinimumWidth(short_minimum_width);
 l3->addWidget(cLE_code_);
 cLE_code_->setMaximumWidth(medium_maximum_width);
 top_form_layout_->addRow(l3);


 questions_group_box_ = new QGroupBox("Questions", this);
 questions_layout_ = new QFormLayout(questions_group_box_);


 questions_layout_->addRow(cLBL_questions_name_, cLE_questions_name_);
 questions_layout_->addRow(cLBL_questions_code_, cLE_questions_code_);
 questions_layout_->addRow(cLBL_questions_tel_, cLE_questions_tel_);
 questions_layout_->addRow(cLBL_questions_cutoff_date_, cLE_questions_cutoff_date_);

 main_layout_ = new QVBoxLayout;

 main_form_layout_ = new QVBoxLayout;

 main_form_layout_->addWidget(project_group_box_);
 main_form_layout_->addWidget(author_title_group_box_);
 main_form_layout_->addWidget(top_form_group_box_);
 main_form_layout_->addWidget(questions_group_box_);

 main_layout_->addLayout(main_form_layout_);

 btn_save_ = new QPushButton("Save", this);
 connect(btn_save_, &QPushButton::clicked, this, &Demo_Form_Frame::handle_save);

 btn_save_->setStyleSheet(basic_button_style_sheet_());


 btn_reset_ = new QPushButton("Reset", this);
 connect(btn_reset_, &QPushButton::clicked, this, &Demo_Form_Frame::handle_reset);

 btn_reset_->setStyleSheet(basic_button_style_sheet_());

 bottom_layout_ = new QHBoxLayout;

 bottom_layout_->addWidget(btn_save_);
 bottom_layout_->addWidget(btn_reset_);
 bottom_layout_->addStretch();

 main_layout_->addLayout(bottom_layout_);

 setLayout(main_layout_);

 load_initial_form_data();

}

void Demo_Form_Frame::handle_reset()
{
 QMap<QString, QString> data;
 coalesce_project_form_data(data);

 if(data.isEmpty())
 {
  save_project_form_data();
  return;
 }

 QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm",
   "Clear all form data?", QMessageBox::Ok | QMessageBox::Cancel);
 if (reply == QMessageBox::Ok)
 {
  reset_form(data);
 }
}

void Demo_Form_Frame::reset_form(QMap<QString, QString> data)
{
 cLE_author_->setText("");
 cLE_author_->setText("");
 cLE_title_->setText("");

 cLE_acquisition_plan_number_->setText("");
 cLE_rev_->setText("");
 cLE_program_title_->setText("");

 cLE_acat_->setText("");
 cLE_program_manager_->setText("");
 cLE_code_->setText("");

 cLE_questions_name_->setText("");
 cLE_questions_code_->setText("");
 cLE_questions_tel_->setText("");
 cLE_questions_cutoff_date_->setText("");

 cLE_classification_->setText("");

 data.insert("Author", "");
 data.insert("affiliation", "");
 data.insert("Title", "");

 data.insert("Classification", "");
 data.insert("APN", "");
 data.insert("REV", "");
 data.insert("program-title", "");

 data.insert("ACAT", "");
 data.insert("Acquisition_Program_Manager", "");
 data.insert("CODE", "");

 data.insert("questions-name", "");
 data.insert("questions-code", "");
 data.insert("questions-telephone", "");

 data.insert("questions-date", "");

 save_project_form_data(data);

}


void Demo_Form_Frame::coalesce_initial_form_data(QMap<QString, QString>& data)
{
 data.insert("project-name", cLE_project_name_->text());
 data.insert("project-folder", cLE_project_folder_->text());
 data.insert("creation-date", init_project_date_);
}

void Demo_Form_Frame::coalesce_project_form_data(QMap<QString, QString>& data)
{
 data.insert("Author", cLE_author_->text());
 data.insert("affiliation", cLE_affiliation_->text());
 data.insert("Title", cLE_title_->text());

 data.insert("Classification", cLE_classification_->text());
 data.insert("APN", cLE_acquisition_plan_number_->text());
 data.insert("REV", cLE_rev_->text());
 data.insert("program-title", cLE_program_title_->text());

 data.insert("ACAT", cLE_acat_->text());
 data.insert("Acquisition_Program_Manager", cLE_program_manager_->text());
 data.insert("CODE", cLE_code_->text());

 data.insert("questions-name", cLE_questions_name_->text());
 data.insert("questions-code", cLE_questions_code_->text());
 data.insert("questions-telephone", cLE_questions_tel_->text());

 data.insert("questions-date", cLE_questions_cutoff_date_->text());

}

void Demo_Form_Frame::save_initial_form_data(QMap<QString, QString>& data)
{
 QFile outfile(initial_form_file_path_);

 if(outfile.open(QIODevice::WriteOnly))
 {
  QDataStream qds(&outfile);
  qds << data;
  outfile.close();
 }
}

void Demo_Form_Frame::save_initial_form_data()
{
 QMap<QString, QString> data;
 coalesce_initial_form_data(data);
 save_initial_form_data(data);
}

void Demo_Form_Frame::save_project_form_data(QMap<QString, QString>& data)
{
 QFile outfile(project_form_file_path_);

 if(outfile.open(QIODevice::WriteOnly))
 {
  QDataStream qds(&outfile);
  qds << data;
  outfile.close();
 }
}

void Demo_Form_Frame::save_project_form_data()
{
 QMap<QString, QString> data;
 coalesce_project_form_data(data);
 save_project_form_data(data);
}



void Demo_Form_Frame::load_form_data(QString file_path, QMap<QString, QString>& data)
{
 QFile infile(file_path);

 if(infile.open(QIODevice::ReadOnly))
 {
  QDataStream qds(&infile);
  qds >> data;
  infile.close();
 }

// if(!data.isEmpty())
//   load_form_data(data);
}

void Demo_Form_Frame::load_initial_form_data()
{
 QMap<QString, QString> data;
 load_form_data(initial_form_file_path_, data);

 if(!data.isEmpty())
   load_initial_form_data(data);
}

void Demo_Form_Frame::load_initial_form_data(QMap<QString, QString>& data)
{
 project_folder_value_ = data.value("project-folder");
 cLE_project_folder_->setText(project_folder_value_);

 cLE_project_name_->setText(data.value("project-name"));

 init_project_date_ = data.value("creation-date");

 cLE_project_created_->setText(init_project_date_);
 cLE_project_created_->setCursorPosition(0);
}

void Demo_Form_Frame::load_project_form_data()
{
 QMap<QString, QString> data;
 load_form_data(project_form_file_path_, data);
 if(!data.isEmpty())
   load_project_form_data(data);
}


void Demo_Form_Frame::load_project_form_data(QMap<QString, QString>& data)
{
 cLE_author_->setText(data.value("Author"));
 cLE_author_->setText(data.value("affiliation"));
 cLE_title_->setText(data.value("Title"));

 cLE_acquisition_plan_number_->setText(data.value("APN"));
 cLE_rev_->setText(data.value("REV"));
 cLE_program_title_->setText(data.value("program-title"));

 cLE_acat_->setText(data.value("ACAT"));
 cLE_program_manager_->setText(data.value("Acquisition_Program_Manager"));
 cLE_code_->setText(data.value("CODE"));

 cLE_questions_name_->setText(data.value("questions-name"));
 cLE_questions_code_->setText(data.value("questions-code"));
 cLE_questions_tel_->setText(data.value("questions-telephone"));
 cLE_questions_cutoff_date_->setText(data.value("questions-date"));

 cLE_classification_->setText(data.value("Classification"));


}


//void Demo_Form_Frame::load_project_form_data(QMap<QString, QString>& data)
//{

//}

void Demo_Form_Frame::handle_empty_project_name()
{
 QMessageBox::warning(this, "Empty Project Name",
   "You need to set a project name at the top of the form");
}


void copyAndReplaceFolderContents(const QString& fromDir, const QString& toDir, bool copyAndRemove = false)
{
 QDirIterator it(fromDir, QDirIterator::Subdirectories);
 QDir dir(fromDir);
 const int absSourcePathLength = dir.absoluteFilePath(fromDir).length();

 while (it.hasNext())
 {
  it.next();
  const auto fileInfo = it.fileInfo();
  if(!fileInfo.isHidden())
  { //filters dot and dotdot
   const QString subPathStructure = fileInfo.absoluteFilePath().mid(absSourcePathLength);
   const QString constructedAbsolutePath = toDir + subPathStructure;

   if(fileInfo.isDir())
   {
    //Create directory in target folder
    dir.mkpath(constructedAbsolutePath);
   }
   else if(fileInfo.isFile())
   {
    //Copy File to target directory

    //Remove file at target location, if it exists, or QFile::copy will fail
    QFile::remove(constructedAbsolutePath);
    QFile::copy(fileInfo.absoluteFilePath(), constructedAbsolutePath);
   }
  }
 }

 if(copyAndRemove)
  dir.removeRecursively();
}


void Demo_Form_Frame::init_project()
{
 QString path = base_projects_folder_ + "/" + current_project_name_;
 QDir qdir(path);

 if(!qdir.exists("."))
   qdir.mkpath(".");

 QString tsrc = text_edit_frame_->templates_folder();
// QString tdest = text_edit_frame_->current_project_templates_folder();

 copyAndReplaceFolderContents(tsrc, path);

 init_project_date_ = QDateTime::currentDateTime().toString();

// qDebug() << "tsrc = " << tsrc;
// qDebug() << "tdest = " << tdest;


}

//void Demo_Form_Frame::save_initial_form_data()
//{
// QMap<QString, QString> data;
// coalesce_initial_form_data(data);
// save_initial_form_data(data);
//}

void Demo_Form_Frame::handle_save()
{
 if(current_project_name_.isEmpty())
 {
  handle_empty_project_name();
  return;
 }

 if(init_project_date_.isEmpty())
 {
  init_project();
 }


 save_initial_form_data();

 return;

 QString dt = text_edit_frame_->document_title();
 QString df = text_edit_frame_->document_folder();

 QString src = df + "/" + dt + ".tex";
 QString gen = df + "/" + dt + ".gen.tex";

 Form_Weaver fw(df);

 QMap<QString, QString> project_data;

 coalesce_project_form_data(project_data);

 save_project_form_data(project_data);

// N_A, Classification, Acquisition_Plan_Number,
// REV, Program_Title, ACAT,
// Acquisition_Program_Manager, CODE,
// Contact_Name, Contact_Code, Contact_Tel,
// Questions_Cutoff_Date

// QLineEdit* cLE_classification_;
// QLineEdit* cLE_acquisition_plan_number_;
// QLineEdit* cLE_rev_;
// QLineEdit* cLE_program_title_;
// QLineEdit* cLE_acat_;
// QLineEdit* cLE_program_manager_;
// QLineEdit* cLE_code_;
// QLineEdit* cLE_questions_name_;
// QLineEdit* cLE_questions_code_;
// QLineEdit* cLE_questions_tel_;
// QLineEdit* cLE_questions_cutoff_date_;


 fw.form_to_latex(src, gen, project_data);

}


void Demo_Form_Frame::handle_select_project_folder()
{

}

void Demo_Form_Frame::create_fields_folder(QString path)
{
 QDir qd(path);

 static QStringList subfolders {
  "0.1",
  "1.1", "1.2", "1.3", "1.4", "1.5", "1.6",
  "2.1", "2.1.1", "2.1.2", "2.1.3", "2.2", "2.2.1", "2.2.2",
  "3.1", "3.2", "3.3",
  "4.1", "4.2", "4.3", "4.4",
  "5.1", "5.2", "5.3", "5.4", "5.5", "5.6", "5.7",
    "5.8", "5.9", "5.10", "5.11", "5.12", "5.13",
  "6.1", "6.2", "6.3", "6.4",
    "6.4.1", "6.4.2", "6.4.3", "6.4.4", "6.4.5",
    "6.4.6", "6.4.7", "6.4.8", "6.4.9",
  "7.1", "7.2", "7.3", "7.4", "7.5", "7.6",
  "8.1", "8.2", "8.3", "8.4", "8.5", "8.6",
    "8.7", "8.8", "8.9", "8.10", "8.11",
 };

 qd.mkdir("gen");
 qd.mkdir("src");

 for(QString sf : subfolders)
 {
  QString sfo = sf;
  sf.replace(".", "_");
  qd.cd("gen");
  qd.mkdir(sf);
  qd.cd(sf);
  {
   QFile f(qd.absoluteFilePath(sf + ".gen.tex"));
   f.open(QIODevice::WriteOnly);
   f.write(QByteArray("\\placeholderTBD{") + sfo.toLatin1() + "}");
   f.close();
  }
  qd.cdUp();
  qd.cdUp();
  qd.cd("src");
  qd.mkdir(sf);
  qd.cd(sf);
  {
   QFile f(qd.absoluteFilePath(sf + ".src.gt"));
   f.open(QIODevice::WriteOnly);
   f.write(QByteArray("/>>\n\n/// ") + sfo.toLatin1());
   f.write(QByteArray("\n\n\n\n\n\n/// //\n"));
   f.close();
  }
  qd.cdUp();
 }
 
}
