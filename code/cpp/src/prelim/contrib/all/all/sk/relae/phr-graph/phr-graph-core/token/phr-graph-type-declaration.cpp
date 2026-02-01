
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "phr-graph-type-declaration.h"

#include <QRegularExpression>

#include <QDebug>

#include "rzns.h"

USING_RZNS(PhrGraphCore)

PHR_Graph_Type_Declaration::PHR_Graph_Type_Declaration(QString symbol_name,
  QString type_name)
 :  symbol_name_(symbol_name), type_name_(type_name)
{
}

PHR_Graph_Type_Declaration::PHR_Graph_Type_Declaration()
{
}
