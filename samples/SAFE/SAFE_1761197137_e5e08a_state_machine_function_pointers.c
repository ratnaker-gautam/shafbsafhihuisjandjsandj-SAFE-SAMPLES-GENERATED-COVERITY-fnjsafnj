//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum state {
    IDLE,
    READING,
    PROCESSING,
    WRITING,
    ERROR,
    EXIT
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int data_value;
};

typedef void (*state_handler)(struct context*);

void idle_handler(struct context* ctx) {
    printf("Enter command (read/process/write/exit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "read") == 0) {
        ctx->current_state = READING;
    } else if (strcmp(input, "process") == 0) {
        ctx->current_state = PROCESSING;
    } else if (strcmp(input, "write") == 0) {
        ctx->current_state = WRITING;
    } else if (strcmp(input, "exit") == 0) {
        ctx->current_state = EXIT;
    } else {
        printf("Invalid command\n");
        ctx->current_state = IDLE;
    }
}

void reading_handler(struct context* ctx) {
    printf("Enter data (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[--ctx->buffer_len] = '\0';
    }
    
    if (ctx->buffer_len == 0) {
        printf("No data entered\n");
        ctx->current_state = IDLE;
        return;
    }
    
    printf("Read %zu characters\n", ctx->buffer_len);
    ctx->current_state = IDLE;
}

void processing_handler(struct context* ctx) {
    if (ctx->buffer_len == 0) {
        printf("No data to process\n");
        ctx->current_state = IDLE;
        return;
    }
    
    ctx->data_value = 0;
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= '0' && ctx->buffer[i] <= '9') {
            ctx->data_value += (ctx->buffer[i] - '0');
        }
    }
    
    printf("Processed data - sum of digits: %d\n", ctx->data_value);
    ctx->current_state = IDLE;
}

void writing_handler(struct context* ctx) {
    if (ctx->buffer_len == 0) {
        printf("No data to write\n");
        ctx->current_state = IDLE;
        return;
    }
    
    printf("Data: %s\n", ctx->buffer);
    if (ctx->data_value != 0) {
        printf("Processed value: %d\n", ctx->data_value);
    }
    ctx->current_state = IDLE;
}

void error_handler(struct context* ctx) {
    printf("Error occurred\n");
    ctx->current_state = EXIT;
}

void exit_handler(struct context* ctx) {
    printf("Goodbye\n");
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer[0] = '\0';
    ctx.buffer_len = 0;
    ctx.data_value = 0;
    
    state_handler handlers[] = {
        idle_handler,
        reading_handler,
        processing_handler,
        writing_handler,
        error_handler,
        exit_handler
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