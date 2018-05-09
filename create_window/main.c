#include <X11/Xlib.h>

int main ()
{
    Display *dpy = XOpenDisplay(NULL);
    Window win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),0,0,100,100,0,0,0);
    XMapWindow(dpy, win);
    XFlush(dpy);
    while(1);
    return 0;
}
