
#ifndef FORM_WEAVER__H
#define FORM_WEAVER__H


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

//RZNS_(QWN)
//namespace RZ{ namespace CTQ{


class Game_Player;


class Form_Weaver
{
 QString base_folder_;
 QString input_folder_;
 QString latex_folder_;
 QString target_folder_;

public:

 Form_Weaver(QString base_folder);

 void gen_latex(QString inpath);
 void weave_to_latex(QString inpath);
 void default_folders();


};

//} } //_RZNS(CTQ)


#endif // FORM_WEAVER__HPP

