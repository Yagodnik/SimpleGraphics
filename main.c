#include <stdio.h>
#include "graphics/graphics.h"

void on_window_resize(void *win) {
    window *temp = NULL;

    if (win != NULL)
        temp = cast_void_to_window(win);

    if (temp == NULL)
        return;

    printf("%s:%d:%d\n", temp->title, temp->width, temp->height);

    set_window_position(temp, 100, 400);
}

void on_window_destroy(void *win) {
    window *temp = NULL;

    if (win != NULL)
        temp = cast_void_to_window(win);

    if (temp == NULL)
        return;

    printf("F\n");
}

void on_window_position_changed(void *win) {
    window *temp = NULL;

    if (win != NULL)
        temp = cast_void_to_window(win);

    if (temp == NULL)
        return;

    printf("%s:%d:%d\n", temp->title, temp->x, temp->y);
}

void window_loop(void *temp) {
    window *win = NULL;

    if (temp != NULL)
        win = cast_void_to_window(temp);

    if (win == NULL)
        return;

    XEvent e;
    int x = 0, y = 0;
    int speed = 0;
    int counter = 0;

    while (1) {
        XNextEvent(win->display, &e);
        window_event_listener(win);


        if (e.type == Expose) {
           // if (e.xexpose.count == 0) {
                XClearWindow(win->display, win->window);
                XSetForeground(win->display, win->gc, WhitePixel(win->display, win->screen));
                XFillRectangle(win->display, win->window, win->gc, x, y, 10, 10);
                x += speed;
                y += speed;

                if (x >= win->width || y >= win->height) {
                    speed = -2;
                } else if (x <= 0 || y <= 0) {
                    speed = 2;
                }
           // }
        }

        if (e.type == KeyPress)
            speed = 0;

        counter = 0;

       //XSendEvent(win->display, win->window, True, 0, &e);
        XSendEvent(win->display, win->window, True, Expose, &e);
        XFlush(win->display);

        int msec = 0, trigger = 10;
        clock_t before = clock();

        while (msec < trigger) {
          clock_t difference = clock() - before;
          msec = difference * 1000 / CLOCKS_PER_SEC;
        }
    }
}

int main()
{
    window *win = get_window();

    win->loop = window_loop;
    set_window_on_resize(win, on_window_resize);
    set_window_on_position_changed(win, on_window_position_changed);
    set_window_on_destroy(win, on_window_destroy);

    set_window_size(win, 512, 512);
   // set_window_position(win, 100, 100);
    set_window_title(win, "Hello, World");
    window_init(win);
    window_show(win);
    window_close(win);

    free_window(win);

    return 0;
}
