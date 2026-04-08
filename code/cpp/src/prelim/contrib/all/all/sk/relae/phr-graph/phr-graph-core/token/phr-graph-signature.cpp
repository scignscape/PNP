
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "phr-graph-signature.h"

#include <QRegularExpression>

#include <QDebug>

#include "rzns.h"

USING_RZNS(PhrGraphCore)

PHR_Graph_Signature::PHR_Graph_Signature(caon_ptr<PHR_Graph_Node> fground_node)
 : fground_node_(fground_node)
{
}

void PHR_Graph_Signature::add_tokens(QList<MG_Token>& mgts)
{
 tokens_.append(mgts);
}
