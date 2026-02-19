
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PGB_IR_BUILD__H
#define PGB_IR_BUILD__H


#include "relae-graph/relae-node-ptr.h"

#include "phr-graph-core/kernel/phr-graph-dominion.h"

#include "accessors.h"

#include "multigraph-token.h"

#include <QTextStream>

#include "rzns.h"

struct MG_Token;

RZNS_(PhrGraphCore)

class PHR_Graph;
class PHR_Graph_Frame;
class PHR_Graph_Query;

enum class Purpose_Codes
{
 N_A, Generic, Make_Token_Node_FSym,
 Copy_To_Last_Statement_Entry_Node
};


class _PGB_IR_Build
{
 QTextStream& qts_;
 Purpose_Codes& purpose_;


public:

 _PGB_IR_Build(QTextStream& qts, Purpose_Codes& purpose);

 ACCESSORS__RGET(QTextStream ,qts)


 Purpose_Codes& make_file_node(QString file_path);

 Purpose_Codes& make_root_node(QString target, MG_Token_Subgroups sg);
 Purpose_Codes& make_root_node(QString target)
 {
  return make_root_node(target.mid(1), get_subgroup(target[0]));
 }

 Purpose_Codes& make_statement_info_node(QString anchor_name,
   MG_Token_Subgroups asg, QString channel_name,
   MG_Token_Subgroups csg, QString anchor_kind,
   MG_Token_Subgroups aksg, QString target, MG_Token_Subgroups tsg);
 Purpose_Codes& make_statement_info_node(QString anchor_name,
   QString channel_name, QString anchor_kind, QString target)
 {
  return make_statement_info_node(anchor_name.mid(1), get_subgroup(anchor_name[0]),
    channel_name.mid(1), get_subgroup(channel_name[0]),
    anchor_kind.mid(1), get_subgroup(anchor_kind[0]),
    target.mid(1), get_subgroup(target[0]));
 }

 Purpose_Codes& comment(QString str);

 Purpose_Codes& macro(QList<MG_Token>& args);
 Purpose_Codes& macro(QStringList _args);


 Purpose_Codes& add_type_declaration(QString arg, MG_Token_Subgroups asg,
   QString ty, MG_Token_Subgroups tsg);
 Purpose_Codes& add_type_declaration(QString arg, QString ty)
 {
  return add_type_declaration(arg.mid(1), get_subgroup(arg[0]),
    ty.mid(1), get_subgroup(ty[0]));
 }

 Purpose_Codes& add_indexed_type_declaration(QString arg, MG_Token_Subgroups asg,
   QString ty, MG_Token_Subgroups tysg, QString ix, MG_Token_Subgroups ixsg,
   QString source, MG_Token_Subgroups ssg, QString target, MG_Token_Subgroups tsg);
 Purpose_Codes& add_indexed_type_declaration(QString arg, QString ty,
   QString ix, QString source, QString target)
 {
  return add_indexed_type_declaration(arg.mid(1), get_subgroup(arg[0]),
    ty.mid(1), get_subgroup(ty[0]), ix.mid(1), get_subgroup(ix[0]),
    source.mid(1), get_subgroup(source[0]),
    target.mid(1), get_subgroup(target[0]));
 }

 Purpose_Codes& make_signatuChasmRZ_Node(QString arg, MG_Token_Subgroups asg,
   QString target, MG_Token_Subgroups tsg);
 Purpose_Codes& make_signatuChasmRZ_Node(QString arg, QString target)
 {
  return make_signatuChasmRZ_Node(arg.mid(1), get_subgroup(arg[0]),
    target.mid(1), get_subgroup(target[0]));
 }

 Purpose_Codes& make_token_node(QString arg, MG_Token_Subgroups asg,
   QString target, MG_Token_Subgroups tsg);
 Purpose_Codes& make_token_node(QString arg, QString target)
 {
  return make_token_node(arg.mid(1), get_subgroup(arg[0]),
    target.mid(1), get_subgroup(target[0]));
 }

