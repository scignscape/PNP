
#           Copyright Nathaniel Christen 2026.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)


PROJECT_NAME = chasm-console

include(../build-group.pri)


exists($$ROOT_DIR/../preferred/sysr.pri): include($$ROOT_DIR/../preferred/sysr.pri)
exists($$ROOT_DIR/../preferred/sysr-c.pri): include($$ROOT_DIR/../preferred/sysr-c.pri)
exists($$ROOT_DIR/../preferred/compiler.pri): include($$ROOT_DIR/../preferred/compiler.pri)


INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR $$SRC_ROOT_DIR

CONFIG += no_keywords

DEFINES += USE_OTNS

DEFINES += ROOT_FOLDER=\\\"$$ROOT_DIR\\\"
DEFINES += DEMO_CVM_FOLDER=\\\"$$ROOT_DIR/../dev/chtr\\\"


HEADERS += \
  $$SRC_DIR/otqr-sdi-parser.h \
  $$SRC_DIR/sdi-sentence-reader.h \
  $$SRC_DIR/sentences/sdi-sentence.h \


SOURCES += \
  $$SRC_DIR/otqr-sdi-parser.cpp \
  $$SRC_DIR/sdi-sentence-reader.cpp \
  $$SRC_DIR/sentences/sdi-sentence.cpp \


LIBS += -L$$TARGETSDIR -lchasm-tr  -lchasm-tr-parser


#  these are not needed if the demo for this project only concerns emitting chvm
#LIBS += -L$$TARGETSDIR -lchasm-lib \
#  -lchasm-vm -lchasm-runtime-bridge -lchasm-procedure-table \
#  -lchasm-lib-X1 -lchasm-lib-X2 \
#  -lchasm-lib-33 -lchasm-lib-43




message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)

