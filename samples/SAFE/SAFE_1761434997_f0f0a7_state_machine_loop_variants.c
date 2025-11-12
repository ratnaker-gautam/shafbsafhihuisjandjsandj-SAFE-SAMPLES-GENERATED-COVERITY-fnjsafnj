//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_WORD, ERROR, DONE };

struct StateMachine {
    enum State current;
    int char_count;
    int word_count;
    int num_count;
};

void init_machine(struct StateMachine *sm) {
    sm->current = START;
    sm->char_count = 0;
    sm->word_count = 0;
    sm->num_count = 0;
}

enum State handle_start(char c, struct StateMachine *sm) {
    if (isspace(c)) {
        return START;
    } else if (isdigit(c)) {
        sm->num_count++;
        return READING_NUM;
    } else if (isalpha(c)) {
        sm->word_count++;
        return READING_WORD;
    } else {
        return ERROR;
    }
}

enum State handle_reading_num(char c, struct StateMachine *sm) {
    if (isspace(c)) {
        return START;
    } else if (isdigit(c)) {
        return READING_NUM;
    } else if (isalpha(c)) {
        return ERROR;
    } else {
        return ERROR;
    }
}

enum State handle_reading_word(char c, struct StateMachine *sm) {
    if (isspace(c)) {
        return START;
    } else if (isalpha(c)) {
        return READING_WORD;
    } else if (isdigit(c)) {
        return ERROR;
    } else {
        return ERROR;
    }
}

enum State process_char(char c, struct StateMachine *sm) {
    sm->char_count++;
    
    switch (sm->current) {
        case START:
            return handle_start(c, sm);
        case READING_NUM:
            return handle_reading_num(c, sm);
        case READING_WORD:
            return handle_reading_word(c, sm);
        case ERROR:
            return ERROR;
        case DONE:
            return DONE;
    }
    return ERROR;
}

int main(void) {
    struct StateMachine sm;
    init_machine(&sm);
    
    printf("Enter text (max 1000 chars, Ctrl+D to end): ");
    
    int c;
    while ((c = getchar()) != EOF && sm.current != ERROR) {
        if (sm.char_count >= 1000) {
            sm.current = ERROR;
            break;
        }
        
        sm.current = process_char((char)c, &sm);
        
        if (sm.current == ERROR) {
            break;
        }
    }
    
    if (sm.current == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Characters processed: %d\n", sm.char_count);
    printf("Words found: %d\n", sm.word_count);
    printf("Numbers found: %d\n", sm.num_count);
    
    return 0;
}