 Purpose_Codes& make_block_info_node(QString target, MG_Token_Subgroups tsg);
 Purpose_Codes& make_block_info_node(QString target)
 {
  return make_block_info_node(target.mid(1), get_subgroup(target[0]));
 }

 Purpose_Codes& enter_anon_signature(QString arg, MG_Token_Subgroups asg,
   QString target, MG_Token_Subgroups tsg);
 Purpose_Codes& enter_anon_signature(QString arg, QString target)
 {
  return enter_anon_signature(arg.mid(1), get_subgroup(arg[0]), target.mid(1), get_subgroup(target[0]));
 }

 Purpose_Codes& leave_anon_signature(QString arg, MG_Token_Subgroups asg);
 Purpose_Codes& leave_anon_signature(QString arg)
 {
  return leave_anon_signature(arg.mid(1), get_subgroup(arg[0]));
 }

 Purpose_Codes& push_expression_entry();
 Purpose_Codes& pop_expression_entry();

 Purpose_Codes& push_block_entry();
 Purpose_Codes& pop_block_entry();

 Purpose_Codes& make_fsym_ground_node(QString arg1,
   MG_Token_Subgroups asg1, QString arg2,
   MG_Token_Subgroups asg2,
   QString target, MG_Token_Subgroups tsg);
 Purpose_Codes& make_fsym_ground_node(QString arg1, QString arg2, QString target)
 {
  return make_fsym_ground_node(arg1.mid(1), get_subgroup(arg1[0]),
    arg2.mid(1), get_subgroup(arg2[0]),
    target.mid(1), get_subgroup(target[0]));
 }

 Purpose_Codes& add_fsym_ground_node(QString t1, MG_Token_Subgroups sg1,
   QString t2, MG_Token_Subgroups sg2,
   QString chn = QString(),
   MG_Token_Subgroups chnsg = MG_Token_Subgroups::N_A,
   QString cfen = QString(),
   MG_Token_Subgroups cfensg = MG_Token_Subgroups::N_A);
 Purpose_Codes& add_fsym_ground_node(QString t1, QString t2,
   QString chn, QString cfen)
 {
  return add_fsym_ground_node(t1.mid(1), get_subgroup(t1[0]),
    t2.mid(1), get_subgroup(t2[0]),
    chn.mid(1), get_subgroup(chn[0]),
    cfen.mid(1), get_subgroup(cfen[0]));
 }
 Purpose_Codes& add_fsym_ground_node(QString t1, QString t2)
 {
  return add_fsym_ground_node(t1.mid(1), get_subgroup(t1[0]),
    t2.mid(1), get_subgroup(t2[0]));
 }

 Purpose_Codes& add_channel_fground_cross_node(QString t1, MG_Token_Subgroups sg1,
   QString t2, MG_Token_Subgroups sg2,
   QString chn = QString(),
   MG_Token_Subgroups chnsg = MG_Token_Subgroups::N_A,
   QString cfen = QString(),
   MG_Token_Subgroups cfensg = MG_Token_Subgroups::N_A);
 Purpose_Codes& add_channel_fground_cross_node(QString t1, QString t2,
   QString chn, QString cfen)
 {
  return add_channel_fground_cross_node(t1.mid(1), get_subgroup(t1[0]),
    t2.mid(1), get_subgroup(t2[0]),
    chn.mid(1), get_subgroup(chn[0]),
    cfen.mid(1), get_subgroup(cfen[0]));
 }
 Purpose_Codes& add_channel_fground_cross_node(QString t1, QString t2)
 {
  return add_channel_fground_cross_node(t1.mid(1), get_subgroup(t1[0]),
    t2.mid(1), get_subgroup(t2[0]));
 }

