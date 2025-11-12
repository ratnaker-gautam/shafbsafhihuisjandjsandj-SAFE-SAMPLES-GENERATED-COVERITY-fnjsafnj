//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_COUNT
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
};

struct StateMachine {
    enum State current_state;
    uint32_t data_value;
    uint32_t processed_value;
    uint32_t error_count;
    uint32_t total_operations;
};

struct TransitionTable {
    enum State next_state[STATE_COUNT][EVENT_COUNT];
};

struct InputBuffer {
    char buffer[64];
    size_t position;
};

void initialize_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->processed_value = 0;
    sm->error_count = 0;
    sm->total_operations = 0;
}

void initialize_transition_table(struct TransitionTable *tt) {
    if (tt == NULL) return;
    
    for (int i = 0; i < STATE_COUNT; i++) {
        for (int j = 0; j < EVENT_COUNT; j++) {
            tt->next_state[i][j] = STATE_ERROR;
        }
    }
    
    tt->next_state[STATE_IDLE][EVENT_START] = STATE_READING;
    tt->next_state[STATE_IDLE][EVENT_RESET] = STATE_IDLE;
    
    tt->next_state[STATE_READING][EVENT_DATA_READY] = STATE_PROCESSING;
    tt->next_state[STATE_READING][EVENT_ERROR] = STATE_ERROR;
    tt->next_state[STATE_READING][EVENT_RESET] = STATE_IDLE;
    
    tt->next_state[STATE_PROCESSING][EVENT_PROCESS_COMPLETE] = STATE_WRITING;
    tt->next_state[STATE_PROCESSING][EVENT_ERROR] = STATE_ERROR;
    tt->next_state[STATE_PROCESSING][EVENT_RESET] = STATE_IDLE;
    
    tt->next_state[STATE_WRITING][EVENT_WRITE_COMPLETE] = STATE_IDLE;
    tt->next_state[STATE_WRITING][EVENT_ERROR] = STATE_ERROR;
    tt->next_state[STATE_WRITING][EVENT_RESET] = STATE_IDLE;
    
    tt->next_state[STATE_ERROR][EVENT_RESET] = STATE_IDLE;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= 64) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

uint32_t safe_atoi(const char *str, size_t len) {
    if (str == NULL || len == 0) return 0;
    
    uint32_t result = 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            if (result > UINT32_MAX / 10) return UINT32_MAX;
            result = result * 10;
            if (result > UINT32_MAX - (str[i] - '0')) return UINT32_MAX;
            result += (str[i] - '0');
        }
    }
    return result;
}

enum Event get_user_input(struct InputBuffer *ibuf) {
    if (ibuf == NULL) return EVENT_ERROR;
    
    printf("Enter command (start, data, process, write, error, reset, quit): ");
    
    if (fgets(ibuf->buffer, sizeof(ibuf->buffer), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    ibuf->position = strcspn(ibuf->buffer, "\n");
    if (ibuf->position < sizeof(ibuf->buffer)) {
        ibuf->buffer[ibuf->position] = '\0';
    }
    
    if (strcmp(ibuf->buffer, "start") == 0) return EVENT_START;
    if (strcmp(ibuf->buffer, "data") == 0) return EVENT_DATA_READY;
    if (strcmp(ibuf->buffer, "process") == 0) return EVENT_PROCESS_COMPLETE;
    if (strcmp(ibuf->buffer, "write") == 0) return EVENT_WRITE_COMPLETE;
    if (strcmp(ibuf->buffer, "error") == 0) return EVENT_ERROR;
    if (strcmp(ibuf->buffer, "reset") == 0) return EVENT_RESET;
    if (strcmp(ibuf->buffer, "quit") == 0) return EVENT_COUNT;
    
    return EVENT_ERROR;
}

void process_state_transition(struct StateMachine *sm, struct TransitionTable *tt, enum Event event) {
    if (sm == NULL || tt == NULL || event >= EVENT_COUNT) return;
    
    enum State old_state = sm->current_state;
    sm->current_state = tt->next_state[old_state][