
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "phr-graph-root.h"

#include "kernel/document/phr-graph-document.h"

#include "rzns.h"

USING_RZNS(PhrGraphCore)


PHR_Graph_Root::PHR_Graph_Root(PHR_Graph_Document* document)
  : document_(document)
{

}

QString PHR_Graph_Root::document_path()
{
 return document_->local_path();
}