 Purpose_Codes& add_channel_fground_coentry_node(QString t1, MG_Token_Subgroups sg1,
   QString t2, MG_Token_Subgroups sg2,
   QString chn = QString(),
   MG_Token_Subgroups chnsg = MG_Token_Subgroups::N_A,
   QString cfen = QString(),
   MG_Token_Subgroups cfensg = MG_Token_Subgroups::N_A);
 Purpose_Codes& add_channel_fground_coentry_node(QString t1, QString t2,
   QString chn, QString cfen)
 {
  return add_channel_fground_coentry_node(t1.mid(1), get_subgroup(t1[0]),
    t2.mid(1), get_subgroup(t2[0]),
    chn.mid(1), get_subgroup(chn[0]),
    cfen.mid(1), get_subgroup(cfen[0]));
 }
 Purpose_Codes& add_channel_fground_coentry_node(QString t1, QString t2)
 {
  return add_channel_fground_coentry_node(t1.mid(1), get_subgroup(t1[0]),
    t2.mid(1), get_subgroup(t2[0]));
 }


 Purpose_Codes& add_channel_entry_block_node(QString t1, MG_Token_Subgroups sg1,
   QString chn, QString t2, MG_Token_Subgroups sg2,
   QString t3, MG_Token_Subgroups sg3, QString t4, MG_Token_Subgroups sg4);
 Purpose_Codes& add_channel_entry_block_node(QString t1, QString chn,
   QString t2, QString t3, QString t4)
 {
  return add_channel_entry_block_node(t1.mid(1), get_subgroup(t1[0]), chn,
    t2.mid(1), get_subgroup(t2[0]), t3.mid(1), get_subgroup(t3[0]),
    t4.mid(1), get_subgroup(t4[0]) );
 }


 Purpose_Codes& add_channel_sequence_block_node(QString t1, MG_Token_Subgroups sg1,
   QString t2, MG_Token_Subgroups sg2,
   QString t3, MG_Token_Subgroups sg3, QString t4, MG_Token_Subgroups sg4);
 Purpose_Codes& add_channel_sequence_block_node(QString t1, QString t2,
   QString t3, QString t4)
 {
  return add_channel_sequence_block_node(t1.mid(1), get_subgroup(t1[0]),
    t2.mid(1), get_subgroup(t2[0]), t3.mid(1), get_subgroup(t3[0]),
    t4.mid(1), get_subgroup(t4[0]) );
 }


 Purpose_Codes& add_channel_cross_block_node(QString t1, MG_Token_Subgroups sg1,
   QString t2, MG_Token_Subgroups sg2,
   QString t3, MG_Token_Subgroups sg3, QString t4, MG_Token_Subgroups sg4);
 Purpose_Codes& add_channel_cross_block_node(QString t1, QString t2,
   QString t3, QString t4)
 {
  return add_channel_cross_block_node(t1.mid(1), get_subgroup(t1[0]),
    t2.mid(1), get_subgroup(t2[0]), t3.mid(1), get_subgroup(t3[0]),
    t4.mid(1), get_subgroup(t4[0]) );
 }

 Purpose_Codes& add_channel_continue_block_node(QString t1, MG_Token_Subgroups sg1,
   QString t2, MG_Token_Subgroups sg2,
   QString t3, MG_Token_Subgroups sg3, QString t4, MG_Token_Subgroups sg4);
 Purpose_Codes& add_channel_continue_block_node(QString t1, QString t2,
   QString t3, QString t4)
 {
  return add_channel_continue_block_node(t1.mid(1), get_subgroup(t1[0]),
    t2.mid(1), get_subgroup(t2[0]), t3.mid(1), get_subgroup(t3[0]),
    t4.mid(1), get_subgroup(t4[0]) );
 }

 Purpose_Codes& add_block_entry_node(QString t1, MG_Token_Subgroups sg1,
   QString t2, MG_Token_Subgroups sg2, QString t3, MG_Token_Subgroups sg3,
   QString anchor_name = QString(),
   MG_Token_Subgroups ansg = MG_Token_Subgroups::N_A);
 Purpose_Codes& add_block_entry_node(QString t1, QString t2, QString t3,
   QString anchor_name)
 {
  return add_block_entry_node(t1.mid(1), get_subgroup(t1[0]),
    t2.mid(1), get_subgroup(t2[0]), t3.mid(1), get_subgroup(t3[0]),
    anchor_name.mid(1), get_subgroup(anchor_name[0]));
 }
 Purpose_Codes& add_block_entry_node(QString t1, QString t2, QString t3)
 {
  return add_block_entry_node(t1.mid(1), get_subgroup(t1[0]),
    t2.mid(1), get_subgroup(t2[0]), t3.mid(1), get_subgroup(t3[0]));
 }

