
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef ChTR_CHVM_GENERATOR__H
#define ChTR_CHVM_GENERATOR__H



#include "flags.h"

#include "accessors.h"

#include "global-types.h"

#include "kernel/graph/chtr-graph.h"

#include "types/chtr-type-system.h"

#include <QStack>


#include "otns.h"
OTNS_(Chasm_TR)

class ChTR_Graph;
class ChTR_Node;
class ChTR_Node_Factory;


class ChTR_Document;
class ChTR_Parser;
class ChTR_Graph;
class ChTR_Relae_Frame;
class ChTR_Relae_Query;


class ChTR_Source_Type;
class ChTR_Source_File;

class ChTR_Channel_Package;
class ChTR_Channel_Object;
class ChTR_Code_Statement;

class ChTR_CHVM_Line;

class ChTR_CHVM_Generator;

struct ChTR_CHVM_Generator_Triple
{
 ChTR_CHVM_Generator* gen;
 QString outer_insertion_code;
 QString inner_insertion_code;
};

class ChTR_CHVM_Generator_Triple_Holder
{
protected:
 ChTR_CHVM_Generator_Triple gtrip_;

public:

 ChTR_CHVM_Generator_Triple_Holder(ChTR_CHVM_Generator_Triple gtrip)
   :  gtrip_(gtrip)
 {

 }

 ChTR_CHVM_Generator_Triple_Holder()
   :  gtrip_({nullptr})
 {

 }

 ChTR_CHVM_Generator& gen()
 {
  return *gtrip_.gen;
 }

 QString ins_outer()
 {
  return gtrip_.outer_insertion_code;
 }

 QString ins_inner()
 {
  return gtrip_.inner_insertion_code;
 }
};

class ChTR_CHVM_Generator
{
public:
 typedef QVector<ChTR_CHVM_Line*> Line_Vector;
 typedef QMap<QString, Line_Vector> Line_Vector_Map;
 typedef QMap<QString, Line_Vector_Map> Lines_Map;
 typedef QPair<QString, QString> Insertion_Pair;

private:

 u2 id_;

 static u2 get_next_id()
 {
  static u2 result = 0;
  return ++result;
 }

 QString acc_;

 QTextStream acc_stream_;

 QStringList held_preambles_;

 QString current_subroutine_name_;
 QString current_insertion_code_;

 QStack<QString> insertion_codes_;

 QStringList known_subroutine_names_;

 QString active_insertion_code_;

 QMap<Insertion_Pair, s4> offsets_;

 Lines_Map* acc_lines_;

 u4 cumulative_line_count_;

 void check_register_current_subroutine_name();

public:

 ChTR_CHVM_Generator(Lines_Map* acc_lines = nullptr);

 ACCESSORS(QString ,active_insertion_code)
 ACCESSORS(QString ,current_insertion_code)

 QString get_active_insertion_code()
 {
  if(active_insertion_code_.isEmpty())
    return current_insertion_code_;

  return active_insertion_code_;
 }

 ChTR_CHVM_Generator& resolve_expression();
 ChTR_CHVM_Generator& expression_to_expression();
 ChTR_CHVM_Generator& expression_to_statement();

 ChTR_CHVM_Generator& from_note(QString ins);
 ChTR_CHVM_Generator& clip_note(QString ins);

 ChTR_CHVM_Generator& write_handoff_rtl();

 ChTR_CHVM_Generator& enter_expression(u4 line_number);


 void ins_target(QString ins)
 {
  active_insertion_code_ = ins;
 }

 void ins_untarget()
 {
  active_insertion_code_.clear();
 }
 //ChTR_CHVM_Generator(QString current_subroutine_name, Lines_Map* acc_lines);

 void push_insertion_code(QString code)
 {
  insertion_codes_.push(current_insertion_code_);
  current_insertion_code_ = code;
 }

 QString pop_insertion_code(QString code)
 {
  current_insertion_code_ = code;
  if(code != insertion_codes_.pop())
    qDebug() << "Unexpected insertion code: " << code;
  return current_insertion_code_;
 }

 static QString Default_insertion_code;

 Lines_Map& acc_lines()
 {
  return *acc_lines_;
 }

 ACCESSORS__GET(u2 ,id)

 s4 size()
 {
  return acc_lines()[current_subroutine_name_][current_insertion_code_].size();
 }

 void chvm_code(QString& result);

 ChTR_CHVM_Generator& preamble(QString p)
 {
  held_preambles_.push_back(p);
  return *this;
 }

 ChTR_CHVM_Generator& preamble_comment(QString p)
 {
  return preamble(" .; "_qt + p + " ;.\n");
 }

 void statement_line();
 void statement_line(QString* ln);
 QString statement_line(QString ln);

// ChTR_CHVM_Generator& cut_to_front();

 ChTR_CHVM_Generator& cut(bool sharp = false);

 ChTR_CHVM_Generator& sharp_cut()
 {
  return cut(true);
 }

// ChTR_CHVM_Generator& cut(s4 pos, bool sharp = false);

 ChTR_CHVM_Generator& cut(QString sub, QString ins, s4 pos, bool sharp = false);
 ChTR_CHVM_Generator& cut(QString sub, QString ins, bool sharp = false);

 ChTR_CHVM_Generator& sharp_cut(QString sub, QString ins)
 {
  return cut(sub, ins, true);
 }

 ChTR_CHVM_Generator& blank();

 ChTR_CHVM_Generator& dissolve(QVector<QString> new_lines);

 ChTR_CHVM_Generator& absorb(Line_Vector& new_lines, QString sub, QString ins);
 ChTR_CHVM_Generator& absorb(QString insertion_code, ChTR_CHVM_Generator& new_lines);

 ChTR_CHVM_Generator& absorb(QString insertion_code);

 template<typename ...ARGS>
 ChTR_CHVM_Generator& absorb_from(QString note, ARGS... args)
 {
  return from_note(note).absorb(args...).clip_note(note);
 }

 ChTR_CHVM_Generator& absorb_from(QString note)
 {
  absorb_from(note, note);
 }

 template<typename TEXT_Type>
 friend ChTR_CHVM_Generator& operator << (ChTR_CHVM_Generator& lhs, TEXT_Type rhs)
 {
  lhs.acc_stream_ << rhs;
  return lhs;
 }



};

_OTNS(Chasm_TR)

#endif //  ChTR_CHVM_GENERATOR

