QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
TARGET = "Graph Visualizer"

SOURCES += \
    a_star_graph_algorithm.cpp \
    bfs_graph_algorithm.cpp \
    dfs_graph_algorithm.cpp \
    graph_algorithm.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    a_star_graph_algorithm.h \
    bfs_graph_algorithm.h \
    dfs_graph_algorithm.h \
    graph_algorithm.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
