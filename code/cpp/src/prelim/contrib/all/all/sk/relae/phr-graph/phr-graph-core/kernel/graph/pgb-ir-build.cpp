
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "pgb-ir-build.h"

#include "pgb-ir-macro.h"

#include "multigraph-token.h"

#include "textio.h"
USING_KANS(TextIO)

#include "rzns.h"

USING_RZNS(PhrGraphCore)

_PGB_IR_Build::_PGB_IR_Build(QTextStream& qts, Purpose_Codes& purpose)
  :  qts_(qts), purpose_(purpose)
{

}

MG_Token _PGB_IR_Build::mgtoken(QString rt, MG_Token_Kind_Groups kg, MG_Token_Subgroups sg)
{
 switch(kg)
 {
 case MG_Token_Kind_Groups::Target:
  if(sg == MG_Token_Subgroups::Ledger)
    return {MG_Token_Kinds::Ledger_Target, rt};
  return {MG_Token_Kinds::Known_Target, rt};

 case MG_Token_Kind_Groups::Arg_Target:
  if(sg == MG_Token_Subgroups::Ledger)
    return {MG_Token_Kinds::Arg_Ledger_Target, rt};
  return {MG_Token_Kinds::Arg_Known_Target, rt};

 case MG_Token_Kind_Groups::Arg:
  if(sg == MG_Token_Subgroups::Value)
  {
   if(rt.startsWith('"'))
     return {MG_Token_Kinds::String_Literal, rt};
   return {MG_Token_Kinds::Arg_Raw_Value, rt};
  }
  if(sg == MG_Token_Subgroups::Symbol)
    return {MG_Token_Kinds::Arg_Raw_Symbol, rt};
  return {MG_Token_Kinds::Generic, rt};

 case MG_Token_Kind_Groups::Generic:
  return {MG_Token_Kinds::Generic, rt};

 default: return {MG_Token_Kinds::N_A, rt};
 }
}


MG_Token_Subgroups _PGB_IR_Build::get_subgroup(QChar c)
{
 switch (c.toLatin1())
 {
 case '@': return MG_Token_Subgroups::Symbol;
 case '$': return MG_Token_Subgroups::Value;
 case '!': return MG_Token_Subgroups::Known;
 case '&': return MG_Token_Subgroups::Ledger;
 case ':': return MG_Token_Subgroups::Generic;
 default: return MG_Token_Subgroups::N_A;
 }
}

Purpose_Codes& _PGB_IR_Build::make_file_node(QString file_path)
{
 MG_Token mgt {MG_Token_Kinds::File_Path, file_path};
 qts_ << ";make_file_node "; end_line({mgt});
 return purpose_;
}


