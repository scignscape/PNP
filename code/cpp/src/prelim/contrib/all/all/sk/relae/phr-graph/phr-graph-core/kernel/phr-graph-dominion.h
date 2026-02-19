
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost SoftwaPHR_Graph License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef PHR_GRAPH_DOMINION__H
#define PHR_GRAPH_DOMINION__H

#include "relae-graph/relae-node-ptr.h"
#include "relae-graph/relae-caon-ptr.h"


#include "rzns.h"
RZNS_(PhrGraphCore)


struct PHR_Graph_Galaxy;
class PHR_Graph_Node;
class PHR_Graph_Connectors;
class PHR_Graph_Connection;
class PHR_Graph_Annotated_Connectors;
class PHR_Graph_Frame;
class PHR_Graph_Document;
class PHR_Graph_Root;
class PHR_Graph_Token;
class PHR_Graph;

class PHR_Graph_State;
class PHR_Graph_Source_Element;


struct PHR_Graph_Dominion
{
 typedef PHR_Graph_Galaxy Galaxy_type;
 typedef PHR_Graph_Node Node_type;
 typedef PHR_Graph_Frame Frame_type;
 typedef PHR_Graph_Connectors Connectors_type;
 typedef PHR_Graph_Connection Connection_type;
 typedef PHR_Graph_Annotated_Connectors Annotated_Connectors_type;
 typedef PHR_Graph_Document Document_type;
 typedef PHR_Graph Graph_type;
 typedef PHR_Graph_Root Root_type;

 enum class Type_Codes { N_A,
   #define DOMINION_TYPE DOMINION_TYPE_ENUM
   #include "dominion/types.h"
   #undef DOMINION_TYPE
 };

 template<typename T>
 Type_Codes get_type_code()
 {
  return Type_Codes::N_A;
 }

};



struct PHR_Graph_Galaxy : Node_Ptr_Default_Galaxy<PHR_Graph_Dominion>
{
 //typedef PHR_Graph_Token PHR_Graph_Token_type;
 typedef PHR_Graph_State PHR_Graph_State_type;
 typedef PHR_Graph_Source_Element Source_Element_type;
};

enum class PHR_Graph_Connectors_Case_Labels
{
 #define DOMINION_CONNECTOR(name, label) \
  name,
 #include "dominion/connectors.h"
 #undef DOMINION_CONNECTOR
};

struct PHR_Graph_Connectors : node_connectors<PHR_Graph_Dominion>
{
 PHR_Graph_Connectors(PHR_Graph_Connectors_Case_Labels cl = PHR_Graph_Connectors_Case_Labels::N_A,
   QString label = QString())
  : node_connectors<PHR_Graph_Dominion>(label), case_label(cl),
    priority(0), order(0){}
 PHR_Graph_Connectors_Case_Labels case_label;
 int priority;
 int order;
 bool operator<(const PHR_Graph_Connectors& rhs) const
 {
  return order < rhs.order;
 }
 operator bool() const
 {
  return order > 0;
 }
};

struct PHR_Graph_Annotated_Connectors :
  node_annotated_connectors<PHR_Graph_Dominion>
{
 PHR_Graph_Annotated_Connectors(const PHR_Graph_Connectors& conn,
   caon_ptr<PHR_Graph_Connection> cion)
   : node_annotated_connectors<PHR_Graph_Dominion>{conn, cion} {}

};


_RZNS(PhrGraphCore)

#endif
