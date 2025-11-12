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

typedef struct {
    int state;
    char buffer[MAX_BUFFER_SIZE];
    size_t buffer_len;
    int error_code;
} StateMachine;

void init_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->buffer_len = 0;
    sm->error_code = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int handle_idle(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return -1;
    
    size_t input_len = strlen(input);
    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) return -1;
    
    if (input_len > 0) {
        if (sm->buffer_len + input_len >= MAX_BUFFER_SIZE) return -1;
        memcpy(sm->buffer + sm->buffer_len, input, input_len);
        sm->buffer_len += input_len;
        TRANSITION(sm, STATE_READING);
        return 0;
    }
    return -1;
}

int handle_reading(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return -1;
    
    size_t input_len = strlen(input);
    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) return -1;
    
    if (input_len == 0) {
        TRANSITION(sm, STATE_PROCESSING);
        return 0;
    }
    
    if (sm->buffer_len + input_len >= MAX_BUFFER_SIZE) return -1;
    memcpy(sm->buffer + sm->buffer_len, input, input_len);
    sm->buffer_len += input_len;
    return 0;
}

int handle_processing(StateMachine *sm) {
    if (sm == NULL) return -1;
    
    if (sm->buffer_len == 0) {
        TRANSITION(sm, STATE_ERROR);
        sm->error_code = 1;
        return -1;
    }
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    TRANSITION(sm, STATE_WRITING);
    return 0;
}

int handle_writing(StateMachine *sm) {
    if (sm == NULL) return -1;
    
    if (sm->buffer_len == 0) {
        TRANSITION(sm, STATE_ERROR);
        sm->error_code = 2;
        return -1;
    }
    
    if (fwrite(sm->buffer, 1, sm->buffer_len, stdout) != sm->buffer_len) {
        TRANSITION(sm, STATE_ERROR);
        sm->error_code = 3;
        return -1;
    }
    
    printf("\n");
    TRANSITION(sm, STATE_IDLE);
    sm->buffer_len = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    return 0;
}

int handle_error(StateMachine *sm) {
    if (sm == NULL) return -1;
    
    const char *error_msg = "Error occurred";
    switch (sm->error_code) {
        case 1: error_msg = "Empty buffer"; break;
        case 2: error_msg = "Nothing to write"; break;
        case 3: error_msg = "Write failed"; break;
        default: error_msg = "Unknown error"; break;
    }
    
    fprintf(stderr, "Error: %s\n", error_msg);
    TRANSITION(sm, STATE_IDLE);
    sm->buffer_len = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->error_code = 0;
    return -1;
}

int process_input(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return -1;
    
    switch (sm->state) {
        case STATE_IDLE:
            return handle_idle(sm, input);
        case STATE_READING:
            return handle_reading(sm, input);
        case STATE_PROCESSING:
            return handle_processing(sm);
        case STATE_WRITING:
            return handle_writing(sm);
        case STATE_ERROR:
            return handle_error(sm);
        default:
            TRANSITION(sm, STATE_ERROR);
            sm->error_code = 4;
            return -1;
    }
}

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LEN + 1];
    
    init_machine(&sm);
    
    printf("State Machine Demo - Enter text lines (empty line to process):\n");
    
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            if (feof(std