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
    return isalnum(c) || is_whitespace(c);
}

void process_char(struct StateMachine *sm, char c) {
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
            } else if (is_whitespace(c)) {
                break;
            } else {
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

    printf("Enter text (alphanumeric words and numbers, max %d chars):\n", MAX_INPUT_LEN);
    
    char input[MAX_INPUT_LEN * 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    if (len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return 1;
    }

    for (size_t i = 0; i < len; i++) {
        process_char(&sm, input[i]);
        if (sm.current == ERROR) {
            printf("Invalid input format\n");
            return 1;
        }
    }

    finalize_machine(&sm);

    printf("Numbers found: %d\n", sm.number_count);
    printf("Words found: %d\n", sm.word_count);

    return 0;
}