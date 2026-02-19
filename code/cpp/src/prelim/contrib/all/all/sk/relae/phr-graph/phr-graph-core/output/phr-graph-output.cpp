
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "phr-graph-output.h"

#include "kernel/document/phr-graph-document.h"

#include "kernel/graph/phr-graph.h"
#include "kernel/graph/phr-graph-node.h"

#include "kernel/phr-graph-root.h"

#include "token/phr-graph-token.h"

#include "token/phr-graph-token.h"

#include "rzns.h"


USING_RZNS(PhrGraphCore)


PHR_Graph_Output::PHR_Graph_Output(caon_ptr<PHR_Graph_Document> document)
 : document_(document), Cf(PHR_Graph_Frame::instance()),
    qy_(PHR_Graph_Query::instance())
{

}


void PHR_Graph_Output::generate_line(QTextStream& qts, QString ln)
{
 qts << ln << " ;.\n";
}

void PHR_Graph_Output::output(QString path)
{
 //? document_->write_report(path, *this);
}

void PHR_Graph_Output::write_report(QTextStream& qts)
{
 caon_ptr<PHR_Graph_Node> node = document_->graph()->root_node();
 output_from_node(qts, *node);
}


void PHR_Graph_Output::output_from_node(QTextStream& qts,
 const PHR_Graph_Node& node, int indent)
{
}

