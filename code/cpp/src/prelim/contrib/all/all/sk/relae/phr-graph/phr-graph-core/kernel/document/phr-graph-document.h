
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PHR_GRAPH_DOCUMENT__H
#define PHR_GRAPH_DOCUMENT__H

#include "relae-graph/relae-node-ptr.h"

#include "phr-graph-core/kernel/phr-graph-dominion.h"

#include "accessors.h"

#include "rzns.h"

#include <QFile>
#include <QTextStream>


RZNS_(PhrGraphCore)

class PHR_Graph;

class PHR_Graph_Document
{
 caon_ptr<PHR_Graph> graph_;

 QString local_path_;
 QString local_directory_;

 void resolve_report_path(QString& path);

public:


 ACCESSORS(QString ,local_path)
 ACCESSORS(QString ,local_directory)
 ACCESSORS(caon_ptr<PHR_Graph> ,graph)

 PHR_Graph_Document(QString path);

 ~PHR_Graph_Document();

 void report_graph(QString path);

 void write_report(QString path);

 template<typename T>
 void write_report(QString path, T& report)
 {
  resolve_report_path(path);
  QFile file(path);
  if(file.open(QFile::WriteOnly | QIODevice::Text))
  {
   QTextStream qts(&file);
   report.write_report(qts);
  }
 }



};

_RZNS(PHR_GraphCore)


#endif //RZ_PHR_Graph_DOCUMENT__H
