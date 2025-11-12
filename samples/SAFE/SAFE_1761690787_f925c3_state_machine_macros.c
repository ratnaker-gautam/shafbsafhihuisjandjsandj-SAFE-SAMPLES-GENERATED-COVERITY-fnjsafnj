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
#define MAX_BUFFER 256

#define TRANSITION(sm, new_state) do { (sm)->state = new_state; } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char buffer[MAX_BUFFER];
    size_t buffer_len;
} StateMachine;

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->buffer_len = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_idle(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) return 0;
    
    if (input_len > 0) {
        if (sm->buffer_len + input_len < MAX_BUFFER) {
            memcpy(sm->buffer + sm->buffer_len, input, input_len);
            sm->buffer_len += input_len;
            TRANSITION(sm, STATE_READING);
            return 1;
        }
    }
    return 0;
}

int state_reading(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) return 0;
    
    if (input_len == 0) {
        TRANSITION(sm, STATE_PROCESSING);
        return 1;
    }
    
    if (sm->buffer_len + input_len < MAX_BUFFER) {
        memcpy(sm->buffer + sm->buffer_len, input, input_len);
        sm->buffer_len += input_len;
        return 1;
    } else {
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
}

int state_processing(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    if (sm->buffer_len == 0) {
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    TRANSITION(sm, STATE_WRITING);
    return 1;
}

int state_writing(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    if (sm->buffer_len > 0) {
        if (fwrite(sm->buffer, 1, sm->buffer_len, stdout) == sm->buffer_len) {
            printf("\n");
            fflush(stdout);
            sm->buffer_len = 0;
            TRANSITION(sm, STATE_IDLE);
            return 1;
        }
    }
    TRANSITION(sm, STATE_ERROR);
    return 0;
}

int state_error(StateMachine *sm) {
    if (sm == NULL) return 0;
    fprintf(stderr, "State machine error occurred\n");
    sm->buffer_len = 0;
    TRANSITION(sm, STATE_IDLE);
    return 0;
}

int process_input(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return 0;
    
    switch(sm->state) {
        case STATE_IDLE:
            return state_idle(sm, input);
        case STATE_READING:
            return state_reading(sm, input);
        case STATE_PROCESSING:
            return state_processing(sm);
        case STATE_WRITING:
            return state_writing(sm);
        case STATE_ERROR:
            return state_error(sm);
        default:
            TRANSITION(sm, STATE_ERROR);
            return 0;
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text lines (empty line to process, 'quit' to exit):\n");
    
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (!process_input(&sm, input)) {
            fprintf(stderr, "Failed to process input\n");
        }
        
        if (sm.state == STATE_PROCESSING) {
            process_input(&sm, "");
        }
    }
    
    if (sm.state == STATE_READING) {
        process_input(&sm, "");
    }
    
    return 0;
}