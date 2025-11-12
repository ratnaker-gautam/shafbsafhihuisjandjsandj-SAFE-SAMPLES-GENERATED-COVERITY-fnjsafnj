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

struct EventData {
    int value;
    char message[32];
};

struct StateMachine {
    enum State current_state;
    int counter;
    char buffer[64];
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->counter = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int handle_idle(struct StateMachine *sm, const struct EventData *event) {
    if (sm == NULL || event == NULL) return 0;
    if (event->value > 0 && event->value < 100) {
        sm->current_state = STATE_READY;
        snprintf(sm->buffer, sizeof(sm->buffer), "Ready: %s", event->message);
        return 1;
    }
    return 0;
}

int handle_ready(struct StateMachine *sm, const struct EventData *event) {
    if (sm == NULL || event == NULL) return 0;
    if (event->value >= 10 && event->value <= 50) {
        sm->current_state = STATE_PROCESSING;
        sm->counter = event->value;
        snprintf(sm->buffer, sizeof(sm->buffer), "Processing: %s", event->message);
        return 1;
    }
    return 0;
}

int handle_processing(struct StateMachine *sm, const struct EventData *event) {
    if (sm == NULL || event == NULL) return 0;
    if (event->value == sm->counter * 2) {
        sm->current_state = STATE_COMPLETE;
        snprintf(sm->buffer, sizeof(sm->buffer), "Complete: %s", event->message);
        return 1;
    } else if (event->value < 0) {
        sm->current_state = STATE_ERROR;
        snprintf(sm->buffer, sizeof(sm->buffer), "Error: %s", event->message);
        return 1;
    }
    return 0;
}

int handle_complete(struct StateMachine *sm, const struct EventData *event) {
    if (sm == NULL || event == NULL) return 0;
    if (event->value == 0) {
        state_machine_init(sm);
        return 1;
    }
    return 0;
}

int handle_error(struct StateMachine *sm, const struct EventData *event) {
    if (sm == NULL || event == NULL) return 0;
    if (event->value == -1) {
        state_machine_init(sm);
        return 1;
    }
    return 0;
}

int process_event(struct StateMachine *sm, const struct EventData *event) {
    if (sm == NULL || event == NULL) return 0;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            return handle_idle(sm, event);
        case STATE_READY:
            return handle_ready(sm, event);
        case STATE_PROCESSING:
            return handle_processing(sm, event);
        case STATE_COMPLETE:
            return handle_complete(sm, event);
        case STATE_ERROR:
            return handle_error(sm, event);
        default:
            return 0;
    }
}

const char* state_to_string(enum State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READY: return "READY";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_COMPLETE: return "COMPLETE";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct EventData events[] = {
        {25, "Start process"},
        {30, "Begin work"},
        {60, "Finish task"},
        {0, "Reset system"},
        {-1, "Clear error"}
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    printf("State Machine Simulation\n");
    printf("Initial state: %s\n", state_to_string(sm.current_state));
    
    for (size_t i = 0; i < num_events; i++) {
        printf("\nEvent %zu: value=%d, message='%s'\n", 
               i + 1, events[i].value, events[i].message);
        
        int result = process_event(&sm, &events[i]);
        
        printf("State transition: %s\n", result ? "SUCCESS" : "FAILED");
        printf("Current state: %s\n", state_to_string(sm.current_state));
        printf("Buffer: %s\n", sm.buffer);
        printf("Counter: %d\n", sm.counter);
    }
    
    printf("\nFinal state: %s\n", state_to_string(sm.current_state));
    
    return 0;
}