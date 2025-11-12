//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 64
#define MAX_STATES 8

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_VALIDATING,
    STATE_COMPLETE,
    STATE_ERROR
} state_t;

typedef enum {
    EVENT_INPUT,
    EVENT_PROCESS,
    EVENT_VALIDATE,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET
} event_t;

typedef struct {
    char buffer[MAX_INPUT_LEN];
    size_t length;
    int processed_value;
    int validation_count;
} context_t;

state_t handle_start(context_t *ctx, event_t event);
state_t handle_reading(context_t *ctx, event_t event);
state_t handle_processing(context_t *ctx, event_t event);
state_t handle_validating(context_t *ctx, event_t event);
state_t handle_complete(context_t *ctx, event_t event);
state_t handle_error(context_t *ctx, event_t event);

state_t transition_state(state_t current, event_t event, context_t *ctx) {
    switch (current) {
        case STATE_START:
            return handle_start(ctx, event);
        case STATE_READING:
            return handle_reading(ctx, event);
        case STATE_PROCESSING:
            return handle_processing(ctx, event);
        case STATE_VALIDATING:
            return handle_validating(ctx, event);
        case STATE_COMPLETE:
            return handle_complete(ctx, event);
        case STATE_ERROR:
            return handle_error(ctx, event);
        default:
            return STATE_ERROR;
    }
}

state_t handle_start(context_t *ctx, event_t event) {
    if (event == EVENT_INPUT) {
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        ctx->length = 0;
        ctx->processed_value = 0;
        ctx->validation_count = 0;
        printf("Enter a string (max %d chars): ", MAX_INPUT_LEN - 1);
        return STATE_READING;
    }
    return STATE_START;
}

state_t handle_reading(context_t *ctx, event_t event) {
    if (event == EVENT_PROCESS) {
        if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
            ctx->length = strnlen(ctx->buffer, sizeof(ctx->buffer));
            if (ctx->length > 0 && ctx->buffer[ctx->length - 1] == '\n') {
                ctx->buffer[ctx->length - 1] = '\0';
                ctx->length--;
            }
            if (ctx->length > 0) {
                return STATE_PROCESSING;
            }
        }
        return STATE_ERROR;
    }
    return STATE_READING;
}

state_t handle_processing(context_t *ctx, event_t event) {
    if (event == EVENT_VALIDATE) {
        ctx->processed_value = 0;
        for (size_t i = 0; i < ctx->length; i++) {
            if (ctx->buffer[i] >= '0' && ctx->buffer[i] <= '9') {
                ctx->processed_value += (ctx->buffer[i] - '0');
            }
        }
        if (ctx->processed_value > 0) {
            return STATE_VALIDATING;
        }
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

state_t handle_validating(context_t *ctx, event_t event) {
    if (event == EVENT_SUCCESS) {
        ctx->validation_count++;
        if (ctx->validation_count >= 2) {
            return STATE_COMPLETE;
        }
        return STATE_VALIDATING;
    } else if (event == EVENT_FAILURE) {
        return STATE_ERROR;
    }
    return STATE_VALIDATING;
}

state_t handle_complete(context_t *ctx, event_t event) {
    if (event == EVENT_RESET) {
        return STATE_START;
    }
    return STATE_COMPLETE;
}

state_t handle_error(context_t *ctx, event_t event) {
    if (event == EVENT_RESET) {
        return STATE_START;
    }
    return STATE_ERROR;
}

int main(void) {
    state_t current_state = STATE_START;
    context_t context;
    memset(&context, 0, sizeof(context));
    
    printf("State Machine Demo - String Digit Sum Validator\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        event_t event = EVENT_RESET;
        
        switch (current_state) {
            case STATE_START:
                event = EVENT_INPUT;
                break;
            case STATE_READING:
                event = EVENT_PROCESS;
                break;
            case STATE_PROCESSING:
                event = EVENT_VALIDATE;
                break;
            case STATE_VALIDATING:
                if (context.processed_value % 2 == 0) {
                    event = EVENT_SUCCESS;
                } else {
                    event = EVENT_FAILURE;
                }
                break;
            case STATE_COMPLETE:
                printf("Validation complete! Sum of digits: %d\n", context.processed_value);
                printf("Press Enter to continue or type 'quit': ");
                char input[16];
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    if (strncmp(input, "quit", 4) == 0) {
                        return 0;
                    }
                }
                event = EVENT_RESET;
                break;
            case STATE_ERROR:
                printf