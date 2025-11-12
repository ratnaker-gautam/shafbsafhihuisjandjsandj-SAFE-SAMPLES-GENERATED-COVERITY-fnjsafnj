//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum State {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
};

struct Event {
    int type;
    int data;
};

struct StateMachine {
    enum State current_state;
    int data_buffer[16];
    size_t data_count;
    int result;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->result = 0;
    memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
}

int state_machine_handle_event(struct StateMachine *sm, struct Event *evt) {
    if (sm == NULL || evt == NULL) return -1;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (evt->type == 1) {
                sm->current_state = STATE_READY;
                sm->data_count = 0;
            }
            break;
            
        case STATE_READY:
            if (evt->type == 2) {
                if (sm->data_count < 16) {
                    sm->data_buffer[sm->data_count] = evt->data;
                    sm->data_count++;
                }
                if (sm->data_count >= 3) {
                    sm->current_state = STATE_PROCESSING;
                }
            } else if (evt->type == 0) {
                sm->current_state = STATE_IDLE;
            }
            break;
            
        case STATE_PROCESSING:
            if (evt->type == 3) {
                int sum = 0;
                for (size_t i = 0; i < sm->data_count; i++) {
                    if (sm->data_buffer[i] > 0 && sum > INT_MAX - sm->data_buffer[i]) {
                        sm->current_state = STATE_ERROR;
                        return -1;
                    }
                    sum += sm->data_buffer[i];
                }
                sm->result = sum;
                sm->current_state = STATE_COMPLETE;
            } else if (evt->type == 0) {
                sm->current_state = STATE_IDLE;
            }
            break;
            
        case STATE_COMPLETE:
            if (evt->type == 0) {
                sm->current_state = STATE_IDLE;
            }
            break;
            
        case STATE_ERROR:
            if (evt->type == 0) {
                sm->current_state = STATE_IDLE;
            }
            break;
    }
    
    return 0;
}

const char* state_to_string(enum State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READY: return "READY";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_COMPLETE: return "COMPLETE";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    printf("State Machine Demo\n");
    printf("Commands: 0=Reset, 1=Start, 2=Add Data, 3=Process, 4=Quit\n");
    
    while (1) {
        printf("\nCurrent state: %s", state_to_string(sm.current_state));
        if (sm.current_state == STATE_COMPLETE) {
            printf(" (Result: %d)", sm.result);
        }
        printf("\nEnter command and data (0-4, data): ");
        
        int command, data;
        if (scanf("%d", &command) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (command == 4) {
            break;
        }
        
        if (command == 2) {
            if (scanf("%d", &data) != 1) {
                while (getchar() != '\n');
                printf("Invalid data\n");
                continue;
            }
        } else {
            data = 0;
        }
        
        struct Event evt = {command, data};
        if (state_machine_handle_event(&sm, &evt) != 0) {
            printf("Error handling event\n");
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}