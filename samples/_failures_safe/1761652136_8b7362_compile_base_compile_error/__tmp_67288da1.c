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
    EVENT_PROCESS_DONE,
    EVENT_WRITE_DONE,
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
    sm->data_size = 0;
    sm->processed_size = 0;
    sm->error_code = 0;
    return 0;
}

int action_reading_to_processing(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    if (sm->data_size == 0 || sm->data_size > 16) return -1;
    for (size_t i = 0; i < sm->data_size; i++) {
        sm->processed_data[i] = sm->data_buffer[i] * 2;
    }
    sm->processed_size = sm->data_size;
    return 0;
}

int action_processing_to_writing(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    if (sm->processed_size == 0 || sm->processed_size > 16) return -1;
    for (size_t i = 0; i < sm->processed_size; i++) {
        printf("Output: %u\n", sm->processed_data[i]);
    }
    return 0;
}

int action_error(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->error_code = 1;
    return 0;
}

int action_reset(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->current_state = STATE_IDLE;
    sm->data_size = 0;
    sm->processed_size = 0;
    sm->error_code = 0;
    return 0;
}

struct Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING, action_idle_to_reading},
    {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING, action_reading_to_processing},
    {STATE_PROCESSING, EVENT_PROCESS_DONE, STATE_WRITING, action_processing_to_writing},
    {STATE_WRITING, EVENT_WRITE_DONE, STATE_IDLE, NULL},
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
    if (sm == NULL) return -1;
    if (sm->current_state != STATE_READING) return -1;
    
    printf("Enter up to 16 numbers (0 to finish): ");
    sm->data_size = 0;
    
    for (size_t i = 0; i < 16; i++) {
        int result = scanf("%u", &sm->data_buffer[i]);
        if (result != 1) {
            return -1;
        }
        if (sm->data_buffer[i] == 0) {
            break;
        }
        sm->data_size++;
        if (sm->data_size >= 16) break;
    }
    
    if (sm->data_size == 0) {
        return -1;
    }
    
    return 0;
}

int main(void) {
    struct StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.data_size = 0;
    sm.processed_size = 0;
    sm.error_code = 0;
    
    int running = 1;
    char input[32];
    
    while (running) {
        switch (sm.current_state) {
            case STATE_IDLE:
                printf("State: IDLE - Enter 'start' to begin or 'quit' to exit: ");
                if (scanf("%31s", input) != 1) {
                    return 1;
                }
                if (strcmp(input, "start") == 0) {
                    if (process_event(&sm