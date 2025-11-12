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
    char data[64];
    uint32_t data_len;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 10;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int validate_event_data(const struct EventData *event_data) {
    if (event_data == NULL) return 0;
    if (event_data->event_type < EVENT_START || event_data->event_type > EVENT_RESET) return 0;
    if (event_data->data_len >= sizeof(event_data->data)) return 0;
    return 1;
}

int safe_strncpy(char *dest, const char *src, size_t dest_size) {
    if (dest == NULL || src == NULL || dest_size == 0) return 0;
    size_t i;
    for (i = 0; i < dest_size - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

void state_machine_handle_event(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) {
        return;
    }

    switch (sm->current_state) {
        case STATE_IDLE:
            if (event_data->event_type == EVENT_START) {
                sm->current_state = STATE_READING;
                sm->data_count = 0;
                sm->buffer_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
            }
            break;
            
        case STATE_READING:
            if (event_data->event_type == EVENT_DATA_READY) {
                if (sm->buffer_pos + event_data->data_len < sizeof(sm->buffer)) {
                    memcpy(sm->buffer + sm->buffer_pos, event_data->data, event_data->data_len);
                    sm->buffer_pos += event_data->data_len;
                    sm->current_state = STATE_PROCESSING;
                } else {
                    sm->current_state = STATE_ERROR;
                }
            } else if (event_data->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event_data->event_type == EVENT_PROCESS_COMPLETE) {
                if (sm->data_count < sm->max_data) {
                    sm->data_count++;
                    sm->current_state = STATE_WRITING;
                } else {
                    sm->current_state = STATE_DONE;
                }
            } else if (event_data->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_WRITING:
            if (event_data->event_type == EVENT_WRITE_COMPLETE) {
                sm->current_state = STATE_READING;
            } else if (event_data->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            if (event_data->event_type == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_count = 0;
                sm->buffer_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
            }
            break;
            
        case STATE_DONE:
            if (event_data->event_type == EVENT_RESET) {
                sm->current_state = STATE_IDLE;
                sm->data_count = 0;
                sm->buffer_pos = 0;
                memset(sm->buffer, 0, sizeof(sm->buffer));
            }
            break;
    }
}

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR", "DONE"
    };
    
    printf("Current State: %s\n", state_names[sm->current_state]);
    printf("Data Count: %u/%u\n", sm->data_count, sm->max_data);
    printf("Buffer Size: %u/256\n", sm->buffer_pos);
    if (sm->buffer_pos > 0) {
        printf("Buffer: ");
        for (uint32_t i = 0; i < sm->buffer_pos && i < 32; i++) {
            if (isprint(sm->buffer[i])) {
                putchar(sm->buffer[i]);
            } else {
                putchar('.');
            }
        }
        printf("\n");
    }
    printf("---\n");
}

int main(void) {