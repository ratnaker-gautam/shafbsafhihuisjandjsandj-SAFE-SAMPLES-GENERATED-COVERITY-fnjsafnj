//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

enum state {
    START,
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
    sm->current = START;
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

enum state handle_start(struct state_machine *sm, char c) {
    if (is_whitespace(c)) {
        return START;
    } else if (isdigit(c)) {
        if (sm->buf_pos < MAX_INPUT_LEN - 1) {
            sm->buffer[sm->buf_pos++] = c;
        }
        return READING_NUMBER;
    } else if (isalpha(c)) {
        if (sm->buf_pos < MAX_INPUT_LEN - 1) {
            sm->buffer[sm->buf_pos++] = c;
        }
        return READING_WORD;
    } else {
        return ERROR;
    }
}

enum state handle_reading_number(struct state_machine *sm, char c) {
    if (isdigit(c)) {
        if (sm->buf_pos < MAX_INPUT_LEN - 1) {
            sm->buffer[sm->buf_pos++] = c;
        }
        return READING_NUMBER;
    } else if (is_whitespace(c)) {
        sm->number_count++;
        sm->buf_pos = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
        return START;
    } else if (c == '\0') {
        sm->number_count++;
        return DONE;
    } else {
        return ERROR;
    }
}

enum state handle_reading_word(struct state_machine *sm, char c) {
    if (isalpha(c)) {
        if (sm->buf_pos < MAX_INPUT_LEN - 1) {
            sm->buffer[sm->buf_pos++] = c;
        }
        return READING_WORD;
    } else if (is_whitespace(c)) {
        sm->word_count++;
        sm->buf_pos = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
        return START;
    } else if (c == '\0') {
        sm->word_count++;
        return DONE;
    } else {
        return ERROR;
    }
}

void process_input(struct state_machine *sm, const char *input) {
    size_t len = strnlen(input, MAX_INPUT_LEN);
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        
        if (i < len && !is_valid_char(c)) {
            sm->current = ERROR;
            break;
        }
        
        switch (sm->current) {
            case START:
                sm->current = handle_start(sm, c);
                break;
            case READING_NUMBER:
                sm->current = handle_reading_number(sm, c);
                break;
            case READING_WORD:
                sm->current = handle_reading_word(sm, c);
                break;
            case ERROR:
            case DONE:
                return;
        }
        
        if (sm->current == ERROR || sm->current == DONE) {
            break;
        }
    }
}

int main(void) {
    struct state_machine sm;
    char input[MAX_INPUT_LEN + 1];
    
    init_machine(&sm);
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) >= MAX_INPUT_LEN) {
        printf("Input too long\n");
        return 1;
    }
    
    process_input(&sm, input);
    
    if (sm.current == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", sm.number_count);
    printf("Words found: %d\n", sm.word_count);
    
    return 0;
}