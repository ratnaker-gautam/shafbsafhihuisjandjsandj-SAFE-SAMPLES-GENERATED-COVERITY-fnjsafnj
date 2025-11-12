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
    uint32_t event_count;
    char buffer[256];
    size_t buffer_pos;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->event_count = 0;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_handle_event(struct StateMachine *sm, const struct Event *event) {
    if (sm == NULL || event == NULL) return -1;
    if (event->type < 0 || event->type > 4) return -1;

    if (sm->event_count == UINT32_MAX) {
        sm->event_count = 0;
    } else {
        sm->event_count++;
    }

    switch (sm->current_state) {
        case STATE_IDLE:
            if (event->type == 1) {
                sm->current_state = STATE_READY;
                sm->buffer_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
            }
            break;
        case STATE_READY:
            if (event->type == 2) {
                sm->current_state = STATE_PROCESSING;
            } else if (event->type == 0) {
                sm->current_state = STATE_IDLE;
            }
            break;
        case STATE_PROCESSING:
            if (event->type == 3) {
                if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
                    size_t data_len = strlen(event->data);
                    if (data_len < sizeof(event->data) && sm->buffer_pos + data_len < sizeof(sm->buffer) - 1) {
                        strncpy(sm->buffer + sm->buffer_pos, event->data, data_len);
                        sm->buffer_pos += data_len;
                        sm->buffer[sm->buffer_pos] = '\0';
                    }
                }
                sm->current_state = STATE_COMPLETE;
            } else if (event->type == 4) {
                sm->current_state = STATE_ERROR;
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
    printf("Commands: 0=Reset, 1=Start, 2=Prepare, 3=Process, 4=Error\n");
    printf("Enter command number and optional data (max 31 chars):\n");

    char input[64];
    while (fgets(input, sizeof(input), stdin) != NULL) {
        if (strlen(input) == 0) continue;

        struct Event event;
        event.type = -1;
        memset(event.data, 0, sizeof(event.data));

        char data_part[32];
        int scan_result = sscanf(input, "%d %31[^\n]", &event.type, data_part);
        if (scan_result >= 1) {
            if (scan_result == 2) {
                strncpy(event.data, data_part, sizeof(event.data) - 1);
                event.data[sizeof(event.data) - 1] = '\0';
            }
        } else {
            printf("Invalid input format\n");
            continue;
        }

        if (event.type < 0 || event.type > 4) {
            printf("Invalid event type: %d\n", event.type);
            continue;
        }

        if (state_machine_handle_event(&sm, &event) == 0) {
            printf("State: %s, Events: %u", state_to_string(sm.current_state), sm.event_count);
            if (sm.buffer_pos > 0) {
                printf(", Buffer: %s", sm.buffer);
            }
            printf("\n");
        } else {
            printf("Error handling event\n");
        }

        if (sm.current_state == STATE_COMPLETE) {
            printf("Processing complete. Buffer content: %s\n", sm.buffer);
        }
    }

    return 0;
}