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

struct EventData {
    enum Event event_type;
    char data[64];
    uint32_t data_len;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 100;
    sm->buffer_pos = 0;
    sm->processed_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int validate_event_data(const struct EventData *event_data) {
    if (event_data == NULL) return 0;
    if (event_data->event_type < EVENT_START || event_data->event_type > EVENT_RESET) return 0;
    if (event_data->data_len >= sizeof(event_data->data)) return 0;
    return 1;
}

int handle_idle_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return 0;
    
    if (event_data->event_type == EVENT_START) {
        sm->current_state = STATE_READING;
        printf("Transition: IDLE -> READING\n");
        return 1;
    }
    return 0;
}

int handle_reading_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return 0;
    
    if (event_data->event_type == EVENT_DATA_READY) {
        if (sm->data_count >= sm->max_data) {
            sm->current_state = STATE_ERROR;
            printf("Transition: READING -> ERROR (data limit exceeded)\n");
            return 1;
        }
        
        if (event_data->data_len > 0) {
            size_t copy_len = event_data->data_len;
            if (copy_len > sizeof(sm->buffer) - sm->buffer_pos - 1) {
                copy_len = sizeof(sm->buffer) - sm->buffer_pos - 1;
            }
            memcpy(sm->buffer + sm->buffer_pos, event_data->data, copy_len);
            sm->buffer_pos += copy_len;
            sm->buffer[sm->buffer_pos] = '\0';
            sm->data_count++;
        }
        
        sm->current_state = STATE_PROCESSING;
        printf("Transition: READING -> PROCESSING (data count: %u)\n", sm->data_count);
        return 1;
    } else if (event_data->event_type == EVENT_ERROR) {
        sm->current_state = STATE_ERROR;
        printf("Transition: READING -> ERROR\n");
        return 1;
    }
    return 0;
}

int handle_processing_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return 0;
    
    if (event_data->event_type == EVENT_PROCESS_COMPLETE) {
        for (uint32_t i = 0; i < sm->buffer_pos && i < sizeof(sm->buffer); i++) {
            if (isalpha(sm->buffer[i])) {
                sm->buffer[i] = toupper(sm->buffer[i]);
            }
        }
        sm->processed_count++;
        
        sm->current_state = STATE_WRITING;
        printf("Transition: PROCESSING -> WRITING (processed: %u)\n", sm->processed_count);
        return 1;
    } else if (event_data->event_type == EVENT_ERROR) {
        sm->current_state = STATE_ERROR;
        printf("Transition: PROCESSING -> ERROR\n");
        return 1;
    }
    return 0;
}

int handle_writing_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return 0;
    
    if (event_data->event_type == EVENT_WRITE_COMPLETE) {
        printf("Output: %s\n", sm->buffer);
        sm->buffer_pos = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
        
        if (sm->processed_count >= 3) {
            sm->current_state = STATE_DONE;
            printf("Transition: WRITING -> DONE\n");
        } else {
            sm->current_state = STATE_READING;
            printf("Transition: WRITING -> READING\n");
        }
        return 1;
    } else if (event_data->event_type == EVENT_ERROR) {
        sm->current_state = STATE_ERROR;
        printf("Transition: WRITING -> ERROR\n");
        return 1;
    }
    return 0;
}

int handle_error_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return 0;
    
    if (event_data->event