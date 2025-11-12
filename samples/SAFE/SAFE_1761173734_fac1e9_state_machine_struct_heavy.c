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
    struct Transition transitions[20];
    size_t transition_count;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->process_count = 0;
    sm->error_count = 0;
}

int state_machine_handle_event(struct StateMachine *sm, const struct StateMachineConfig *config, enum Event event) {
    if (sm == NULL || config == NULL) return 0;
    
    for (size_t i = 0; i < config->transition_count; i++) {
        if (config->transitions[i].current == sm->current_state && 
            config->transitions[i].event == event) {
            sm->current_state = config->transitions[i].next;
            return 1;
        }
    }
    return 0;
}

void state_machine_config_init(struct StateMachineConfig *config) {
    if (config == NULL) return;
    
    config->transition_count = 0;
    
    struct Transition transitions[] = {
        {STATE_IDLE, EVENT_START, STATE_READY},
        {STATE_READY, EVENT_DATA_READY, STATE_PROCESSING},
        {STATE_PROCESSING, EVENT_SUCCESS, STATE_COMPLETE},
        {STATE_PROCESSING, EVENT_FAILURE, STATE_ERROR},
        {STATE_COMPLETE, EVENT_RESET, STATE_IDLE},
        {STATE_ERROR, EVENT_RESET, STATE_IDLE},
        {STATE_READY, EVENT_RESET, STATE_IDLE},
        {STATE_PROCESSING, EVENT_RESET, STATE_IDLE}
    };
    
    size_t count = sizeof(transitions) / sizeof(transitions[0]);
    if (count > 20) count = 20;
    
    for (size_t i = 0; i < count; i++) {
        config->transitions[i] = transitions[i];
    }
    config->transition_count = count;
}

void state_machine_process_data(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    if (sm->current_state == STATE_PROCESSING) {
        if (sm->data_value > 1000) {
            sm->error_count++;
        } else {
            sm->process_count++;
            sm->data_value *= 2;
        }
    }
}

void state_machine_print_status(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    if (sm->current_state < STATE_COUNT) {
        printf("State: %s, Data: %u, Processed: %u, Errors: %u\n",
               state_names[sm->current_state],
               sm->data_value,
               sm->process_count,
               sm->error_count);
    }
}

int main(void) {
    struct StateMachine sm;
    struct StateMachineConfig config;
    
    state_machine_init(&sm);
    state_machine_config_init(&config);
    
    printf("State Machine Demo\n");
    printf("Commands: 0=Start, 1=Data Ready, 2=Process, 3=Success, 4=Failure, 5=Reset, 6=Quit\n");
    
    int running = 1;
    while (running) {
        state_machine_print_status(&sm);
        
        printf("Enter command: ");
        int command;
        if (scanf("%d", &command) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (command < 0 || command > 6) {
            printf("Invalid command\n");
            continue;
        }
        
        if (command == 6) {
            running = 0;
            continue;
        }
        
        enum Event event = (enum Event)command;
        int handled = state_machine_handle_event(&sm, &config, event);
        
        if (handled) {
            printf("Event handled successfully\n");
            
            if (event == EVENT_DATA_READY && sm.current_state == STATE_READY) {
                sm.data_value = (uint32_t)(rand() % 1500);
                printf("Generated data value: %u\n", sm.data_value);
            }
            
            if (event == EVENT_PROCESS && sm.current_state == STATE_PROCESSING) {
                state_machine_process_data(&sm);
            }
        } else {
            printf("Event not allowed in current state\n");
        }
    }
    
    printf("Final state: ");
    state_machine_print_status(&sm);
    
    return 0;
}