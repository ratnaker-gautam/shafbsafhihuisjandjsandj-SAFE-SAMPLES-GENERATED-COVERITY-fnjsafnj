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
    int data_count;
};

typedef void (*StateHandler)(struct Context*);

void handle_idle(struct Context* ctx) {
    printf("Enter command (read, exit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "read") == 0) {
        ctx->buffer_pos = 0;
        ctx->data_count = 0;
        ctx->current_state = READING;
    } else if (strcmp(input, "exit") == 0) {
        ctx->current_state = EXIT;
    } else {
        printf("Invalid command\n");
    }
}

void handle_reading(struct Context* ctx) {
    printf("Enter data (max 255 chars, 'done' to finish): ");
    char input[256];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "done") == 0) {
        if (ctx->buffer_pos > 0) {
            ctx->current_state = PROCESSING;
        } else {
            printf("No data entered\n");
            ctx->current_state = IDLE;
        }
        return;
    }
    
    size_t input_len = strlen(input);
    if (ctx->buffer_pos + input_len >= sizeof(ctx->buffer)) {
        printf("Buffer full, processing data\n");
        ctx->current_state = PROCESSING;
        return;
    }
    
    memcpy(ctx->buffer + ctx->buffer_pos, input, input_len);
    ctx->buffer_pos += input_len;
    ctx->buffer[ctx->buffer_pos] = ' ';
    ctx->buffer_pos++;
    ctx->data_count++;
    
    printf("Data added. Total entries: %d\n", ctx->data_count);
}

void handle_processing(struct Context* ctx) {
    if (ctx->buffer_pos > 0 && ctx->buffer[ctx->buffer_pos - 1] == ' ') {
        ctx->buffer[ctx->buffer_pos - 1] = '\0';
    }
    
    printf("Processing %d data entries...\n", ctx->data_count);
    
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    ctx->current_state = WRITING;
}

void handle_writing(struct Context* ctx) {
    printf("Processed data: %s\n", ctx->buffer);
    printf("Total entries processed: %d\n", ctx->data_count);
    ctx->current_state = IDLE;
}

void handle_error(struct Context* ctx) {
    printf("Error occurred. Resetting to idle state.\n");
    ctx->current_state = IDLE;
    ctx->buffer_pos = 0;
    ctx->data_count = 0;
}

int main() {
    struct Context ctx = {IDLE, {0}, 0, 0};
    StateHandler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error,
        NULL
    };
    
    while (ctx.current_state != EXIT) {
        if (ctx.current_state >= 0 && ctx.current_state < EXIT) {
            handlers[ctx.current_state](&ctx);
        } else {
            ctx.current_state = ERROR;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}