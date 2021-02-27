TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lX11

HEADERS += \
    graphics/canvas.h \
    graphics/window.h \
    graphics/graphics.h \
    graphics/x11.h

SOURCES += \
    graphics/canvas.c \
    graphics/window.c \
    main.c
        #graphics/x11.c