Purpose_Codes& _PGB_IR_Build::make_root_node(QString target, MG_Token_Subgroups sg)
{
 MG_Token mgt = mgtoken(target, MG_Token_Kind_Groups::Target, sg);
 qts_ << ";make_root_node "; end_line({mgt});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::enter_anon_signature(QString arg, MG_Token_Subgroups asg,
  QString target, MG_Token_Subgroups tsg)
{
 MG_Token amgt = mgtoken(arg, MG_Token_Kind_Groups::Arg_Target, asg);
 MG_Token tmgt = mgtoken(target, MG_Token_Kind_Groups::Target, tsg);

 qts_ << ";enter_anon_signature "; end_line({amgt, tmgt});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::leave_anon_signature(QString arg, MG_Token_Subgroups asg)
{
 MG_Token amgt = mgtoken(arg, MG_Token_Kind_Groups::Arg_Target, asg);

 qts_ << ";leave_anon_signature "; end_line({amgt});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::make_block_info_node(QString target, MG_Token_Subgroups tsg)
{
 MG_Token tmgt = mgtoken(target, MG_Token_Kind_Groups::Target, tsg);

 qts_ << ";make_block_info_node "; end_line({tmgt});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::add_type_declaration(QString arg, MG_Token_Subgroups asg,
  QString ty, MG_Token_Subgroups tsg)
{
 MG_Token amgt = mgtoken(arg, MG_Token_Kind_Groups::Arg, asg);
 MG_Token tmgt = mgtoken(ty, MG_Token_Kind_Groups::Arg, tsg);

 qts_ << ";add_type_declaration "; end_line({amgt, tmgt});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::add_indexed_type_declaration(QString arg, MG_Token_Subgroups asg,
  QString ty, MG_Token_Subgroups tysg, QString ix, MG_Token_Subgroups ixsg,
  QString source, MG_Token_Subgroups ssg, QString target, MG_Token_Subgroups tsg)
{
 MG_Token amgt = mgtoken(arg, MG_Token_Kind_Groups::Arg, asg);
 MG_Token tymgt = mgtoken(ty, MG_Token_Kind_Groups::Arg, tysg);
 MG_Token imgt = mgtoken(ix, MG_Token_Kind_Groups::Arg, ixsg);
 MG_Token smgt = mgtoken(source, MG_Token_Kind_Groups::Arg_Target, ssg);
 MG_Token tmgt = mgtoken(source, MG_Token_Kind_Groups::Target, ssg);

 qts_ << ";add_indexed_type_declaration "; end_line({imgt, amgt, tymgt, smgt, tmgt});
 return purpose_;
}



Purpose_Codes& _PGB_IR_Build::make_signatuChasmRZ_Node(QString arg, MG_Token_Subgroups asg,
  QString target, MG_Token_Subgroups tsg)
{
 MG_Token amgt = mgtoken(arg, MG_Token_Kind_Groups::Arg_Target, asg);
 MG_Token tmgt = mgtoken(target, MG_Token_Kind_Groups::Target, tsg);

 qts_ << ";make_signatuChasmRZ_Node "; end_line({amgt, tmgt});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::make_token_node(QString arg, MG_Token_Subgroups asg,
  QString target, MG_Token_Subgroups tsg)
{
 MG_Token amgt = mgtoken(arg, MG_Token_Kind_Groups::Arg, asg);
 MG_Token tmgt = mgtoken(target, MG_Token_Kind_Groups::Target, tsg);

 qts_ << ";make_token_node "; end_line({amgt, tmgt});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::make_fsym_ground_node(QString arg1,
  MG_Token_Subgroups asg1, QString arg2,
  MG_Token_Subgroups asg2,
  QString target, MG_Token_Subgroups tsg)
{
 MG_Token mgt1 = mgtoken(arg1, MG_Token_Kind_Groups::Generic, asg1);
 MG_Token mgt2 = mgtoken(arg2, MG_Token_Kind_Groups::Generic, asg2);
 MG_Token tmgt = mgtoken(target, MG_Token_Kind_Groups::Target, tsg);

 qts_ << ";make_fsym_ground_node "; end_line({mgt1, mgt2, tmgt});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::make_statement_info_node(QString anchor_name,
  MG_Token_Subgroups asg, QString channel_name, MG_Token_Subgroups csg,
  QString anchor_kind, MG_Token_Subgroups aksg,
  QString target, MG_Token_Subgroups tsg)
{
 MG_Token amgt = mgtoken(anchor_name, MG_Token_Kind_Groups::Arg, asg);
 MG_Token cmgt = mgtoken(channel_name, MG_Token_Kind_Groups::Generic, csg);
 MG_Token kmgt = mgtoken(anchor_kind, MG_Token_Kind_Groups::Generic, aksg);
 MG_Token tmgt = mgtoken(target, MG_Token_Kind_Groups::Target, tsg);
 qts_ << ";make_statement_info_node  ";
 end_line({amgt, cmgt, kmgt, tmgt});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::add_fsym_ground_node(QString t1, MG_Token_Subgroups sg1,
  QString t2, MG_Token_Subgroups sg2,
  QString chn,
  MG_Token_Subgroups chnsg,
  QString cfen,
  MG_Token_Subgroups cfensg)
{
 MG_Token mgt1 = mgtoken(t1, MG_Token_Kind_Groups::Arg_Target, sg1);
 MG_Token mgt2 = mgtoken(t2, MG_Token_Kind_Groups::Arg_Target, sg2);

 if(cfen.isEmpty())
 {
  qts_ << ";add_fsym_ground_node "; end_line({mgt1, mgt2});
 }
 else
 {
  MG_Token mgt3 = mgtoken(chn, MG_Token_Kind_Groups::Generic, chnsg);
  MG_Token mgt4 = mgtoken(cfen, MG_Token_Kind_Groups::Arg_Target, cfensg);
  qts_ << ";add_fsym_ground_node ";
  end_line({mgt1, mgt2, mgt3, mgt4});
 }
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::add_channel_fground_cross_node(QString t1, MG_Token_Subgroups sg1,
  QString t2, MG_Token_Subgroups sg2,
  QString chn,
  MG_Token_Subgroups chnsg,
  QString cfen,
  MG_Token_Subgroups cfensg)
{
 MG_Token mgt1 = mgtoken(t1, MG_Token_Kind_Groups::Arg_Target, sg1);
 MG_Token mgt2 = mgtoken(t2, MG_Token_Kind_Groups::Arg_Target, sg2);

 if(cfen.isEmpty())
 {
  qts_ << ";add_channel_fground_cross_node "; end_line({mgt1, mgt2});
 }
 else
 {
  MG_Token mgt3 = mgtoken(chn, MG_Token_Kind_Groups::Generic, chnsg);
  MG_Token mgt4 = mgtoken(cfen, MG_Token_Kind_Groups::Arg_Target, cfensg);
  qts_ << ";add_channel_fground_cross_node ";
  end_line({mgt1, mgt2, mgt3, mgt4});
 }
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::add_channel_fground_coentry_node(QString t1, MG_Token_Subgroups sg1,
  QString t2, MG_Token_Subgroups sg2,
  QString chn,
  MG_Token_Subgroups chnsg,
  QString cfen,
  MG_Token_Subgroups cfensg)
{
 MG_Token mgt1 = mgtoken(t1, MG_Token_Kind_Groups::Arg_Target, sg1);
 MG_Token mgt2 = mgtoken(t2, MG_Token_Kind_Groups::Arg_Target, sg2);

 if(cfen.isEmpty())
 {
  qts_ << ";add_channel_fground_coentry_node "; end_line({mgt1, mgt2});
 }
 else
 {
  MG_Token mgt3 = mgtoken(chn, MG_Token_Kind_Groups::Generic, chnsg);
  MG_Token mgt4 = mgtoken(cfen, MG_Token_Kind_Groups::Arg_Target, cfensg);
  qts_ << ";add_channel_fground_coentry_node ";
  end_line({mgt1, mgt2, mgt3, mgt4});
 }
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::add_channel_entry_block_node(QString t1, MG_Token_Subgroups sg1,
  QString chn, QString t2, MG_Token_Subgroups sg2,
  QString t3, MG_Token_Subgroups sg3, QString t4, MG_Token_Subgroups sg4)
{
 MG_Token mgt1 = mgtoken(t1, MG_Token_Kind_Groups::Arg_Target, sg1);
 MG_Token mgtch = {MG_Token_Kinds::Arg_String_Literal, chn};
 MG_Token mgt2 = mgtoken(t2, MG_Token_Kind_Groups::Arg_Target, sg2);
 MG_Token mgt3 = mgtoken(t3, MG_Token_Kind_Groups::Arg_Target, sg3);
 MG_Token mgt4 = mgtoken(t4, MG_Token_Kind_Groups::Target, sg4);

 qts_ << ";add_channel_entry_block_node "; end_line({mgt1,
   mgtch, mgt2, mgt3, mgt4});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::push_block_entry()
{
 qts_ << ";push_block_entry "; end_line({});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::pop_block_entry()
{
 qts_ << ";pop_block_entry "; end_line({});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::push_expression_entry()
{
 qts_ << ";push_expression_entry "; end_line({});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::pop_expression_entry()
{
 qts_ << ";pop_expression_entry "; end_line({});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::add_channel_continue_block_node(QString t1, MG_Token_Subgroups sg1,
  QString t2, MG_Token_Subgroups sg2, QString t3, MG_Token_Subgroups sg3,
  QString t4, MG_Token_Subgroups sg4)
{
 MG_Token mgt1 = mgtoken(t1, MG_Token_Kind_Groups::Arg_Target, sg1);
 MG_Token mgt2 = mgtoken(t2, MG_Token_Kind_Groups::Arg_Target, sg2);
 MG_Token mgt3 = mgtoken(t3, MG_Token_Kind_Groups::Arg_Target, sg3);
 MG_Token mgt4 = mgtoken(t4, MG_Token_Kind_Groups::Target, sg4);

 qts_ << ";add_channel_continue_block_node "; end_line({mgt1, mgt2, mgt3, mgt4});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::add_channel_cross_block_node(QString t1, MG_Token_Subgroups sg1,
  QString t2, MG_Token_Subgroups sg2, QString t3, MG_Token_Subgroups sg3,
  QString t4, MG_Token_Subgroups sg4)
{
 MG_Token mgt1 = mgtoken(t1, MG_Token_Kind_Groups::Arg_Target, sg1);
 MG_Token mgt2 = mgtoken(t2, MG_Token_Kind_Groups::Arg_Target, sg2);
 MG_Token mgt3 = mgtoken(t3, MG_Token_Kind_Groups::Arg_Target, sg3);
 MG_Token mgt4 = mgtoken(t4, MG_Token_Kind_Groups::Target, sg4);

 qts_ << ";add_channel_cross_block_node "; end_line({mgt1, mgt2, mgt3, mgt4});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::add_channel_sequence_block_node(QString t1, MG_Token_Subgroups sg1,
  QString t2, MG_Token_Subgroups sg2, QString t3, MG_Token_Subgroups sg3,
  QString t4, MG_Token_Subgroups sg4)
{
 MG_Token mgt1 = mgtoken(t1, MG_Token_Kind_Groups::Arg_Target, sg1);
 MG_Token mgt2 = mgtoken(t2, MG_Token_Kind_Groups::Arg_Target, sg2);
 MG_Token mgt3 = mgtoken(t3, MG_Token_Kind_Groups::Arg_Target, sg3);
 MG_Token mgt4 = mgtoken(t4, MG_Token_Kind_Groups::Target, sg4);

 qts_ << ";add_channel_sequence_block_node "; end_line({mgt1, mgt2, mgt3, mgt4});
 return purpose_;
}



Purpose_Codes& _PGB_IR_Build::add_block_entry_node(QString t1, MG_Token_Subgroups sg1,
  QString t2, MG_Token_Subgroups sg2, QString t3, MG_Token_Subgroups sg3,
  QString anchor_name, MG_Token_Subgroups ansg)
{
 MG_Token mgt1 = mgtoken(t1, MG_Token_Kind_Groups::Arg_Target, sg1);
 MG_Token mgt2 = mgtoken(t2, MG_Token_Kind_Groups::Arg_Target, sg2);
 MG_Token mgt3 = mgtoken(t3, MG_Token_Kind_Groups::Target, sg3);

 if(anchor_name.isEmpty())
 {
  qts_ << ";add_block_entry_node "; end_line({mgt1, mgt2, mgt3});
 }
 else
 {
  MG_Token mgtan = mgtoken(anchor_name, MG_Token_Kind_Groups::Arg_Target, ansg);
  qts_ << ";add_block_entry_node "; end_line({mgt1, mgt2, mgtan, mgt3});
 }
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::cond_block_entry_or_statement_sequence(QString t1, MG_Token_Subgroups sg1,
  QString t2, MG_Token_Subgroups sg2, QString t3, MG_Token_Subgroups sg3)
{
 MG_Token mgt1 = mgtoken(t1, MG_Token_Kind_Groups::Arg_Target, sg1);
 MG_Token mgt2 = mgtoken(t2, MG_Token_Kind_Groups::Arg_Target, sg2);
 MG_Token mgt3 = mgtoken(t3, MG_Token_Kind_Groups::Arg_Target, sg3);

 qts_ << ";cond_block_entry_or_statement_sequence "; end_line({mgt1, mgt2, mgt3});
}

Purpose_Codes& _PGB_IR_Build::add_statement_sequence_node(QString t1, MG_Token_Subgroups sg1,
  QString t2, MG_Token_Subgroups sg2, QString anchor_name, MG_Token_Subgroups ansg)
{
 MG_Token mgt1 = mgtoken(t1, MG_Token_Kind_Groups::Arg_Target, sg1);
 MG_Token mgt2 = mgtoken(t2, MG_Token_Kind_Groups::Arg_Target, sg2);

 if(anchor_name.isEmpty())
 {
  qts_ << ";add_statement_sequence_node "; end_line({mgt1, mgt2});
 }
 else
 {
  MG_Token mgt3 = mgtoken(anchor_name, MG_Token_Kind_Groups::Arg_Target, ansg);
  qts_ << ";add_statement_sequence_node "; end_line({mgt1, mgt2, mgt3});
 }
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::copy_value(QString t1, MG_Token_Subgroups sg1,
  QString t2, MG_Token_Subgroups sg2)
{
 MG_Token mgt1 = mgtoken(t1, MG_Token_Kind_Groups::Arg_Target, sg1);
 MG_Token mgt2 = mgtoken(t2, MG_Token_Kind_Groups::Target, sg2);

 qts_ << ";copy_value "; end_line({mgt1, mgt2});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::cond_copy(QString t1, MG_Token_Subgroups sg1,
  QString t2, MG_Token_Subgroups sg2)
{
 MG_Token mgt1 = mgtoken(t1, MG_Token_Kind_Groups::Arg_Target, sg1);
 MG_Token mgt2 = mgtoken(t2, MG_Token_Kind_Groups::Target, sg2);

 qts_ << ";cond_copy "; end_line({mgt1, mgt2});
 return purpose_;
}


void _PGB_IR_Build::end_line(QList<MG_Token>&& mgts)
{
 QListIterator<MG_Token> it(mgts);

 while(it.hasNext())
 {
  qts_ << const_cast<const MG_Token&>(it.next()).encode();
  if(it.hasNext())
    qts_ << ' ';
 }
 qts_ << " <-";
}

Purpose_Codes& _PGB_IR_Build::add_channel_token(QString src, MG_Token_Subgroups srcsg,
  QString tok, MG_Token_Subgroups toksg,
  QString target, MG_Token_Subgroups tsg)
{
 MG_Token mgt1 = mgtoken(src, MG_Token_Kind_Groups::Arg_Target, srcsg);
 MG_Token mgt2 = mgtoken(tok, MG_Token_Kind_Groups::Arg, toksg);
 MG_Token mgt3 = mgtoken(target, MG_Token_Kind_Groups::Target, tsg);

 qts_ << ";add_channel_token "; end_line({mgt1, mgt2, mgt3});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::add_channel_continue_token(QString src, MG_Token_Subgroups srcsg,
  QString tok, MG_Token_Subgroups toksg,
  QString target, MG_Token_Subgroups tsg)
{
 MG_Token mgt1 = mgtoken(src, MG_Token_Kind_Groups::Arg_Target, srcsg);
 MG_Token mgt2 = mgtoken(tok, MG_Token_Kind_Groups::Arg, toksg);
 MG_Token mgt3 = mgtoken(target, MG_Token_Kind_Groups::Target, tsg);

 qts_ << ";add_channel_continue_token "; end_line({mgt1, mgt2, mgt3});
 return purpose_;
}


Purpose_Codes& _PGB_IR_Build::add_statement_sequence_token(QString src, MG_Token_Subgroups srcsg,
  QString tok, MG_Token_Subgroups toksg,
  QString sin, MG_Token_Subgroups sinsg,
  QString target, MG_Token_Subgroups tsg)
{
 MG_Token mgt1 = mgtoken(src, MG_Token_Kind_Groups::Arg_Target, srcsg);
 MG_Token mgt2 = mgtoken(tok, MG_Token_Kind_Groups::Arg, toksg);
 MG_Token mgt3 = mgtoken(sin, MG_Token_Kind_Groups::Arg_Target, sinsg);
 MG_Token mgt4 = mgtoken(target, MG_Token_Kind_Groups::Target, tsg);

 qts_ << ";add_statement_sequence_token "; end_line({mgt1, mgt2, mgt3, mgt4});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::add_block_entry_token(QString src, MG_Token_Subgroups srcsg,
  QString tok, MG_Token_Subgroups toksg, QString sin, MG_Token_Subgroups sinsg,
  QString target, MG_Token_Subgroups tsg)
{
 MG_Token mgt1 = mgtoken(src, MG_Token_Kind_Groups::Arg_Target, srcsg);
 MG_Token mgt2 = mgtoken(tok, MG_Token_Kind_Groups::Arg, toksg);
 MG_Token mgt3 = mgtoken(sin, MG_Token_Kind_Groups::Arg_Target, sinsg);
 MG_Token mgt4 = mgtoken(target, MG_Token_Kind_Groups::Target, tsg);

 qts_ << ";add_block_entry_token "; end_line({mgt1, mgt2, mgt3, mgt4});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::add_channel_entry_token(QString src, MG_Token_Subgroups srcsg,
  QString chn, QString tok, MG_Token_Subgroups toksg,
  QString target, MG_Token_Subgroups tsg)
{
 MG_Token mgt1 = mgtoken(src, MG_Token_Kind_Groups::Arg_Target, srcsg);
 MG_Token mgt2 = {MG_Token_Kinds::Arg_String_Literal, chn};
 MG_Token mgt3 = mgtoken(tok, MG_Token_Kind_Groups::Arg, toksg);
 MG_Token mgt4 = mgtoken(target, MG_Token_Kind_Groups::Target, tsg);

 qts_ << ";add_channel_entry_token "; end_line({mgt1, mgt2, mgt3, mgt4});
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::add_channel_xentry_token(QString src, MG_Token_Subgroups srcsg,
  QString chn, QString tok, MG_Token_Subgroups toksg,
  QString target, MG_Token_Subgroups tsg)
{
 MG_Token mgt1 = mgtoken(src, MG_Token_Kind_Groups::Arg_Target, srcsg);
 MG_Token mgt2 = {MG_Token_Kinds::Arg_String_Literal, chn};
 MG_Token mgt3 = mgtoken(tok, MG_Token_Kind_Groups::Arg, toksg);
 MG_Token mgt4 = mgtoken(target, MG_Token_Kind_Groups::Target, tsg);

 qts_ << ";add_channel_xentry_token "; end_line({mgt1, mgt2, mgt3, mgt4});
 return purpose_;
}



Purpose_Codes& _PGB_IR_Build::comment(QString str)
{
 qts_ << ";comment \"" << str << "\")";
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::macro(QStringList _args)
{
 QList<MG_Token> args = {{MG_Token_Kinds::Macro_Name, _args.takeFirst()}};
 std::transform(_args.begin(), _args.end(),
   std::back_inserter(args), [](const QString& str) -> MG_Token
 {
  return {MG_Token_Kinds::Macro_TBD, str};
 });
 return macro(args);
}

Purpose_Codes& _PGB_IR_Build::macro(QList<MG_Token>& args)
{
 qts_ << ";macro "; end_line(std::move(args));
 return purpose_;
}

Purpose_Codes& _PGB_IR_Build::write_with_tokens(QString fn, QList<MG_Token>& mgts)
{
 qts_ << ";" << fn << ' '; end_line(std::move(mgts));
 return purpose_;
}

PGB_IR_Build::PGB_IR_Build(QString primary_source_file_path, QString out_file)
  :  primary_source_file_path_(primary_source_file_path), out_file_(out_file)
{

}

_PGB_IR_Build PGB_IR_Build::operator()(Text_With_Purpose& tp)
{
 qts_.setString(&tp.text);
 _PGB_IR_Build result(qts_, tp.purpose);
 return result;
}

_PGB_IR_Build PGB_IR_Build::operator()(QList<Text_With_Purpose>& tps)
{
 tps.push_back({QString(), Purpose_Codes::N_A});
 Text_With_Purpose& tp = tps.back();
 return operator()(tp);
}

_PGB_IR_Build PGB_IR_Build::operator[](QList<Text_With_Purpose>& tps)
{
 tps.push_front({QString(), Purpose_Codes::N_A});
 Text_With_Purpose& tp = tps.front();
 return operator()(tp);
}

_PGB_IR_Build PGB_IR_Build::insert_after_purpose(QList<Text_With_Purpose>& tps,
   Purpose_Codes purpose)
{
 static Text_With_Purpose dummy {QString(), Purpose_Codes::N_A};
 QList<Text_With_Purpose>::iterator it = std::find_if(tps.begin(),
   tps.end(), [purpose](const Text_With_Purpose& tp)
 {
  return tp.purpose == purpose;
 });
 if(it == tps.end())
 {
  return operator ()(dummy);
 }
 ++it;
 it = tps.insert(it, {QString(), Purpose_Codes::N_A});
 Text_With_Purpose& tp = *it;
 return operator()(tp);
}


_PGB_IR_Build PGB_IR_Build::insert_before_purpose(QList<Text_With_Purpose>& tps,
   Purpose_Codes purpose)
{
 static Text_With_Purpose dummy {QString(), Purpose_Codes::N_A};
 QList<Text_With_Purpose>::iterator it = std::find_if(tps.begin(),
   tps.end(), [purpose](const Text_With_Purpose& tp)
 {
  return tp.purpose == purpose;
 });
 if(it == tps.end())
 {
  return operator ()(dummy);
 }
 it = tps.insert(it, {QString(), Purpose_Codes::N_A});
 Text_With_Purpose& tp = *it;
 return operator()(tp);
}

void PGB_IR_Build::generate_file(QString path, QList<Text_With_Purpose>& tps)
{
 if(tps.isEmpty())
   return;

 int i = 0;
 QListIterator<Text_With_Purpose> it(tps);

 save_file(path, [&i, &it](QString& line)
 {
  line = it.next().text + "\n";
  if(it.hasNext())
    return ++i;
  return 0;
 });
}

// // utility for parse_line...
int count_backslashes(QString s)
{
 if(!s.endsWith('\\'))
   return 0;
 if(s.size() == 1)
   return 1;
 for(int i = 1, j = s.size() - 2; j >= 0; ++i, --j)
 {
  if(s[j] != '\\')
    return i;
 }
}

QString PGB_IR_Build::parse_line(QString line, QMultiMap<MG_Token_Kinds, QPair<MG_Token, int>>& mgtm)
{
 if(line.startsWith('('))
   line = line.mid(1);
 if(line.endsWith(')'))
   line.chop(1);

 QString acc;
 int q_count = 0;

 bool quoted = false;

 QStringList split = line.split('\"');
 QStringList qsl;
 for(QString s: split)
 {
  if(quoted)
  {
   int c = count_backslashes(s);
   if(c % 2) // // odd
   {
    s.chop(1);
    s += '\"';
    acc += s;
    ++q_count;
    continue; // skip flipping quoted ...
   }
   else if(q_count > 0)
   {
    qsl.push_back(QString("\"%1%2\"").arg(acc).arg(s));
    q_count = 0;
    acc.clear();
   }
   else
     qsl.push_back(QString("\"%1\"").arg(s));
  }
  else
  {
   // // If 's' is outside quotes ...
   qsl.append(s.split(' ', QString::SkipEmptyParts)); // // ... get the splitted string
  }
  quoted = !quoted;
 }

 QString fn = qsl.takeFirst();
 int count = 0;
 for(QString qs: qsl)
 {
  MG_Token mgt = MG_Token::decode_symbol(qs);
  mgtm.insert(mgt.kind, {mgt, count++});
 }
 return fn;
}

QList<MG_Token> PGB_IR_Build::mgts_by_kind_group(const QMultiMap<MG_Token_Kinds, QPair<MG_Token, int>>& mgtm,
  MG_Token_Kind_Groups g)
{
 QList<QPair<MG_Token, int>> qlp;
 QList<MG_Token_Kinds> ks = MG_Token_Kind_Group_to_kinds(g);
 for(MG_Token_Kinds k : ks)
   qlp.append(mgtm.values(k));
 qSort(qlp.begin(), qlp.end(), [](const QPair<MG_Token, int>& pr1,
   const QPair<MG_Token, int>& pr2)
 {
  return pr1.second < pr2.second;
 });

 QList<MG_Token> result;
 std::transform(qlp.begin(), qlp.end(),
                std::back_inserter(result),
   std::bind(&QPair<MG_Token, int>::first, std::placeholders::_1));
 return result;
}

void PGB_IR_Build::hold_token(const MG_Token& mgt)
{
 held_tokens_.push_back(mgt);
}

void PGB_IR_Build::hold_token(QString str, MG_Token_Kinds k)
{
 MG_Token mgt{k, str};
 hold_token(mgt);
}

void PGB_IR_Build::clear_held_tokens()
{
 held_tokens_.clear();
}

Purpose_Codes& PGB_IR_Build::write_with_held_tokens(QString fn, QList<Text_With_Purpose>& tps)
{
 (*this)(tps).write_with_tokens(fn, held_tokens_);
}

void PGB_IR_Build::expand_macros(QList<Text_With_Purpose>& tps)
{
 QList<Text_With_Purpose>::iterator it(tps.begin());
 while(it != tps.end())
 {
  const Text_With_Purpose& tp = *it;
  if(tp.text.startsWith(";macro"))
  {
   QMultiMap<MG_Token_Kinds, QPair<MG_Token, int>> mgtm;
   PGB_IR_Build::parse_line(tp.text, mgtm);
   MG_Token mn = mgtm.take(MG_Token_Kinds::Macro_Name).first;
   QList<MG_Token> mgts = mgts_by_kind_group(mgtm, MG_Token_Kind_Groups::Macro);
   PGB_IR_Macro macro(mn.raw_text, mgts);

   QList<Text_With_Purpose> itps;
   macro.write_expand(*this, itps);

   auto itn = it + 1;

   for(const Text_With_Purpose& tp: itps)
   {
    itn = tps.insert(itn, tp);
    ++itn;
   }
   it = itn;
  }
  else
    ++it;
 }
}

