QT       += core gui widgets network

TARGET = TicTacToe
TEMPLATE = app

HEADERS  += hcpp/mainwindow.h\
        hcpp/textwindow.h\
        hcpp/connection.h\
        hcpp/server.h\
        hcpp/peermanager.h\
        hcpp/client.h \
        hcpp/game.h \
        hcpp/clickablelabel.h

SOURCES += hcpp/main.cpp\
        hcpp/mainwindow.cpp\
        hcpp/textwindow.cpp\
        hcpp/connection.cpp\
        hcpp/server.cpp\
        hcpp/peermanager.cpp\
        hcpp/client.cpp \
        hcpp/game.cpp \
        hcpp/clickablelabel.cpp

FORMS    += ui/mainwindow.ui\
        ui/textwindow.ui

RESOURCES += \
        resources/resources.qrc

win32 {
    RC_ICONS = TicTacToe.ico
} else:macx {
    ICON = TicTacToe.icns
    FILETYPES.files = TicTacToe.icns
    FILETYPES.path = Contents/Resources
    QMAKE_BUNDLE_DATA += FILETYPES
} else {
}
