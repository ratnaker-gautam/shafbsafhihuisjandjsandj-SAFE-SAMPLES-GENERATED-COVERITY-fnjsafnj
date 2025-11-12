//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
} State;

typedef struct {
    State current_state;
    char buffer[256];
    size_t buffer_pos;
    int data_value;
} Context;

typedef State (*StateHandler)(Context*);

State handle_idle(Context* ctx) {
    printf("Enter command (read/quit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return STATE_ERROR;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "read") == 0) {
        ctx->buffer_pos = 0;
        ctx->buffer[0] = '\0';
        return STATE_READING;
    } else if (strcmp(input, "quit") == 0) {
        return STATE_DONE;
    } else {
        printf("Unknown command\n");
        return STATE_IDLE;
    }
}

State handle_reading(Context* ctx) {
    printf("Enter data (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return STATE_ERROR;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
    }
    
    ctx->buffer_pos = strlen(ctx->buffer);
    
    if (ctx->buffer_pos == 0) {
        printf("No data entered\n");
        return STATE_IDLE;
    }
    
    return STATE_PROCESSING;
}

State handle_processing(Context* ctx) {
    int sum = 0;
    int valid_numbers = 0;
    
    char* token = strtok(ctx->buffer, " ");
    while (token != NULL) {
        char* endptr;
        long num = strtol(token, &endptr, 10);
        
        if (endptr != token && *endptr == '\0' && num >= -1000 && num <= 1000) {
            sum += (int)num;
            valid_numbers++;
        }
        
        token = strtok(NULL, " ");
    }
    
    if (valid_numbers > 0) {
        ctx->data_value = sum;
        return STATE_WRITING;
    } else {
        printf("No valid numbers found\n");
        return STATE_IDLE;
    }
}

State handle_writing(Context* ctx) {
    printf("Processed result: %d\n", ctx->data_value);
    return STATE_IDLE;
}

State handle_error(Context* ctx) {
    printf("An error occurred\n");
    return STATE_DONE;
}

State handle_done(Context* ctx) {
    return STATE_DONE;
}

int main() {
    Context ctx = {
        .current_state = STATE_IDLE,
        .buffer_pos = 0,
        .data_value = 0
    };
    ctx.buffer[0] = '\0';
    
    StateHandler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error,
        handle_done
    };
    
    while (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        if (ctx.current_state >= 0 && ctx.current_state < sizeof(handlers)/sizeof(handlers[0])) {
            ctx.current_state = handlers[ctx.current_state](&ctx);
        } else {
            ctx.current_state = STATE_ERROR;
        }
    }
    
    if (ctx.current_state == STATE_ERROR) {
        handle_error(&ctx);
    }
    
    printf("Program terminated\n");
    return 0;
}