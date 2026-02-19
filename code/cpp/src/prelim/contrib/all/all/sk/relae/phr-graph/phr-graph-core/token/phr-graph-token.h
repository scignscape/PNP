
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PHR_GRAPH_TOKEN__H
#define PHR_GRAPH_TOKEN__H

#include "accessors.h"
#include "flags.h"

#include <QString>

#include <functional>


#include "relae-graph/relae-caon-ptr.h"

#include "rzns.h"


RZNS_(PhrGraphCore)


class PHR_Graph_Token
{
public:
 flags_(1)
  bool is_keyword:1;
  bool is_symbol_declaration:1;
  bool is_quoted:1;
  bool is_back_quoted:1;
  bool is_string_literal:1;
  bool is_numeric_literal:1;
  bool gen_raw_value:1;
 _flags


private:

 QString type_name_;
 QString raw_text_;

public:

 ACCESSORS(QString ,raw_text)
 ACCESSORS(QString ,type_name)

 PHR_Graph_Token(QString raw_text, QString type_name = QString());

};

_RZNS(PhrGraphCore)

#endif //PHR_GRAPH_TOKEN__H
