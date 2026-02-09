
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "phr-graph-fground-entry.h"

#include <QRegularExpression>

#include <QDebug>

#include "rzns.h"

USING_RZNS(PhrGraphCore)

PHR_Graph_FGround_Entry::PHR_Graph_FGround_Entry(QString channel_name,
  QString result_type_name)
 :  channel_name_(channel_name), result_type_name_(result_type_name), depth_(0)
{
}


