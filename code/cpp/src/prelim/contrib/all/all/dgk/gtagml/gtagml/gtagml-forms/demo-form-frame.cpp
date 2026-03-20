
#include "demo-form-frame.h"

//#include "clg-db-antemodel.h";

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


Demo_Form_Frame::Demo_Form_Frame(QWidget* parent) : QFrame(parent)
{
 cLE_classification_ = new QLineEdit(this);
 cLE_classification_->setPlaceholderText("Defaults to \"FOR OFFICIAL USE ONLY\"");

 cLE_acquisition_plan_number_ = new QLineEdit(this);
 cLE_rev_ = new QLineEdit(this);


 cLE_program_title_ = new QLineEdit(this);
 cLE_act_ = new QLineEdit(this);
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


 main_form_layout_ = new QVBoxLayout;

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
 l2->addWidget(cLE_act_);
 cLE_act_->setMaximumWidth(medium_maximum_width);
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
 main_form_layout_->addWidget(top_form_group_box_);
 main_form_layout_->addWidget(questions_group_box_);

 main_layout_->addItem(main_form_layout_);


 setLayout(main_layout_);

}


