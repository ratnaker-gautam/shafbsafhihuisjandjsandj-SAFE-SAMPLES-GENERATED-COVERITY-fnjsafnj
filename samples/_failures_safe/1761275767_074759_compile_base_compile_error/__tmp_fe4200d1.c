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
};

struct EventData {
    enum Event event_type;
    char data[128];
    uint32_t data_len;
};

struct StateTransition {
    enum State from_state;
    enum Event event;
    enum State to_state;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 10;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int is_valid_event(enum Event evt) {
    return (evt >= EVENT_START && evt <= EVENT_RESET);
}

int is_valid_state(enum State st) {
    return (st >= STATE_IDLE && st <= STATE_DONE);
}

int validate_transition(struct StateMachine *sm, enum Event evt) {
    if (sm == NULL || !is_valid_state(sm->current_state) || !is_valid_event(evt)) {
        return 0;
    }
    
    struct StateTransition transitions[] = {
        {STATE_IDLE, EVENT_START, STATE_READING},
        {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING},
        {STATE_READING, EVENT_ERROR, STATE_ERROR},
        {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING},
        {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
        {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_READING},
        {STATE_WRITING, EVENT_ERROR, STATE_ERROR},
        {STATE_ERROR, EVENT_RESET, STATE_IDLE},
        {STATE_READING, EVENT_RESET, STATE_IDLE},
        {STATE_WRITING, EVENT_RESET, STATE_IDLE},
        {STATE_PROCESSING, EVENT_RESET, STATE_IDLE}
    };
    
    size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].from_state == sm->current_state && 
            transitions[i].event == evt) {
            return 1;
        }
    }
    return 0;
}

void process_state_idle(struct StateMachine *sm, struct EventData *evt_data) {
    printf("State: IDLE\n");
    if (evt_data != NULL && evt_data->event_type == EVENT_START) {
        printf("Starting data processing...\n");
        sm->data_count = 0;
        sm->buffer_pos = 0;
    }
}

void process_state_reading(struct StateMachine *sm, struct EventData *evt_data) {
    printf("State: READING\n");
    if (evt_data != NULL && evt_data->event_type == EVENT_DATA_READY) {
        if (evt_data->data_len > 0 && evt_data->data_len < sizeof(sm->buffer) - sm->buffer_pos) {
            memcpy(sm->buffer + sm->buffer_pos, evt_data->data, evt_data->data_len);
            sm->buffer_pos += evt_data->data_len;
            printf("Read %u bytes of data\n", evt_data->data_len);
        }
    }
}

void process_state_processing(struct StateMachine *sm, struct EventData *evt_data) {
    printf("State: PROCESSING\n");
    if (sm->buffer_pos > 0) {
        for (uint32_t i = 0; i < sm->buffer_pos; i++) {
            if (isalpha(sm->buffer[i])) {
                sm->buffer[i] = toupper(sm->buffer[i]);
            }
        }
        printf("Processed %u bytes\n", sm->buffer_pos);
    }
}

void process_state_writing(struct StateMachine *sm, struct EventData *evt_data) {
    printf("State: WRITING\n");
    if (sm->buffer_pos > 0) {
        printf("Output: ");
        for (uint32_t i = 0; i < sm->buffer_pos; i++) {
            putchar(sm->buffer[i]);
        }
        putchar('\n');
        sm->buffer_pos = 0;
        sm->data_count++;
    }
}

void process_state_error(struct StateMachine *sm, struct EventData *evt_data) {
    printf("State: ERROR\n");
    if (evt_data != NULL && evt_data->data_len > 0) {
        printf("Error: %.*s\n", (int)evt_data->data_len, evt_data->data);
    }
}

void process_state_done(struct StateMachine *sm, struct EventData *evt_data) {
    printf("State: DONE\n");
    printf("Processed %u data items\n", sm->data_count);
}

void