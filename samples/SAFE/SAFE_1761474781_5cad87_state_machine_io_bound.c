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

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void process_char(struct state_machine *sm, char c) {
    switch (sm->current) {
        case INIT:
            if (is_digit(c)) {
                sm->buffer[sm->buf_pos++] = c;
                sm->current = READING_NUM;
            } else if (is_alpha(c)) {
                sm->buffer[sm->buf_pos++] = c;
                sm->current = READING_WORD;
            } else if (!is_whitespace(c)) {
                sm->current = ERROR;
            }
            break;
            
        case READING_NUM:
            if (is_digit(c)) {
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
            if (is_alpha(c)) {
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
            break;
            
        case DONE:
            break;
    }
}

void finalize_processing(struct state_machine *sm) {
    if (sm->current == READING_NUM) {
        sm->buffer[sm->buf_pos] = '\0';
        sm->num_count++;
    } else if (sm->current == READING_WORD) {
        sm->buffer[sm->buf_pos] = '\0';
        sm->word_count++;
    }
    sm->current = DONE;
}

int main(void) {
    struct state_machine sm;
    char input[MAX_INPUT_LEN * 2];
    int c;
    
    init_machine(&sm);
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN * 2 - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    if (strlen(input) >= sizeof(input) - 1) {
        while ((c = getchar()) != '\n' && c != EOF);
        printf("Input too long\n");
        return 1;
    }
    
    for (int i = 0; input[i] != '\0'; i++) {
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