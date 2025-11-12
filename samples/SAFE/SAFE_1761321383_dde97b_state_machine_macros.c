//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_WRITING 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_BUFFER_SIZE 1024

#define TRANSITION(sm, new_state) do { (sm)->state = new_state; } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')

typedef struct {
    int state;
    char buffer[MAX_BUFFER_SIZE];
    size_t buffer_len;
    int processed_count;
} StateMachine;

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->buffer_len = 0;
    sm->processed_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_idle(StateMachine *sm) {
    if (sm == NULL) return -1;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    char input[MAX_INPUT_LEN + 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(sm, STATE_ERROR);
        return -1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) {
        TRANSITION(sm, STATE_ERROR);
        return -1;
    }
    
    if (input_len == 0) {
        return 0;
    }
    
    if (!CHECK_BOUNDS(input_len, sizeof(sm->buffer))) {
        TRANSITION(sm, STATE_ERROR);
        return -1;
    }
    
    memcpy(sm->buffer, input, input_len);
    sm->buffer[input_len] = '\0';
    sm->buffer_len = input_len;
    TRANSITION(sm, STATE_READING);
    return 0;
}

int state_reading(StateMachine *sm) {
    if (sm == NULL) return -1;
    
    sm->processed_count = 0;
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (IS_DIGIT(sm->buffer[i])) {
            sm->processed_count++;
        }
    }
    
    TRANSITION(sm, STATE_PROCESSING);
    return 0;
}

int state_processing(StateMachine *sm) {
    if (sm == NULL) return -1;
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (IS_DIGIT(sm->buffer[i])) {
            sm->buffer[i] = 'X';
        }
    }
    
    TRANSITION(sm, STATE_WRITING);
    return 0;
}

int state_writing(StateMachine *sm) {
    if (sm == NULL) return -1;
    
    printf("Processed text: %s\n", sm->buffer);
    printf("Digits replaced: %d\n", sm->processed_count);
    
    TRANSITION(sm, STATE_IDLE);
    return 0;
}

int state_error(StateMachine *sm) {
    if (sm == NULL) return -1;
    
    printf("Error: Invalid input or buffer overflow\n");
    state_machine_init(sm);
    return 0;
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    printf("State Machine Demo - Replace digits with 'X'\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        switch (sm.state) {
            case STATE_IDLE:
                if (state_idle(&sm) != 0) break;
                break;
            case STATE_READING:
                if (state_reading(&sm) != 0) break;
                break;
            case STATE_PROCESSING:
                if (state_processing(&sm) != 0) break;
                break;
            case STATE_WRITING:
                if (state_writing(&sm) != 0) break;
                break;
            case STATE_ERROR:
                if (state_error(&sm) != 0) break;
                break;
            default:
                sm.state = STATE_ERROR;
                break;
        }
        
        if (sm.state == STATE_IDLE && sm.buffer_len == 4 && 
            strncmp(sm.buffer, "quit", 4) == 0) {
            break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}