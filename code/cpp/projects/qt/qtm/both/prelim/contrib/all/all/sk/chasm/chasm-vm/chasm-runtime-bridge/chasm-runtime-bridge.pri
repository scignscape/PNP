
#           Copyright Nathaniel Christen 2026.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

QT -= gui


include(../build-group.pri)


exists($$ROOT_DIR/../preferred/sysr.pri): include($$ROOT_DIR/../preferred/sysr.pri)


INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR $$SRC_ROOT_DIR

# INCLUDEPATH += $$CHASM_LIB_SRC_GROUP_DIR
INCLUDEPATH += $$SRC_PROSET_DIR/chasm-lib


#INCLUDEPATH += $$NTXH_SRC_GROUP_DIR
#INCLUDEPATH += $$NTXH_SRC_GROUP_DIR/ntxh

INCLUDEPATH += $$PHAON_GRAPH_SRC_GROUP_DIR
INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR


#INCLUDEPATH += $$HTXN_SRC_GROUP_DIR
#INCLUDEPATH += $$SRC_GROUP_DIR/gtagml


 # Most projects don't use ROOT_DIR as a basis
 # for includes but this one loads some generated
 # source code to experiment with ...
INCLUDEPATH += $$ROOT_DIR


DEFINES += ROOT_FOLDER=\\\"$$ROOT_DIR\\\"


DEFINES += CAON_DEBUG
DEFINES += RELAE_LABEL_NODES

CONFIG += no_keywords


DEFINES += USE_OTNS


HEADERS += \
  $$SRC_DIR/chasm-runtime-bridge.h \
  $$SRC_DIR/runner/chasm-runner.h \
  $$SRC_DIR/runner/chasm-runner.templates.h \
  $$SRC_DIR/runner/chasm-value-holder.h \
  $$SRC_DIR/runner/chasm-result-holder.h \
  $$SRC_DIR/runner/chasm-run-router.h \
  $$SRC_DIR/runner/chasm-run-router--cast-needed-templates.h \
  $$SRC_DIR/runner/procs/chasm-runner--procs2.h \
  $$SRC_DIR/runner/graph-run/asg-proc-declarations.h \
  $$SRC_DIR/runner/graph-run/asg-proc-families.h \
  $$SRC_DIR/runner/graph-run/call/asg-call-v-v.h \
  $$SRC_DIR/runner/graph-run/call/asg-proc-v-v.h \
  $$SRC_DIR/csm-ghost-scope.h \
  $$SRC_DIR/chasm-runtime-eval.h \
  $$SRC_DIR/chvm-lexical-scope.h \



SOURCES += \
  $$SRC_DIR/chasm-runtime-bridge.cpp \
  $$SRC_DIR/runner/chasm-runner.cpp \
  $$SRC_DIR/runner/chasm-value-holder.cpp \
  $$SRC_DIR/runner/chasm-result-holder.cpp \
  $$SRC_DIR/runner/chasm-run-router.cpp \
  $$SRC_DIR/runner/procs/chasm-runner--add2.cpp \
  $$SRC_DIR/runner/procs/chasm-runner--div2.cpp \
  $$SRC_DIR/csm-ghost-scope.cpp \
  $$SRC_DIR/chasm-runtime-eval.cpp \
  $$SRC_DIR/chvm-lexical-scope.cpp \



message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)


