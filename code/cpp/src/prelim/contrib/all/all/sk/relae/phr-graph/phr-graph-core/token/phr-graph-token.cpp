
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "phr-graph-token.h"

#include <QRegularExpression>

#include <QDebug>

#include "rzns.h"

USING_RZNS(PhrGraphCore)

PHR_Graph_Token::PHR_Graph_Token(QString raw_text, QString type_name)
 : Flags(0), raw_text_(raw_text), type_name_(type_name)
{
}


