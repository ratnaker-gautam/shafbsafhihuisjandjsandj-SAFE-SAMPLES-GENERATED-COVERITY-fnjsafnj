//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    EVENT_COMPLETE,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_processed;
} StateMachine;

State handle_idle(StateMachine *sm, Event event, const char *data);
State handle_reading(StateMachine *sm, Event event, const char *data);
State handle_processing(StateMachine *sm, Event event, const char *data);
State handle_writing(StateMachine *sm, Event event, const char *data);
State handle_error(StateMachine *sm, Event event, const char *data);
State handle_done(StateMachine *sm, Event event, const char *data);

State (*state_handlers[])(StateMachine*, Event, const char*) = {
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
    sm->data_processed = 0;
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

State handle_idle(StateMachine *sm, Event event, const char *data) {
    if (event == EVENT_START) {
        printf("State: IDLE -> READING\n");
        return STATE_READING;
    }
    return STATE_IDLE;
}

State handle_reading(StateMachine *sm, Event event, const char *data) {
    if (event == EVENT_DATA && data != NULL) {
        size_t data_len = strlen(data);
        if (validate_input(data, data_len) && sm->buffer_len + data_len < MAX_INPUT_LEN) {
            memcpy(sm->buffer + sm->buffer_len, data, data_len);
            sm->buffer_len += data_len;
            sm->buffer[sm->buffer_len] = '\0';
            printf("Read %zu bytes, total: %zu\n", data_len, sm->buffer_len);
            return STATE_READING;
        } else {
            return STATE_ERROR;
        }
    } else if (event == EVENT_PROCESS && sm->buffer_len > 0) {
        printf("State: READING -> PROCESSING\n");
        return STATE_PROCESSING;
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_READING;
}

State handle_processing(StateMachine *sm, Event event, const char *data) {
    if (event == EVENT_WRITE) {
        if (sm->buffer_len > 0) {
            for (size_t i = 0; i < sm->buffer_len; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                }
            }
            sm->data_processed = 1;
            printf("State: PROCESSING -> WRITING\n");
            return STATE_WRITING;
        } else {
            return STATE_ERROR;
        }
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

State handle_writing(StateMachine *sm, Event event, const char *data) {
    if (event == EVENT_COMPLETE) {
        if (sm->data_processed) {
            printf("Output: %s\n", sm->buffer);
            printf("State: WRITING -> DONE\n");
            return STATE_DONE;
        } else {
            return STATE_ERROR;
        }
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_WRITING;
}

State handle_error(StateMachine *sm, Event event, const char *data) {
    if (event == EVENT_RESET) {
        printf("State: ERROR -> IDLE (reset)\n");
        initialize_machine(sm);
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

State handle_done(StateMachine *sm, Event event, const char *data) {
    if (event == EVENT_RESET) {
        printf("State: DONE -> IDLE (reset)\n");
        initialize_machine(sm);
        return STATE_IDLE;
    }
    return STATE_DONE;
}

Event get_user_event(void) {
    char input[16];
    printf("Enter event (0=START,1=DATA,2=PROCESS,3=WRITE,4=ERROR,5=COMPLETE,6=RESET): ");
    if (fgets(input, sizeof(input), stdin) != NULL) {