QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17
CONFIG += sdk_no_version_check
macx: QMAKE_LFLAGS += -Wl,-w,-undefined,dynamic_lookup

INCLUDEPATH += HEADERS

SOURCES += $$files(CPP/*.cpp)
HEADERS += $$files(HEADERS/*.h)
FORMS += $$files(UI/*.ui)
# Fix for macOS - suppress AGL framework
macx {
    QMAKE_LFLAGS += -Wl,-w
    CONFIG -= agl
    QMAKE_MAC_SDK = macosx
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
