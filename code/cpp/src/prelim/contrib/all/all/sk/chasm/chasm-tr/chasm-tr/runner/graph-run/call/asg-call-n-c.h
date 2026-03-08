
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASG_CALL_N_C__H
#define ASG_CALL_N_C__H

#include "otns.h"

//#include "token/rz-asg-token.h"
//#include "rz-graph-valuer/valuer/rz-asg-valuer.h"

#include "relae-graph/relae-caon-ptr.h"

#include "../../chasm-value-holder.h"

//#include "../../procs/chasm-runner--procs2.h"

#include "chasm-lib/chasm/types/chasm-type-object.h"

#include "chtr-expression-object.h"

#include "writers/chvm-logger-writer.h"

#include "chtr-proc-token.h"

#include "chtr-statement-body.h"
#include "chtr-source-token.h"

//class Chasm_Result_Holder;

//USING_OTNS(Chasm_TR)

//template<>
//struct ASG_Proc_Family_<ASG_Proc_Family::ASG_Graph_Call_VV>
//{
// enum Code { ASG_Graph_Call_VV };

//};

OTNS_(Chasm_TR)

//#include "functions/rz-asg-function-families.h"
//#include "rzns.h"


#define ASG_PROC_DECLARE(str, name, arity, status) name,

ASG_PROC_CODES_(ASG_Graph_Call_NC)
 null = 0,
 #include "asg-proc-n-c.h"

_ASG_PROC_CODES

#undef ASG_PROC_DECLARE


#define ASG_GCALL_IMPLEMENT template<> struct ASG_Proc_Run

ASG_GCALL_IMPLEMENT
<ASG_GCALL_NC(Write_Statement, Internal)>
{

// template<typename T1, typename T2>
// static void run(Chasm_Result_Holder& rh, caon_ptr<ChTR_Node> n,
//   T2& t2)
// {


// }


 template<typename T1, typename T2>
 static void run(Chasm_Result_Holder& rh, caon_ptr<ChTR_Node> n,
  ChTR_Source_Token& cst)
 {
//  ChVM_Logger_Writer& clw = *rh.lwriter();


  if(caon_ptr<ChTR_Proc_Token> pt = n->proc_token())
  {
   CAON_PTR_DEBUG(ChTR_Proc_Token ,pt)
   ChVM_Logger_Writer& clw = *rh.lwriter();

   QString text = pt->text();

   clw.gen()
     .dissolve({"add-new-channel $ proc"})
     << "load-proc-name $ " << text;

   clw.gen().cut().blank();

   clw.gen()
     .dissolve({"add-new-channel $ lambda"});

   clw.write_symbol_token(cst.text());

  }


  //csw.gen().blank();

 }

 template<typename T1, typename T2>
 static void run(Chasm_Result_Holder& rh, caon_ptr<ChTR_Node> n,
  T2& t2)
 {

 }

 template<typename T1, typename T2>
 static void run(Chasm_Result_Holder& rh, caon_ptr<ChTR_Node> n,
  ChTR_Statement_Body& csb)
 {
  CAON_PTR_DEBUG(ChTR_Node ,n)

  if(caon_ptr<ChTR_Proc_Token> pt = n->proc_token())
  {
   CAON_PTR_DEBUG(ChTR_Proc_Token ,pt)
   ChVM_Logger_Writer& clw = *rh.lwriter();


   QString text = pt->text();

   clw.gen()
     .dissolve({"add-new-channel $ proc"})
     << "load-proc-name $ " << text;

   clw.gen().cut().blank();

   //?clw.merge(csb.gen());
  }



  //csw.gen().blank();

 }


 template<typename T1, typename T2>
 static void run(Chasm_Result_Holder& rh, caon_ptr<ChTR_Node> n,
  ChTR_Expression_Object& ceo)
 {
  CAON_PTR_DEBUG(ChTR_Node ,n)

  if(caon_ptr<ChTR_Proc_Token> pt = n->proc_token())
  {
   CAON_PTR_DEBUG(ChTR_Proc_Token ,pt)
   ChVM_Logger_Writer& clw = *rh.lwriter();


   QString text = pt->text();

   clw.gen()
     .dissolve({"add-new-channel $ proc"})
     << "load-proc-name $ " << text;

   clw.gen().cut().blank();

   //?clw.gen().absorb(ceo.gen());
  }



  //csw.gen().blank();

 }


};


//ASG_GCALL_IMPLEMENT
//<ASG_GCALL_VV(Div2, Internal)>
//{
// template<typename T1, typename T2>
// static void run(Chasm_Result_Holder& rh, Chasm_Value_Holder& v1,
//  Chasm_Value_Holder& v2)
// {
//  u1 type_codes = Chasm_Type_Object::merge_pretype_codes(*v1.type_object(), *v2.type_object());
//  div2(rh, type_codes, v1.value(), v2.value());
// }

//};


//RZ_GCALL_IMPLEMENT
//<RZ_GCALL_VV(Init_or_Assign, Internal)>
//{
// static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Value_Holder& v1,
//                 RZ_ASG_Value_Holder& v2)
// {
//  caon_ptr<RZ_ASG_Token> ft = rh.get_lead_function_token();
//  if(ft)
//  {
//   CAON_PTR_DEBUG(RZ_ASG_Token ,ft)

//   caon_ptr<RZ_Type_Object> rto1 = v1.type_object();
//   CAON_PTR_DEBUG(RZ_Type_Object ,rto1)

//   caon_ptr<RZ_Type_Object> rto2 = v2.type_object();
//   CAON_PTR_DEBUG(RZ_Type_Object ,rto2)

//   rh.valuer().set_equal(rh, *ft, v1, v2);
//  }
// }
//};



//ASG_GCALL_IMPLEMENT

//#define ASG_GCALL_VV(name, fam) ASG_Graph_Call_VV, \
// ASG_Proc_Family_<ASG_Graph_Call_VV>::name, ASG_Type_Family::fam


//template<> struct ASG_Proc_Run
//<ASG_GCALL_VV(Add2, Internal)>
//{
// static void run(Chasm_Result_Holder& rh, Chasm_Value_Holder& v1,
//   Chasm_Value_Holder& v2)
// {
// }
//};

//template<>
//struct ASG_Proc_Run<ASG_Graph_Call_VV, ASG_Proc_Family_<ASG_Graph_Call_VV>::Add2, ASG_Type_Family::Internal>
//{
// template<typename T1, typename T2>
// static void run(Chasm_Result_Holder& rh, Chasm_Value_Holder& v1,
//   Chasm_Value_Holder& v2)
// {
//  qDebug() << v1.value();
// }

//};




_OTNS(Chasm_TR)

#endif // ASG_CALL_C_C__H
