
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "chasm-channel.h"

USING_OTNS(Chasm_TR)


#include <QDebug>

//#include "compilation-unit-object.h"
//#include "unified-runtime-object.h"
//#include "metatype-object.h"


Chasm_Channel::Chasm_Channel(QString name)
 :  name_(name)
{

}

void Chasm_Channel::coalesce_to_query(Chasm_Carrier& cc)
{
 typedef QVector<QPair<QString, QVector<QVariant>>> w_qvariant;
 typedef QVector<QPair<QString, QVector<Chasm_Carrier>>> w_qsl;

 w_qvariant* qvptr = new w_qvariant;

 w_qsl& qslref = first_carrier().value_as<w_qsl>();

 for(auto pr : qslref)
 {
  qvptr->push_back({pr.first, {}});
  for(Chasm_Carrier& qcc : pr.second)
  {
   QStringList qsl = qcc.value_as<QStringList>();
   qcc.dispose();
   QVariant qv(qsl);
   qvptr->last().second.push_back(qv);
  }
 }

 cc.take_value(&qvptr);
}

void Chasm_Channel::pasn8vector(QVector<n8>& result, u1 size)
{
 result.resize(size);
 for(s1 i = 0; i < size; ++i)
   result[i] = pasn8(i + 1);
}

void Chasm_Channel::add_carriers(std::deque<Chasm_Carrier>& ccs)
{
 for(Chasm_Carrier& cc : ccs)
   carriers_.push_back(cc);
}

Chasm_Carrier& Chasm_Channel::first_carrier_ref()
{
 return carriers_.first();
}

Chasm_Carrier Chasm_Channel::first_carrier()
{
 if(carriers_.isEmpty())
   return {};
 return carriers_.first();
}
