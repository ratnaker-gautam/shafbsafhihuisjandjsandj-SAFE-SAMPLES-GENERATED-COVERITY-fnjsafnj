//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    IDLE,
    READING,
    PROCESSING,
    WRITING,
    ERROR,
    EXIT
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int processed_count;
};

typedef void (*StateHandler)(struct Context*);

void handle_idle(struct Context* ctx) {
    printf("Enter command (read/process/write/exit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "read") == 0) {
        ctx->current_state = READING;
        ctx->buffer_pos = 0;
    } else if (strcmp(input, "process") == 0) {
        ctx->current_state = PROCESSING;
    } else if (strcmp(input, "write") == 0) {
        ctx->current_state = WRITING;
    } else if (strcmp(input, "exit") == 0) {
        ctx->current_state = EXIT;
    } else {
        printf("Invalid command\n");
    }
}

void handle_reading(struct Context* ctx) {
    printf("Enter text to read (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->buffer_pos = strlen(ctx->buffer);
    if (ctx->buffer_pos > 0 && ctx->buffer[ctx->buffer_pos - 1] == '\n') {
        ctx->buffer[ctx->buffer_pos - 1] = '\0';
        ctx->buffer_pos--;
    }
    
    printf("Read %zu characters\n", ctx->buffer_pos);
    ctx->current_state = IDLE;
}

void handle_processing(struct Context* ctx) {
    if (ctx->buffer_pos == 0) {
        printf("No data to process\n");
        ctx->current_state = IDLE;
        return;
    }
    
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    ctx->processed_count++;
    printf("Processed data (converted to uppercase), total processed: %d\n", ctx->processed_count);
    ctx->current_state = IDLE;
}

void handle_writing(struct Context* ctx) {
    if (ctx->buffer_pos == 0) {
        printf("No data to write\n");
        ctx->current_state = IDLE;
        return;
    }
    
    printf("Current buffer content: %s\n", ctx->buffer);
    ctx->current_state = IDLE;
}

void handle_error(struct Context* ctx) {
    printf("Error occurred in state machine\n");
    ctx->current_state = EXIT;
}

void handle_exit(struct Context* ctx) {
    printf("Exiting state machine\n");
}

int main(void) {
    struct Context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_pos = 0;
    ctx.processed_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    StateHandler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error,
        handle_exit
    };
    
    while (ctx.current_state != EXIT) {
        if (ctx.current_state >= 0 && ctx.current_state < (int)(sizeof(handlers)/sizeof(handlers[0]))) {
            handlers[ctx.current_state](&ctx);
        } else {
            ctx.current_state = ERROR;
        }
    }
    
    return 0;
}