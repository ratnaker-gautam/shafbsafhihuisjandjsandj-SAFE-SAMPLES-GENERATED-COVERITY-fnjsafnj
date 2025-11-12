//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

struct StateMachine {
    enum State current;
    int number_count;
    int word_count;
};

void init_machine(struct StateMachine *sm) {
    sm->current = START;
    sm->number_count = 0;
    sm->word_count = 0;
}

enum State handle_start(char c) {
    if (isspace(c)) return START;
    if (isdigit(c)) return READING_NUMBER;
    if (isalpha(c)) return READING_WORD;
    return ERROR;
}

enum State handle_reading_number(char c) {
    if (isdigit(c)) return READING_NUMBER;
    if (isspace(c)) return START;
    return ERROR;
}

enum State handle_reading_word(char c) {
    if (isalpha(c)) return READING_WORD;
    if (isspace(c)) return START;
    return ERROR;
}

enum State transition(struct StateMachine *sm, char c) {
    switch (sm->current) {
        case START:
            return handle_start(c);
        case READING_NUMBER:
            return handle_reading_number(c);
        case READING_WORD:
            return handle_reading_word(c);
        case ERROR:
        case DONE:
            return sm->current;
    }
    return ERROR;
}

void process_input(struct StateMachine *sm, const char *input) {
    size_t len = strlen(input);
    if (len >= MAX_INPUT_LEN) {
        sm->current = ERROR;
        return;
    }

    for (size_t i = 0; i < len; i++) {
        enum State next = transition(sm, input[i]);
        
        if (next == READING_NUMBER && sm->current != READING_NUMBER) {
            sm->number_count++;
        } else if (next == READING_WORD && sm->current != READING_WORD) {
            sm->word_count++;
        }
        
        sm->current = next;
        if (sm->current == ERROR) break;
    }
    
    if (sm->current != ERROR) {
        sm->current = DONE;
    }
}

int main(void) {
    struct StateMachine sm;
    char input[MAX_INPUT_LEN + 2];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else if (len >= MAX_INPUT_LEN) {
        printf("Input too long\n");
        return 1;
    }
    
    init_machine(&sm);
    process_input(&sm, input);
    
    if (sm.current == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", sm.number_count);
    printf("Words found: %d\n", sm.word_count);
    
    return 0;
}