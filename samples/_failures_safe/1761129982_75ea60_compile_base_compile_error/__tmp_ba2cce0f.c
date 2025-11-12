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
            }
            break;
            
        case STATE_COMPLETE:
            if (event->type == 4) {
                sm->current_state = STATE_IDLE;
                sm->buffer[0] = '\0';
                sm->buffer_len = 0;
                return 0;
            }
            break;
            
        case STATE_ERROR:
            if (event->type == 5) {
                sm->current_state = STATE_IDLE;
                sm->buffer[0] = '\0';
                sm->buffer_len = 0;
                sm->counter = 0;
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
    
    if (sm->buffer_len > 0) {
        printf("Buffer: %s\n", sm->buffer);
    }
    printf("Counter: %u\n", sm->counter);
}

int main(void) {
    struct StateMachine sm;
    struct Event event;
    char input[64];
    int choice;
    
    state_machine_init(&sm);
    
    printf("State Machine Demo\n");
    printf("Available events:\n");
    printf("1: Start (IDLE -> READY)\n");
    printf("2: Load Data (READY -> PROCESSING)\n");
    printf("3: Process (PROCESSING -> COMPLETE)\n");
    printf("4: Reset (COMPLETE -> IDLE)\n");
    printf("5: Reset Error (ERROR -> IDLE)\n");
    printf("0: Exit\n\n");
    
    while (1) {
        print_state_info(&sm);
        printf("\nEnter event type (0-5): ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input\n");
            continue;
        }
        
        if (choice == 0) break;
        
        if (choice < 1 || choice > 5) {
            printf("Invalid event type\n");
            continue;
        }
        
        event.type = choice;
        event.data[0] = '\0';
        
        if (choice == 2) {
            printf("Enter data to process: ");
            if (fgets(input, sizeof(input), stdin) != NULL) {
                size_t len = strlen(input);
                if (len > 0 && input[len-1] == '\n') {
                    input[len-1] = '\0';
                }
                strncpy(event.data, input, sizeof(event.data) - 1);
                event.data[sizeof(event.data) - 1] = '\0';
            }
        }