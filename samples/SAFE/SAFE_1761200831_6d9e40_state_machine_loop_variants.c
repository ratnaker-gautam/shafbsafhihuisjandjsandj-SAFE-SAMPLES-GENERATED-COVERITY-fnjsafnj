//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

struct StateMachine {
    enum State current;
    int number_count;
    int word_count;
    char buffer[256];
    size_t buf_pos;
};

void init_machine(struct StateMachine *sm) {
    sm->current = START;
    sm->number_count = 0;
    sm->word_count = 0;
    sm->buf_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\n' || c == '\t';
}

void process_char(struct StateMachine *sm, char c) {
    if (!is_valid_char(c)) {
        sm->current = ERROR;
        return;
    }

    switch (sm->current) {
        case START:
            if (isdigit((unsigned char)c)) {
                sm->current = READING_NUMBER;
                sm->buffer[sm->buf_pos++] = c;
            } else if (isalpha((unsigned char)c)) {
                sm->current = READING_WORD;
                sm->buffer[sm->buf_pos++] = c;
            } else if (c == ' ' || c == '\t' || c == '\n') {
                sm->current = START;
            }
            break;

        case READING_NUMBER:
            if (isdigit((unsigned char)c)) {
                if (sm->buf_pos < sizeof(sm->buffer) - 1) {
                    sm->buffer[sm->buf_pos++] = c;
                } else {
                    sm->current = ERROR;
                }
            } else if (c == ' ' || c == '\t' || c == '\n') {
                sm->number_count++;
                sm->buf_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                sm->current = START;
            } else {
                sm->current = ERROR;
            }
            break;

        case READING_WORD:
            if (isalpha((unsigned char)c)) {
                if (sm->buf_pos < sizeof(sm->buffer) - 1) {
                    sm->buffer[sm->buf_pos++] = c;
                } else {
                    sm->current = ERROR;
                }
            } else if (c == ' ' || c == '\t' || c == '\n') {
                sm->word_count++;
                sm->buf_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                sm->current = START;
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
    struct StateMachine sm;
    init_machine(&sm);

    printf("Enter text (numbers and words separated by spaces):\n");

    int c;
    while ((c = getchar()) != EOF && sm.current != ERROR && sm.current != DONE) {
        if (c == '\n') {
            if (sm.current == READING_NUMBER) {
                sm.number_count++;
            } else if (sm.current == READING_WORD) {
                sm.word_count++;
            }
            sm.current = DONE;
            break;
        }
        process_char(&sm, (char)c);
    }

    if (sm.current == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }

    printf("Numbers found: %d\n", sm.number_count);
    printf("Words found: %d\n", sm.word_count);

    return 0;
}