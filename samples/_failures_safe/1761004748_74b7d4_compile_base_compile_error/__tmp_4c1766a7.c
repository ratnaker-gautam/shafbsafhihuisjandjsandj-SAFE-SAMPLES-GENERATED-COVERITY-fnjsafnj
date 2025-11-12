//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
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

#define TRANSITION_OK 0
#define TRANSITION_ERROR -1

#define CHECK_BOUNDS(val, max) ((val) >= 0 && (val) < (max))

typedef struct {
    int current_state;
    char buffer[MAX_BUFFER];
    size_t buffer_len;
    int error_code;
} StateMachine;

static int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 0;
    }
    return 1;
}

static int transition_state(StateMachine* sm, int new_state) {
    if (sm == NULL) return TRANSITION_ERROR;
    if (!CHECK_BOUNDS(new_state, 5)) return TRANSITION_ERROR;
    
    sm->current_state = new_state;
    return TRANSITION_OK;
}

static int process_idle(StateMachine* sm, const char* input, size_t len) {
    if (sm == NULL || input == NULL) return TRANSITION_ERROR;
    if (!validate_input(input, len)) return TRANSITION_ERROR;
    
    if (len > 0) {
        if (len >= MAX_BUFFER) return TRANSITION_ERROR;
        memcpy(sm->buffer, input, len);
        sm->buffer_len = len;
        sm->buffer[sm->buffer_len] = '\0';
        return transition_state(sm, STATE_READING);
    }
    return TRANSITION_OK;
}

static int process_reading(StateMachine* sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    
    if (sm->buffer_len == 0) {
        return transition_state(sm, STATE_ERROR);
    }
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] < 32 || sm->buffer[i] > 126) {
            sm->error_code = 1;
            return transition_state(sm, STATE_ERROR);
        }
    }
    
    return transition_state(sm, STATE_PROCESSING);
}

static int process_processing(StateMachine* sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    return transition_state(sm, STATE_WRITING);
}

static int process_writing(StateMachine* sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    
    if (sm->buffer_len > 0) {
        if (fwrite(sm->buffer, 1, sm->buffer_len, stdout) != sm->buffer_len) {
            return transition_state(sm, STATE_ERROR);
        }
        printf("\n");
    }
    
    sm->buffer_len = 0;
    return transition_state(sm, STATE_IDLE);
}

static int process_error(StateMachine* sm) {
    if (sm == NULL) return TRANSITION_ERROR;
    
    const char* error_msg = "Error: Invalid input detected\n";
    fwrite(error_msg, 1, strlen(error_msg), stdout);
    sm->buffer_len = 0;
    sm->error_code = 0;
    
    return transition_state(sm, STATE_IDLE);
}

static int run_state_machine(StateMachine* sm, const char* input, size_t len) {
    if (sm == NULL || input == NULL) return TRANSITION_ERROR;
    if (!validate_input(input, len)) return TRANSITION_ERROR;
    
    int result = TRANSITION_OK;
    
    while (sm->current_state != STATE_IDLE && result == TRANSITION_OK) {
        switch (sm->current_state) {
            case STATE_IDLE:
                result = process_idle(sm, input, len);
                break;
            case STATE_READING:
                result = process_reading(sm);
                break;
            case STATE_PROCESSING:
                result = process_processing(sm);
                break;
            case STATE_WRITING:
                result = process_writing(sm);
                break;
            case STATE_ERROR:
                result = process_error(sm);
                break;
            default:
                result = TRANSITION_ERROR;
                break;
        }
        
        if (result != TRANSITION_OK) {
            transition_state(sm, STATE_ERROR);
        }
    }
    
    return result;
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.buffer_len = 0;
    sm.error_code = 0;
    
    char input[MAX_INPUT_LEN];
    
    printf("Enter text (empty line to exit): ");
    fflush(stdout);
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }