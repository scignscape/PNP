
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PHR_GRAPH_OUTPUT__H
#define PHR_GRAPH_OUTPUT__H

#include "accessors.h"
#include "flags.h"

#include "relae-graph/relae-node-ptr.h"

#include "phr-graph-core/kernel/query/phr-graph-query.h"
#include "phr-graph-core/kernel/frame/phr-graph-frame.h"

#include <QString>
#include <QTextStream>
#include <QMap>

#include <functional>

#include "rzns.h"
RZNS_(PhrGraphCore)


class PHR_Graph_Document;
class PHR_Graph_Node;

class PHR_Graph_Output
{
protected:

 caon_ptr<PHR_Graph_Document> document_;

 const PHR_Graph_Frame& Cf;
 const PHR_Graph_Query& qy_;

 virtual void output_from_node(QTextStream& qts,
  const PHR_Graph_Node& node, int indent = 0);

 void generate_line(QTextStream& qts, QString ln);

public:

 PHR_Graph_Output(caon_ptr<PHR_Graph_Document> document);

 virtual void output(QString path);
 virtual void write_report(QTextStream& qts);

};

_RZNS(PhrGraphCore)

#endif //PHR_GRAPH_OUTPUT__H
