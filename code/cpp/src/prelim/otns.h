

//          Copyright Nathaniel Christen 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



#ifndef OTNS__H
#define OTNS__H

#ifdef USE_OTNS

#define OTNS_(X) \
 namespace OTQR { namespace X {

#define _OTNS(X) \
 } }


#define OTNS_CLASS_DECLARE(X ,C) \
 namespace OTQR{ namespace X { class C; } }



#define USING_OTNS(x) \
 using namespace OTQR::x;


#else

#define OTNS_(X) \

#define _OTNS(X) \

#define USING_OTNS(x) \

#define OTNS_CLASS_DECLARE(X ,C) \
  class C;

#endif


#endif // OTNS__H
