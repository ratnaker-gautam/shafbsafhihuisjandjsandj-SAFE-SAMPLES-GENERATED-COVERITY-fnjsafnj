//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_COUNT
};

struct StateMachine {
    enum State current_state;
    uint32_t data_value;
    char buffer[64];
    size_t buffer_pos;
    uint32_t processed_count;
};

struct StateTransition {
    enum State from_state;
    enum State to_state;
    int (*condition)(struct StateMachine*);
    void (*action)(struct StateMachine*);
};

void state_idle_action(struct StateMachine* sm) {
    printf("Entering idle state\n");
    sm->data_value = 0;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

void state_reading_action(struct StateMachine* sm) {
    printf("Entering reading state\n");
    printf("Enter a number (0-1000): ");
    
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current_state = STATE_ERROR;
        return;
    }
    
    if (strlen(input) == 0 || input[0] == '\n') {
        sm->current_state = STATE_IDLE;
        return;
    }
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        sm->current_state = STATE_ERROR;
        return;
    }
    
    if (value < 0 || value > 1000) {
        sm->current_state = STATE_ERROR;
        return;
    }
    
    sm->data_value = (uint32_t)value;
    sm->current_state = STATE_PROCESSING;
}

void state_processing_action(struct StateMachine* sm) {
    printf("Entering processing state\n");
    
    if (sm->data_value == 0) {
        sm->current_state = STATE_IDLE;
        return;
    }
    
    sm->processed_count++;
    uint32_t processed_value = sm->data_value * 2;
    
    if (processed_value < sm->data_value) {
        sm->current_state = STATE_ERROR;
        return;
    }
    
    int written = snprintf(sm->buffer, sizeof(sm->buffer), "Processed: %u", processed_value);
    if (written < 0 || (size_t)written >= sizeof(sm->buffer)) {
        sm->current_state = STATE_ERROR;
        return;
    }
    
    sm->buffer_pos = (size_t)written;
    sm->current_state = STATE_WRITING;
}

void state_writing_action(struct StateMachine* sm) {
    printf("Entering writing state\n");
    
    if (sm->buffer_pos == 0 || sm->buffer_pos >= sizeof(sm->buffer)) {
        sm->current_state = STATE_ERROR;
        return;
    }
    
    printf("%s\n", sm->buffer);
    sm->current_state = STATE_IDLE;
}

void state_error_action(struct StateMachine* sm) {
    printf("Entering error state\n");
    printf("Error occurred - resetting\n");
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int always_true(struct StateMachine* sm) {
    return 1;
}

int has_data(struct StateMachine* sm) {
    return sm->data_value > 0;
}

int no_data(struct StateMachine* sm) {
    return sm->data_value == 0;
}

struct StateTransition transitions[] = {
    {STATE_IDLE, STATE_READING, always_true, state_idle_action},
    {STATE_READING, STATE_PROCESSING, has_data, state_reading_action},
    {STATE_READING, STATE_IDLE, no_data, state_reading_action},
    {STATE_PROCESSING, STATE_WRITING, always_true, state_processing_action},
    {STATE_WRITING, STATE_IDLE, always_true, state_writing_action},
    {STATE_ERROR, STATE_IDLE, always_true, state_error_action}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void initialize_state_machine(struct StateMachine* sm) {
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->buffer_pos = 0;
    sm->processed_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

void run_state_machine(struct StateMachine* sm) {
    int max_iterations = 100;
    
    while (max_iterations-- > 0) {
        int transition_found = 0;
        
        for (size_t i = 0; i < num_transitions; i++) {
            if (transitions[i].from_state == sm->current_state) {
                if (transitions[i].condition(sm)) {
                    transitions[i].action(sm);
                    transition_found = 1;
                    break;
                }
            }
        }
        
        if (!transition_found) {
            sm->current_state = STATE_ERROR;
        }
        
        if (sm->current_state == STATE_IDLE) {
            printf("Return to idle? (y/n): ");
            char response[8];
            if (fgets(response, sizeof(response), stdin) == NULL) {
                break;