//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

enum state {
    INIT,
    READING_NUM,
    READING_WORD,
    ERROR,
    DONE
};

struct state_machine {
    enum state current;
    int num_count;
    int word_count;
    char buffer[MAX_INPUT_LEN];
    int buf_pos;
};

void init_machine(struct state_machine *sm) {
    sm->current = INIT;
    sm->num_count = 0;
    sm->word_count = 0;
    sm->buf_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n' || c == EOF;
}

void process_char(struct state_machine *sm, char c) {
    if (!is_valid_char(c)) {
        sm->current = ERROR;
        return;
    }

    switch (sm->current) {
        case INIT:
            if (isdigit(c)) {
                sm->current = READING_NUM;
                sm->buffer[sm->buf_pos++] = c;
            } else if (isalpha(c)) {
                sm->current = READING_WORD;
                sm->buffer[sm->buf_pos++] = c;
            } else if (c == ' ' || c == '\t' || c == '\n') {
                sm->current = INIT;
            } else if (c == EOF) {
                sm->current = DONE;
            }
            break;

        case READING_NUM:
            if (isdigit(c)) {
                if (sm->buf_pos < MAX_INPUT_LEN - 1) {
                    sm->buffer[sm->buf_pos++] = c;
                } else {
                    sm->current = ERROR;
                }
            } else if (c == ' ' || c == '\t' || c == '\n') {
                sm->num_count++;
                sm->buf_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                sm->current = INIT;
            } else if (isalpha(c)) {
                sm->current = ERROR;
            } else if (c == EOF) {
                sm->num_count++;
                sm->current = DONE;
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
            } else if (isdigit(c)) {
                sm->current = ERROR;
            } else if (c == EOF) {
                sm->word_count++;
                sm->current = DONE;
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

    printf("Enter text (Ctrl+D to end):\n");

    int c;
    while ((c = getchar()) != EOF && sm.current != ERROR && sm.current != DONE) {
        process_char(&sm, (char)c);
    }

    if (c == EOF && sm.current != ERROR && sm.current != DONE) {
        process_char(&sm, EOF);
    }

    if (sm.current == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }

    printf("Numbers found: %d\n", sm.num_count);
    printf("Words found: %d\n", sm.word_count);

    return 0;
}