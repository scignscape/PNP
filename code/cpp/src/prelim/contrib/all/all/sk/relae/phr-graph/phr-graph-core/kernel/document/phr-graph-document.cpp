
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "phr-graph-document.h"

#include "kernel/phr-graph-root.h"

#include "kernel/graph/phr-graph.h"

#include "textio.h"


#include "rzns.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>

#include <QRegularExpression>

USING_RZNS(PhrGraphCore)
USING_KANS(TextIO)


PHR_Graph_Document::PHR_Graph_Document(QString path)
 : graph_(nullptr)
{
 local_path_ = path;
 if(!path.isEmpty())
 {
  QFileInfo qfi(local_path_);
  local_directory_ = qfi.absoluteDir().absolutePath();
 }
}

void PHR_Graph_Document::resolve_report_path(QString& path)
{
 if(path.startsWith('.'))
 {
  path = path.mid(1);
  if(path.startsWith('.'))
  {
   path = local_path_ + path;
  }
  else
  {
  }
 }
}

void PHR_Graph_Document::report_graph(QString path)
{
 resolve_report_path(path);
 QFile file(path);
 if(file.open(QFile::WriteOnly | QIODevice::Text))
 {
  QTextStream qts(&file);
  if(graph_)
    graph_->report(qts);
 }
}



PHR_Graph_Document::~PHR_Graph_Document()
{

}

