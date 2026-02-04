
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#ifndef CHTR_SOURCE_TOKEN__H
#define CHTR_SOURCE_TOKEN__H


#include "accessors.h"

#include "otns.h"

#include "flags.h"

#include <QFile>
#include <QVector>

//?#include "tsl/ordered_map.h"

#include "global-types.h"

OTNS_(Chasm_TR)


class ChTR_Source_Token
{

private:

 QString text_;

public:


 ChTR_Source_Token(QString text = {});

 ACCESSORS(QString ,text)


};

_OTNS(Chasm_TR)


#endif // CHTR_SOURCE__TOKEN__H




