//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_COUNT
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef struct {
    int data;
    int processed_value;
    int error_code;
} Context;

typedef State (*StateHandler)(Context*, Event);

State handle_idle(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_START:
            ctx->data = 0;
            ctx->processed_value = 0;
            ctx->error_code = 0;
            return STATE_READY;
        default:
            return STATE_IDLE;
    }
}

State handle_ready(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_DATA:
            if (ctx->data >= 0 && ctx->data < 1000) {
                ctx->data += 10;
            }
            return STATE_READY;
        case EVENT_PROCESS:
            if (ctx->data > 0) {
                if (ctx->data > 10000) return STATE_ERROR;
                ctx->processed_value = ctx->data * 2;
                return STATE_PROCESSING;
            }
            return STATE_READY;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_READY;
    }
}

State handle_processing(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_SUCCESS:
            if (ctx->processed_value > 0) {
                return STATE_COMPLETE;
            }
            return STATE_PROCESSING;
        case EVENT_FAILURE:
            ctx->error_code = 1;
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_complete(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_COMPLETE;
    }
}

State handle_error(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_RESET:
            ctx->error_code = 0;
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

int main(void) {
    Context ctx = {0, 0, 0};
    State current_state = STATE_IDLE;
    
    printf("State Machine Demo\n");
    printf("Commands: start, data, process, success, failure, reset, quit\n");
    
    char input[32];
    while (current_state != STATE_ERROR || 1) {
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
            default: break;
        }
        printf(" | Data: %d | Processed: %d | Error: %d\n", 
               ctx.data, ctx.processed_value, ctx.error_code);
        
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        Event event = EVENT_COUNT;
        if (strcmp(input, "start") == 0) {
            event = EVENT_START;
        } else if (strcmp(input, "data") == 0) {
            event = EVENT_DATA;
        } else if (strcmp(input, "process") == 0) {
            event = EVENT_PROCESS;
        } else if (strcmp(input, "success") == 0) {
            event = EVENT_SUCCESS;
        } else if (strcmp(input, "failure") == 0) {
            event = EVENT_FAILURE;
        } else if (strcmp(input, "reset") == 0) {
            event = EVENT_RESET;
        } else if (strcmp(input, "quit") == 0) {
            break;
        } else {
            printf("Unknown command\n");
            continue;
        }
        
        if (event < EVENT_COUNT && current_state < STATE_COUNT) {
            StateHandler handler = state_handlers[current_state];
            if (handler != NULL) {
                State new_state = handler(&ctx, event);
                if (new_state < STATE_COUNT) {
                    current_state = new_state;
                }
            }
        }
    }
    
    return 0;
}