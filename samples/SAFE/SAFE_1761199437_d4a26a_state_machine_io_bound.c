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
    return isprint(c) && !iscntrl(c);
}

void process_char(struct state_machine *sm, char c) {
    if (!is_valid_char(c) && !is_whitespace(c)) {
        sm->current = ERROR;
        return;
    }

    switch (sm->current) {
        case INIT:
            if (isdigit(c)) {
                sm->buffer[sm->buf_pos++] = c;
                sm->current = READING_NUMBER;
            } else if (isalpha(c)) {
                sm->buffer[sm->buf_pos++] = c;
                sm->current = READING_WORD;
            } else if (!is_whitespace(c)) {
                sm->current = ERROR;
            }
            break;

        case READING_NUMBER:
            if (isdigit(c)) {
                if (sm->buf_pos < MAX_INPUT_LEN - 1) {
                    sm->buffer[sm->buf_pos++] = c;
                } else {
                    sm->current = ERROR;
                }
            } else if (is_whitespace(c)) {
                sm->buffer[sm->buf_pos] = '\0';
                sm->number_count++;
                sm->buf_pos = 0;
                sm->current = INIT;
            } else {
                sm->current = ERROR;
            }
            break;

        case READING_WORD:
            if (isalpha(c)) {
                if (sm->buf_pos < MAX_INPUT_LEN - 1) {
                    sm->buffer[sm->buf_pos++] = c;
                } else {
                    sm->current = ERROR;
                }
            } else if (is_whitespace(c)) {
                sm->buffer[sm->buf_pos] = '\0';
                sm->word_count++;
                sm->buf_pos = 0;
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
    if (sm->current == READING_NUMBER || sm->current == READING_WORD) {
        if (sm->buf_pos > 0) {
            sm->buffer[sm->buf_pos] = '\0';
            if (sm->current == READING_NUMBER) {
                sm->number_count++;
            } else {
                sm->word_count++;
            }
        }
        sm->current = DONE;
    } else if (sm->current == INIT) {
        sm->current = DONE;
    }
}

int main(void) {
    struct state_machine sm;
    char input[MAX_INPUT_LEN * 2];
    size_t input_len;

    init_machine(&sm);

    printf("Enter text (numbers and words separated by spaces): ");
    fflush(stdout);

    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    input_len = strlen(input);
    if (input_len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    for (size_t i = 0; i < input_len; i++) {
        if (sm.current == ERROR) break;
        process_char(&sm, input[i]);
    }

    finalize_machine(&sm);

    if (sm.current == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }

    printf("Numbers found: %d\n", sm.number_count);
    printf("Words found: %d\n", sm.word_count);

    return 0;
}