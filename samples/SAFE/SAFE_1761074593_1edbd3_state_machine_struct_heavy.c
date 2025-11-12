//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: state_machine
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
                }
                sm->current_state = STATE_ERROR;
                return -1;
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
                    sm->counter = sm->buffer_len;
                    sm->current_state = STATE_COMPLETE;
                    return 0;
                }
                sm->current_state = STATE_ERROR;
                return -1;
            }
            break;
            
        case STATE_COMPLETE:
            if (event->type == 4) {
                state_machine_init(sm);
                return 0;
            }
            break;
            
        case STATE_ERROR:
            if (event->type == 5) {
                state_machine_init(sm);
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
    printf("\n");
    
    if (sm->current_state == STATE_COMPLETE) {
        printf("Processed data: %s\n", sm->buffer);
        printf("Character count: %u\n", sm->counter);
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    printf("State Machine Demo\n");
    printf("Available events:\n");
    printf("1: Start (IDLE -> READY)\n");
    printf("2: Load data (READY -> PROCESSING)\n");
    printf("3: Process (PROCESSING -> COMPLETE)\n");
    printf("4: Reset (COMPLETE -> IDLE)\n");
    printf("5: Reset from error (ERROR -> IDLE)\n");
    printf("0: Exit\n\n");
    
    int running = 1;
    while (running) {
        print_state_info(&sm);
        printf("\nEnter event type (0-5): ");
        
        int event_type;
        if (scanf("%d", &event_type) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (event_type == 0) {
            running = 0;
            continue;
        }
        
        struct Event event;
        event.type = event_type;
        event.data[0] = '\0';
        
        if (event_type == 2 && sm.current_state == STATE_READY) {
            printf("Enter data to process (max 31 chars): ");
            if (scanf("%31s", event.data) != 1) {
                printf("Failed to read data\n");
                continue;
            }
        }
        
        int result = state_machine_handle_event(&sm, &event);
        if (result != 0) {
            printf("Event %d not allowed in current state or invalid\n", event_type);
        }
        
        printf("\n");
    }
    
    printf("Exiting state machine demo\n");
    return 0;
}