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
    int error_code;
    int processed_count;
};

struct InputData {
    char data[256];
    size_t length;
};

struct OutputData {
    char result[512];
    size_t length;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->error_code = 0;
    sm->processed_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int validate_input(const struct InputData *input) {
    if (input == NULL) return 0;
    if (input->length == 0) return 0;
    if (input->length >= sizeof(input->data)) return 0;
    return 1;
}

void process_data(const struct InputData *input, struct OutputData *output) {
    if (input == NULL || output == NULL) return;
    
    output->length = 0;
    memset(output->result, 0, sizeof(output->result));
    
    for (size_t i = 0; i < input->length; i++) {
        if (output->length >= sizeof(output->result) - 1) break;
        char c = input->data[i];
        if (isalnum((unsigned char)c) || c == ' ') {
            output->result[output->length++] = c;
        }
    }
    output->result[output->length] = '\0';
}

int state_machine_transition(struct StateMachine *sm, const struct InputData *input, struct OutputData *output) {
    if (sm == NULL) return 0;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (input != NULL && validate_input(input)) {
                sm->current_state = STATE_READING;
                strncpy(sm->buffer, input->data, sizeof(sm->buffer) - 1);
                sm->buffer[sizeof(sm->buffer) - 1] = '\0';
                sm->buffer_pos = input->length;
            } else {
                sm->current_state = STATE_ERROR;
                sm->error_code = 1;
            }
            break;
            
        case STATE_READING:
            if (sm->buffer_pos > 0) {
                sm->current_state = STATE_PROCESSING;
            } else {
                sm->current_state = STATE_ERROR;
                sm->error_code = 2;
            }
            break;
            
        case STATE_PROCESSING:
            {
                struct InputData process_input;
                strncpy(process_input.data, sm->buffer, sizeof(process_input.data) - 1);
                process_input.data[sizeof(process_input.data) - 1] = '\0';
                process_input.length = sm->buffer_pos;
                
                process_data(&process_input, output);
                sm->processed_count++;
                sm->current_state = STATE_WRITING;
            }
            break;
            
        case STATE_WRITING:
            if (output != NULL && output->length > 0) {
                sm->current_state = STATE_DONE;
            } else {
                sm->current_state = STATE_ERROR;
                sm->error_code = 3;
            }
            break;
            
        case STATE_ERROR:
            break;
            
        case STATE_DONE:
            break;
    }
    
    return 1;
}

void print_state_info(const struct StateMachine *sm, const struct OutputData *output) {
    if (sm == NULL) return;
    
    printf("Current state: ");
    switch (sm->current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_WRITING: printf("WRITING"); break;
        case STATE_ERROR: printf("ERROR"); break;
        case STATE_DONE: printf("DONE"); break;
    }
    printf("\n");
    
    if (sm->current_state == STATE_ERROR) {
        printf("Error code: %d\n", sm->error_code);
    }
    
    if (output != NULL && output->length > 0 && sm->current_state >= STATE_WRITING) {
        printf("Processed output: %s\n", output->result);
    }
    
    printf("Processed count: %d\n", sm->processed_count);
}

int main(void) {
    struct StateMachine sm;
    struct InputData input;
    struct OutputData output;
    
    state_machine_init(&sm);
    
    printf("Enter text to process (max 255 characters): ");
    if (fgets(input.data, sizeof(input.data), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input.length = strlen(input.data);
    if (input.length > 0 && input.data[input.length - 1] == '\n') {
        input.data[input.length - 1] = '\0';
        input.length--;
    }
    
    while (sm.current_state != STATE_DONE && sm.current_state != STATE_ERROR) {
        if (!state_machine_transition