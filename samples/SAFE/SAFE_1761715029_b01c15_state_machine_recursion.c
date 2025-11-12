//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t position;
    int value;
};

enum State handle_start(struct Context* ctx, char input) {
    if (input == 'B') {
        ctx->position = 0;
        ctx->buffer[ctx->position++] = input;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context* ctx, char input) {
    if (ctx->position >= sizeof(ctx->buffer) - 1) {
        return ERROR;
    }
    if (input == 'E') {
        ctx->buffer[ctx->position] = '\0';
        return PROCESSING;
    }
    if ((input >= '0' && input <= '9') || input == ',') {
        ctx->buffer[ctx->position++] = input;
        return READING;
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx) {
    char* token = strtok(ctx->buffer + 1, ",");
    ctx->value = 0;
    int count = 0;
    
    while (token != NULL) {
        if (count >= 10) {
            return ERROR;
        }
        char* endptr;
        long num = strtol(token, &endptr, 10);
        if (*endptr != '\0' || num < 0 || num > 1000) {
            return ERROR;
        }
        ctx->value += (int)num;
        count++;
        token = strtok(NULL, ",");
    }
    
    if (count == 0) {
        return ERROR;
    }
    return FINISHED;
}

void process_input_recursive(struct Context* ctx, const char* input, size_t index) {
    if (ctx->current_state == ERROR || ctx->current_state == FINISHED) {
        return;
    }
    
    if (index >= strlen(input)) {
        if (ctx->current_state == PROCESSING) {
            ctx->current_state = handle_processing(ctx);
        } else if (ctx->current_state != FINISHED) {
            ctx->current_state = ERROR;
        }
        return;
    }
    
    char current_char = input[index];
    
    switch (ctx->current_state) {
        case START:
            ctx->current_state = handle_start(ctx, current_char);
            break;
        case READING:
            ctx->current_state = handle_reading(ctx, current_char);
            break;
        case PROCESSING:
            ctx->current_state = handle_processing(ctx);
            break;
        default:
            break;
    }
    
    process_input_recursive(ctx, input, index + 1);
}

int main(void) {
    char input[512];
    
    printf("Enter sequence (format: Bnum1,num2,...E): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.value = 0;
    
    process_input_recursive(&ctx, input, 0);
    
    if (ctx.current_state == FINISHED) {
        printf("Success! Sum: %d\n", ctx.value);
        return 0;
    } else {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
}