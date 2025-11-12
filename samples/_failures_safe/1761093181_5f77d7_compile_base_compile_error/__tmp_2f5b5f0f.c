//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: state_machine
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

struct Context {
    enum State current_state;
    uint32_t data_count;
    uint32_t processed_count;
    uint32_t error_count;
    char buffer[256];
};

struct Event {
    int type;
    char data[128];
};

struct StateMachine {
    struct Context context;
    struct Event current_event;
    uint32_t total_events;
};

void initialize_machine(struct StateMachine *machine) {
    if (machine == NULL) return;
    machine->context.current_state = STATE_IDLE;
    machine->context.data_count = 0;
    machine->context.processed_count = 0;
    machine->context.error_count = 0;
    memset(machine->context.buffer, 0, sizeof(machine->context.buffer));
    memset(&machine->current_event, 0, sizeof(machine->current_event));
    machine->total_events = 0;
}

int validate_event(const struct Event *event) {
    if (event == NULL) return 0;
    if (event->type < 0 || event->type > 4) return 0;
    for (size_t i = 0; i < sizeof(event->data); i++) {
        if (event->data[i] == '\0') break;
        if (event->data[i] < 32 || event->data[i] > 126) return 0;
    }
    return 1;
}

void handle_idle_state(struct StateMachine *machine) {
    if (machine == NULL) return;
    if (machine->current_event.type == 1) {
        machine->context.current_state = STATE_READY;
        machine->context.data_count = 0;
        machine->context.processed_count = 0;
        machine->context.error_count = 0;
    }
}

void handle_ready_state(struct StateMachine *machine) {
    if (machine == NULL) return;
    if (machine->current_event.type == 2) {
        size_t len = strnlen(machine->current_event.data, sizeof(machine->current_event.data));
        if (len > 0 && len < sizeof(machine->context.buffer)) {
            strncpy(machine->context.buffer, machine->current_event.data, sizeof(machine->context.buffer) - 1);
            machine->context.buffer[sizeof(machine->context.buffer) - 1] = '\0';
            machine->context.current_state = STATE_PROCESSING;
            machine->context.data_count++;
        } else {
            machine->context.current_state = STATE_ERROR;
            machine->context.error_count++;
        }
    } else if (machine->current_event.type == 0) {
        machine->context.current_state = STATE_IDLE;
    }
}

void handle_processing_state(struct StateMachine *machine) {
    if (machine == NULL) return;
    if (machine->current_event.type == 3) {
        size_t len = strnlen(machine->context.buffer, sizeof(machine->context.buffer));
        int valid = 1;
        for (size_t i = 0; i < len; i++) {
            if (machine->context.buffer[i] < '0' || machine->context.buffer[i] > '9') {
                valid = 0;
                break;
            }
        }
        if (valid && len > 0) {
            machine->context.current_state = STATE_COMPLETE;
            machine->context.processed_count++;
        } else {
            machine->context.current_state = STATE_ERROR;
            machine->context.error_count++;
        }
    } else if (machine->current_event.type == 0) {
        machine->context.current_state = STATE_IDLE;
    }
}

void handle_complete_state(struct StateMachine *machine) {
    if (machine == NULL) return;
    if (machine->current_event.type == 0) {
        machine->context.current_state = STATE_IDLE;
    } else if (machine->current_event.type == 2) {
        machine->context.current_state = STATE_READY;
    }
}

void handle_error_state(struct StateMachine *machine) {
    if (machine == NULL) return;
    if (machine->current_event.type == 0) {
        machine->context.current_state = STATE_IDLE;
    } else if (machine->current_event.type == 4) {
        machine->context.current_state = STATE_READY;
    }
}

void process_event(struct StateMachine *machine) {
    if (machine == NULL || !validate_event(&machine->current_event)) return;
    
    switch (machine->context.current_state) {
        case STATE_IDLE:
            handle_idle_state(machine);
            break;
        case STATE_READY:
            handle_ready_state(machine);
            break;
        case STATE_PROCESSING:
            handle_processing_state(machine);
            break;
        case STATE_COMPLETE:
            handle_complete_state(machine);
            break;
        case STATE_ERROR:
            handle_error_state(machine);
            break;
    }
    machine->total_events++;
}

void print_state_info(const struct StateMachine *machine) {
    if (machine == NULL) return;
    const char *state_names[] = {"IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"};
    printf("Current State: %s\n", state_names[machine->context.current_state]);
    printf("Data Count: %u\n", machine->context