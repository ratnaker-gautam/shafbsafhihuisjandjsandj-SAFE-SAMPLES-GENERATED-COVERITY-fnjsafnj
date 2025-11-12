//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    END
};

struct StateMachine {
    enum State current;
    int number_count;
    int word_count;
    char buffer[MAX_INPUT_LEN];
    int buf_pos;
};

void init_machine(struct StateMachine *sm) {
    sm->current = START;
    sm->number_count = 0;
    sm->word_count = 0;
    sm->buf_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n';
}

void process_char(struct StateMachine *sm, char c) {
    if (!is_valid_char(c)) {
        sm->current = ERROR;
        return;
    }

    switch (sm->current) {
        case START:
            if (isdigit(c)) {
                sm->current = READING_NUMBER;
                sm->buffer[sm->buf_pos++] = c;
            } else if (isalpha(c)) {
                sm->current = READING_WORD;
                sm->buffer[sm->buf_pos++] = c;
            } else if (c == ' ' || c == '\t') {
                sm->current = START;
            } else if (c == '\n') {
                sm->current = END;
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
                sm->buffer[sm->buf_pos] = '\0';
                sm->number_count++;
                sm->buf_pos = 0;
                if (c == '\n') {
                    sm->current = END;
                } else {
                    sm->current = START;
                }
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
                sm->buffer[sm->buf_pos] = '\0';
                sm->word_count++;
                sm->buf_pos = 0;
                if (c == '\n') {
                    sm->current = END;
                } else {
                    sm->current = START;
                }
            } else {
                sm->current = ERROR;
            }
            break;

        case ERROR:
        case END:
            break;
    }
}

int main(void) {
    struct StateMachine sm;
    init_machine(&sm);

    printf("Enter text (numbers and words separated by spaces): ");
    
    int c;
    while ((c = getchar()) != EOF && sm.current != END && sm.current != ERROR) {
        if (c == '\n') {
            process_char(&sm, '\n');
            break;
        }
        process_char(&sm, (char)c);
    }

    if (sm.current == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }

    if (sm.buf_pos > 0) {
        if (isdigit(sm.buffer[0])) {
            sm.number_count++;
        } else if (isalpha(sm.buffer[0])) {
            sm.word_count++;
        }
    }

    printf("Numbers found: %d\n", sm.number_count);
    printf("Words found: %d\n", sm.word_count);

    return 0;
}