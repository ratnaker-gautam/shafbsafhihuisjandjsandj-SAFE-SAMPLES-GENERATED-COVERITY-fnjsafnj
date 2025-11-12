//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    IDLE,
    READING,
    PROCESSING,
    WRITING,
    ERROR,
    DONE
};

struct DataBuffer {
    char data[256];
    size_t length;
};

struct StateMachine {
    enum State current_state;
    struct DataBuffer buffer;
    int error_code;
    int processed_count;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = IDLE;
    sm->buffer.length = 0;
    sm->error_code = 0;
    sm->processed_count = 0;
    memset(sm->buffer.data, 0, sizeof(sm->buffer.data));
}

int validate_input(const char *input, size_t max_len) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= max_len) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

enum State handle_idle(struct StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return ERROR;
    if (!validate_input(input, sizeof(sm->buffer.data) - 1)) return ERROR;
    
    size_t len = strlen(input);
    if (len >= sizeof(sm->buffer.data)) return ERROR;
    
    memcpy(sm->buffer.data, input, len);
    sm->buffer.data[len] = '\0';
    sm->buffer.length = len;
    return READING;
}

enum State handle_reading(struct StateMachine *sm) {
    if (sm == NULL) return ERROR;
    if (sm->buffer.length == 0) return ERROR;
    
    for (size_t i = 0; i < sm->buffer.length; i++) {
        if (sm->buffer.data[i] >= 'a' && sm->buffer.data[i] <= 'z') {
            sm->buffer.data[i] = sm->buffer.data[i] - 'a' + 'A';
        }
    }
    sm->processed_count++;
    return PROCESSING;
}

enum State handle_processing(struct StateMachine *sm) {
    if (sm == NULL) return ERROR;
    if (sm->buffer.length == 0) return ERROR;
    
    size_t new_len = 0;
    for (size_t i = 0; i < sm->buffer.length; i++) {
        if (sm->buffer.data[i] != ' ') {
            sm->buffer.data[new_len++] = sm->buffer.data[i];
        }
    }
    sm->buffer.data[new_len] = '\0';
    sm->buffer.length = new_len;
    return WRITING;
}

enum State handle_writing(struct StateMachine *sm) {
    if (sm == NULL) return ERROR;
    if (sm->buffer.length == 0) return ERROR;
    
    printf("Processed: %s\n", sm->buffer.data);
    sm->buffer.length = 0;
    memset(sm->buffer.data, 0, sizeof(sm->buffer.data));
    
    if (sm->processed_count >= 3) {
        return DONE;
    }
    return IDLE;
}

enum State handle_error(struct StateMachine *sm) {
    if (sm == NULL) return ERROR;
    sm->error_code = 1;
    printf("Error occurred in state machine\n");
    return ERROR;
}

void run_state_machine(struct StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) return;
    
    switch (sm->current_state) {
        case IDLE:
            sm->current_state = handle_idle(sm, input);
            break;
        case READING:
            sm->current_state = handle_reading(sm);
            break;
        case PROCESSING:
            sm->current_state = handle_processing(sm);
            break;
        case WRITING:
            sm->current_state = handle_writing(sm);
            break;
        case ERROR:
            sm->current_state = handle_error(sm);
            break;
        case DONE:
            break;
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    const char *test_inputs[] = {
        "hello world",
        "state machine",
        "final input",
        "extra input"
    };
    size_t num_inputs = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    for (size_t i = 0; i < num_inputs && sm.current_state != DONE && sm.current_state != ERROR; i++) {
        run_state_machine(&sm, test_inputs[i]);
        
        while (sm.current_state != IDLE && sm.current_state != DONE && sm.current_state != ERROR) {
            run_state_machine(&sm, "");
        }
    }
    
    printf("Final state: %d, Processed count: %d\n", sm.current_state, sm.processed_count);
    
    return sm.error_code;
}