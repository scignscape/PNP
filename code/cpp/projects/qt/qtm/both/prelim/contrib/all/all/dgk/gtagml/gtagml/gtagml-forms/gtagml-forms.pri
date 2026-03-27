
#           Copyright Nathaniel Christen 2020.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

PROJECT_NAME = gtagml-console

#QT -= gui

QT += xmlpatterns gui widgets webenginewidgets

include(../build-group.pri)


DEFINES += HAVE_NO_QT_GUI_MODULE


TEMPLATE = app


exists($$ROOT_DIR/../preferred/sysr.pri): include($$ROOT_DIR/../preferred/sysr.pri)


INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR $$SRC_ROOT_DIR

INCLUDEPATH += $$PHAON_GRAPH_SRC_GROUP_DIR
INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR


INCLUDEPATH += $$SRC_GROUP_DIR/gtagml


DEFINES += ROOT_FOLDER=\\\"$$ROOT_DIR\\\"

DEFINES += DEFAULT_GTAGML_FOLDER=\\\"$$ROOT_DIR/dev/consoles/gtagml/gt\\\"
DEFINES += DEFAULT_SDI_FOLDER=\\\"$$ROOT_DIR/dev/consoles/gtagml/sdi\\\"


DEFINES += DEFAULT_HTML_FOLDER=\\\"$$ROOT_DIR/html\\\"
DEFINES += DEFAULT_FIELDS_FOLDER=\\\"$$ROOT_DIR/../latex/fields\\\"
DEFINES += DEFAULT_FIELDS_SRC_FOLDER=\\\"$$ROOT_DIR/../latex/fields/src\\\"
DEFINES += DEFAULT_LATEX_FOLDER=\\\"$$ROOT_DIR/../latex\\\"



DEFINES += CAON_DEBUG
DEFINES += RELAE_LABEL_NODES

#CONFIG += no_keywords


DEFINES += USE_KANS


HEADERS += \
  $$SRC_DIR/MRichTextEditor/mrichtextedit.h \
  $$SRC_DIR/MRichTextEditor/mtextedit.h \
  $$SRC_DIR/MRichTextEditor/text-edit-frame.h \
  $$SRC_DIR/qh/JsInterface.h \
  $$SRC_DIR/qh/qh-web-engine-page.h \
  $$SRC_DIR/qh/qh-web-engine-view.h \
  $$SRC_DIR/qh/qh-web-page.h \
  $$SRC_DIR/qh/qh-web-view-dialog.h \
  $$SRC_DIR/demo-form-frame.h \
  $$SRC_DIR/form-weaver.h \


FORMS +=   $$SRC_DIR/MRichTextEditor/mrichtextedit.ui \

INCLUDEPATH += $$SRC_DIR/MRichTextEditor/

SOURCES +=  \
  $$SRC_DIR/main.cpp \
  $$SRC_DIR/MRichTextEditor/mrichtextedit.cpp \
  $$SRC_DIR/MRichTextEditor/mtextedit.cpp \
  $$SRC_DIR/MRichTextEditor/text-edit-frame.cpp \
  $$SRC_DIR/qh/qh-web-engine-page.cpp \
  $$SRC_DIR/qh/qh-web-engine-view.cpp \
  $$SRC_DIR/qh/qh-web-page.cpp \
  $$SRC_DIR/qh/qh-web-view-dialog.cpp \
  $$SRC_DIR/demo-form-frame.cpp \
  $$SRC_DIR/form-weaver.cpp \

#LIBS += -L$$TARGETSDIR -lgtagml

message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)

