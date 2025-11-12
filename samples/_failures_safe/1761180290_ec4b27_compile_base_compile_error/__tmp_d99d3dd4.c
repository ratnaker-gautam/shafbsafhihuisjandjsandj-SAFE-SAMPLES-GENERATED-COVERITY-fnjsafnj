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
    uint32_t data_value;
    uint32_t processed_value;
    uint32_t write_count;
    uint32_t error_count;
    uint32_t max_cycles;
};

struct Transition {
    enum State next_state;
    int (*action)(struct StateMachine*);
};

static int action_idle(struct StateMachine* sm) {
    sm->data_value = 0;
    sm->processed_value = 0;
    sm->write_count = 0;
    sm->error_count = 0;
    return 0;
}

static int action_reading(struct StateMachine* sm) {
    if (sm->max_cycles > 1000) {
        return -1;
    }
    sm->data_value = (sm->data_value * 17 + 23) % 100;
    return 0;
}

static int action_processing(struct StateMachine* sm) {
    if (sm->data_value > UINT32_MAX / 2) {
        return -1;
    }
    sm->processed_value = sm->data_value * 2;
    return 0;
}

static int action_writing(struct StateMachine* sm) {
    if (sm->write_count >= sm->max_cycles) {
        return -1;
    }
    sm->write_count++;
    return 0;
}

static int action_error(struct StateMachine* sm) {
    sm->error_count++;
    return 0;
}

static const struct Transition transition_table[STATE_COUNT][EVENT_COUNT] = {
    [STATE_IDLE] = {
        [EVENT_START] = {STATE_READING, action_reading},
        [EVENT_RESET] = {STATE_IDLE, action_idle}
    },
    [STATE_READING] = {
        [EVENT_DATA_READY] = {STATE_PROCESSING, action_processing},
        [EVENT_ERROR] = {STATE_ERROR, action_error},
        [EVENT_RESET] = {STATE_IDLE, action_idle}
    },
    [STATE_PROCESSING] = {
        [EVENT_PROCESS_COMPLETE] = {STATE_WRITING, action_writing},
        [EVENT_ERROR] = {STATE_ERROR, action_error},
        [EVENT_RESET] = {STATE_IDLE, action_idle}
    },
    [STATE_WRITING] = {
        [EVENT_WRITE_COMPLETE] = {STATE_READING, action_reading},
        [EVENT_ERROR] = {STATE_ERROR, action_error},
        [EVENT_RESET] = {STATE_IDLE, action_idle}
    },
    [STATE_ERROR] = {
        [EVENT_RESET] = {STATE_IDLE, action_idle}
    }
};

static int handle_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL || event >= EVENT_COUNT || sm->current_state >= STATE_COUNT) {
        return -1;
    }
    
    const struct Transition* trans = &transition_table[sm->current_state][event];
    if (trans->action == NULL) {
        return -1;
    }
    
    int result = trans->action(sm);
    if (result != 0) {
        return -1;
    }
    
    sm->current_state = trans->next_state;
    return 0;
}

static void print_state_info(const struct StateMachine* sm) {
    const char* state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    
    if (sm->current_state < STATE_COUNT) {
        printf("State: %s, Data: %u, Processed: %u, Writes: %u, Errors: %u\n",
               state_names[sm->current_state],
               sm->data_value,
               sm->processed_value,
               sm->write_count,
               sm->error_count);
    }
}

int main(void) {
    struct StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.data_value = 0;
    sm.processed_value = 0;
    sm.write_count = 0;
    sm.error_count = 0;
    sm.max_cycles = 10;
    
    printf("Starting state machine simulation\n");
    
    if (handle_event(&sm, EVENT_START) != 0) {
        fprintf(stderr, "Failed to start state machine\n");
        return 1;
    }
    
    print_state_info(&sm);
    
    for (uint32_t cycle = 0; cycle < sm.max_cycles; cycle++) {
        enum Event next_event;
        
        switch (sm.current_state) {
            case STATE_READING:
                next_event = EVENT_DATA_READY;
                break;
            case STATE_PROCESSING:
                next_event = EVENT_PROCESS_COMPLETE;
                break;
            case STATE_WRITING:
                next_event = EVENT_WRITE_COMPLETE;
                break;
            default:
                next_event = EVENT_RESET;
                break;
        }
        
        if (handle_event(&sm, next_event) != 0)