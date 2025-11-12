//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 64
#define MAX_STATES 8

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
    EVENT_RESET,
    EVENT_EXIT
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
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

void initialize_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') {
            return 0;
        }
    }
    return 1;
}

State handle_idle(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_START:
            printf("State: IDLE -> READING\n");
            return STATE_READING;
        case EVENT_EXIT:
            return STATE_DONE;
        default:
            return STATE_IDLE;
    }
}

State handle_reading(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_DATA:
            printf("Enter data (max %d chars): ", MAX_INPUT_LEN - 1);
            if (fgets(sm->buffer, MAX_INPUT_LEN, stdin) != NULL) {
                sm->buffer_len = strlen(sm->buffer);
                if (sm->buffer_len > 0 && sm->buffer[sm->buffer_len - 1] == '\n') {
                    sm->buffer[--sm->buffer_len] = '\0';
                }
                if (validate_input(sm->buffer, sm->buffer_len)) {
                    sm->data_count++;
                    printf("State: READING -> PROCESSING\n");
                    return STATE_PROCESSING;
                }
            }
            printf("State: READING -> ERROR\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("State: READING -> IDLE\n");
            return STATE_IDLE;
        case EVENT_EXIT:
            return STATE_DONE;
        default:
            return STATE_READING;
    }
}

State handle_processing(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_PROCESS:
            if (sm->buffer_len > 0) {
                printf("Processing: '");
                for (size_t i = 0; i < sm->buffer_len; i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        putchar(sm->buffer[i] - 32);
                    } else {
                        putchar(sm->buffer[i]);
                    }
                }
                printf("' (%zu chars)\n", sm->buffer_len);
                printf("State: PROCESSING -> WRITING\n");
                return STATE_WRITING;
            }
            printf("State: PROCESSING -> ERROR\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("State: PROCESSING -> IDLE\n");
            return STATE_IDLE;
        case EVENT_EXIT:
            return STATE_DONE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_WRITE:
            printf("Data written successfully. Total processed: %d\n", sm->data_count);
            printf("State: WRITING -> IDLE\n");
            return STATE_IDLE;
        case EVENT_RESET:
            printf("State: WRITING -> IDLE\n");
            return STATE_IDLE;
        case EVENT_EXIT:
            return STATE_DONE;
        default:
            return STATE_WRITING;
    }
}

State handle_error(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_RESET:
            printf("State: ERROR -> IDLE\n");
            return STATE_IDLE;
        case EVENT_EXIT:
            return STATE_DONE;
        default:
            return STATE_ERROR;
    }
}

State handle_done(StateMachine *sm, Event event) {
    return STATE_DONE;
}

Event get_user_event(void) {
    printf("\nEvents: 0=START, 1=DATA, 2=PROCESS, 3=WRITE, 4=ERROR, 5=RESET, 6=EXIT\n");
    printf("Choose event: ");
    
    char input[16];