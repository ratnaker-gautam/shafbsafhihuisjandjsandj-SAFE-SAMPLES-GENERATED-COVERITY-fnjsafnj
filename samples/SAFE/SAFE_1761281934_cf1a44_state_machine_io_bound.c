//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READING_ID,
    READING_NUMBER,
    ERROR,
    DONE
};

struct parser {
    enum state current;
    int id_count;
    int num_count;
    char buffer[256];
    size_t buf_pos;
};

void parser_init(struct parser *p) {
    p->current = INIT;
    p->id_count = 0;
    p->num_count = 0;
    p->buf_pos = 0;
    memset(p->buffer, 0, sizeof(p->buffer));
}

int is_valid_id_char(char c) {
    return isalnum(c) || c == '_';
}

int is_valid_number_char(char c) {
    return isdigit(c) || c == '.';
}

void process_char(struct parser *p, char c) {
    if (p->current == ERROR || p->current == DONE) return;

    switch (p->current) {
        case INIT:
            if (isalpha(c) || c == '_') {
                p->current = READING_ID;
                p->buffer[p->buf_pos++] = c;
            } else if (isdigit(c)) {
                p->current = READING_NUMBER;
                p->buffer[p->buf_pos++] = c;
            } else if (!isspace(c)) {
                p->current = ERROR;
            }
            break;

        case READING_ID:
            if (is_valid_id_char(c)) {
                if (p->buf_pos < sizeof(p->buffer) - 1) {
                    p->buffer[p->buf_pos++] = c;
                } else {
                    p->current = ERROR;
                }
            } else if (isspace(c)) {
                p->id_count++;
                p->buf_pos = 0;
                memset(p->buffer, 0, sizeof(p->buffer));
                p->current = INIT;
            } else {
                p->current = ERROR;
            }
            break;

        case READING_NUMBER:
            if (is_valid_number_char(c)) {
                if (p->buf_pos < sizeof(p->buffer) - 1) {
                    p->buffer[p->buf_pos++] = c;
                } else {
                    p->current = ERROR;
                }
            } else if (isspace(c)) {
                p->num_count++;
                p->buf_pos = 0;
                memset(p->buffer, 0, sizeof(p->buffer));
                p->current = INIT;
            } else {
                p->current = ERROR;
            }
            break;

        default:
            break;
    }
}

void finalize_parse(struct parser *p) {
    if (p->current == READING_ID) {
        p->id_count++;
    } else if (p->current == READING_NUMBER) {
        p->num_count++;
    }
    p->current = DONE;
}

int main(void) {
    struct parser p;
    parser_init(&p);

    printf("Enter text with identifiers and numbers (Ctrl+D to end):\n");

    int c;
    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            finalize_parse(&p);
            break;
        }
        process_char(&p, (char)c);
        if (p.current == ERROR) {
            break;
        }
    }

    if (p.current == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }

    printf("Identifiers: %d\n", p.id_count);
    printf("Numbers: %d\n", p.num_count);

    return 0;
}