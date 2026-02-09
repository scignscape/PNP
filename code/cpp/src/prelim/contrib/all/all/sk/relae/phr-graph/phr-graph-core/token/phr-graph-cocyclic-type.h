
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PHR_GRAPH_COCYCLIC_TYPE__H
#define PHR_GRAPH_COCYCLIC_TYPE__H

#include "accessors.h"
#include "flags.h"


#include <QString>

#include <functional>
#include <QVector>


#include "relae-graph/relae-caon-ptr.h"

#include "rzns.h"


RZNS_(PhrGraphCore)


class PHR_Graph_Type_Declaration;
class PHR_Graph_Node;

class PHR_Graph_Cocyclic_Type
{
 QString type_name_;

 QVector<caon_ptr<PHR_Graph_Node>> precycle_fields_;
 QVector<caon_ptr<PHR_Graph_Node>> cocycle_fields_;

 caon_ptr<PHR_Graph_Node> node_;

public:

 ACCESSORS(QString ,type_name)
 ACCESSORS__RGET(QVector<caon_ptr<PHR_Graph_Node>> ,precycle_fields)
 ACCESSORS__RGET(QVector<caon_ptr<PHR_Graph_Node>> ,cocycle_fields)
 ACCESSORS(caon_ptr<PHR_Graph_Node> ,node)

 PHR_Graph_Cocyclic_Type(QString type_name);

 caon_ptr<PHR_Graph_Type_Declaration> get_precycle_field(quint16 index);
 caon_ptr<PHR_Graph_Type_Declaration> get_cocycle_field(quint16 index);

 void add_precycle_field(quint16 index, caon_ptr<PHR_Graph_Node> n);
 void add_cocycle_field(quint16 index, caon_ptr<PHR_Graph_Node> n);


};

_RZNS(PhrGraphCore)

#endif //PHR_GRAPH_COCYCLIC_TYPE__H
