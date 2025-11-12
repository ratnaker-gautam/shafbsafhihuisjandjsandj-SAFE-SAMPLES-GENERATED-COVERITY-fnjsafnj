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

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
};

struct StateMachine {
    enum State current_state;
    uint32_t data_buffer[16];
    size_t data_count;
    uint32_t processed_data[16];
    size_t processed_count;
    uint32_t error_code;
};

struct InputData {
    char input_line[256];
    size_t input_length;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->processed_count = 0;
    sm->error_code = 0;
    memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
    memset(sm->processed_data, 0, sizeof(sm->processed_data));
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isdigit(input[i]) && input[i] != ' ' && input[i] != '\n') return 0;
    }
    return 1;
}

int parse_numbers(const char *input, size_t len, uint32_t *buffer, size_t buffer_size, size_t *count) {
    if (input == NULL || buffer == NULL || count == NULL) return 0;
    *count = 0;
    char temp[256];
    if (len >= sizeof(temp)) return 0;
    memcpy(temp, input, len);
    temp[len] = '\0';
    
    char *token = strtok(temp, " ");
    while (token != NULL && *count < buffer_size) {
        if (strlen(token) > 10) return 0;
        char *endptr;
        unsigned long val = strtoul(token, &endptr, 10);
        if (*endptr != '\0' || val > UINT32_MAX) return 0;
        buffer[*count] = (uint32_t)val;
        (*count)++;
        token = strtok(NULL, " ");
    }
    return 1;
}

int process_data(struct StateMachine *sm) {
    if (sm == NULL || sm->data_count == 0) return 0;
    if (sm->data_count > 16) return 0;
    sm->processed_count = 0;
    for (size_t i = 0; i < sm->data_count; i++) {
        if (sm->data_buffer[i] > 1000000) return 0;
        if (sm->data_buffer[i] > UINT32_MAX / 2) return 0;
        sm->processed_data[i] = sm->data_buffer[i] * 2;
        sm->processed_count++;
    }
    return 1;
}

void handle_event(struct StateMachine *sm, enum Event event, const struct InputData *input) {
    if (sm == NULL) return;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm->current_state = STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA_READY && input != NULL) {
                if (validate_input(input->input_line, input->input_length)) {
                    if (parse_numbers(input->input_line, input->input_length, 
                                    sm->data_buffer, 16, &sm->data_count)) {
                        sm->current_state = STATE_PROCESSING;
                    } else {
                        sm->error_code = 2;
                        sm->current_state = STATE_ERROR;
                    }
                } else {
                    sm->error_code = 1;
                    sm->current_state = STATE_ERROR;
                }
            } else if (event == EVENT_ERROR) {
                sm->error_code = 3;
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_PROCESS_COMPLETE) {
                if (process_data(sm)) {
                    sm->current_state = STATE_WRITING;
                } else {
                    sm->error_code = 4;
                    sm->current_state = STATE_ERROR;
                }
            } else if (event == EVENT_ERROR) {
                sm->error_code = 5;
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_WRITING:
            if (event == EVENT_WRITE_COMPLETE) {
                sm->current_state = STATE_DONE;
            } else if (event == EVENT_ERROR) {
                sm->error_code = 6;
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                state_machine_init(sm);
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                state_machine_init(sm);
            }
            break;
    }
}

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    printf