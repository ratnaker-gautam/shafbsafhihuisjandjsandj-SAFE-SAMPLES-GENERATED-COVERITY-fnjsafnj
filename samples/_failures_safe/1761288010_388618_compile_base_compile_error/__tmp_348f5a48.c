//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
};

struct Context {
    uint32_t counter;
    char buffer[256];
    size_t buffer_len;
};

struct StateMachine {
    enum State current_state;
    struct Context context;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->context.counter = 0;
    sm->context.buffer_len = 0;
    memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
}

int state_machine_validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0) return 0;
    if (len >= 256) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

enum State state_idle_handler(struct StateMachine *sm, const char *input, size_t len) {
    if (sm == NULL || !state_machine_validate_input(input, len)) return STATE_ERROR;
    if (len > 0 && input[0] == 'S') {
        if (len - 1 < sizeof(sm->context.buffer)) {
            memcpy(sm->context.buffer, input + 1, len - 1);
            sm->context.buffer_len = len - 1;
            sm->context.buffer[sm->context.buffer_len] = '\0';
            return STATE_READY;
        }
    }
    return STATE_IDLE;
}

enum State state_ready_handler(struct StateMachine *sm, const char *input, size_t len) {
    if (sm == NULL || !state_machine_validate_input(input, len)) return STATE_ERROR;
    if (len == 1 && input[0] == 'G') {
        return STATE_PROCESSING;
    }
    return STATE_READY;
}

enum State state_processing_handler(struct StateMachine *sm, const char *input, size_t len) {
    if (sm == NULL || !state_machine_validate_input(input, len)) return STATE_ERROR;
    if (len == 1 && input[0] == 'C') {
        for (size_t i = 0; i < sm->context.buffer_len; i++) {
            if (sm->context.counter < UINT32_MAX) {
                sm->context.counter++;
            }
        }
        return STATE_COMPLETE;
    }
    return STATE_PROCESSING;
}

enum State state_complete_handler(struct StateMachine *sm, const char *input, size_t len) {
    if (sm == NULL || !state_machine_validate_input(input, len)) return STATE_ERROR;
    if (len == 1 && input[0] == 'R') {
        state_machine_init(sm);
        return STATE_IDLE;
    }
    return STATE_COMPLETE;
}

enum State state_error_handler(struct StateMachine *sm, const char *input, size_t len) {
    if (sm == NULL || !state_machine_validate_input(input, len)) return STATE_ERROR;
    if (len == 1 && input[0] == 'R') {
        state_machine_init(sm);
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

void state_machine_step(struct StateMachine *sm, const char *input, size_t len) {
    if (sm == NULL || input == NULL) return;
    
    enum State next_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            next_state = state_idle_handler(sm, input, len);
            break;
        case STATE_READY:
            next_state = state_ready_handler(sm, input, len);
            break;
        case STATE_PROCESSING:
            next_state = state_processing_handler(sm, input, len);
            break;
        case STATE_COMPLETE:
            next_state = state_complete_handler(sm, input, len);
            break;
        case STATE_ERROR:
            next_state = state_error_handler(sm, input, len);
            break;
        default:
            next_state = STATE_ERROR;
            break;
    }
    
    sm->current_state = next_state;
}

void state_machine_print_status(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("State: %s | Counter: %u | Buffer: ", 
           state_names[sm->current_state], sm->context.counter);
    
    for (size_t i = 0; i < sm->context.buffer_len && i < sizeof(sm->context.buffer); i++) {
        putchar(sm->context.buffer[i]);
    }
    printf("\n");
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    char input[512];
    size_t input_len;
    
    printf("State Machine Controller\n");
    printf("Commands: S<text> (Start), G (Go), C (Complete), R (Reset)\n");
    printf("Enter 'quit' to exit\n");