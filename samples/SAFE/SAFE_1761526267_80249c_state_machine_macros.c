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
#define MAX_BUFFER_SIZE 200

#define TRANSITION(sm, new_state) do { (sm)->state = (new_state); } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char buffer[MAX_BUFFER_SIZE];
    size_t buffer_len;
    int error_code;
} state_machine_t;

static void state_machine_init(state_machine_t *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->buffer_len = 0;
    sm->error_code = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

static int state_idle(state_machine_t *sm) {
    if (sm == NULL) return -1;
    
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN);
    
    char input[MAX_INPUT_LEN + 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->error_code = 1;
        TRANSITION(sm, STATE_ERROR);
        return -1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) {
        sm->error_code = 2;
        TRANSITION(sm, STATE_ERROR);
        return -1;
    }
    
    if (input_len == 0) {
        return 0;
    }
    
    if (!CHECK_BOUNDS(input_len, MAX_BUFFER_SIZE)) {
        sm->error_code = 3;
        TRANSITION(sm, STATE_ERROR);
        return -1;
    }
    
    memcpy(sm->buffer, input, input_len);
    sm->buffer_len = input_len;
    TRANSITION(sm, STATE_READING);
    return 0;
}

static int state_reading(state_machine_t *sm) {
    if (sm == NULL) return -1;
    
    printf("Read %zu characters: '%.*s'\n", sm->buffer_len, (int)sm->buffer_len, sm->buffer);
    TRANSITION(sm, STATE_PROCESSING);
    return 0;
}

static int state_processing(state_machine_t *sm) {
    if (sm == NULL) return -1;
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    printf("Processed text: '%.*s'\n", (int)sm->buffer_len, sm->buffer);
    TRANSITION(sm, STATE_WRITING);
    return 0;
}

static int state_writing(state_machine_t *sm) {
    if (sm == NULL) return -1;
    
    printf("Writing completed. Result: '%.*s'\n", (int)sm->buffer_len, sm->buffer);
    TRANSITION(sm, STATE_IDLE);
    return 0;
}

static int state_error(state_machine_t *sm) {
    if (sm == NULL) return -1;
    
    printf("Error %d occurred. Resetting state machine.\n", sm->error_code);
    state_machine_init(sm);
    return 0;
}

static int state_machine_step(state_machine_t *sm) {
    if (sm == NULL) return -1;
    
    switch (sm->state) {
        case STATE_IDLE:
            return state_idle(sm);
        case STATE_READING:
            return state_reading(sm);
        case STATE_PROCESSING:
            return state_processing(sm);
        case STATE_WRITING:
            return state_writing(sm);
        case STATE_ERROR:
            return state_error(sm);
        default:
            sm->error_code = 4;
            TRANSITION(sm, STATE_ERROR);
            return -1;
    }
}

int main(void) {
    state_machine_t sm;
    state_machine_init(&sm);
    
    printf("State Machine Demo - Enter text to convert to uppercase\n");
    printf("Enter empty line to exit\n\n");
    
    int iterations = 0;
    const int max_iterations = 10;
    
    while (iterations < max_iterations) {
        if (state_machine_step(&sm) != 0) {
            break;
        }
        
        if (sm.state == STATE_IDLE && sm.buffer_len == 0) {
            break;
        }
        
        iterations++;
    }
    
    if (iterations >= max_iterations) {
        printf("Maximum iterations reached. Exiting.\n");
    } else {
        printf("Normal termination.\n");
    }
    
    return 0;
}