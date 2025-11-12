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

struct Event {
    int type;
    char data[32];
};

struct StateMachine {
    enum State current_state;
    uint32_t counter;
    char buffer[64];
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->counter = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_handle_event(struct StateMachine *sm, const struct Event *event) {
    if (sm == NULL || event == NULL) return -1;
    if (event->type < 0 || event->type > 4) return -1;

    switch (sm->current_state) {
        case STATE_IDLE:
            if (event->type == 1) {
                sm->current_state = STATE_READY;
                sm->counter = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
            }
            break;
        case STATE_READY:
            if (event->type == 2) {
                if (strlen(event->data) < sizeof(sm->buffer)) {
                    strncpy(sm->buffer, event->data, sizeof(sm->buffer) - 1);
                    sm->buffer[sizeof(sm->buffer) - 1] = '\0';
                    sm->current_state = STATE_PROCESSING;
                } else {
                    sm->current_state = STATE_ERROR;
                }
            } else if (event->type == 0) {
                sm->current_state = STATE_IDLE;
            }
            break;
        case STATE_PROCESSING:
            if (event->type == 3) {
                if (sm->counter < 1000) {
                    sm->counter++;
                    if (sm->counter >= 5) {
                        sm->current_state = STATE_COMPLETE;
                    }
                } else {
                    sm->current_state = STATE_ERROR;
                }
            } else if (event->type == 0) {
                sm->current_state = STATE_IDLE;
            }
            break;
        case STATE_COMPLETE:
            if (event->type == 0) {
                sm->current_state = STATE_IDLE;
            }
            break;
        case STATE_ERROR:
            if (event->type == 0) {
                sm->current_state = STATE_IDLE;
            }
            break;
    }
    return 0;
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

    printf("State Machine Demo\n");
    printf("Commands: 0=Reset, 1=Start, 2=Load, 3=Process, 4=Invalid\n");

    char input[64];
    struct Event event;

    while (1) {
        printf("Current state: %s\n", state_to_string(sm.current_state));
        printf("Enter command (0-4): ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        if (strlen(input) == 0) continue;
        
        int cmd;
        if (sscanf(input, "%d", &cmd) != 1) {
            printf("Invalid input\n");
            continue;
        }

        event.type = cmd;
        memset(event.data, 0, sizeof(event.data));

        if (cmd == 2) {
            printf("Enter data (max 31 chars): ");
            if (fgets(input, sizeof(input), stdin) == NULL) break;
            input[strcspn(input, "\n")] = '\0';
            strncpy(event.data, input, sizeof(event.data) - 1);
            event.data[sizeof(event.data) - 1] = '\0';
        }

        if (state_machine_handle_event(&sm, &event) != 0) {
            printf("Error handling event\n");
        }

        if (sm.current_state == STATE_COMPLETE) {
            printf("Processing complete! Counter: %u, Data: %s\n", sm.counter, sm.buffer);
        } else if (sm.current_state == STATE_ERROR) {
            printf("Error state reached\n");
        }
    }

    return 0;
}