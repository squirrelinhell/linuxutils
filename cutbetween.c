
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

const char * const INFO =
    "\n"
    "Cut a section of input between two given strings.\n"
    "\n"
    "Usage:\n"
    "    cutbetween <string1> <string2>\n"
    "\n";

int * kmp_buf = 0;
unsigned kmp_matched = 0;
int kmp_print = 0;

void kmp_start(const char * needle, int delayprint) {
    unsigned k, i;
    kmp_print = delayprint;
    if (kmp_buf) free(kmp_buf);
    kmp_buf = malloc(sizeof(unsigned)*strlen(needle));
    if (!kmp_buf) return;
    kmp_buf[0] = 0;
    for (i = 1; needle[i]; i++) {
        kmp_buf[i] = 0;
        k = i;
        while (k >= 1) {
            k = kmp_buf[k-1];
            if (needle[i] == needle[k]) {
                kmp_buf[i] = k+1;
                break;
            }
        }
    }
    kmp_matched = 0;
}

void kmp_free(const char * needle) {
    unsigned i;
    if (kmp_buf) {
        if (kmp_print && kmp_matched) {
            for (i = 0; i < kmp_matched; i++) putc(needle[i], stdout);
        }
        free(kmp_buf);
    }
    kmp_buf = 0;
}

int kmp_next(const char * needle, char next) {
    unsigned k, i;
    if (needle[kmp_matched] == next) {
        kmp_matched++;
        if (needle[kmp_matched] == 0) {
            kmp_matched = 0;
            return 1;
        }
        return 0;
    }
    while (kmp_matched > 0) {
        k = kmp_matched;
        kmp_matched = kmp_buf[kmp_matched-1];
        if (kmp_print) {
            for (i = 0; i < k-kmp_matched; i++) putc(needle[i], stdout);
        }
        if (needle[kmp_matched] == next) {
            kmp_matched++;
            return 0;
        }
    }
    if (kmp_print) putc(next, stdout);
    return 0;
}

int main(int argc, char ** argv) {
    if (argc != 3) {
        fprintf(stderr, "%s", INFO);
        return 2;
    }
    int c;
    if (argv[1][0]) {
        kmp_start(argv[1], 0);
        while ((c = getc(stdin)) >= 0) {
            if (kmp_next(argv[1], c)) break;
        }
        kmp_free(argv[1]);
        if (c < 0) return 1;
    }
    if (argv[2][0]) {
        kmp_start(argv[2], 1);
        while ((c = getc(stdin)) >= 0) {
            if (kmp_next(argv[2], c)) break;
        }
        if (c >= 0) kmp_print = 0;
        kmp_free(argv[2]);
        while ((c = getc(stdin)) >= 0);
    } else {
        while ((c = getc(stdin)) >= 0) putc(c, stdout);
    }
    return 0;
}
