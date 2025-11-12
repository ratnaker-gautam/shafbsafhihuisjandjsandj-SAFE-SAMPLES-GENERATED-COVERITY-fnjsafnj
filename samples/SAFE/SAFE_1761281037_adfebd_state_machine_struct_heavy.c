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

struct EventData {
    int value;
    char message[32];
};

struct StateMachine {
    enum State current_state;
    int counter;
    char buffer[64];
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->counter = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_handle_event(struct StateMachine *sm, int event_type, struct EventData *data) {
    if (sm == NULL) return -1;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event_type == 1) {
                if (data != NULL && data->value > 0) {
                    sm->counter = data->value;
                    sm->current_state = STATE_READY;
                    return 0;
                }
                return -1;
            }
            break;
            
        case STATE_READY:
            if (event_type == 2) {
                if (data != NULL && strlen(data->message) < sizeof(sm->buffer)) {
                    strncpy(sm->buffer, data->message, sizeof(sm->buffer) - 1);
                    sm->buffer[sizeof(sm->buffer) - 1] = '\0';
                    sm->current_state = STATE_PROCESSING;
                    return 0;
                }
                return -1;
            }
            break;
            
        case STATE_PROCESSING:
            if (event_type == 3) {
                if (sm->counter > 0 && sm->counter < 1000) {
                    sm->counter *= 2;
                    if (sm->counter > 1000) {
                        sm->current_state = STATE_ERROR;
                        return -1;
                    }
                    sm->current_state = STATE_COMPLETE;
                    return 0;
                }
                return -1;
            }
            break;
            
        case STATE_COMPLETE:
            if (event_type == 4) {
                state_machine_init(sm);
                return 0;
            }
            break;
            
        case STATE_ERROR:
            if (event_type == 5) {
                state_machine_init(sm);
                return 0;
            }
            break;
    }
    
    return -1;
}

void print_state_machine_status(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Current state: ");
    switch (sm->current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READY: printf("READY"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_COMPLETE: printf("COMPLETE"); break;
        case STATE_ERROR: printf("ERROR"); break;
    }
    printf("\nCounter: %d\n", sm->counter);
    printf("Buffer: %s\n", sm->buffer);
}

int main(void) {
    struct StateMachine sm;
    struct EventData data;
    int choice;
    
    state_machine_init(&sm);
    
    printf("State Machine Demo\n");
    
    while (1) {
        printf("\n");
        print_state_machine_status(&sm);
        printf("\n1. Initialize (value > 0)\n");
        printf("2. Set message (max 31 chars)\n");
        printf("3. Process data\n");
        printf("4. Reset from complete\n");
        printf("5. Reset from error\n");
        printf("0. Exit\n");
        printf("Choose action: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (choice == 0) {
            break;
        }
        
        int result = -1;
        
        switch (choice) {
            case 1:
                printf("Enter initial value (1-999): ");
                if (scanf("%d", &data.value) == 1 && data.value > 0 && data.value < 1000) {
                    result = state_machine_handle_event(&sm, 1, &data);
                } else {
                    while (getchar() != '\n');
                    printf("Invalid value\n");
                }
                break;
                
            case 2:
                printf("Enter message: ");
                if (scanf("%31s", data.message) == 1) {
                    result = state_machine_handle_event(&sm, 2, &data);
                } else {
                    while (getchar() != '\n');
                    printf("Invalid message\n");
                }
                break;
                
            case 3:
                result = state_machine_handle_event(&sm, 3, NULL);
                break;
                
            case 4:
                result = state_machine_handle_event(&sm, 4, NULL);
                break;
                
            case 5:
                result = state_machine_handle_event(&sm, 5, NULL);
                break;
                
            default:
                printf("Invalid choice\n");
                continue;
        }
        
        if (result != 0) {
            printf("Operation failed - invalid state transition\n");
        }
    }
    
    return 0;
}