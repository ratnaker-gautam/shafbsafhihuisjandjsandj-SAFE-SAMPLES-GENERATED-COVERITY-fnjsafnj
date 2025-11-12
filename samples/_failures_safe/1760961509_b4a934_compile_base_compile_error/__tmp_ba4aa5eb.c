//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAIL,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[256];
    size_t data_length;
} StateMachine;

typedef void (*StateHandler)(StateMachine*, Event);

void handle_idle(StateMachine* sm, Event event);
void handle_ready(StateMachine* sm, Event event);
void handle_processing(StateMachine* sm, Event event);
void handle_done(StateMachine* sm, Event event);
void handle_error(StateMachine* sm, Event event);

StateHandler handlers[] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_done,
    handle_error
};

void handle_idle(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_START:
            printf("State: IDLE -> READY\n");
            sm->current_state = STATE_READY;
            sm->data_length = 0;
            break;
        case EVENT_RESET:
            printf("Already in IDLE state\n");
            break;
        default:
            printf("Invalid event for IDLE state\n");
            sm->current_state = STATE_ERROR;
    }
}

void handle_ready(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_DATA:
            printf("Enter data (max 255 chars): ");
            if (fgets(sm->buffer, sizeof(sm->buffer), stdin) != NULL) {
                sm->data_length = strlen(sm->buffer);
                if (sm->data_length > 0 && sm->buffer[sm->data_length-1] == '\n') {
                    sm->buffer[--sm->data_length] = '\0';
                }
                if (sm->data_length > 0) {
                    printf("State: READY -> PROCESSING\n");
                    sm->current_state = STATE_PROCESSING;
                } else {
                    printf("Empty input, staying in READY\n");
                }
            } else {
                printf("Input error\n");
                sm->current_state = STATE_ERROR;
            }
            break;
        case EVENT_RESET:
            printf("State: READY -> IDLE\n");
            sm->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for READY state\n");
            sm->current_state = STATE_ERROR;
    }
}

void handle_processing(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_PROCESS:
            if (sm->data_length > 0) {
                printf("Processing data: '");
                for (size_t i = 0; i < sm->data_length; i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        putchar(sm->buffer[i] - 32);
                    } else {
                        putchar(sm->buffer[i]);
                    }
                }
                printf("'\n");
                printf("State: PROCESSING -> DONE\n");
                sm->current_state = STATE_DONE;
            } else {
                printf("No data to process\n");
                sm->current_state = STATE_ERROR;
            }
            break;
        case EVENT_RESET:
            printf("State: PROCESSING -> IDLE\n");
            sm->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for PROCESSING state\n");
            sm->current_state = STATE_ERROR;
    }
}

void handle_done(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_SUCCESS:
            printf("Processing completed successfully\n");
            printf("State: DONE -> IDLE\n");
            sm->current_state = STATE_IDLE;
            break;
        case EVENT_RESET:
            printf("State: DONE -> IDLE\n");
            sm->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for DONE state\n");
            sm->current_state = STATE_ERROR;
    }
}

void handle_error(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_RESET:
            printf("State: ERROR -> IDLE\n");
            sm->current_state = STATE_IDLE;
            break;
        default:
            printf("Reset required from ERROR state\n");
    }
}

void process_event(StateMachine* sm, Event event) {
    if (sm == NULL) return;
    if (sm->current_state < STATE_IDLE || sm->current_state > STATE_ERROR) {
        sm->current_state = STATE_ERROR;
        return;
    }
    handlers[sm->current_state](sm, event);
}

int main() {
    StateMachine sm = {STATE_IDLE, "", 0};
    char input[32];
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Commands: start, data, process, success, fail, reset, quit\n");
    
    while (running) {
        printf("\nCurrent state: ");
        switch(sm.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_DONE: printf("DONE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }