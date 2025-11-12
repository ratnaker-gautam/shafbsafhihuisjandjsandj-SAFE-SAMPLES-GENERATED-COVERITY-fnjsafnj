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
#define MAX_BUFFER 50

#define TRANSITION(sm, new_state) do { (sm)->state = new_state; } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char buffer[MAX_BUFFER];
    size_t buf_len;
    int error_code;
} StateMachine;

void init_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->state = STATE_IDLE;
    sm->buf_len = 0;
    sm->error_code = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int handle_idle(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) {
        sm->error_code = 1;
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    
    if (input_len > 0) {
        TRANSITION(sm, STATE_READING);
        return 1;
    }
    return 0;
}

int handle_reading(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) {
        sm->error_code = 2;
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    
    if (sm->buf_len + input_len >= MAX_BUFFER) {
        sm->error_code = 3;
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    
    memcpy(sm->buffer + sm->buf_len, input, input_len);
    sm->buf_len += input_len;
    
    if (sm->buf_len >= 10) {
        TRANSITION(sm, STATE_PROCESSING);
        return 1;
    }
    
    TRANSITION(sm, STATE_READING);
    return 1;
}

int handle_processing(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    if (sm->buf_len == 0) {
        sm->error_code = 4;
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    
    for (size_t i = 0; i < sm->buf_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    TRANSITION(sm, STATE_WRITING);
    return 1;
}

int handle_writing(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    if (sm->buf_len == 0) {
        sm->error_code = 5;
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    
    if (fwrite(sm->buffer, 1, sm->buf_len, stdout) != sm->buf_len) {
        sm->error_code = 6;
        TRANSITION(sm, STATE_ERROR);
        return 0;
    }
    
    printf("\n");
    sm->buf_len = 0;
    TRANSITION(sm, STATE_IDLE);
    return 1;
}

void handle_error(StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *error_msg = "Unknown error";
    switch (sm->error_code) {
        case 1: error_msg = "Input too long"; break;
        case 2: error_msg = "Invalid input length"; break;
        case 3: error_msg = "Buffer overflow"; break;
        case 4: error_msg = "Empty buffer"; break;
        case 5: error_msg = "No data to write"; break;
        case 6: error_msg = "Write failed"; break;
    }
    
    fprintf(stderr, "Error %d: %s\n", sm->error_code, error_msg);
    TRANSITION(sm, STATE_IDLE);
    sm->error_code = 0;
}

int process_input(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return 0;
    
    int result = 0;
    switch (sm->state) {
        case STATE_IDLE:
            result = handle_idle(sm, input);
            break;
        case STATE_READING:
            result = handle_reading(sm, input);
            break;
        case STATE_PROCESSING:
            result = handle_processing(sm);
            break;
        case STATE_WRITING:
            result = handle_writing(sm);
            break;
        case STATE_ERROR:
            handle_error(sm);
            break;
        default:
            sm->error_code = 7;
            TRANSITION(sm, STATE_ERROR);
            break;
    }
    
    return result;
}

int main(void) {
    StateMachine sm;
    init_machine(&sm);
    
    char input[MAX_INPUT_LEN + 1