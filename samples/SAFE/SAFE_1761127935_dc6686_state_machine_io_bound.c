//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

enum state {
    INIT,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

struct state_machine {
    enum state current;
    int number_count;
    int word_count;
    char buffer[MAX_INPUT_LEN];
    int buf_pos;
};

void init_machine(struct state_machine *sm) {
    sm->current = INIT;
    sm->number_count = 0;
    sm->word_count = 0;
    sm->buf_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || is_whitespace(c);
}

void process_char(struct state_machine *sm, char c) {
    if (!is_valid_char(c)) {
        sm->current = ERROR;
        return;
    }

    switch (sm->current) {
        case INIT:
            if (isdigit((unsigned char)c)) {
                sm->current = READING_NUMBER;
                sm->buffer[sm->buf_pos++] = c;
            } else if (isalpha((unsigned char)c)) {
                sm->current = READING_WORD;
                sm->buffer[sm->buf_pos++] = c;
            } else if (is_whitespace(c)) {
                sm->current = INIT;
            } else {
                sm->current = ERROR;
            }
            break;

        case READING_NUMBER:
            if (isdigit((unsigned char)c)) {
                if (sm->buf_pos < MAX_INPUT_LEN - 1) {
                    sm->buffer[sm->buf_pos++] = c;
                } else {
                    sm->current = ERROR;
                }
            } else if (is_whitespace(c)) {
                sm->number_count++;
                sm->buf_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                sm->current = INIT;
            } else {
                sm->current = ERROR;
            }
            break;

        case READING_WORD:
            if (isalpha((unsigned char)c)) {
                if (sm->buf_pos < MAX_INPUT_LEN - 1) {
                    sm->buffer[sm->buf_pos++] = c;
                } else {
                    sm->current = ERROR;
                }
            } else if (is_whitespace(c)) {
                sm->word_count++;
                sm->buf_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                sm->current = INIT;
            } else {
                sm->current = ERROR;
            }
            break;

        case ERROR:
        case DONE:
            break;
    }
}

void finalize_machine(struct state_machine *sm) {
    if (sm->current == READING_NUMBER) {
        sm->number_count++;
    } else if (sm->current == READING_WORD) {
        sm->word_count++;
    }
    sm->current = DONE;
}

int main(void) {
    struct state_machine sm;
    init_machine(&sm);

    printf("Enter text (alphanumeric words and numbers separated by spaces):\n");

    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (sm.current == ERROR) {
            break;
        }
        process_char(&sm, (char)c);
    }

    if (sm.current != ERROR) {
        finalize_machine(&sm);
    }

    if (sm.current == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }

    printf("Numbers found: %d\n", sm.number_count);
    printf("Words found: %d\n", sm.word_count);

    return 0;
}