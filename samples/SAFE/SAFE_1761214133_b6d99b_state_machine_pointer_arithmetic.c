//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    FINISHED
} State;

typedef struct {
    State current;
    char *input;
    char *pos;
    int number_count;
    int word_count;
} StateMachine;

void init_machine(StateMachine *sm, char *input) {
    sm->current = START;
    sm->input = input;
    sm->pos = input;
    sm->number_count = 0;
    sm->word_count = 0;
}

int is_delimiter(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

State process_start(StateMachine *sm) {
    while (*sm->pos && isspace(*sm->pos)) {
        sm->pos++;
    }
    
    if (!*sm->pos) {
        return FINISHED;
    }
    
    if (isdigit(*sm->pos)) {
        return READING_NUMBER;
    } else if (isalpha(*sm->pos)) {
        return READING_WORD;
    }
    
    return ERROR;
}

State process_number(StateMachine *sm) {
    while (*sm->pos && isdigit(*sm->pos)) {
        sm->pos++;
    }
    
    sm->number_count++;
    
    if (!*sm->pos) {
        return FINISHED;
    }
    
    if (is_delimiter(*sm->pos)) {
        return START;
    }
    
    return ERROR;
}

State process_word(StateMachine *sm) {
    while (*sm->pos && isalpha(*sm->pos)) {
        sm->pos++;
    }
    
    sm->word_count++;
    
    if (!*sm->pos) {
        return FINISHED;
    }
    
    if (is_delimiter(*sm->pos)) {
        return START;
    }
    
    return ERROR;
}

void run_state_machine(StateMachine *sm) {
    while (sm->current != FINISHED && sm->current != ERROR) {
        switch (sm->current) {
            case START:
                sm->current = process_start(sm);
                break;
            case READING_NUMBER:
                sm->current = process_number(sm);
                break;
            case READING_WORD:
                sm->current = process_word(sm);
                break;
            default:
                sm->current = ERROR;
                break;
        }
    }
}

int main(void) {
    char input_buffer[MAX_INPUT + 1];
    StateMachine sm;
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len >= sizeof(input_buffer)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    init_machine(&sm, input_buffer);
    run_state_machine(&sm);
    
    if (sm.current == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", sm.number_count);
    printf("Words found: %d\n", sm.word_count);
    
    return 0;
}