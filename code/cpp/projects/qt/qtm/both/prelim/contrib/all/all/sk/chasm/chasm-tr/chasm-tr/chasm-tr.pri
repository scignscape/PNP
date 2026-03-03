
#           Copyright Nathaniel Christen 2026.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)


PROJECT_NAME = chtr

include(../build-group.pri)

QT -= gui

exists($$ROOT_DIR/../preferred/sysr.pri): include($$ROOT_DIR/../preferred/sysr.pri)
exists($$ROOT_DIR/../preferred/sysr-c.pri): include($$ROOT_DIR/../preferred/sysr-c.pri)
exists($$ROOT_DIR/../preferred/compiler.pri): include($$ROOT_DIR/../preferred/compiler.pri)


INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR $$SRC_ROOT_DIR

INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR
INCLUDEPATH += $$PHAON_GRAPH_SRC_GROUP_DIR

INCLUDEPATH += $$SRC_PROSET_DIR/chasm-lib

CONFIG += no_keywords

DEFINES += USE_KANS
DEFINES += USE_OTNS


DEFINES += CAON_DEBUG


HEADERS += \
  $$SRC_DIR/kernel/dominion/types.h \
  $$SRC_DIR/kernel/dominion/connectors.h \
  $$SRC_DIR/types/chtr-type-object.h \
  $$SRC_DIR/types/chtr-type-system.h \
  $$SRC_DIR/chtr-channel-object.h \
  $$SRC_DIR/chtr-channel-package.h \
  $$SRC_DIR/chtr-source-token.h \
  $$SRC_DIR/chtr-proc-token.h \
  $$SRC_DIR/chtr-expression-entry.h \
  $$SRC_DIR/chtr-carrier.h \
  $$SRC_DIR/chtr-code-statement.h \
  $$SRC_DIR/chtr-lexical-scope.h \
  $$SRC_DIR/chtr-statement-body.h \
  $$SRC_DIR/chvm/chvm-code-generator.h \
  $$SRC_DIR/chvm/chvm-code-block.h \
  $$SRC_DIR/chvm/chvm-code-statement.h \
  $$SRC_DIR/chvm/chvm-procedure.h \
  $$SRC_DIR/chvm/generators/chvm-code-statement-generator.h \
  $$SRC_DIR/eval/chvm-code-runner.h \
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
  $$SRC_DIR/runner/graph-run/call/asg-call-c-c.h \
  $$SRC_DIR/runner/graph-run/call/asg-proc-c-c.h \
  $$SRC_DIR/runner/graph-run/call/asg-call-n-c.h \
  $$SRC_DIR/runner/graph-run/call/asg-proc-n-c.h \
  $$SRC_DIR/writers/chtr-statement-writer.h \
  $$SRC_DIR/writers/chvm-logger-writer.h \
  $$SRC_DIR/codegen/chtr-chvm-generator.h \
  $$SRC_DIR/codegen/chtr-chvm-line.h \


SOURCES += \
  $$SRC_DIR/chtr-channel-object.cpp \
  $$SRC_DIR/chtr-channel-package.cpp \
  $$SRC_DIR/chtr-source-token.cpp \
  $$SRC_DIR/chtr-proc-token.cpp \
  $$SRC_DIR/chtr-expression-entry.cpp \
  $$SRC_DIR/chtr-carrier.cpp \
  $$SRC_DIR/chtr-code-statement.cpp \
  $$SRC_DIR/chtr-lexical-scope.cpp \
  $$SRC_DIR/chtr-statement-body.cpp \
  $$SRC_DIR/types/chtr-type-object.cpp \
  $$SRC_DIR/types/chtr-type-system.cpp \
  $$SRC_DIR/chvm/chvm-code-generator.cpp \
  $$SRC_DIR/chvm/chvm-code-block.cpp \
  $$SRC_DIR/chvm/chvm-code-statement.cpp \
  $$SRC_DIR/chvm/chvm-procedure.cpp \
  $$SRC_DIR/chvm/generators/chvm-code-statement-generator.cpp \
  $$SRC_DIR/chvm/generators/chvm-code-statement-generator.generic.cpp \
  $$SRC_DIR/eval/chvm-code-runner.cpp \
  $$SRC_DIR/runner/chasm-runner.cpp \
  $$SRC_DIR/runner/chasm-value-holder.cpp \
  $$SRC_DIR/runner/chasm-result-holder.cpp \
  $$SRC_DIR/runner/chasm-run-router.cpp \
  $$SRC_DIR/runner/procs/chasm-runner--add2.cpp \
  $$SRC_DIR/runner/procs/chasm-runner--div2.cpp \
  $$SRC_DIR/writers/chtr-statement-writer.cpp \
  $$SRC_DIR/writers/chvm-logger-writer.cpp \
  $$SRC_DIR/codegen/chtr-chvm-generator.cpp \
  $$SRC_DIR/codegen/chtr-chvm-line.cpp \





HEADERS += \
  $$SRC_DIR/kernel/chtr-dominion.h \
  $$SRC_DIR/kernel/frame/chtr-frame.h \
  $$SRC_DIR/kernel/query/chtr-query.h \
  $$SRC_DIR/kernel/graph/chtr-node.h \
  $$SRC_DIR/kernel/graph/chtr-graph.h \


SOURCES += \
  $$SRC_DIR/kernel/chtr-dominion.cpp \
  $$SRC_DIR/kernel/frame/chtr-frame.cpp \
  $$SRC_DIR/kernel/query/chtr-query.cpp \
  $$SRC_DIR/kernel/graph/chtr-node.cpp \
  $$SRC_DIR/kernel/graph/chtr-graph.cpp \




message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)

