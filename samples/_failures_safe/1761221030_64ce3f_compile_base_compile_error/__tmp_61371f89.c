//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
};

struct StateMachine {
    enum State current_state;
    uint32_t data_count;
    uint32_t max_data;
    char buffer[256];
    uint32_t buffer_pos;
    uint32_t processed_count;
};

struct Transition {
    enum State current;
    enum Event event;
    enum State next;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 10;
    sm->buffer_pos = 0;
    sm->processed_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 255) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)input[i]) && input[i] != ' ') return 0;
    }
    return 1;
}

enum Event state_machine_parse_event(const char *input) {
    if (input == NULL) return EVENT_ERROR;
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA_READY;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS_COMPLETE;
    if (strcmp(input, "write") == 0) return EVENT_WRITE_COMPLETE;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    return EVENT_ERROR;
}

int state_machine_handle_transition(struct StateMachine *sm, enum Event event) {
    static const struct Transition transitions[] = {
        {STATE_IDLE, EVENT_START, STATE_READING},
        {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING},
        {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING},
        {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_READING},
        {STATE_READING, EVENT_ERROR, STATE_ERROR},
        {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
        {STATE_WRITING, EVENT_ERROR, STATE_ERROR},
        {STATE_ERROR, EVENT_RESET, STATE_IDLE},
        {STATE_READING, EVENT_RESET, STATE_IDLE},
        {STATE_PROCESSING, EVENT_RESET, STATE_IDLE},
        {STATE_WRITING, EVENT_RESET, STATE_IDLE},
        {STATE_DONE, EVENT_RESET, STATE_IDLE}
    };
    static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            sm->current_state = transitions[i].next;
            return 1;
        }
    }
    return 0;
}

void state_machine_execute_state(struct StateMachine *sm) {
    switch (sm->current_state) {
        case STATE_IDLE:
            printf("State: IDLE - Ready to start processing\n");
            break;
        case STATE_READING:
            if (sm->data_count < sm->max_data) {
                printf("State: READING - Enter data: ");
                if (fgets(sm->buffer, sizeof(sm->buffer), stdin) != NULL) {
                    size_t len = strlen(sm->buffer);
                    if (len > 0 && sm->buffer[len-1] == '\n') sm->buffer[len-1] = '\0';
                    if (state_machine_validate_input(sm->buffer)) {
                        sm->data_count++;
                        printf("Data accepted. Total: %u/%u\n", sm->data_count, sm->max_data);
                    } else {
                        printf("Invalid input. Only alphanumeric characters and spaces allowed.\n");
                    }
                }
            } else {
                printf("State: READING - Maximum data count reached\n");
            }
            break;
        case STATE_PROCESSING:
            if (sm->data_count > 0) {
                printf("State: PROCESSING - Processing data: %s\n", sm->buffer);
                for (size_t i = 0; sm->buffer[i] != '\0'; i++) {
                    if (islower((unsigned char)sm->buffer[i])) {
                        sm->buffer[i] = (char)toupper((unsigned char)sm->buffer[i]);
                    }
                }
                sm->processed_count++;
                sm->data_count--;
            }
            break;
        case STATE_WRITING:
            printf("State: WRITING - Output: %s\n", sm->buffer);
            sm->buffer[0] = '\0';