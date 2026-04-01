
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
   N_A, Classification, Acquisition_Plan_Number,
   REV, Program_Title, ACAT,
   Acquisition_Program_Manager, CODE,
   Contact_Name, Contact_Code, Contact_Tel,
   Questions_Cutoff_Date
 };

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

 QHBoxLayout* bottom_layout_;
 QVBoxLayout* main_layout_;

 Text_Edit_Frame* text_edit_frame_;

public:

 Demo_Form_Frame(Text_Edit_Frame* text_edit_frame, QWidget* parent = nullptr);

 static void create_fields_folder(QString path);

 void handle_save();

};

//} } //_RZNS(CTQ)


#endif

