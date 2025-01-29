QT       += charts
QT += gui #core gui
QT += widgets
QT += sql

# PostgreSQL
INCLUDEPATH += /usr/local/Cellar/libpq/16.2_1/include
LIBS += -L/usr/local/Cellar/libpq/16.2_1/lib -lpq

# Qt PostgreSQL Plugin
QTPLUGIN += qsqlpsql

INCLUDEPATH += /usr/local/opt/opencv/include/opencv4
#LDFLAGS: -L/usr/local/Cellar/opencv/4.8.0_6/lib -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc

LIBS += -L/usr/local/opt/opencv/lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgcodecs \
        -lopencv_imgproc
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport #original(without printsupport):  QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    asterismsdetector.cpp \
    boundingboxcreator.cpp \
    chart_histogram.cpp \
    chart_projections.cpp \
    coordinatesinspector.cpp \
    custom_view.cpp \
    dbstoragehandler.cpp \
    derivatives.cpp \
    gaussian1dfilter.cpp \
    gridlinesdetector.cpp \
    main.cpp \
    carteduciel.cpp \
    otsualgorithm.cpp \
    projectionscalculator.cpp \
    sobelfilter.cpp

HEADERS += \
    Boundary.h \
    asterismsdetector.h \
    boundingboxcreator.h \
    carteduciel.h \
    chart_histogram.h \
    chart_projections.h \
    coordinatesinspector.h \
    custom_view.h \
    dbstoragehandler.h \
    derivatives.h \
    gaussian1dfilter.h \
    gridlinesdetector.h \
    otsualgorithm.h \
    projectionscalculator.h \
    sobelfilter.h

FORMS += \
    carteduciel.ui \
    chart_histogram.ui \
    chart_projections.ui

TRANSLATIONS += \
    CarteDuCiel_en_150.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
