//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

struct StateMachine {
    enum State current;
    int number_count;
    int word_count;
    char buffer[256];
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
    return isalnum(c) || isspace(c) || c == '\0';
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
            } else if (c == '\0') {
                sm->current = DONE;
            }
            break;

        case READING_NUMBER:
            if (isdigit(c)) {
                if (sm->buf_pos < 255) {
                    sm->buffer[sm->buf_pos++] = c;
                } else {
                    sm->current = ERROR;
                }
            } else if (isspace(c) || c == '\0') {
                sm->number_count++;
                sm->buf_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                if (c == '\0') {
                    sm->current = DONE;
                } else {
                    sm->current = START;
                }
            } else {
                sm->current = ERROR;
            }
            break;

        case READING_WORD:
            if (isalpha(c)) {
                if (sm->buf_pos < 255) {
                    sm->buffer[sm->buf_pos++] = c;
                } else {
                    sm->current = ERROR;
                }
            } else if (isspace(c) || c == '\0') {
                sm->word_count++;
                sm->buf_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
                if (c == '\0') {
                    sm->current = DONE;
                } else {
                    sm->current = START;
                }
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
    char input[1024];
    
    printf("Enter text (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }

    init_machine(&sm);

    int i = 0;
    while (input[i] != '\0' && sm.current != ERROR && sm.current != DONE) {
        process_char(&sm, input[i]);
        i++;
    }

    if (sm.current != ERROR) {
        process_char(&sm, '\0');
    }

    if (sm.current == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }

    printf("Numbers: %d\n", sm.number_count);
    printf("Words: %d\n", sm.word_count);

    return 0;
}