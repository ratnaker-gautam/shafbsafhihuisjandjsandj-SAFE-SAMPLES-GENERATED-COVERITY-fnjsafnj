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
    uint32_t data_buffer[16];
    size_t data_count;
    uint32_t processed_data[16];
    size_t processed_count;
    uint32_t error_code;
};

struct EventData {
    enum Event event_type;
    uint32_t value;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->processed_count = 0;
    sm->error_code = 0;
    memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
    memset(sm->processed_data, 0, sizeof(sm->processed_data));
}

int state_machine_handle_event(struct StateMachine *sm, struct EventData *event) {
    if (sm == NULL || event == NULL) return 0;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event->event_type == EVENT_START) {
                sm->current_state = STATE_READING;
                sm->data_count = 0;
                return 1;
            }
            break;
            
        case STATE_READING:
            if (event->event_type == EVENT_DATA_READY) {
                if (sm->data_count < 16) {
                    sm->data_buffer[sm->data_count] = event->value;
                    sm->data_count++;
                    if (sm->data_count >= 16) {
                        sm->current_state = STATE_PROCESSING;
                    }
                    return 1;
                } else {
                    sm->current_state = STATE_ERROR;
                    sm->error_code = 1;
                    return 0;
                }
            } else if (event->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
                sm->error_code = event->value;
                return 0;
            }
            break;
            
        case STATE_PROCESSING:
            if (event->event_type == EVENT_PROCESS_COMPLETE) {
                sm->processed_count = 0;
                for (size_t i = 0; i < sm->data_count && sm->processed_count < 16; i++) {
                    if (sm->data_buffer[i] > 1000) continue;
                    sm->processed_data[sm->processed_count] = sm->data_buffer[i] * 2;
                    sm->processed_count++;
                }
                sm->current_state = STATE_WRITING;
                return 1;
            } else if (event->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
                sm->error_code = event->value;
                return 0;
            }
            break;
            
        case STATE_WRITING:
            if (event->event_type == EVENT_WRITE_COMPLETE) {
                sm->current_state = STATE_DONE;
                return 1;
            } else if (event->event_type == EVENT_ERROR) {
                sm->current_state = STATE_ERROR;
                sm->error_code = event->value;
                return 0;
            }
            break;
            
        case STATE_ERROR:
            if (event->event_type == EVENT_RESET) {
                state_machine_init(sm);
                return 1;
            }
            break;
            
        case STATE_DONE:
            if (event->event_type == EVENT_RESET) {
                state_machine_init(sm);
                return 1;
            }
            break;
    }
    
    return 0;
}

void print_state_machine_status(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Current state: ");
    switch (sm->current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_WRITING: printf("WRITING"); break;
        case STATE_ERROR: printf("ERROR"); break;
        case STATE_DONE: printf("DONE"); break;
    }
    printf("\n");
    
    if (sm->current_state == STATE_ERROR) {
        printf("Error code: %u\n", sm->error_code);
    }
    
    if (sm->data_count > 0) {
        printf("Data buffer (%zu items): ", sm->data_count);
        for (size_t i = 0; i < sm->data_count && i < 16; i++) {
            printf("%u ", sm->data_buffer[i]);
        }
        printf("\n");
    }
    
    if (sm->processed_count > 0) {
        printf("Processed data (%zu items): ", sm->processed_count);
        for (size_t i = 0; i < sm->processed_count && i < 16; i++) {
            printf("%u ", sm->processed_data[i]);
        }
        printf("\n");
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    printf("State Machine Demo\n");
    printf("Available events: 0=START,