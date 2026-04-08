
#ifndef DEMO_FORM_FRAME__H
#define DEMO_FORM_FRAME__H


#include <QString>
#include <QMainWindow>

#include <QTextEdit>
//?#include <QWebView>

//#include <QWebEngineView>
#include <QLabel>
#include <QPushButton>

#include <QDialog>

#include <QComboBox>

#include "accessors.h"

//#include "flags.h"

class QPushButton;
class QTextEdit;
class QPlainTextEdit;
class QLineEdit;
class QTabWidget;
class QDialogButtonBox;
class QVBoxLayout;
class QHBoxLayout;
class QCheckBox;
class QFormLayout;
class QSplitter;
class QGridLayout;
class QListWidget;
class QTableWidget;
class QScrollArea;
class QGroupBox;
class QTableWidgetItem;


#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFrame>
#include <QStackedWidget>
#include <QComboBox>
#include <QFormLayout>

#include "global-types.h"

class Text_Edit_Frame;


class Demo_Form_Frame : public QFrame
{
 Q_OBJECT

 enum class Field_Keys : u2 {
   N_A, Author, Affiliation, Title, Classification, Acquisition_Plan_Number,
   REV, Program_Title, ACAT,
   Acquisition_Program_Manager, CODE,
   Contact_Name, Contact_Code, Contact_Tel,
   Questions_Cutoff_Date
 };

 QGroupBox* project_group_box_;
 QHBoxLayout* project_layout_;
 QLineEdit* cLE_project_name_;
 QLineEdit* cLE_project_folder_;
 QLineEdit* cLE_project_created_;

 QString project_folder_edited_value_;
 QString project_folder_value_;

 QLabel* cLB_project_name_;
 QLabel* cLB_project_folder_;
 QLabel* cLB_project_created_;
 QPushButton* btn_select_project_folder_;

 QGroupBox* author_title_group_box_;
 QFormLayout* author_title_layout_;

 QHBoxLayout* author_layout_;
 QLineEdit* cLE_author_;
 QLineEdit* cLE_affiliation_;
 QLabel* cLB_author_;
 QLabel* cLB_affiliation_;

 QLineEdit* cLE_title_;

 QLineEdit* cLE_classification_;
 QLineEdit* cLE_acquisition_plan_number_;
 QLineEdit* cLE_rev_;
 QLineEdit* cLE_program_title_;
 QLineEdit* cLE_acat_;
 QLineEdit* cLE_program_manager_;
 QLineEdit* cLE_code_;
 QLineEdit* cLE_questions_name_;
 QLineEdit* cLE_questions_code_;
 QLineEdit* cLE_questions_tel_;
 QLineEdit* cLE_questions_cutoff_date_;

 QLabel* cLBL_classification_;
 QLabel* cLBL_acquisition_plan_number_;
 QLabel* cLBL_rev_;
 QLabel* cLBL_program_title_;
 QLabel* cLBL_act_;
 QLabel* cLBL_program_manager_;
 QLabel* cLBL_code_;
 QLabel* cLBL_questions_name_;
 QLabel* cLBL_questions_code_;
 QLabel* cLBL_questions_tel_;
 QLabel* cLBL_questions_cutoff_date_;

 QGroupBox* questions_group_box_;
 QFormLayout* questions_layout_;

 QGroupBox* top_form_group_box_;
 QFormLayout* top_form_layout_;
 QVBoxLayout* main_form_layout_;

 QPushButton* btn_save_;
 QPushButton* btn_reset_;

 QHBoxLayout* bottom_layout_;
 QVBoxLayout* main_layout_;

 Text_Edit_Frame* text_edit_frame_;

 QString project_form_file_path_;
 QString initial_form_file_path_;
 QString base_projects_folder_;

 QString current_project_name_;

 QString init_project_date_;

 void handle_empty_project_name();

 void save_project_form_data(QMap<QString, QString>& data);
 void save_project_form_data();

 void save_initial_form_data(QMap<QString, QString>& data);
 void save_initial_form_data();

 void load_initial_form_data(QMap<QString, QString>& data);
 void load_project_form_data(QMap<QString, QString>& data);

 void load_form_data(QString file_path, QMap<QString, QString>& data);

 void load_initial_form_data();
 void load_project_form_data();

 void init_project();



public:

 Demo_Form_Frame(Text_Edit_Frame* text_edit_frame,
   QString initial_form_file_path, QString base_projects_folder, QWidget* parent = nullptr);


 static void create_fields_folder(QString path);

 void coalesce_initial_form_data(QMap<QString, QString>& data);
 void coalesce_project_form_data(QMap<QString, QString>& data);

 void handle_select_project_folder();
 void handle_save();

 void reset_form(QMap<QString, QString> data);
 void handle_reset();


};

//} } //_RZNS(CTQ)


#endif

