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
    if (sm == NULL || data == NULL) return -1;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event_type == 1) {
                if (data->value >= 0 && data->value < 100) {
                    sm->counter = data->value;
                    sm->current_state = STATE_READY;
                    return 0;
                }
                return -1;
            }
            break;
            
        case STATE_READY:
            if (event_type == 2) {
                if (strlen(data->message) < sizeof(sm->buffer) - 1) {
                    strncpy(sm->buffer, data->message, sizeof(sm->buffer) - 1);
                    sm->buffer[sizeof(sm->buffer) - 1] = '\0';
                    sm->current_state = STATE_PROCESSING;
                    return 0;
                }
                return -1;
            } else if (event_type == 0) {
                sm->current_state = STATE_IDLE;
                return 0;
            }
            break;
            
        case STATE_PROCESSING:
            if (event_type == 3) {
                if (sm->counter > 0) {
                    sm->counter--;
                    if (sm->counter == 0) {
                        sm->current_state = STATE_COMPLETE;
                    }
                    return 0;
                }
                return -1;
            } else if (event_type == 4) {
                sm->current_state = STATE_ERROR;
                return 0;
            }
            break;
            
        case STATE_COMPLETE:
            if (event_type == 0) {
                state_machine_init(sm);
                return 0;
            }
            break;
            
        case STATE_ERROR:
            if (event_type == 0) {
                state_machine_init(sm);
                return 0;
            }
            break;
    }
    
    return -1;
}

void print_state_info(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Current state: ");
    switch (sm->current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READY: printf("READY"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_COMPLETE: printf("COMPLETE"); break;
        case STATE_ERROR: printf("ERROR"); break;
    }
    printf(", Counter: %d, Buffer: %s\n", sm->counter, sm->buffer);
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct EventData data;
    int running = 1;
    int choice;
    
    printf("State Machine Demo\n");
    printf("0: Reset, 1: Set Counter, 2: Set Message, 3: Process, 4: Error, 5: Exit\n");
    
    while (running) {
        print_state_info(&sm);
        printf("Enter event type: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (choice == 5) {
            running = 0;
            continue;
        }
        
        switch (choice) {
            case 0:
                data.value = 0;
                strcpy(data.message, "");
                break;
            case 1:
                printf("Enter counter value (0-99): ");
                if (scanf("%d", &data.value) != 1) {
                    while (getchar() != '\n');
                    printf("Invalid input\n");
                    continue;
                }
                strcpy(data.message, "");
                break;
            case 2:
                printf("Enter message (max 31 chars): ");
                if (scanf("%31s", data.message) != 1) {
                    while (getchar() != '\n');
                    printf("Invalid input\n");
                    continue;
                }
                data.value = 0;
                break;
            case 3:
                data.value = 0;
                strcpy(data.message, "");
                break;
            case 4:
                data.value = 0;
                strcpy(data.message, "");
                break;
            default:
                printf("Invalid event type\n");
                continue;
        }
        
        if (state_machine_handle_event(&sm, choice, &data) != 0) {
            printf("Event rejected in current state\n");
        }
    }
    
    return 0;
}