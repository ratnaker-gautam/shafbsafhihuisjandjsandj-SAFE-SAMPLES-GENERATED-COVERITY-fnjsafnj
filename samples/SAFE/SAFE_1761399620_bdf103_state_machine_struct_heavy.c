//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_COUNT
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_COUNT
};

struct StateMachine {
    enum State current_state;
    uint32_t data_value;
    uint32_t process_count;
    uint32_t error_count;
};

struct Transition {
    enum State current;
    enum Event event;
    enum State next;
};

struct StateMachineConfig {
    struct Transition *transitions;
    size_t transition_count;
};

struct StateMachine create_state_machine(void) {
    struct StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.data_value = 0;
    sm.process_count = 0;
    sm.error_count = 0;
    return sm;
}

struct StateMachineConfig create_state_machine_config(void) {
    static struct Transition transitions[] = {
        {STATE_IDLE, EVENT_START, STATE_READY},
        {STATE_READY, EVENT_DATA_READY, STATE_PROCESSING},
        {STATE_PROCESSING, EVENT_SUCCESS, STATE_COMPLETE},
        {STATE_PROCESSING, EVENT_FAILURE, STATE_ERROR},
        {STATE_COMPLETE, EVENT_RESET, STATE_IDLE},
        {STATE_ERROR, EVENT_RESET, STATE_IDLE}
    };
    
    struct StateMachineConfig config;
    config.transitions = transitions;
    config.transition_count = sizeof(transitions) / sizeof(transitions[0]);
    return config;
}

int handle_event(struct StateMachine *sm, struct StateMachineConfig *config, enum Event event) {
    if (sm == NULL || config == NULL) {
        return 0;
    }
    
    for (size_t i = 0; i < config->transition_count; i++) {
        if (config->transitions[i].current == sm->current_state && 
            config->transitions[i].event == event) {
            sm->current_state = config->transitions[i].next;
            return 1;
        }
    }
    return 0;
}

void process_data(struct StateMachine *sm, struct StateMachineConfig *config) {
    if (sm == NULL || config == NULL) return;
    
    if (sm->current_state == STATE_PROCESSING) {
        if (sm->data_value > 1000) {
            sm->error_count++;
            handle_event(sm, config, EVENT_FAILURE);
        } else {
            sm->process_count++;
            if (sm->data_value > UINT32_MAX / 2) {
                sm->error_count++;
                handle_event(sm, config, EVENT_FAILURE);
            } else {
                sm->data_value *= 2;
                handle_event(sm, config, EVENT_SUCCESS);
            }
        }
    }
}

void print_state_info(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    if (sm->current_state < STATE_COUNT) {
        printf("Current State: %s\n", state_names[sm->current_state]);
    } else {
        printf("Current State: UNKNOWN\n");
    }
    printf("Data Value: %u\n", sm->data_value);
    printf("Process Count: %u\n", sm->process_count);
    printf("Error Count: %u\n", sm->error_count);
    printf("---\n");
}

int main(void) {
    struct StateMachine sm = create_state_machine();
    struct StateMachineConfig config = create_state_machine_config();
    
    int running = 1;
    char input[32];
    
    printf("State Machine Controller\n");
    printf("Commands: start, data, process, reset, quit\n");
    
    while (running) {
        print_state_info(&sm);
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "start") == 0) {
            handle_event(&sm, &config, EVENT_START);
        } else if (strcmp(input, "data") == 0) {
            if (sm.current_state == STATE_READY) {
                sm.data_value = 10;
                handle_event(&sm, &config, EVENT_DATA_READY);
            }
        } else if (strcmp(input, "process") == 0) {
            if (sm.current_state == STATE_PROCESSING) {
                process_data(&sm, &config);
            }
        } else if (strcmp(input, "reset") == 0) {
            handle_event(&sm, &config, EVENT_RESET);
            sm.data_value = 0;
        } else if (strcmp(input, "quit") == 0) {
            running = 0;
        } else {
            printf("Invalid command\n");
        }
    }
    
    return 0;
}