
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PHR_GRAPH_PHR_OUTPUT__H
#define PHR_GRAPH_PHR_OUTPUT__H

#include "accessors.h"
#include "flags.h"

#include "phr-graph-output.h"

#include "generators/statement-generator.h"
#include "generators/expression-generator.h"


#include <QTextStream>
#include <QMap>

#include <functional>

#include "rzns.h"
RZNS_(PhrGraphCore)


class PHR_Graph_Document;
class PHR_Graph_Node;

class PHR_Graph_PHR_Output : public PHR_Graph_Output
{
 QString output_path_;

 void generate_from_node(QTextStream& qts,
  const PHR_Graph_Node& node);

 Expression_Generator expression_generator_;
 Statement_Generator statement_generator_;

public:

 PHR_Graph_PHR_Output(QString outpath, QString inpath = QString());

 ACCESSORS__GET(caon_ptr<PHR_Graph_Document> ,document)

 void generate();

 void generate(QTextStream& qts);

};

_RZNS(PhrGraphCore)

#endif //PHR_GRAPH_PHR_OUTPUT__H
