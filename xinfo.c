
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/scrnsaver.h>

const char * const INFO =
    "\n"
    "Print information about the current X session.\n"
    "\n"
    "Usage:\n"
    "    xinfo [ options ]\n"
    "\n"
    "Available options:\n"
    "    -r    screen resolution\n"
    "    -i    idle time in seconds\n"
    "\n";

void printResolution(Display * d, Screen * s) {
    printf("%d %d\n", s->width, s->height);
}

void printIdle(Display * d, Screen * s) {
    XScreenSaverInfo * info = XScreenSaverAllocInfo();
    XScreenSaverQueryInfo(d, s->root, info);
    printf("%lu\n", info->idle / 1000);
    XFree(info);
}

int main(int argc, char ** argv) {
    if (argc < 2) {
        fprintf(stderr, "%s", INFO);
        return 2;
    }
    Display * d = XOpenDisplay(0);
    if (!d) {
        fprintf(stderr, "Error: Could not open display\n");
        return 3;
    }
    Screen * s = DefaultScreenOfDisplay(d);
    char * print = argv[1];
    int result = 0;
    while (*print) {
        switch (*print) {
            case '-':
                break;
            case 'r':
                printResolution(d, s);
                break;
            case 'i':
                printIdle(d, s);
                break;
            default:
                printf("Unknown option '%c'\n", *print);
                result = 1;
                break;
        }
        ++print;
    }
    XCloseDisplay(d);
    return result;
}
