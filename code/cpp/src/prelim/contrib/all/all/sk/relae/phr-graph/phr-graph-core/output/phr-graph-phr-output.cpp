
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "phr-graph-phr-output.h"

#include "kernel/document/phr-graph-document.h"

#include "kernel/graph/phr-graph.h"
#include "kernel/graph/phr-graph-node.h"
#include "kernel/graph/phr-graph-connection.h"

#include "kernel/phr-graph-root.h"

#include "token/phr-graph-token.h"

#include "token/phr-graph-token.h"

#include "token/phr-graph-statement-info.h"

#include "textio.h"
USING_KANS(TextIO)

#include "rzns.h"
USING_RZNS(PhrGraphCore)


PHR_Graph_PHR_Output::PHR_Graph_PHR_Output(QString outpath, QString inpath)
 : PHR_Graph_Output(nullptr), output_path_(outpath),
   expression_generator_(), statement_generator_(expression_generator_)
{
 document_ = new PHR_Graph_Document(inpath);
}

#define in_Gf PHR_Graph_Frame::instance(), //"graph"),


void PHR_Graph_PHR_Output::generate_from_node(QTextStream& qts,
 const PHR_Graph_Node& node)
{
 caon_ptr<PHR_Graph_Connection> cion;
 if(caon_ptr<PHR_Graph_Node> ben = qy_.Block_Entry[cion](in_Gf &node))
 {
  PHR_Graph_Statement_Info* sin = nullptr;
  PHR_Graph_Block_Info* bin = nullptr;

  if(cion)
  {
   bin = cion->phr_node()->block_info().raw_pointer();
   if(cion->phr_node(1))
     sin = cion->phr_node(1)->statement_info().raw_pointer();
  }
  statement_generator_.generate_from_node(qts, *ben, sin);
 }
}

void PHR_Graph_PHR_Output::generate(QTextStream& qts)
{
 CAON_PTR_DEBUG(PHR_Graph_Document ,document_)

 caon_ptr<PHR_Graph_Node> node = document_->graph()->root_node();

 generate_line(qts, "init_program_stack");
 generate_line(qts, "enter_lexical_scope");
 generate_line(qts, "reset_program_stack");

 CAON_PTR_DEBUG(PHR_Graph_Node ,node)

 generate_from_node(qts, *node);

}

void PHR_Graph_PHR_Output::generate()
{
 QString text;
 QTextStream qts(&text);
 generate(qts);
 save_file(output_path_, text);
}