 Purpose_Codes& add_statement_sequence_node(QString t1, MG_Token_Subgroups sg1,
   QString t2, MG_Token_Subgroups sg2, QString anchor_name = QString(),
   MG_Token_Subgroups ansg = MG_Token_Subgroups::N_A);
 Purpose_Codes& add_statement_sequence_node(QString t1, QString t2, QString anchor_name)
 {
  return add_statement_sequence_node(t1.mid(1), get_subgroup(t1[0]),
    t2.mid(1), get_subgroup(t2[0]),
    anchor_name.mid(1), get_subgroup(anchor_name[0]));
 }
 Purpose_Codes& add_statement_sequence_node(QString t1, QString t2)
 {
  return add_statement_sequence_node(t1.mid(1), get_subgroup(t1[0]),
    t2.mid(1), get_subgroup(t2[0]));
 }

 Purpose_Codes& add_channel_token(QString src, MG_Token_Subgroups srcsg,
   QString tok, MG_Token_Subgroups toksg,
   QString target, MG_Token_Subgroups tsg);
 Purpose_Codes& add_channel_token(QString src,
   QString tok, QString target)
 {
  return add_channel_token(src.mid(1), get_subgroup(src[0]),
    tok.mid(1), get_subgroup(tok[0]),
    target.mid(1), get_subgroup(target[0]));
 }

 Purpose_Codes& add_channel_continue_token(QString src, MG_Token_Subgroups srcsg,
   QString tok, MG_Token_Subgroups toksg,
   QString target, MG_Token_Subgroups tsg);
 Purpose_Codes& add_channel_continue_token(QString src,
   QString tok, QString target)
 {
  return add_channel_continue_token(src.mid(1), get_subgroup(src[0]),
    tok.mid(1), get_subgroup(tok[0]),
    target.mid(1), get_subgroup(target[0]));
 }


 Purpose_Codes& add_channel_entry_token(QString src, MG_Token_Subgroups srcsg,
   QString chn, QString tok, MG_Token_Subgroups toksg,
   QString target, MG_Token_Subgroups tsg);
 Purpose_Codes& add_channel_entry_token(QString src, QString chn,
   QString tok, QString target)
 {
  return add_channel_entry_token(src.mid(1), get_subgroup(src[0]), chn,
    tok.mid(1), get_subgroup(tok[0]),
    target.mid(1), get_subgroup(target[0]));
 }

 Purpose_Codes& add_channel_xentry_token(QString src, MG_Token_Subgroups srcsg,
   QString chn, QString tok, MG_Token_Subgroups toksg,
   QString target, MG_Token_Subgroups tsg);
 Purpose_Codes& add_channel_xentry_token(QString src, QString chn,
   QString tok, QString target)
 {
  return add_channel_xentry_token(src.mid(1), get_subgroup(src[0]), chn,
    tok.mid(1), get_subgroup(tok[0]),
    target.mid(1), get_subgroup(target[0]));
 }

 Purpose_Codes& add_block_entry_token(QString src, MG_Token_Subgroups srcsg,
   QString tok, MG_Token_Subgroups toksg,
   QString sin, MG_Token_Subgroups sinsg,
   QString target, MG_Token_Subgroups tsg);
 Purpose_Codes& add_block_entry_token(QString src,
   QString tok, QString sin, QString target)
 {
  return add_block_entry_token(src.mid(1), get_subgroup(src[0]),
    tok.mid(1), get_subgroup(tok[0]), sin.mid(1), get_subgroup(sin[0]),
    target.mid(1), get_subgroup(target[0]));
 }

