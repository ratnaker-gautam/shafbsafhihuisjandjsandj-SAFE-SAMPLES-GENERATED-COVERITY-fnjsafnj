//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
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
    size_t buffer_len;
    int data_value;
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
        ctx->current_state = READING;
    } else if (strcmp(input, "exit") == 0) {
        ctx->current_state = EXIT;
    } else {
        printf("Invalid command\n");
    }
}

void handle_reading(struct Context* ctx) {
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
    } else {
        ctx->current_state = PROCESSING;
    }
}

void handle_processing(struct Context* ctx) {
    int sum = 0;
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        sum += (int)ctx->buffer[i];
    }
    
    if (sum < 0) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->data_value = sum % 1000;
    printf("Processed value: %d\n", ctx->data_value);
    ctx->current_state = WRITING;
}

void handle_writing(struct Context* ctx) {
    char filename[] = "output.txt";
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    if (fprintf(file, "Data: %s\nValue: %d\n", ctx->buffer, ctx->data_value) < 0) {
        fclose(file);
        ctx->current_state = ERROR;
        return;
    }
    
    if (fclose(file) != 0) {
        ctx->current_state = ERROR;
        return;
    }
    
    printf("Data written to %s\n", filename);
    ctx->current_state = IDLE;
}

void handle_error(struct Context* ctx) {
    printf("An error occurred\n");
    ctx->current_state = IDLE;
}

void handle_exit(struct Context* ctx) {
    printf("Goodbye\n");
}

int main(void) {
    struct Context ctx = {IDLE, "", 0, 0};
    
    StateHandler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error,
        handle_exit
    };
    
    while (ctx.current_state != EXIT) {
        if (ctx.current_state >= 0 && ctx.current_state <= EXIT) {
            handlers[ctx.current_state](&ctx);
        } else {
            ctx.current_state = ERROR;
        }
    }
    
    return 0;
}