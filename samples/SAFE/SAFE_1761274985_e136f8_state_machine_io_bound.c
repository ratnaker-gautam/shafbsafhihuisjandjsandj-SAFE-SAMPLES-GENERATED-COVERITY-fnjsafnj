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
    char buffer[MAX_INPUT_LEN + 1];
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
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

enum State handle_start(struct StateMachine *sm, char c) {
    if (isdigit(c)) {
        sm->buffer[sm->buf_pos++] = c;
        return READING_NUMBER;
    } else if (isalpha(c)) {
        sm->buffer[sm->buf_pos++] = c;
        return READING_WORD;
    } else if (c == ' ' || c == '\t' || c == '\n') {
        return START;
    } else if (c == '\0') {
        return END;
    } else {
        return ERROR;
    }
}

enum State handle_reading_number(struct StateMachine *sm, char c) {
    if (isdigit(c)) {
        if (sm->buf_pos < MAX_INPUT_LEN) {
            sm->buffer[sm->buf_pos++] = c;
        }
        return READING_NUMBER;
    } else if (c == ' ' || c == '\t' || c == '\n' || c == '\0') {
        sm->buffer[sm->buf_pos] = '\0';
        sm->number_count++;
        sm->buf_pos = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
        if (c == '\0') return END;
        return START;
    } else {
        return ERROR;
    }
}

enum State handle_reading_word(struct StateMachine *sm, char c) {
    if (isalpha(c)) {
        if (sm->buf_pos < MAX_INPUT_LEN) {
            sm->buffer[sm->buf_pos++] = c;
        }
        return READING_WORD;
    } else if (c == ' ' || c == '\t' || c == '\n' || c == '\0') {
        sm->buffer[sm->buf_pos] = '\0';
        sm->word_count++;
        sm->buf_pos = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
        if (c == '\0') return END;
        return START;
    } else {
        return ERROR;
    }
}

enum State process_char(struct StateMachine *sm, char c) {
    if (!is_valid_char(c)) {
        return ERROR;
    }

    switch (sm->current) {
        case START:
            return handle_start(sm, c);
        case READING_NUMBER:
            return handle_reading_number(sm, c);
        case READING_WORD:
            return handle_reading_word(sm, c);
        case ERROR:
        case END:
            return sm->current;
        default:
            return ERROR;
    }
}

int main(void) {
    struct StateMachine sm;
    init_machine(&sm);
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    char input[MAX_INPUT_LEN + 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (int i = 0; i < len && input[i] != '\0'; i++) {
        sm.current = process_char(&sm, input[i]);
        if (sm.current == ERROR) {
            fprintf(stderr, "Invalid character at position %d\n", i);
            return 1;
        }
        if (sm.current == END) {
            break;
        }
    }
    
    if (sm.current != END) {
        sm.current = process_char(&sm, '\0');
    }
    
    printf("Numbers found: %d\n", sm.number_count);
    printf("Words found: %d\n", sm.word_count);
    
    return 0;
}