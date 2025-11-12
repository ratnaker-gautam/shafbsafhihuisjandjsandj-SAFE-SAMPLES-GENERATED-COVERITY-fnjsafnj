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
    STATE_EXIT
} State;

typedef struct {
    State current_state;
    char buffer[256];
    size_t buffer_pos;
    int error_code;
} Context;

typedef State (*StateHandler)(Context*);

State handle_idle(Context* ctx) {
    printf("Enter command (read/process/write/quit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->error_code = 1;
        return STATE_ERROR;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "read") == 0) {
        ctx->buffer_pos = 0;
        return STATE_READING;
    } else if (strcmp(input, "process") == 0) {
        return STATE_PROCESSING;
    } else if (strcmp(input, "write") == 0) {
        return STATE_WRITING;
    } else if (strcmp(input, "quit") == 0) {
        return STATE_EXIT;
    } else {
        printf("Invalid command\n");
        return STATE_IDLE;
    }
}

State handle_reading(Context* ctx) {
    printf("Enter text (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->error_code = 2;
        return STATE_ERROR;
    }
    
    ctx->buffer_pos = strlen(ctx->buffer);
    if (ctx->buffer_pos > 0 && ctx->buffer[ctx->buffer_pos - 1] == '\n') {
        ctx->buffer[ctx->buffer_pos - 1] = '\0';
        ctx->buffer_pos--;
    }
    
    printf("Read %zu characters\n", ctx->buffer_pos);
    return STATE_IDLE;
}

State handle_processing(Context* ctx) {
    if (ctx->buffer_pos == 0) {
        printf("No data to process\n");
        return STATE_IDLE;
    }
    
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    printf("Text converted to uppercase\n");
    return STATE_IDLE;
}

State handle_writing(Context* ctx) {
    if (ctx->buffer_pos == 0) {
        printf("No data to write\n");
        return STATE_IDLE;
    }
    
    printf("Current buffer: %s\n", ctx->buffer);
    return STATE_IDLE;
}

State handle_error(Context* ctx) {
    printf("Error occurred (code: %d)\n", ctx->error_code);
    return STATE_EXIT;
}

State handle_exit(Context* ctx) {
    printf("Goodbye!\n");
    return STATE_EXIT;
}

int main(void) {
    Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_pos = 0;
    ctx.error_code = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    StateHandler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error,
        handle_exit
    };
    
    while (ctx.current_state != STATE_EXIT) {
        if (ctx.current_state < 0 || ctx.current_state >= (int)(sizeof(handlers)/sizeof(handlers[0]))) {
            printf("Invalid state\n");
            break;
        }
        
        StateHandler handler = handlers[ctx.current_state];
        if (handler == NULL) {
            printf("No handler for state\n");
            break;
        }
        
        ctx.current_state = handler(&ctx);
    }
    
    return 0;
}