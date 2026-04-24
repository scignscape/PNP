
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef GTAGML_PREGRAPH__H
#define GTAGML_PREGRAPH__H


#include <QString>
#include <QTextStream>

#include "global-types.h"


#include "kans.h"

KANS_(GTagML)

class GTagML_Parser;

class GTagML_Pregraph
{
 QString acc_;
 QTextStream acc;

 void write_raw_text(QString text);

 public:

 GTagML_Pregraph();

 void new_text_node(u2 ply_id, QString text);
 void init(GTagML_Parser* parser);


};


_KANS(GTagML)


#endif GTAGML_PREGRAPH__H