 Purpose_Codes& add_statement_sequence_token(QString src, MG_Token_Subgroups srcsg,
   QString tok, MG_Token_Subgroups toksg,
   QString sin, MG_Token_Subgroups sinsg,
   QString target, MG_Token_Subgroups tsg);
 Purpose_Codes& add_statement_sequence_token(QString src,
   QString tok, QString sin, QString target)
 {
  return add_statement_sequence_token(src.mid(1), get_subgroup(src[0]),
    tok.mid(1), get_subgroup(tok[0]), sin.mid(1), get_subgroup(sin[0]),
    target.mid(1), get_subgroup(target[0]));
 }


 Purpose_Codes& copy_value(QString t1, MG_Token_Subgroups sg1,
   QString t2, MG_Token_Subgroups sg2);
 Purpose_Codes& copy_value(QString t1, QString t2)
 {
  return copy_value(t1.mid(1), get_subgroup(t1[0]),
    t2.mid(1), get_subgroup(t2[0]));
 }

 Purpose_Codes& cond_copy(QString t1, MG_Token_Subgroups sg1,
   QString t2, MG_Token_Subgroups sg2);
 Purpose_Codes& cond_copy(QString t1, QString t2)
 {
  return cond_copy(t1.mid(1), get_subgroup(t1[0]),
    t2.mid(1), get_subgroup(t2[0]));
 }

 Purpose_Codes& cond_block_entry_or_statement_sequence(QString t1, MG_Token_Subgroups sg1,
   QString t2, MG_Token_Subgroups sg2, QString t3, MG_Token_Subgroups sg3);
 Purpose_Codes& cond_block_entry_or_statement_sequence(QString t1, QString t2, QString t3)
 {
  return cond_block_entry_or_statement_sequence(t1.mid(1), get_subgroup(t1[0]),
    t2.mid(1), get_subgroup(t2[0]), t3.mid(1), get_subgroup(t3[0]));
 }


 Purpose_Codes& write_with_tokens(QString fn, QList<MG_Token>& mgts);

 void end_line(QList<MG_Token>&& mgts);

 MG_Token_Subgroups get_subgroup(QChar c);


 void generate_file();

 static MG_Token mgtoken(QString rt, MG_Token_Kind_Groups kg, MG_Token_Subgroups sg);
// static MG_Token mgtoken(QString rt, MG_Token_Kinds k);

};

class PGB_IR_Build
{
 QString primary_source_file_path_;

 QString out_file_;
 QTextStream qts_;

 QList<MG_Token> held_tokens_;

public:

 struct Text_With_Purpose
 {
  QString text;
  Purpose_Codes purpose;
 };

 PGB_IR_Build(QString primary_source_file_path, QString out_file);

 ACCESSORS(QString ,primary_source_file_path)
 ACCESSORS(QString ,out_file)

 _PGB_IR_Build operator()(Text_With_Purpose& qs);
 _PGB_IR_Build operator()(QList<Text_With_Purpose>& tps);
 _PGB_IR_Build operator[](QList<Text_With_Purpose>& tps);

 _PGB_IR_Build insert_after_purpose(QList<Text_With_Purpose>& tps,
    Purpose_Codes purpose);

 _PGB_IR_Build insert_before_purpose(QList<Text_With_Purpose>& tps,
    Purpose_Codes purpose);

 void generate_file(QString path, QList<Text_With_Purpose>& tps);

 void generate_file(QList<Text_With_Purpose>& tps)
 {
  generate_file(out_file_, tps);
 }

 Purpose_Codes& write_with_held_tokens(QString fn, QList<Text_With_Purpose>& tps);

 static QList<MG_Token> mgts_by_kind_group(const QMultiMap<MG_Token_Kinds, QPair<MG_Token, int>>& mgtm,
   MG_Token_Kind_Groups g);
 static QString parse_line(QString line, QMultiMap<MG_Token_Kinds, QPair<MG_Token, int>>& mgtm);

 void expand_macros(QList<Text_With_Purpose>& tps);

 void hold_token(const MG_Token& mgt);
 void hold_token(QString str, MG_Token_Kinds k);
 void clear_held_tokens();

 void generate_premacro_file(QList<Text_With_Purpose>& tps)
 {
  generate_file(out_file_ + "-prem", tps);
 }

};

_RZNS(PhrGraphCore)



#endif // PGB_IR_BUILD__H
