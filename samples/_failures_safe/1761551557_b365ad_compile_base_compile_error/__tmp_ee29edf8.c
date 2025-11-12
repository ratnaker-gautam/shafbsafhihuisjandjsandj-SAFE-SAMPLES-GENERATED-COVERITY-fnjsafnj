//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    NUM_STATES
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    NUM_EVENTS
} Event;

typedef struct {
    int data;
    int processed_value;
    int error_code;
} Context;

typedef void (*StateHandler)(Context*);

void handle_idle(Context* ctx) {
    if (ctx == NULL) return;
    ctx->data = 0;
    ctx->processed_value = 0;
    ctx->error_code = 0;
}

void handle_ready(Context* ctx) {
    if (ctx == NULL) return;
    if (ctx->data < 0) {
        ctx->error_code = 1;
    }
}

void handle_processing(Context* ctx) {
    if (ctx == NULL) return;
    if (ctx->data > 1000) {
        ctx->error_code = 2;
        return;
    }
    if (ctx->data > 0 && ctx->data > INT_MAX / 2) {
        ctx->error_code = 3;
        return;
    }
    ctx->processed_value = ctx->data * 2;
    if (ctx->processed_value < 0) {
        ctx->error_code = 3;
    }
}

void handle_complete(Context* ctx) {
    if (ctx == NULL) return;
    printf("Processing complete. Result: %d\n", ctx->processed_value);
}

void handle_error(Context* ctx) {
    if (ctx == NULL) return;
    printf("Error occurred. Code: %d\n", ctx->error_code);
}

StateHandler state_handlers[NUM_STATES] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

State transition_table[NUM_STATES][NUM_EVENTS] = {
    {STATE_READY,   STATE_IDLE, STATE_IDLE,      STATE_IDLE,    STATE_IDLE,    STATE_IDLE},
    {STATE_READY,   STATE_READY, STATE_PROCESSING, STATE_READY,  STATE_ERROR,   STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR, STATE_IDLE},
    {STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE},
    {STATE_ERROR,   STATE_ERROR, STATE_ERROR,   STATE_ERROR,   STATE_ERROR,   STATE_IDLE}
};

int get_user_input(void) {
    char input[32];
    int value;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(input, "%d", &value) != 1) {
        return -1;
    }
    
    return value;
}

Event get_next_event(State current_state) {
    printf("\nCurrent state: ");
    switch (current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READY: printf("READY"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_COMPLETE: printf("COMPLETE"); break;
        case STATE_ERROR: printf("ERROR"); break;
        default: break;
    }
    printf("\n");
    
    printf("Available events:\n");
    switch (current_state) {
        case STATE_IDLE:
            printf("0 - START\n");
            break;
        case STATE_READY:
            printf("1 - DATA\n");
            printf("5 - RESET\n");
            break;
        case STATE_PROCESSING:
            printf("2 - PROCESS\n");
            printf("3 - SUCCESS\n");
            printf("4 - FAILURE\n");
            printf("5 - RESET\n");
            break;
        case STATE_COMPLETE:
            printf("5 - RESET\n");
            break;
        case STATE_ERROR:
            printf("5 - RESET\n");
            break;
        default:
            break;
    }
    
    printf("Enter event number: ");
    int event_num = get_user_input();
    
    if (event_num < 0 || event_num >= NUM_EVENTS) {
        return NUM_EVENTS;
    }
    
    return (Event)event_num;
}

int main(void) {
    State current_state = STATE_IDLE;
    Context context = {0};
    
    printf("State Machine Demo\n");
    printf("==================\n");
    
    while (1) {
        if (current_state < 0 || current_state >= NUM_STATES) {
            current_state = STATE_IDLE;
        }
        
        state_handlers[current_state](&context);
        
        Event event = get_next_event(current_state);
        if (event >= NUM_EVENTS) {
            printf("Invalid event. Please try again.\n");
            continue;
        }
        
        State next_state = transition_table[current_state][event];
        
        if (event == EVENT_DATA && current_state == STATE_READY) {
            printf("Enter data value: ");
            int data = get_user_input();
            if (data == -1) {
                printf("Invalid data input.\n");
                continue;
            }
            context.data = data;
        }
        
        current_state