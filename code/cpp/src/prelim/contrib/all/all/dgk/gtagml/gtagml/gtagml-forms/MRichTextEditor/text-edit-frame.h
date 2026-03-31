
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

#include "kans.h"

#include "accessors.h"

class MRichTextEdit;

KANS_CLASS_DECLARE(GTagML ,GTagML_Project_Info)
USING_KANS(GTagML)

class Text_Edit_Frame : public QFrame
{
 Q_OBJECT

 enum class View_Modes {
  N_A, GTagML, HTML, Preview_
 };

 View_Modes view_mode_;

 MRichTextEdit* rte_; // = new MRichTextEdit(fr);
 QVBoxLayout* main_layout_;
 QHBoxLayout* bottom_layout_;

 QPushButton* btn_save_;
 QPushButton* btn_full_back_;
 QPushButton* btn_back_;
 QPushButton* btn_forward_;

 QPushButton* btn_gt_mode_;

 QLabel* current_subfolder_label_;
 QComboBox* subfolder_options_;

 QLabel* description_label_;
 QLabel* path_label_;
 QHBoxLayout* description_path_layout_;

 u2 current_file_counter_;
 u2 max_file_count_;

 QStringList subfolder_names_;
 QStringList subfolder_descriptions_;
 QStringList subfolder_paths_;
 QStringList subfolder_briefs_;
 QStringList gen_paths_;

 QString base_folder_;
 QString gen_folder_;
 QString templates_folder_;

 QString document_title_;
 QString document_folder_;

 void reset_subfolder_label();
 void reset_text_view();

 void subfolder_fw();
 void subfolder_bk();
 void subfolder_bb();
 void subfolder_nav(int index);
 void handle_save();

 void process_gtagml_file(QString file, QString template_path, QString folder);
 void process_gtagml_file(QString path, QString template_path, GTagML_Project_Info& gpi);


public:

 Text_Edit_Frame(QString base_folder, QWidget* parent = nullptr);

 ACCESSORS(QString ,document_title)
 ACCESSORS(QString ,document_folder)

 void init_descriptions(QStringList arefs);




};

#endif // TEXT_EDIT_FRAME__H
