#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "canvas.h"

#ifdef __linux__
#include "x11.h"
#else
#error "Sorry only linux"
#endif

typedef struct {
    uint16_t x;
    uint16_t y;

    uint16_t width;
    uint16_t height;
    char *title;

#ifdef __linux__
    int screen;

    Display *display;
    Window window;
    GC gc;
#endif
    //Loop
    void (*loop)(void *win);

    //Events
    void (*on_show)(void *win);
    void (*on_resize)(void *win);
    void (*on_position_changed)(void *win);
    void (*on_destroy)(void *win);

    canvas *canvas;

    //Maybe platform context
    //Input source etc
} window;

window *get_window();
int window_init(window *win);
void free_window(window *win);
int window_close(window *win);

window *cast_void_to_window(void *win);

void window_event_listener(window *win);
void set_window_on_show(window *win, void (*event)(void *win));
void set_window_on_resize(window *win, void (*event)(void *win));
void set_window_on_position_changed(window *win, void (*event)(void *win));
void set_window_on_destroy(window *win, void (*event)(void *win));

int set_window_position(window *win, uint16_t x, uint16_t y);
int set_window_size(window *win, uint16_t width, uint16_t height);
int set_window_title(window *win, char *title);

void window_attach_canvas(window *win, canvas *arg);
void window_attach_input(window *win, canvas *arg);
void window_show(window *win);

#endif // WINDOW_H
