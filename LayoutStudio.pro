# -------------------------------------------------
# Project created by QtCreator 2010-10-03T04:03:27
# -------------------------------------------------
QT += opengl
TARGET = LayoutStudio
TEMPLATE = app
SOURCES += main.cpp \
    lsmainwindow.cpp \
    lsglobals.cpp \
    lyt/packagebase.cpp \
    lyt/directorypackage.cpp \
    lyt/layout.cpp \
    lyt/binaryfile.cpp \
    lyt/binaryfilesection.cpp \
    lyt/materials/material.cpp \
    lyt/pane.cpp \
    lyt/textbox.cpp \
    lyt/picture.cpp \
    lyt/window.cpp \
    lyt/bounding.cpp \
    lyt/group.cpp \
    lyt/materials/texmap.cpp \
    lyt/materials/texsrt.cpp \
    lyt/materials/texcoordgen.cpp \
    lyt/materials/chanctrl.cpp \
    lyt/materials/tevswaptable.cpp \
    lyt/materials/indirectstage.cpp \
    lyt/materials/tevstage.cpp \
    lyt/materials/alphacompare.cpp \
    lyt/materials/blendmode.cpp \
    wii/common.cpp \
    wii/archiveu8.cpp \
    wii/filesystem.cpp
HEADERS += lsmainwindow.h \
    lsglobals.h \
    lyt/packagebase.h \
    lyt/directorypackage.h \
    lyt/layout.h \
    lyt/binaryfile.h \
    lyt/binaryfilesection.h \
    lyt/materials/material.h \
    lyt/pane.h \
    lyt/common.h \
    lyt/textbox.h \
    lyt/picture.h \
    lyt/window.h \
    lyt/bounding.h \
    lyt/group.h \
    lyt/materials/texmap.h \
    lyt/materials/texsrt.h \
    lyt/materials/texcoordgen.h \
    lyt/materials/chanctrl.h \
    lyt/materials/tevswaptable.h \
    lyt/materials/indirectstage.h \
    lyt/materials/tevstage.h \
    lyt/materials/alphacompare.h \
    lyt/materials/blendmode.h \
    wii/archiveu8.h \
    wii/common.h \
    wii/filesystem.h
FORMS += lsmainwindow.ui
RESOURCES += resources.qrc

OTHER_FILES += \
    icons/window.png \
    icons/textbox.png \
    icons/picture.png \
    icons/pane.png \
    icons/bounding.png \
    README.markdown \
    LICENSE.txt
