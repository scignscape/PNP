
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "chtr-chvm-generator.h"

//#include "


#include <QMap>

#include "global-types.h"

#include "chtr-chvm-line.h"


USING_OTNS(Chasm_TR)


QString ChTR_CHVM_Generator::Default_insertion_code = "@>><<@";

//ChTR_CHVM_Generator::ChTR_CHVM_Generator(QString current_subroutine_name, Lines_Map* acc_lines)
//  :  acc_stream_(&acc_), current_subroutine_name_(current_subroutine_name), acc_lines_(acc_lines)
//{
// id_ = get_next_id();
//}

ChTR_CHVM_Generator::ChTR_CHVM_Generator(Lines_Map* acc_lines)
  :  acc_stream_(&acc_), acc_lines_(acc_lines), cumulative_line_count_(0)
{
 if(!acc_lines_)
   acc_lines_ = new Lines_Map;

 id_ = get_next_id();

 current_insertion_code_ = Default_insertion_code;

 current_subroutine_name_ = "--sf--";
 check_register_current_subroutine_name();
}

void ChTR_CHVM_Generator::chvm_code(QString& result)
{
 for(QString sn : known_subroutine_names_)
 {
  result += "\n\n";
  for(auto ccl : acc_lines()[sn][Default_insertion_code])
  {
   result += ccl->get_text() + "\n";
  }
 }
}

void ChTR_CHVM_Generator::check_register_current_subroutine_name()
{
 auto& lines = acc_lines()[current_subroutine_name_][current_insertion_code_];

 if(lines.isEmpty())
 {
//  ChTR_CHVM_Line* ccl = new ChTR_CHVM_Line(0, "@fn %1 ;."_qt.arg(current_subroutine_name_));
//  lines.push_back(ccl);

  dissolve({"@fn %1"_qt.arg(current_subroutine_name_)});

  known_subroutine_names_.push_back(current_subroutine_name_);
 }
}

ChTR_CHVM_Generator& ChTR_CHVM_Generator::from_note(QString ins)
{
 dissolve({"@from %1"_qt.arg(ins)});
 return *this;
}


ChTR_CHVM_Generator& ChTR_CHVM_Generator::absorb(QString insertion_code)
{
 Line_Vector& new_lines = acc_lines()[current_subroutine_name_][insertion_code];

 absorb(new_lines, current_subroutine_name_, get_active_insertion_code());
}



ChTR_CHVM_Generator& ChTR_CHVM_Generator::absorb(Line_Vector& new_lines, QString sub, QString ins)
{
 auto& lines = acc_lines()[sub][ins];
// u4 base_id = lines.size();

 s4 pos = offsets_.value({sub, ins}, 1);

 u4 base_id = 0;

 for(ChTR_CHVM_Line* line : new_lines)
 {
  ChTR_CHVM_Line* cl = line->clone(++base_id);
  lines.insert(pos - 1, cl);
  ++pos;
 }

 offsets_[{sub, ins}] = pos;


 return *this;
}

ChTR_CHVM_Generator& ChTR_CHVM_Generator::absorb(QString insertion_code, ChTR_CHVM_Generator& new_lines)
{
// auto& lines = acc_lines()[current_subroutine_name_];
// u4 base_id = lines.size();

// s4 insertion_point = 0;

// for(ChTR_CHVM_Line* line : new_lines.acc_lines()[new_lines.current_subroutine_name_])
// {
//  ChTR_CHVM_Line* cl = line->clone(base_id);
//  lines.insert(insertion_point++, cl);
// }
 return *this;
}


ChTR_CHVM_Generator& ChTR_CHVM_Generator::dissolve(QVector<QString> new_lines)
{
 for (QString line : new_lines)
 {
  acc_stream_ << line; cut();
//  acc_stream_ << statement_line(line); cut();
 }
 return *this;
}

ChTR_CHVM_Generator& ChTR_CHVM_Generator::blank()
{
 preamble("\n");
 return *this;
}


//ChTR_CHVM_Generator& ChTR_CHVM_Generator::cut_to_front()
//{
// return cut(0);
//}

ChTR_CHVM_Generator& ChTR_CHVM_Generator::cut(bool sharp)
{
 return cut(current_subroutine_name_, get_active_insertion_code(), sharp);
}


ChTR_CHVM_Generator& ChTR_CHVM_Generator::cut(QString sub, QString ins, bool sharp) //s4 pos, bool sharp)
{
 s4 pos = offsets_.value({sub, ins}, 1);
 cut(sub, ins, pos, sharp);
 offsets_[{sub, ins}] = pos + 1;
 return *this;
}


ChTR_CHVM_Generator& ChTR_CHVM_Generator::cut(QString sub, QString ins, s4 pos, bool sharp) //s4 pos, bool sharp)
{
//?acc << " ;.";

 if(!sharp)
   statement_line();

 auto& lines = acc_lines()[sub][ins];

 ++cumulative_line_count_;

 //u4 ln = pos == -1? acc_lines.size() + 1 : pos;

 ChTR_CHVM_Line* ccl = new ChTR_CHVM_Line(cumulative_line_count_, acc_);

 if(!held_preambles_.isEmpty())
 {
  ccl->pre(held_preambles_);
  held_preambles_.clear();
 }

 acc_.clear();

// if(pos == -1)
//   acc_lines.push_back(ccl);

// else if(pos == 0)
//   acc_lines.push_front(ccl);
// else

 lines.insert(pos - 1, ccl);

 return *this;
}

void ChTR_CHVM_Generator::statement_line()
{
 statement_line(&acc_);
}


void ChTR_CHVM_Generator::statement_line(QString* ln)
{
 *ln += " ;.";
}

QString ChTR_CHVM_Generator::statement_line(QString ln)
{
 return ln + " ;.";
}


