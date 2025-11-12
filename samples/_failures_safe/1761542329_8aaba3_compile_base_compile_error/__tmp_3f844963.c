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

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
};

struct StateMachine {
    enum State current_state;
    uint32_t data_buffer[16];
    size_t data_count;
    uint32_t processed_data[16];
    size_t processed_count;
    uint32_t error_code;
};

struct Transition {
    enum State current_state;
    enum Event event;
    enum State next_state;
    int (*action)(struct StateMachine*);
};

int action_idle_to_reading(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->data_count = 0;
    sm->processed_count = 0;
    sm->error_code = 0;
    return 0;
}

int action_reading_to_processing(struct StateMachine* sm) {
    if (sm == NULL || sm->data_count == 0 || sm->data_count > 16) return -1;
    for (size_t i = 0; i < sm->data_count; i++) {
        sm->processed_data[i] = sm->data_buffer[i] * 2;
    }
    sm->processed_count = sm->data_count;
    return 0;
}

int action_processing_to_writing(struct StateMachine* sm) {
    if (sm == NULL || sm->processed_count == 0 || sm->processed_count > 16) return -1;
    for (size_t i = 0; i < sm->processed_count; i++) {
        printf("Processed data[%zu]: %u\n", i, sm->processed_data[i]);
    }
    return 0;
}

int action_error(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    printf("Error occurred with code: %u\n", sm->error_code);
    return 0;
}

int action_reset(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->data_count = 0;
    sm->processed_count = 0;
    sm->error_code = 0;
    return 0;
}

struct Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING, action_idle_to_reading},
    {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING, action_reading_to_processing},
    {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING, action_processing_to_writing},
    {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_IDLE, NULL},
    {STATE_IDLE, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_READING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_reset}
};

const size_t transition_count = sizeof(transitions) / sizeof(transitions[0]);

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL || event >= EVENT_COUNT) return -1;
    
    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].current_state == sm->current_state && transitions[i].event == event) {
            if (transitions[i].action != NULL) {
                if (transitions[i].action(sm) != 0) {
                    return -1;
                }
            }
            sm->current_state = transitions[i].next_state;
            return 0;
        }
    }
    return -1;
}

int read_input_data(struct StateMachine* sm) {
    if (sm == NULL || sm->current_state != STATE_READING) return -1;
    
    printf("Enter up to 16 numbers (0-1000), one per line. Enter -1 to finish:\n");
    sm->data_count = 0;
    
    for (int i = 0; i < 16; i++) {
        int input;
        if (scanf("%d", &input) != 1) {
            sm->error_code = 1;
            return -1;
        }
        
        if (input == -1) {
            break;
        }
        
        if (input < 0 || input > 1000) {
            sm->error_code = 2;
            return -1;
        }
        
        sm->data_buffer[sm->data_count] = (uint32_t)input;
        sm->data_count++;
    }
    
    if (sm->data_count == 0) {
        sm->error_code = 3;
        return -1;
    }
    
    return 0;
}

void print_menu(void) {
    printf("\nState Machine Menu:\n");
    printf("1. Start (IDLE -> READING)\n");
    printf("2. Data Ready (READING -> PROCESSING)\n");
    printf("3. Process Complete (PROCESSING -> WRITING)\n");
    printf("4. Write Complete (WRITING -> IDLE)\n");
    printf("5. Error (Any -> ERROR)\n");
    printf