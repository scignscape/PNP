
#include "text-edit-frame.h"

#include "mrichtextedit.h"

#include "styles.h"

Text_Edit_Frame::Text_Edit_Frame(QWidget *parent)
  :  QFrame(parent), current_file_counter_(1)
{
 rte_ = new MRichTextEdit(this);

 main_layout_ = new QVBoxLayout(this);

 main_layout_->addWidget(rte_);
 bottom_layout_ = new QHBoxLayout;

 btn_save_ = new QPushButton("Save", this);
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
 bottom_layout_->addWidget(btn_back_);
 bottom_layout_->addWidget(btn_forward_);
 bottom_layout_->addStretch();

 make_light_hover_back_button(btn_back_);
 make_light_hover_forward_button(btn_forward_);

// add_style_sheet(btn_back_, "QPushButton:hover {background:pink;} ");

// btn_back_->setStyleSheet(light_back_forward_button_style_sheet_red_());
// btn_forward_->setStyleSheet(light_back_forward_button_style_sheet_red_());

 connect(btn_forward_, &QPushButton::clicked, this, &Text_Edit_Frame::subfolder_ff);

 connect(btn_back_, &QPushButton::clicked, this, &Text_Edit_Frame::subfolder_bk);
//    [this](){subfolder_ff();});

 main_layout_->addLayout(bottom_layout_);

}

void Text_Edit_Frame::subfolder_nav(int index)
{
 current_file_counter_ = index + 1;
 if(current_file_counter_ == 1)
   btn_back_->setEnabled(false);
 else if(current_file_counter_ == max_file_count_)
   btn_forward_->setEnabled(false);
 reset_subfolder_label();
}


void Text_Edit_Frame::reset_subfolder_label()
{
 QString n = subfolder_names_.value(current_file_counter_ - 1);
 current_subfolder_label_->setText(n);
}

void Text_Edit_Frame::subfolder_ff()
{
 ++current_file_counter_;
 reset_subfolder_label();
 btn_back_->setEnabled(true);

 if(current_file_counter_ == max_file_count_)
   btn_forward_->setEnabled(false);


}

void Text_Edit_Frame::subfolder_bk()
{
 --current_file_counter_;
 reset_subfolder_label();

 btn_forward_->setEnabled(true);

 if(current_file_counter_ == 1)
   btn_back_->setEnabled(false);

}
