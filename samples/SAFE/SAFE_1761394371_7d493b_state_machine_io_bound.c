//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

enum State {
    INIT,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

struct StateMachine {
    enum State current;
    int number_count;
    int word_count;
    char buffer[MAX_INPUT_LEN + 1];
    int buf_pos;
};

void init_machine(struct StateMachine *sm) {
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

void process_char(struct StateMachine *sm, char c) {
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
                if (sm->buf_pos < MAX_INPUT_LEN) {
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
            if (isalpha(c)) {
                if (sm->buf_pos < MAX_INPUT_LEN) {
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

void finalize_machine(struct StateMachine *sm) {
    if (sm->current == READING_NUMBER) {
        sm->number_count++;
    } else if (sm->current == READING_WORD) {
        sm->word_count++;
    }
    sm->current = DONE;
}

int main(void) {
    struct StateMachine sm;
    init_machine(&sm);

    printf("Enter text (max %d chars per token): ", MAX_INPUT_LEN);
    printf("Input will be processed character by character.\n");
    printf("Type '.' on a line by itself to finish.\n");

    int c;
    while ((c = getchar()) != EOF && sm.current != ERROR) {
        if (c == '.' && sm.buf_pos == 0) {
            int next = getchar();
            if (next == '\n' || next == EOF) {
                break;
            }
            ungetc(next, stdin);
        }

        process_char(&sm, (char)c);

        if (sm.buf_pos >= MAX_INPUT_LEN && sm.current != INIT) {
            sm.current = ERROR;
        }
    }

    if (sm.current == ERROR) {
        printf("Error: Invalid input format detected.\n");
        return 1;
    }

    finalize_machine(&sm);

    printf("Numbers found: %d\n", sm.number_count);
    printf("Words found: %d\n", sm.word_count);

    return 0;
}