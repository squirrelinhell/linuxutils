
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

const char * const INFO =
    "\n"
    "Find lines starting with a hex number with a small"
        " Hamming distance from the specified argument.\n"
    "\n"
    "Usage:\n"
    "    hashgrep [ -d <bits> ] <hex string>\n"
    "\n";

int hex_value(int c) {
    if ((c >= '0') && (c <= '9'))
        return c - '0';
    if ((c >= 'a') && (c <= 'f'))
        return (c - 'a') + 10;
    if ((c >= 'A') && (c <= 'F'))
        return (c - 'A') + 10;
    return -1;
}

int main(int argc, char ** argv) {
    int tolerance = -1;

    if ((argc >= 4) && (strcmp(argv[1], "-d") == 0)) {
        tolerance = atoi(argv[2]);
        argc -= 2;
        argv += 2;
    }

    if (argc != 2) {
        fprintf(stderr, "%s", INFO);
        return 2;
    }

    int match_len = strlen(argv[1]);
    char * match_values = malloc(match_len);
    char * read_buf = malloc(match_len);

    if (tolerance < 0)
        tolerance = (match_len + 1) / 2;

    int match_pos = 0;
    int diff = 0;
    int c;
    int c_value;
    int exit_code = 1;

    // Convert the argument to hex values
    for (c = 0; c < match_len; ++c) {
        c_value = hex_value(argv[1][c]);
        if (c_value < 0) {
            fprintf(stderr, "Error: argument is not a hex number\n");
            return 2;
        }
        match_values[c] = c_value;
    }

    // If match string is empty, copy over everything
    if (match_len <= 0) {
        while ((c = getc(stdin)) >= 0)
            putc(c, stdout);
        return 0;
    }

    while ((c = getc(stdin)) >= 0) {
        c_value = hex_value(c);

        if (c_value < 0) {
            // No match, skip the rest of the line
            match_pos = 0;
            diff = 0;
            while ((c != '\n') && (c >= 0))
                c = getc(stdin);
            continue;
        }

        read_buf[match_pos] = c;
        diff += __builtin_popcount(c_value ^ match_values[match_pos]);
        ++match_pos;

        if ((match_pos == match_len) && (diff <= tolerance)) {
            // Successful match, print out the whole line (buffer first)
            exit_code = 0;
            for (c = 0; c < match_len; ++c)
                putc(read_buf[c], stdout);
            while ((c = getc(stdin)) >= 0) {
                putc(c, stdout);
                if (c == '\n')
                    break;
            }
            match_pos = 0;
            diff = 0;
        }
    }

    return exit_code;
}
