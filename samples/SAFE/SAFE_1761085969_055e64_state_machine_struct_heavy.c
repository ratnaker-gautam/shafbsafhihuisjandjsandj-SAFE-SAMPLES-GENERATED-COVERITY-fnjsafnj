//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
};

struct StateMachine {
    enum State current_state;
    uint32_t data_count;
    char buffer[256];
    size_t buffer_pos;
    uint32_t processed_items;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_pos = 0;
    sm->processed_items = 0;
}

int state_machine_transition(struct StateMachine *sm, char input) {
    if (sm == NULL) return -1;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (input == 'S') {
                sm->current_state = STATE_READING;
                sm->data_count = 0;
                sm->processed_items = 0;
            }
            break;
            
        case STATE_READING:
            if (input == 'E') {
                if (sm->data_count > 0) {
                    sm->current_state = STATE_PROCESSING;
                } else {
                    sm->current_state = STATE_ERROR;
                }
            } else if (isdigit(input)) {
                if (sm->data_count < 100) {
                    sm->data_count = sm->data_count * 10 + (input - '0');
                    if (sm->data_count > 1000) {
                        sm->current_state = STATE_ERROR;
                    }
                }
            } else {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (isalpha(input)) {
                if (sm->buffer_pos < sizeof(sm->buffer) - 1) {
                    sm->buffer[sm->buffer_pos++] = input;
                    sm->processed_items++;
                    if (sm->processed_items >= sm->data_count) {
                        sm->current_state = STATE_COMPLETE;
                    }
                } else {
                    sm->current_state = STATE_ERROR;
                }
            } else {
                sm->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_COMPLETE:
        case STATE_ERROR:
            break;
    }
    
    return 0;
}

void state_machine_process(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    if (sm->current_state == STATE_COMPLETE) {
        printf("Processing complete. Data: ");
        for (size_t i = 0; i < sm->buffer_pos; i++) {
            putchar(sm->buffer[i]);
        }
        printf("\nProcessed %u items.\n", sm->processed_items);
    } else if (sm->current_state == STATE_ERROR) {
        printf("Error in state machine processing.\n");
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    printf("Enter commands: S to start, digits for count, E to end, letters for data\n");
    
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n') {
        if (ch == 'Q') break;
        
        if (state_machine_transition(&sm, (char)ch) != 0) {
            printf("State transition error.\n");
            return 1;
        }
        
        if (sm.current_state == STATE_COMPLETE || sm.current_state == STATE_ERROR) {
            break;
        }
    }
    
    state_machine_process(&sm);
    
    return 0;
}