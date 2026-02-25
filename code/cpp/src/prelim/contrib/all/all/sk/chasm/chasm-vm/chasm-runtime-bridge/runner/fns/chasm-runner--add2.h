
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef CHASM_RUNNER__ADD_2__H
#define CHASM_RUNNER__ADD_2__H

#include <QString>

#include <QStack>
#include <QVariant>

#include <QDebug>

#include "chasm-lib/chasm/chasm-carrier.h"


#include "accessors.h"
#include "flags.h"

#include "global-types.h"
#include "otns.h"

#include "chasm-runtime-bridge/runner/chasm-runner.h"


OTNS_(Chasm_TR)


class Chasm_Runtime;

class Chasm_Runtime_Bridge;


template<>
u2 Chasm_Runner::add2::proceed(u2 lhs, u2 rhs)
{
 return lhs + rhs;
}

_OTNS(Chasm_TR)

#endif // CHASM_RUNNER__H
