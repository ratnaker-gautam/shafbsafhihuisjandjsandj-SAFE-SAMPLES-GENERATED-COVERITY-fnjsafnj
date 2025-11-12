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
    char buffer[256];
    size_t buffer_len;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->counter = 0;
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
}

int state_machine_handle_event(struct StateMachine *sm, const struct Event *event) {
    if (sm == NULL || event == NULL) return -1;
    if (event->type < 0 || event->type > 4) return -1;

    switch (sm->current_state) {
        case STATE_IDLE:
            if (event->type == 1) {
                sm->current_state = STATE_READY;
                sm->counter = 0;
                return 0;
            }
            break;
        case STATE_READY:
            if (event->type == 2) {
                if (strlen(event->data) < sizeof(sm->buffer)) {
                    strncpy(sm->buffer, event->data, sizeof(sm->buffer) - 1);
                    sm->buffer[sizeof(sm->buffer) - 1] = '\0';
                    sm->buffer_len = strlen(sm->buffer);
                    sm->current_state = STATE_PROCESSING;
                    return 0;
                } else {
                    sm->current_state = STATE_ERROR;
                    return -1;
                }
            }
            break;
        case STATE_PROCESSING:
            if (event->type == 3) {
                if (sm->buffer_len > 0) {
                    for (size_t i = 0; i < sm->buffer_len; i++) {
                        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                        }
                    }
                    sm->counter++;
                    sm->current_state = STATE_COMPLETE;
                    return 0;
                } else {
                    sm->current_state = STATE_ERROR;
                    return -1;
                }
            }
            break;
        case STATE_COMPLETE:
            if (event->type == 4) {
                sm->current_state = STATE_IDLE;
                sm->buffer[0] = '\0';
                sm->buffer_len = 0;
                return 0;
            }
            break;
        case STATE_ERROR:
            if (event->type == 0) {
                sm->current_state = STATE_IDLE;
                sm->buffer[0] = '\0';
                sm->buffer_len = 0;
                sm->counter = 0;
                return 0;
            }
            break;
    }
    return -1;
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

    printf("State Machine Demo - Enter event type (0-4):\n");
    printf("0: Reset, 1: Start, 2: Load Data, 3: Process, 4: Complete\n");

    int event_type;
    char input_data[32];

    while (1) {
        printf("\nCurrent state: %s\n", state_to_string(sm.current_state));
        printf("Enter event type (-1 to exit): ");
        
        if (scanf("%d", &event_type) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        if (event_type == -1) {
            break;
        }

        struct Event event;
        event.type = event_type;

        if (event_type == 2) {
            printf("Enter data (max 31 chars): ");
            if (scanf("%31s", input_data) != 1) {
                printf("Invalid data input\n");
                continue;
            }
            strncpy(event.data, input_data, sizeof(event.data) - 1);
            event.data[sizeof(event.data) - 1] = '\0';
        } else {
            event.data[0] = '\0';
        }

        int result = state_machine_handle_event(&sm, &event);
        if (result == 0) {
            printf("Event handled successfully\n");
            if (sm.current_state == STATE_COMPLETE) {
                printf("Processed data: %s (processed %u times)\n", sm.buffer, sm.counter);
            }
        } else {
            printf("Event failed - invalid transition\n");
        }
    }

    printf("Final state: %s\n", state_to_string(sm.current_state));
    printf("Total processing cycles: %u\n", sm.counter);

    return 0;
}