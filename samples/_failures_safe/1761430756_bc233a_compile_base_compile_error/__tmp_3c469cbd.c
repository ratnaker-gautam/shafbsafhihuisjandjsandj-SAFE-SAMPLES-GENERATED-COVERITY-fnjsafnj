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

struct StateMachine {
    enum State current_state;
    uint32_t data_value;
    char buffer[64];
    size_t buffer_pos;
    uint32_t processed_count;
};

struct InputEvent {
    int type;
    uint32_t value;
    char text[32];
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_pos = 0;
    sm->processed_count = 0;
}

int validate_input_event(const struct InputEvent *event) {
    if (event == NULL) return 0;
    if (event->type < 0 || event->type > 3) return 0;
    if (event->value > 1000000) return 0;
    for (size_t i = 0; i < sizeof(event->text); i++) {
        if (event->text[i] == '\0') break;
        if (event->text[i] < 32 || event->text[i] > 126) return 0;
    }
    return 1;
}

enum State handle_idle_state(struct StateMachine *sm, const struct InputEvent *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    if (event->type == 1) {
        sm->data_value = event->value;
        return STATE_READING;
    }
    return STATE_IDLE;
}

enum State handle_reading_state(struct StateMachine *sm, const struct InputEvent *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    if (event->type == 2) {
        size_t text_len = strlen(event->text);
        if (text_len > 0 && sm->buffer_pos + text_len < sizeof(sm->buffer)) {
            memcpy(sm->buffer + sm->buffer_pos, event->text, text_len);
            sm->buffer_pos += text_len;
            sm->buffer[sm->buffer_pos] = '\0';
        }
        if (sm->buffer_pos >= 10) {
            return STATE_PROCESSING;
        }
    }
    return STATE_READING;
}

enum State handle_processing_state(struct StateMachine *sm, const struct InputEvent *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    if (event->type == 3) {
        for (size_t i = 0; i < sm->buffer_pos; i++) {
            if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
            }
        }
        sm->processed_count++;
        return STATE_WRITING;
    }
    return STATE_PROCESSING;
}

enum State handle_writing_state(struct StateMachine *sm, const struct InputEvent *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    if (event->type == 0) {
        printf("Data: %u, Processed: %s, Count: %u\n", 
               sm->data_value, sm->buffer, sm->processed_count);
        memset(sm->buffer, 0, sizeof(sm->buffer));
        sm->buffer_pos = 0;
        return STATE_IDLE;
    }
    return STATE_WRITING;
}

enum State handle_error_state(struct StateMachine *sm, const struct InputEvent *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    if (event->type == 0) {
        state_machine_init(sm);
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

void process_event(struct StateMachine *sm, const struct InputEvent *event) {
    if (sm == NULL || event == NULL) return;
    if (!validate_input_event(event)) {
        sm->current_state = STATE_ERROR;
        return;
    }
    
    switch (sm->current_state) {
        case STATE_IDLE:
            sm->current_state = handle_idle_state(sm, event);
            break;
        case STATE_READING:
            sm->current_state = handle_reading_state(sm, event);
            break;
        case STATE_PROCESSING:
            sm->current_state = handle_processing_state(sm, event);
            break;
        case STATE_WRITING:
            sm->current_state = handle_writing_state(sm, event);
            break;
        case STATE_ERROR:
            sm->current_state = handle_error_state(sm, event);
            break;
        default:
            sm->current_state = STATE_ERROR;
            break;
    }
}

const char* state_to_string(enum State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READING: return "READING";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_WRITING: return "WRITING";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct InputEvent events[] = {
        {1, 100