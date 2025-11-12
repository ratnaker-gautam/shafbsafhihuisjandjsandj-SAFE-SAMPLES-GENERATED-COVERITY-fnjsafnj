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

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

int is_valid_char(char c) {
    return isalnum(c) || is_whitespace(c);
}

void process_char(struct state_machine *sm, char c) {
    if (!is_valid_char(c)) {
        sm->current = ERROR;
        return;
    }

    switch (sm->current) {
        case INIT:
            if (isdigit(c)) {
                sm->buffer[sm->buf_pos++] = c;
                sm->current = READING_NUM;
            } else if (isalpha(c)) {
                sm->buffer[sm->buf_pos++] = c;
                sm->current = READING_WORD;
            } else if (is_whitespace(c)) {
                break;
            } else {
                sm->current = ERROR;
            }
            break;

        case READING_NUM:
            if (isdigit(c)) {
                if (sm->buf_pos < MAX_INPUT_LEN - 1) {
                    sm->buffer[sm->buf_pos++] = c;
                } else {
                    sm->current = ERROR;
                }
            } else if (is_whitespace(c)) {
                sm->buffer[sm->buf_pos] = '\0';
                sm->num_count++;
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

void finalize_processing(struct state_machine *sm) {
    if (sm->current == READING_NUM || sm->current == READING_WORD) {
        if (sm->buf_pos > 0) {
            sm->buffer[sm->buf_pos] = '\0';
            if (sm->current == READING_NUM) {
                sm->num_count++;
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
    char line[MAX_INPUT_LEN * 2];

    init_machine(&sm);

    printf("Enter text (alphanumeric words and numbers separated by spaces):\n");
    
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 1;
    }

    size_t line_len = strlen(line);
    if (line_len > 0 && line[line_len - 1] == '\n') {
        line[line_len - 1] = '\0';
        line_len--;
    }

    if (line_len >= sizeof(input)) {
        return 1;
    }

    strncpy(input, line, sizeof(input) - 1);
    input[sizeof(input) - 1] = '\0';

    for (size_t i = 0; i < strlen(input); i++) {
        if (sm.current == ERROR) {
            break;
        }
        process_char(&sm, input[i]);
    }

    finalize_processing(&sm);

    if (sm.current == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }

    printf("Numbers found: %d\n", sm.num_count);
    printf("Words found: %d\n", sm.word_count);

    return 0;
}