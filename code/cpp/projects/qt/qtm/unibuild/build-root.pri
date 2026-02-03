
#           Copyright Nathaniel Christen 2026.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)


UNIBUILD_TARGET_DIR = uni

defined(QMAKE_CONSOLE_TARGET_DIR, var){
 CHOICE_CODE = qmake-console-choices
 WHICH_BUILD_DIR_CODE = qmake-console
 exists(../_choices/$${WHICH_BUILD_DIR_CODE}_choices.pri): include(../_choices/$${WHICH_BUILD_DIR_CODE}_choices.pri)
 BUILD_DIR_CODE = qmake-console
} else {
 isEmpty(UNI_BUILD_CODE) {
  UNI_BUILD_CODE = nomark
 }
 CHOICE_CODE = unibuild-choices
 WHICH_BUILD_DIR_CODE = uni-$$UNI_BUILD_CODE
 exists(../_choices/$${WHICH_BUILD_DIR_CODE}_choices.pri): include(../_choices/$${WHICH_BUILD_DIR_CODE}_choices.pri)
 BUILD_DIR_CODE = _subdirs_
}

include(../build-root-both.pri)


