//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
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

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

void process_char(struct state_machine *sm, char c) {
    if (!is_valid_char(c)) {
        sm->current = ERROR;
        return;
    }

    switch (sm->current) {
        case INIT:
            if (isdigit(c)) {
                sm->current = READING_NUMBER;
                sm->buffer[sm->buf_pos++] = c;
            } else if (isalpha(c)) {
                sm->current = READING_WORD;
                sm->buffer[sm->buf_pos++] = c;
            } else if (c == ' ' || c == '\t' || c == '\n') {
                sm->current = INIT;
            } else {
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
            } else if (c == ' ' || c == '\t' || c == '\n') {
                sm->number_count++;
                sm->buf_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
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
            } else if (c == ' ' || c == '\t' || c == '\n') {
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

int main(void) {
    struct state_machine sm;
    init_machine(&sm);

    printf("Enter text (alphanumeric words and numbers separated by spaces):\n");

    char input[MAX_INPUT_LEN * 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    for (size_t i = 0; i < len; i++) {
        process_char(&sm, input[i]);
        if (sm.current == ERROR) {
            fprintf(stderr, "Invalid input format\n");
            return 1;
        }
    }

    if (sm.current == READING_NUMBER) {
        sm.number_count++;
    } else if (sm.current == READING_WORD) {
        sm.word_count++;
    }

    printf("Numbers found: %d\n", sm.number_count);
    printf("Words found: %d\n", sm.word_count);

    return 0;
}