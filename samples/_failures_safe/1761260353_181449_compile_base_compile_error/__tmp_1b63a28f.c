//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_WRITE,
    EVENT_ERROR,
    EVENT_COMPLETE
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int processed_count;
} StateMachine;

State handle_idle(StateMachine *sm, Event event);
State handle_reading(StateMachine *sm, Event event);
State handle_processing(StateMachine *sm, Event event);
State handle_writing(StateMachine *sm, Event event);
State handle_error(StateMachine *sm, Event event);
State handle_done(StateMachine *sm, Event event);

State (*state_handlers[])(StateMachine*, Event) = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error,
    handle_done
};

State handle_idle(StateMachine *sm, Event event) {
    if (event == EVENT_START) {
        sm->buffer_len = 0;
        sm->processed_count = 0;
        printf("State: IDLE -> READING\n");
        return STATE_READING;
    }
    return STATE_IDLE;
}

State handle_reading(StateMachine *sm, Event event) {
    if (event == EVENT_DATA) {
        if (sm->buffer_len < MAX_INPUT_LEN - 1) {
            char input[MAX_INPUT_LEN];
            if (fgets(input, sizeof(input), stdin) != NULL) {
                size_t len = strlen(input);
                if (len > 0 && input[len-1] == '\n') {
                    input[len-1] = '\0';
                    len--;
                }
                if (strcmp(input, "quit") == 0) {
                    printf("Early exit requested\n");
                    return STATE_DONE;
                }
                if (sm->buffer_len + len < MAX_INPUT_LEN) {
                    memcpy(sm->buffer + sm->buffer_len, input, len);
                    sm->buffer_len += len;
                    sm->buffer[sm->buffer_len] = '\0';
                    printf("Read: %s (total: %zu chars)\n", input, sm->buffer_len);
                    return STATE_READING;
                } else {
                    printf("Buffer full, moving to processing\n");
                    return STATE_PROCESSING;
                }
            } else {
                printf("Read error, moving to processing\n");
                return STATE_PROCESSING;
            }
        } else {
            printf("Buffer full, moving to processing\n");
            return STATE_PROCESSING;
        }
    }
    return STATE_READING;
}

State handle_processing(StateMachine *sm, Event event) {
    if (event == EVENT_PROCESS) {
        if (sm->buffer_len > 0) {
            for (size_t i = 0; i < sm->buffer_len; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                }
            }
            sm->processed_count++;
            printf("Processed: %s\n", sm->buffer);
            printf("State: PROCESSING -> WRITING\n");
            return STATE_WRITING;
        } else {
            printf("State: PROCESSING -> ERROR\n");
            return STATE_ERROR;
        }
    }
    return STATE_PROCESSING;
}

State handle_writing(StateMachine *sm, Event event) {
    if (event == EVENT_WRITE) {
        printf("Output: %s\n", sm->buffer);
        if (sm->processed_count >= 3) {
            printf("State: WRITING -> DONE\n");
            return STATE_DONE;
        } else {
            printf("State: WRITING -> IDLE\n");
            return STATE_IDLE;
        }
    }
    return STATE_WRITING;
}

State handle_error(StateMachine *sm, Event event) {
    if (event == EVENT_ERROR) {
        printf("Error state - resetting\n");
        sm->buffer_len = 0;
        sm->processed_count = 0;
        printf("State: ERROR -> IDLE\n");
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

State handle_done(StateMachine *sm, Event event) {
    return STATE_DONE;
}

void initialize_state_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->buffer_len = 0;
    sm->processed_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

Event get_next_event(State current_state) {
    switch (current_state) {
        case STATE_IDLE:
            return EVENT_START;
        case STATE_READING:
            return EVENT_DATA;
        case STATE_PROCESSING:
            return EVENT_PROCESS;
        case STATE_WRITING:
            return EVENT_WRITE;
        case STATE_ERROR:
            return EVENT_ERROR;
        case STATE_DONE:
            return EVENT_COMPLETE;
        default:
            return EVENT_ERROR;
    }
}

int main(void) {
    StateMachine sm;
    initialize_state_machine(&sm);