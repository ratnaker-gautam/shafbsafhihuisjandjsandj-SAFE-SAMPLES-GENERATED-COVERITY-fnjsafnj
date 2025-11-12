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
    size_t data_size;
    uint32_t processed_data[16];
    size_t processed_size;
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
    printf("Transition: IDLE -> READING\n");
    sm->data_size = 0;
    return 0;
}

int action_reading_to_processing(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    if (sm->data_size == 0 || sm->data_size > 16) return -1;
    printf("Transition: READING -> PROCESSING\n");
    for (size_t i = 0; i < sm->data_size; i++) {
        sm->processed_data[i] = sm->data_buffer[i] * 2;
    }
    sm->processed_size = sm->data_size;
    return 0;
}

int action_processing_to_writing(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    if (sm->processed_size == 0 || sm->processed_size > 16) return -1;
    printf("Transition: PROCESSING -> WRITING\n");
    printf("Writing processed data: ");
    for (size_t i = 0; i < sm->processed_size; i++) {
        printf("%u ", sm->processed_data[i]);
    }
    printf("\n");
    return 0;
}

int action_writing_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    printf("Transition: WRITING -> IDLE\n");
    sm->data_size = 0;
    sm->processed_size = 0;
    return 0;
}

int action_error(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    printf("Transition: -> ERROR (Code: %u)\n", sm->error_code);
    return 0;
}

int action_reset(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    printf("Transition: -> IDLE (Reset)\n");
    sm->current_state = STATE_IDLE;
    sm->data_size = 0;
    sm->processed_size = 0;
    sm->error_code = 0;
    return 0;
}

struct Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING, action_idle_to_reading},
    {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING, action_reading_to_processing},
    {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING, action_processing_to_writing},
    {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_IDLE, action_writing_to_idle},
    {STATE_IDLE, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_READING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_reset}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL) return -1;
    if (event >= EVENT_COUNT) return -1;
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == sm->current_state && 
            transitions[i].event == event) {
            if (transitions[i].action != NULL) {
                int result = transitions[i].action(sm);
                if (result == 0) {
                    sm->current_state = transitions[i].next_state;
                }
                return result;
            }
        }
    }
    return -1;
}

int read_input_data(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    if (sm->current_state != STATE_READING) return -1;
    
    printf("Enter number of data elements (1-16): ");
    int count;
    if (scanf("%d", &count) != 1) return -1;
    if (count < 1 || count > 16) return -1;
    
    sm->data_size = (size_t)count;
    printf("Enter %d unsigned integers: ", count);
    for (int i = 0; i < count; i++) {
        if (scanf("%u", &sm->data_buffer[i]) != 1) return -1;
    }
    return 0;
}

void print_menu(struct StateMachine* sm) {
    if (sm == NULL) return;
    printf("\nCurrent State: ");
    switch (sm