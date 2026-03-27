
#ifndef TEXT_EDIT_FRAME__H
#define TEXT_EDIT_FRAME__H

#include <QTextEdit>
#include <QFrame>
#include <QImage>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

#include "global-types.h"


class MRichTextEdit;

class Text_Edit_Frame : public QFrame
{
 Q_OBJECT

 MRichTextEdit* rte_;// = new MRichTextEdit(fr);
 QVBoxLayout* main_layout_;
 QHBoxLayout* bottom_layout_;

 QPushButton* btn_save_;
 QPushButton* btn_back_;
 QPushButton* btn_forward_;

 QLabel* current_subfolder_label_;
 QComboBox* subfolder_options_;

 u2 current_file_counter_;
 u2 max_file_count_;

 QStringList subfolder_names_;

 void reset_subfolder_label();
 void subfolder_ff();
 void subfolder_bk();
 void subfolder_nav(int index);

public:

 Text_Edit_Frame(QWidget* parent = nullptr);

};

#endif // TEXT_EDIT_FRAME__H
