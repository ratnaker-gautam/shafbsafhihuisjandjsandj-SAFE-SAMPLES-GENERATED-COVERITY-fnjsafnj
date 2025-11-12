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
    EVENT_FAILURE,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[256];
    size_t data_length;
    int process_count;
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
            printf("Transition: IDLE -> READY\n");
            sm->current_state = STATE_READY;
            sm->data_length = 0;
            sm->process_count = 0;
            memset(sm->buffer, 0, sizeof(sm->buffer));
            break;
        case EVENT_RESET:
            printf("Already in IDLE state\n");
            break;
        default:
            printf("Invalid event for IDLE state\n");
            sm->current_state = STATE_ERROR;
            break;
    }
}

void handle_ready(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_DATA:
            if(sm->data_length < sizeof(sm->buffer) - 1) {
                char data[] = "sample_data";
                size_t data_len = strlen(data);
                if(sm->data_length + data_len < sizeof(sm->buffer)) {
                    memcpy(sm->buffer + sm->data_length, data, data_len);
                    sm->data_length += data_len;
                    printf("Data added. Current length: %zu\n", sm->data_length);
                }
            }
            break;
        case EVENT_PROCESS:
            if(sm->data_length > 0) {
                printf("Transition: READY -> PROCESSING\n");
                sm->current_state = STATE_PROCESSING;
            } else {
                printf("No data to process\n");
                sm->current_state = STATE_ERROR;
            }
            break;
        case EVENT_RESET:
            printf("Transition: READY -> IDLE\n");
            sm->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for READY state\n");
            sm->current_state = STATE_ERROR;
            break;
    }
}

void handle_processing(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_SUCCESS:
            sm->process_count++;
            printf("Processing successful. Count: %d\n", sm->process_count);
            printf("Transition: PROCESSING -> DONE\n");
            sm->current_state = STATE_DONE;
            break;
        case EVENT_FAILURE:
            printf("Processing failed\n");
            printf("Transition: PROCESSING -> ERROR\n");
            sm->current_state = STATE_ERROR;
            break;
        case EVENT_RESET:
            printf("Transition: PROCESSING -> IDLE\n");
            sm->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for PROCESSING state\n");
            sm->current_state = STATE_ERROR;
            break;
    }
}

void handle_done(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_START:
            printf("Transition: DONE -> READY\n");
            sm->current_state = STATE_READY;
            sm->data_length = 0;
            memset(sm->buffer, 0, sizeof(sm->buffer));
            break;
        case EVENT_RESET:
            printf("Transition: DONE -> IDLE\n");
            sm->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for DONE state\n");
            sm->current_state = STATE_ERROR;
            break;
    }
}

void handle_error(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_RESET:
            printf("Transition: ERROR -> IDLE\n");
            sm->current_state = STATE_IDLE;
            sm->data_length = 0;
            sm->process_count = 0;
            memset(sm->buffer, 0, sizeof(sm->buffer));
            break;
        default:
            printf("Invalid event for ERROR state. Only RESET allowed.\n");
            break;
    }
}

void process_event(StateMachine* sm, Event event) {
    if(sm == NULL) return;
    if(sm->current_state < STATE_IDLE || sm->current_state > STATE_ERROR) {
        sm->current_state = STATE_ERROR;
        return;
    }
    handlers[sm->current_state](sm, event);
}

Event get_user_event() {
    printf("\nAvailable events:\n");
    printf("0: START, 1: DATA, 2: PROCESS, 3: SUCCESS, 4: FAILURE, 5: RESET\n");
    printf("Enter event number (0-5): ");
    
    int input;
    if(scanf("%d", &input) != 1) {
        while(getchar() != '\n');
        return EVENT_RESET;
    }
    
    if(input < 0