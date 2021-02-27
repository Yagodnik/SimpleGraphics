#include "window.h"

window *get_window()
{
    window *win = malloc(sizeof(window));

    win->display = NULL;
    win->window = 0;
    win->on_resize = NULL;
    win->on_position_changed = NULL;
    win->on_destroy = NULL;

    win->title = (char *) malloc(sizeof(char));

    return win;
}

int window_init(window *win)
{
    if (win == NULL)
        return 1;

#ifdef __linux__
    win->display = XOpenDisplay(getenv("DISPLAY"));

    if (win->display == NULL) {
        printf("X11 error!\n");
        return 1;
    }

    win->screen = XDefaultScreen(win->display);

    unsigned long black = BlackPixel(win->display, win->screen);
    unsigned long white = WhitePixel(win->display, win->screen);

    //NO MOVEMENT BUG
    win->window = XCreateSimpleWindow(win->display, RootWindow(win->display, win->screen),
                                      win->x, win->y, win->width, win->height,
                                      1, white, black);

    XSelectInput(win->display, win->window, ExposureMask|ButtonPressMask|KeyPressMask);
    XSetStandardProperties(win->display, win->window,
                           win->title, "Test", None, NULL, 0, NULL);

    win->gc = DefaultGC(win->display, win->screen);
    XSetBackground(win->display, win->gc, white);
    XSetForeground(win->display, win->gc, black);
#endif

    return 0;
}

void free_window(window *win)
{
    if (win != NULL) {
        free(win);
        win = NULL;
    }
}

int window_close(window *win) {
    if (win == NULL)
        return 1;

    if (win->on_destroy)
        win->on_destroy(win);

#ifdef __linux__
    XDestroyWindow(win->display, win->window);
    XCloseDisplay(win->display);
#endif

    return 0;
}

window *cast_void_to_window(void *win)
{
    if (win == NULL)
        return NULL;

    return (window *) (win);
}

void window_event_listener(window *win)
{
    XWindowAttributes *attr = (XWindowAttributes *) malloc(sizeof(XWindowAttributes));
    XGetWindowAttributes(win->display, win->window, attr);


    if (attr->width != win->width || attr->height != win->height) {
        win->width = attr->width;
        win->height = attr->height;

        if (win->on_resize)
            win->on_resize((window *) win);
    }

    if (attr->x != win->x || attr->y != win->y) {
        win->x = attr->x;
        win->y = attr->y;

        if (win->on_position_changed)
            win->on_position_changed((window *) win);
    }

    free(attr);
}

void set_window_on_show(window *win, void (*event)(void *))
{
    if (event == NULL)
        return;

    win->on_show = event;
}

void set_window_on_resize(window *win, void (*event)(void *))
{
    if (event == NULL)
        return;

    win->on_resize = event;
}

void set_window_on_position_changed(window *win, void (*event)(void *))
{
    if (event == NULL)
        return;

    win->on_position_changed = event;
}

//Destroy event doesnt work
void set_window_on_destroy(window *win, void (*event)(void *))
{
    if (event == NULL)
        return;

    win->on_destroy = event;
}

int set_window_position(window *win, uint16_t x, uint16_t y)
{
    if (win == NULL)
        return 1;

    win->x = x;
    win->y = y;

#ifdef __linux__
    //NO MOVEMENT BUG
    if (win->display != NULL && win->window != 0)
        XMoveWindow(win->display, win->window, x, y);
#endif

    return 0;
}

int set_window_size(window *win, uint16_t width, uint16_t height)
{
    if (win == NULL)
        return 1;

    win->width = width;
    win->height = height;

    return 0;
}

int set_window_title(window *win, char *title)
{
    if (win == NULL)
        return 1;

    size_t title_size = sizeof (title) * strlen(title);

    (void)(realloc((char *) win->title, title_size));
    memcpy(win->title, title, title_size);

    return 0;
}

void window_show(window *win)
{
    if (win->on_show)
        win->on_show(win);

#ifdef __linux__
    XMapWindow(win->display, win->window);

    if (win->loop)
        win->loop((window *) win);
#endif
}

