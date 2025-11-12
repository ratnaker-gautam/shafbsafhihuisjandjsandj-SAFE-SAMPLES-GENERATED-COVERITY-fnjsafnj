//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_START;
    memset(machine->buffer, 0, sizeof(machine->buffer));
    machine->buffer_len = 0;
    machine->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 0;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

State handle_start_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    if (machine == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_START:
            initialize_machine(machine);
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    if (machine == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_DATA:
            if (!validate_input(data, data_len)) return STATE_ERROR;
            if (machine->buffer_len + data_len >= MAX_INPUT_LEN) return STATE_ERROR;
            memcpy(machine->buffer + machine->buffer_len, data, data_len);
            machine->buffer_len += data_len;
            machine->data_count++;
            if (machine->data_count >= 3) return STATE_PROCESSING;
            return STATE_READING;
        case EVENT_COMPLETE:
            return STATE_PROCESSING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    if (machine == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processed %zu characters: ", machine->buffer_len);
            for (size_t i = 0; i < machine->buffer_len; i++) {
                if (i < MAX_INPUT_LEN) {
                    printf("%c", machine->buffer[i]);
                }
            }
            printf("\n");
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_finished_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    if (machine == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            return STATE_FINISHED;
    }
}

State handle_error_state(StateMachine *machine, Event event, const char *data, size_t data_len) {
    if (machine == NULL) return STATE_ERROR;
    switch (event) {
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State process_event(StateMachine *machine, Event event, const char *data, size_t data_len) {
    if (machine == NULL) return STATE_ERROR;
    switch (machine->current_state) {
        case STATE_START:
            return handle_start_state(machine, event, data, data_len);
        case STATE_READING:
            return handle_reading_state(machine, event, data, data_len);
        case STATE_PROCESSING:
            return handle_processing_state(machine, event, data, data_len);
        case STATE_FINISHED:
            return handle_finished_state(machine, event, data, data_len);
        case STATE_ERROR:
            return handle_error_state(machine, event, data, data_len);
        default:
            return STATE_ERROR;
    }
}

Event get_user_event(void) {
    char input[MAX_INPUT_LEN];
    if (fgets(input, sizeof(input), stdin) == NULL) return EVENT_ERROR;
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') input[--len] = '\0';
    if (len == 0) return EVENT_ERROR;
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "complete") == 0) return EVENT_COMPLETE;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    return EVENT_ERROR;
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    printf