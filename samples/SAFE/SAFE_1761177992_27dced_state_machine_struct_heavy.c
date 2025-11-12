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
    STATE_ERROR
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET
};

struct StateMachine {
    enum State current_state;
    uint32_t data_count;
    uint32_t max_data;
    char buffer[256];
};

struct EventData {
    enum Event event_type;
    char payload[128];
    uint32_t payload_length;
};

void initialize_machine(struct StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->data_count = 0;
    machine->max_data = 100;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

int validate_event(const struct EventData *event) {
    if (event == NULL) return 0;
    if (event->event_type < EVENT_START || event->event_type > EVENT_RESET) return 0;
    if (event->payload_length >= sizeof(event->payload)) return 0;
    return 1;
}

int process_event(struct StateMachine *machine, const struct EventData *event) {
    if (machine == NULL || !validate_event(event)) return 0;

    switch (machine->current_state) {
        case STATE_IDLE:
            if (event->event_type == EVENT_START) {
                machine->current_state = STATE_READY;
                return 1;
            }
            break;
        case STATE_READY:
            if (event->event_type == EVENT_DATA_READY) {
                if (event->payload_length > 0 && event->payload_length < sizeof(machine->buffer)) {
                    strncpy(machine->buffer, event->payload, event->payload_length);
                    machine->buffer[event->payload_length] = '\0';
                    machine->current_state = STATE_PROCESSING;
                    return 1;
                }
            }
            break;
        case STATE_PROCESSING:
            if (event->event_type == EVENT_SUCCESS) {
                machine->data_count++;
                if (machine->data_count < machine->max_data) {
                    machine->current_state = STATE_READY;
                } else {
                    machine->current_state = STATE_COMPLETE;
                }
                return 1;
            } else if (event->event_type == EVENT_FAILURE) {
                machine->current_state = STATE_ERROR;
                return 1;
            }
            break;
        case STATE_COMPLETE:
            if (event->event_type == EVENT_RESET) {
                initialize_machine(machine);
                return 1;
            }
            break;
        case STATE_ERROR:
            if (event->event_type == EVENT_RESET) {
                initialize_machine(machine);
                return 1;
            }
            break;
    }
    return 0;
}

void print_state_info(const struct StateMachine *machine) {
    if (machine == NULL) return;
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    printf("Current State: %s\n", state_names[machine->current_state]);
    printf("Data Count: %u/%u\n", machine->data_count, machine->max_data);
    if (strlen(machine->buffer) > 0) {
        printf("Buffer: %s\n", machine->buffer);
    }
}

int main(void) {
    struct StateMachine machine;
    initialize_machine(&machine);
    
    struct EventData events[] = {
        {EVENT_START, "", 0},
        {EVENT_DATA_READY, "Sample Data", 11},
        {EVENT_SUCCESS, "", 0},
        {EVENT_DATA_READY, "More Data", 9},
        {EVENT_SUCCESS, "", 0},
        {EVENT_RESET, "", 0}
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    printf("State Machine Simulation\n");
    printf("========================\n");
    
    for (size_t i = 0; i < num_events; i++) {
        printf("\nEvent %zu: ", i + 1);
        switch (events[i].event_type) {
            case EVENT_START: printf("START"); break;
            case EVENT_DATA_READY: printf("DATA_READY"); break;
            case EVENT_PROCESS: printf("PROCESS"); break;
            case EVENT_SUCCESS: printf("SUCCESS"); break;
            case EVENT_FAILURE: printf("FAILURE"); break;
            case EVENT_RESET: printf("RESET"); break;
        }
        printf("\n");
        
        if (process_event(&machine, &events[i])) {
            printf("Event processed successfully.\n");
        } else {
            printf("Event processing failed.\n");
        }
        
        print_state_info(&machine);
    }
    
    return 0;
}