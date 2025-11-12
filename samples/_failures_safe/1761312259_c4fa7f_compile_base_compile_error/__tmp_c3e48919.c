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

static void initialize_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_size = 0;
    sm->processed_size = 0;
    sm->error_code = 0;
    memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
    memset(sm->processed_data, 0, sizeof(sm->processed_data));
}

static int validate_state_transition(enum State current, enum Event event) {
    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].current == current && transitions[i].event == event) {
            return 1;
        }
    }
    return 0;
}

static enum State get_next_state(enum State current, enum Event event) {
    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].current == current && transitions[i].event == event) {
            return transitions[i].next;
        }
    }
    return current;
}

static int read_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_READING) return 0;
    
    size_t count = 0;
    printf("Enter number of data elements to read (1-16): ");
    if (scanf("%zu", &count) != 1) return 0;
    if (count < 1 || count > 16) return 0;
    
    printf("Enter %zu unsigned integers:\n", count);
    for (size_t i = 0; i < count; i++) {
        if (scanf("%u", &sm->data_buffer[i]) != 1) return 0;
    }
    sm->data_size = count;
    return 1;
}

static int process_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_PROCESSING) return 0;
    if (sm->data_size == 0 || sm->data_size > 16) return 0;
    
    for (size_t i = 0; i < sm->data_size; i++) {
        if (sm->data_buffer[i] > UINT32_MAX - 100) return 0;
        sm->processed_data[i] = sm->data_buffer[i] + 100;
    }
    sm->processed_size = sm->data_size;
    return 1;
}

static int write_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_WRITING) return 0;
    if (sm->processed_size == 0 || sm->processed_size > 16) return 0;
    
    printf("Processed data: ");
    for (size_t i = 0; i < sm->processed_size; i++) {
        printf("%u ", sm->processed_data[i]);
    }
    printf("\n");
    return 1;
}

static void handle_error(struct StateMachine *sm, uint32_t error_code) {
    if (sm == NULL) return;
    sm->error_code = error_code;
    printf("Error occurred: %u\n", error_code);
}

static void print_state_info(enum State state) {
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    if (state < STATE_COUNT) {
        printf("Current state: %s\n", state_names[state]);
    }
}

int main(void) {
    struct StateMachine sm;
    initialize_state_machine(&sm);
    
    printf("State Machine Demo - Data Processing Pipeline\n");
    printf("Available events: 0=START, 1=DATA_READY, 2=PROCESS_COMPLETE, 3=WRITE_COMPLETE, 4=ERROR, 5=RESET\n");
    
    while (1) {
        print_state_info(sm.current_state);
        printf("Enter event number (or -1 to exit): ");