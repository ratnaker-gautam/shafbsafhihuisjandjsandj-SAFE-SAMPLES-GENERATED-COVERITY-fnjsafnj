//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
};

struct Event {
    int type;
    char data[32];
};

struct StateMachine {
    enum State current_state;
    uint32_t counter;
    char buffer[256];
    size_t buffer_len;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->counter = 0;
    sm->buffer[0] = '\0';
    sm->buffer_len = 0;
}

int state_machine_handle_event(struct StateMachine *sm, const struct Event *event) {
    if (sm == NULL || event == NULL) return -1;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event->type == 1) {
                sm->current_state = STATE_READY;
                sm->counter = 0;
                return 0;
            }
            break;
            
        case STATE_READY:
            if (event->type == 2) {
                if (strlen(event->data) < sizeof(sm->buffer)) {
                    strncpy(sm->buffer, event->data, sizeof(sm->buffer) - 1);
                    sm->buffer[sizeof(sm->buffer) - 1] = '\0';
                    sm->buffer_len = strlen(sm->buffer);
                    sm->current_state = STATE_PROCESSING;
                    return 0;
                } else {
                    sm->current_state = STATE_ERROR;
                    return -1;
                }
            } else if (event->type == 0) {
                sm->current_state = STATE_IDLE;
                return 0;
            }
            break;
            
        case STATE_PROCESSING:
            if (event->type == 3) {
                if (sm->buffer_len > 0) {
                    for (size_t i = 0; i < sm->buffer_len; i++) {
                        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                        }
                    }
                    sm->counter++;
                    sm->current_state = STATE_COMPLETE;
                    return 0;
                } else {
                    sm->current_state = STATE_ERROR;
                    return -1;
                }
            } else if (event->type == 0) {
                sm->current_state = STATE_IDLE;
                return 0;
            }
            break;
            
        case STATE_COMPLETE:
            if (event->type == 0) {
                sm->current_state = STATE_IDLE;
                return 0;
            } else if (event->type == 1) {
                sm->current_state = STATE_READY;
                return 0;
            }
            break;
            
        case STATE_ERROR:
            if (event->type == 0) {
                sm->current_state = STATE_IDLE;
                return 0;
            }
            break;
    }
    
    return -1;
}

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Current state: ");
    switch (sm->current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READY: printf("READY"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_COMPLETE: printf("COMPLETE"); break;
        case STATE_ERROR: printf("ERROR"); break;
    }
    printf("\nCounter: %u\n", sm->counter);
    printf("Buffer: %s\n", sm->buffer);
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    printf("State Machine Demo\n");
    printf("Available events: 0=Reset, 1=Start, 2=Load, 3=Process\n");
    printf("Enter event type and data (for Load events):\n");
    
    char input[64];
    while (1) {
        printf("\n> ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        if (strlen(input) == 0) continue;
        
        struct Event event;
        event.type = -1;
        event.data[0] = '\0';
        
        char *token = strtok(input, " \n");
        if (token != NULL) {
            int num = atoi(token);
            if (num >= 0 && num <= 3) {
                event.type = num;
            }
        }
        
        if (event.type == 2) {
            token = strtok(NULL, "\n");
            if (token != NULL && strlen(token) < sizeof(event.data)) {
                strncpy(event.data, token, sizeof(event.data) - 1);
                event.data[sizeof(event.data) - 1] = '\0';
            } else if (token != NULL) {
                printf("Error: Data too long\n");
                continue;
            }
        }
        
        if (event.type == -1) {
            printf("Invalid event type\n");
            continue;
        }
        
        int result = state_machine_handle_event(&sm, &event);
        if (result != 0) {
            printf("Event handling failed\n");
        }
        
        print_state_info(&sm);
        
        if (sm