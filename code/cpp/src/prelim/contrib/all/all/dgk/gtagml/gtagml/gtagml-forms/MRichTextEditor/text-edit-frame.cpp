
#include "text-edit-frame.h"

#include "mrichtextedit.h"

#include "styles.h"

#include "textio.h"

USING_KANS(TextIO)


Text_Edit_Frame::Text_Edit_Frame(QString base_folder, QWidget* parent)
  :  QFrame(parent), view_mode_(View_Modes::GTagML),
     base_folder_(base_folder), current_file_counter_(1)
{
 rte_ = new MRichTextEdit(this);

 rte_->activate_gtagml();

 main_layout_ = new QVBoxLayout(this);

 description_label_ = new QLabel(this);
 path_label_ = new QLabel(this);

// path_label_->setMaximumWidth(200);

 description_path_layout_ = new QHBoxLayout;

 description_path_layout_->addWidget(path_label_);
 description_path_layout_->addWidget(description_label_);

 main_layout_->addLayout(description_path_layout_);


 main_layout_->addWidget(rte_);
 bottom_layout_ = new QHBoxLayout;

 btn_save_ = new QPushButton("Save", this);
 btn_full_back_ = new QPushButton("<<-", this);
 btn_full_back_->setEnabled(false);
 btn_back_ = new QPushButton("<-", this);
 btn_back_->setEnabled(false);
 btn_forward_ = new QPushButton("->", this);

 subfolder_names_ = QStringList {
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

 for(QString sf : subfolder_names_)
 {
  QString sfo = sf;
  sf.replace(".", "_");
  subfolder_paths_.push_back(base_folder_ + "/" + sf + "/" + sf + ".src.gt");
  subfolder_briefs_.push_back("# " + sfo);
  subfolder_descriptions_.push_back("(contents of file %1)"_qt.arg(sf + ".src.gt"));
 }



 max_file_count_ = subfolder_names_.size();

 current_subfolder_label_ = new QLabel("", this);

 reset_subfolder_label();

 bottom_layout_->addWidget(btn_save_);
 bottom_layout_->addStretch();
 bottom_layout_->addWidget(current_subfolder_label_);

 subfolder_options_ = new QComboBox(this);
 subfolder_options_->addItems(subfolder_names_);
 bottom_layout_->addWidget(subfolder_options_);

 connect(subfolder_options_, QOverload<int>::of(&QComboBox::activated),
   this, &Text_Edit_Frame::subfolder_nav);

 bottom_layout_->addStretch();
 bottom_layout_->addWidget(btn_full_back_);
 bottom_layout_->addWidget(btn_back_);
 bottom_layout_->addWidget(btn_forward_);
 bottom_layout_->addStretch();

 make_light_hover_full_back_button(btn_full_back_);
 make_light_hover_back_button(btn_back_);
 make_light_hover_forward_button(btn_forward_);

// add_style_sheet(btn_back_, "QPushButton:hover {background:pink;} ");

// btn_back_->setStyleSheet(light_back_forward_button_style_sheet_red_());
// btn_forward_->setStyleSheet(light_back_forward_button_style_sheet_red_());

 connect(btn_forward_, &QPushButton::clicked, this, &Text_Edit_Frame::subfolder_fw);
 connect(btn_back_, &QPushButton::clicked, this, &Text_Edit_Frame::subfolder_bk);
 connect(btn_full_back_, &QPushButton::clicked, this, &Text_Edit_Frame::subfolder_bb);

 btn_save_->setStyleSheet(basic_button_style_sheet_());

 btn_gt_mode_ = new QPushButton("Gt", this);
 btn_gt_mode_->setMinimumWidth(49);
 btn_gt_mode_->setStyleSheet(colorful_toggle_button_style_sheet_());
 btn_gt_mode_->setCheckable(true);
 btn_gt_mode_->setChecked(true);

 bottom_layout_->addStretch();
 bottom_layout_->addWidget(btn_gt_mode_);

 main_layout_->addLayout(bottom_layout_);

}


void Text_Edit_Frame::init_descriptions(QStringList arefs)
{
 //for(u2 i = 0; i <)
 //subfolder_descriptions_.insert();

 std::copy(arefs.begin(), arefs.end(), subfolder_descriptions_.begin());
 reset_subfolder_label();
 reset_text_view();
}

void Text_Edit_Frame::reset_text_view()
{
 QString path = subfolder_paths_.value(current_file_counter_ - 1);
 QString text = load_file(path);

 rte_->setText(text);
}

void Text_Edit_Frame::subfolder_nav(int index)
{
 current_file_counter_ = index + 1;
 if(current_file_counter_ == 1)
 {
  btn_full_back_->setEnabled(false);
  btn_back_->setEnabled(false);
 }
 else
 {
  btn_full_back_->setEnabled(true);
  btn_back_->setEnabled(true);
 }
 btn_forward_->setEnabled(current_file_counter_ == max_file_count_);
 reset_subfolder_label();
 reset_text_view();
}


void Text_Edit_Frame::reset_subfolder_label()
{
 QString n = subfolder_names_.value(current_file_counter_ - 1);
 current_subfolder_label_->setText(n);

 QString d = subfolder_descriptions_.value(current_file_counter_ - 1);
 description_label_->setText(d);

 QString p1 = subfolder_paths_.value(current_file_counter_ - 1);
 QString p2 = subfolder_briefs_.value(current_file_counter_ - 1);

 path_label_->setText("(%1) %2"_qt.arg(p1).arg(p2));
}

void Text_Edit_Frame::subfolder_fw()
{
 ++current_file_counter_;
 reset_subfolder_label();
 reset_text_view();

 btn_back_->setEnabled(true);
 btn_full_back_->setEnabled(true);

 if(current_file_counter_ == max_file_count_)
   btn_forward_->setEnabled(false);
}

void Text_Edit_Frame::subfolder_bk()
{
 --current_file_counter_;
 reset_subfolder_label();
 reset_text_view();

 btn_forward_->setEnabled(true);

 if(current_file_counter_ == 1)
 {
  btn_back_->setEnabled(false);
  btn_full_back_->setEnabled(false);
 }
}

void Text_Edit_Frame::subfolder_bb()
{
 current_file_counter_ = 1;
 reset_subfolder_label();
 reset_text_view();

 btn_forward_->setEnabled(true);

 btn_back_->setEnabled(false);
 btn_full_back_->setEnabled(false);
}

