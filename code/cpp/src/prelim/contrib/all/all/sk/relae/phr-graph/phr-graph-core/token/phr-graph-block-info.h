
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PHR_GRAPH_BLOCK_INFO__H
#define PHR_GRAPH_BLOCK_INFO__H

#include "accessors.h"
#include "flags.h"



#include <QString>

#include <functional>


#include "relae-graph/relae-caon-ptr.h"

#include "rzns.h"


RZNS_(PhrGraphCore)

class PHR_Graph_Signature;

class PHR_Graph_Block_Info
{
 QString fn_name_;
 QString channel_name_;

 PHR_Graph_Signature* signature_;

public:

 ACCESSORS(QString ,fn_name)
 ACCESSORS(QString ,channel_name)

 PHR_Graph_Block_Info(QString fn_name = QString());

 ACCESSORS(PHR_Graph_Signature* ,signature)

};

_RZNS(PhrGraphCore)

#endif //PHR_GRAPH_BLOCK_INFO
