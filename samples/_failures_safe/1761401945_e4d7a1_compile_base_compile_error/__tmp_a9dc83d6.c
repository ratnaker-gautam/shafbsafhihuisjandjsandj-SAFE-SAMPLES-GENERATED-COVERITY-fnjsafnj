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
    int32_t value;
    char description[32];
};

struct StateMachine {
    enum State current_state;
    uint32_t transition_count;
    struct EventData last_event;
};

struct Command {
    char name[16];
    int32_t code;
};

void initialize_machine(struct StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->transition_count = 0;
    machine->last_event.value = 0;
    memset(machine->last_event.description, 0, sizeof(machine->last_event.description));
}

int validate_event(const struct EventData *event) {
    if (event == NULL) return 0;
    if (event->value < -1000 || event->value > 1000) return 0;
    for (size_t i = 0; i < sizeof(event->description); i++) {
        if (event->description[i] == '\0') break;
        if (event->description[i] < 32 || event->description[i] > 126) return 0;
    }
    return 1;
}

enum State handle_idle_state(const struct EventData *event) {
    if (!validate_event(event)) return STATE_ERROR;
    if (event->value == 1 && strcmp(event->description, "start") == 0) {
        return STATE_READY;
    }
    return STATE_ERROR;
}

enum State handle_ready_state(const struct EventData *event) {
    if (!validate_event(event)) return STATE_ERROR;
    if (event->value == 2 && strcmp(event->description, "process") == 0) {
        return STATE_PROCESSING;
    }
    if (event->value == 0 && strcmp(event->description, "cancel") == 0) {
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

enum State handle_processing_state(const struct EventData *event) {
    if (!validate_event(event)) return STATE_ERROR;
    if (event->value == 3 && strcmp(event->description, "complete") == 0) {
        return STATE_COMPLETE;
    }
    if (event->value == -1 && strcmp(event->description, "error") == 0) {
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

enum State handle_complete_state(const struct EventData *event) {
    if (!validate_event(event)) return STATE_ERROR;
    if (event->value == 0 && strcmp(event->description, "reset") == 0) {
        return STATE_IDLE;
    }
    return STATE_COMPLETE;
}

enum State handle_error_state(const struct EventData *event) {
    if (!validate_event(event)) return STATE_ERROR;
    if (event->value == 0 && strcmp(event->description, "reset") == 0) {
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

int process_event(struct StateMachine *machine, const struct EventData *event) {
    if (machine == NULL || !validate_event(event)) return 0;
    
    enum State new_state = machine->current_state;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            new_state = handle_idle_state(event);
            break;
        case STATE_READY:
            new_state = handle_ready_state(event);
            break;
        case STATE_PROCESSING:
            new_state = handle_processing_state(event);
            break;
        case STATE_COMPLETE:
            new_state = handle_complete_state(event);
            break;
        case STATE_ERROR:
            new_state = handle_error_state(event);
            break;
        default:
            return 0;
    }
    
    if (new_state != machine->current_state) {
        machine->current_state = new_state;
        machine->transition_count++;
        machine->last_event = *event;
        return 1;
    }
    
    return 0;
}

void print_state_info(const struct StateMachine *machine) {
    if (machine == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("Current State: %s\n", state_names[machine->current_state]);
    printf("Transitions: %u\n", machine->transition_count);
    printf("Last Event: value=%d, description='%s'\n", 
           machine->last_event.value, machine->last_event.description);
    printf("---\n");
}

int main(void) {
    struct StateMachine machine;
    initialize_machine(&machine);
    
    struct Command commands[] = {
        {"start", 1},
        {"process", 2},
        {"complete", 3},
        {"cancel", 0},
        {"reset", 0},
        {"error", -1}
    };
    
    size_t num_commands = sizeof(commands) / sizeof(commands[0]);
    
    printf("State Machine Demo\n");
    printf("Available commands: ");
    for (size_t i = 0; i < num_commands; i++) {
        printf("%s", commands[i].name);
        if (i < num_commands - 1) printf(", ");
    }
    printf("\n---\n");
    
    char input[64];
    struct EventData event;
    
    while (1) {