//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

struct StateMachine {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int processed_count;
    int error_code;
};

struct InputData {
    char data[512];
    size_t length;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->processed_count = 0;
    sm->error_code = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int validate_input(const struct InputData *input) {
    if (input == NULL) return 0;
    if (input->length == 0) return 0;
    if (input->length >= sizeof(input->data)) return 0;
    return 1;
}

void transition_state(struct StateMachine *sm, enum State new_state) {
    if (sm == NULL) return;
    sm->current_state = new_state;
}

int process_buffer(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_pos == 0) return 0;
    
    for (size_t i = 0; i < sm->buffer_pos; i++) {
        if (!isalnum((unsigned char)sm->buffer[i]) && sm->buffer[i] != ' ') {
            sm->error_code = 1;
            return 0;
        }
    }
    
    sm->processed_count++;
    return 1;
}

int write_output(const struct StateMachine *sm) {
    if (sm == NULL) return 0;
    printf("Processed buffer: %s\n", sm->buffer);
    printf("Total processed: %d\n", sm->processed_count);
    return 1;
}

void handle_idle(struct StateMachine *sm, const struct InputData *input) {
    if (sm == NULL || input == NULL) return;
    
    if (validate_input(input)) {
        sm->buffer_pos = 0;
        if (input->length < sizeof(sm->buffer)) {
            memcpy(sm->buffer, input->data, input->length);
            sm->buffer_pos = input->length;
            sm->buffer[sm->buffer_pos] = '\0';
            transition_state(sm, STATE_READING);
        } else {
            sm->error_code = 2;
            transition_state(sm, STATE_ERROR);
        }
    } else {
        sm->error_code = 3;
        transition_state(sm, STATE_ERROR);
    }
}

void handle_reading(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    if (sm->buffer_pos > 0) {
        transition_state(sm, STATE_PROCESSING);
    } else {
        transition_state(sm, STATE_DONE);
    }
}

void handle_processing(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    if (process_buffer(sm)) {
        transition_state(sm, STATE_WRITING);
    } else {
        transition_state(sm, STATE_ERROR);
    }
}

void handle_writing(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    if (write_output(sm)) {
        transition_state(sm, STATE_IDLE);
    } else {
        transition_state(sm, STATE_ERROR);
    }
}

void handle_error(struct StateMachine *sm) {
    if (sm == NULL) return;
    printf("Error occurred: code %d\n", sm->error_code);
    transition_state(sm, STATE_DONE);
}

void run_state_machine(struct StateMachine *sm, const struct InputData *input) {
    if (sm == NULL || input == NULL) return;
    
    while (sm->current_state != STATE_DONE && sm->current_state != STATE_ERROR) {
        switch (sm->current_state) {
            case STATE_IDLE:
                handle_idle(sm, input);
                break;
            case STATE_READING:
                handle_reading(sm);
                break;
            case STATE_PROCESSING:
                handle_processing(sm);
                break;
            case STATE_WRITING:
                handle_writing(sm);
                break;
            default:
                sm->error_code = 4;
                transition_state(sm, STATE_ERROR);
                break;
        }
    }
    
    if (sm->current_state == STATE_ERROR) {
        handle_error(sm);
    }
}

int main(void) {
    struct StateMachine sm;
    struct InputData input;
    
    state_machine_init(&sm);
    
    printf("Enter text to process (max 511 characters): ");
    if (fgets(input.data, sizeof(input.data), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        return 1;
    }
    
    input.length = strlen(input.data);
    if (input.length > 0 && input.data[input.length - 1] == '\n') {
        input.data[input.length - 1] = '\0';
        input.length--;
    }
    
    run_state_machine(&sm, &input);
    
    if (sm.current_state == STATE_DONE) {
        printf("Processing completed successfully\n");
    } else {
        printf("Processing failed\n");
    }
    
    return 0;
}