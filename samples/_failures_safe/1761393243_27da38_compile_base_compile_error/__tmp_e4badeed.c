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
    enum State current;
    enum Event event;
    enum State next;
};

static const struct Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING},
    {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING},
    {STATE_READING, EVENT_ERROR, STATE_ERROR},
    {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
    {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_IDLE},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE}
};

static const size_t transition_count = sizeof(transitions) / sizeof(transitions[0]);

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_size = 0;
    sm->processed_size = 0;
    sm->error_code = 0;
    memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
    memset(sm->processed_data, 0, sizeof(sm->processed_data));
}

int state_machine_handle_event(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return 0;
    
    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            sm->current_state = transitions[i].next;
            return 1;
        }
    }
    return 0;
}

int simulate_data_read(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_READING) return 0;
    
    sm->data_size = 8;
    for (size_t i = 0; i < sm->data_size; i++) {
        sm->data_buffer[i] = (uint32_t)(i * 10 + 1);
    }
    return 1;
}

int simulate_data_process(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_PROCESSING) return 0;
    if (sm->data_size == 0 || sm->data_size > 16) return 0;
    
    sm->processed_size = sm->data_size;
    for (size_t i = 0; i < sm->processed_size; i++) {
        if (sm->data_buffer[i] > UINT32_MAX - 100) return 0;
        sm->processed_data[i] = sm->data_buffer[i] + 100;
    }
    return 1;
}

int simulate_data_write(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_WRITING) return 0;
    if (sm->processed_size == 0 || sm->processed_size > 16) return 0;
    
    for (size_t i = 0; i < sm->processed_size; i++) {
        printf("Output[%zu]: %u\n", i, sm->processed_data[i]);
    }
    return 1;
}

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    
    if (sm->current_state < STATE_COUNT) {
        printf("Current state: %s\n", state_names[sm->current_state]);
    }
    
    if (sm->current_state == STATE_ERROR) {
        printf("Error code: %u\n", sm->error_code);
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    printf("State Machine Demo\n");
    printf("==================\n");
    
    int step = 0;
    char input[32];
    
    while (step < 10) {
        print_state_info(&sm);
        printf("\nEnter command (start, read, process, write, reset, quit): ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        } else if (strcmp(input, "start") == 0) {
            if (state_machine_handle_event(&sm, EVENT_START)) {
                printf("Transitioned to READING state\n");
            } else {
                printf("Invalid transition from current state\n");
            }
        } else if (strcmp(input, "read") == 0) {
            if (simulate_data_read