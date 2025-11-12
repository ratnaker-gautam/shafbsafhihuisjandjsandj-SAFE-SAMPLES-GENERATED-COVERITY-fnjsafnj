//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t position;
    int result;
};

enum State handle_start(struct Context *ctx, char input) {
    if (input == 'B') {
        ctx->buffer[0] = input;
        ctx->position = 1;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, char input) {
    if (ctx->position >= 255) return ERROR;
    if (input == 'E') {
        ctx->buffer[ctx->position] = '\0';
        return PROCESSING;
    }
    if ((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z')) {
        ctx->buffer[ctx->position] = input;
        ctx->position++;
        return READING;
    }
    return ERROR;
}

enum State handle_processing(struct Context *ctx) {
    size_t len = strlen(ctx->buffer);
    if (len < 3) return ERROR;
    ctx->result = (int)len;
    return FINISHED;
}

void process_state_machine(struct Context *ctx, const char *input, size_t input_len, size_t index) {
    if (ctx->current_state == FINISHED || ctx->current_state == ERROR) return;
    if (index >= input_len) {
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
            ctx->current_state = ERROR;
            return;
    }
    
    process_state_machine(ctx, input, input_len, index + 1);
}

int main(void) {
    char user_input[512];
    printf("Enter input sequence: ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Context ctx = {START, "", 0, 0};
    process_state_machine(&ctx, user_input, input_len, 0);
    
    if (ctx.current_state == FINISHED) {
        printf("Success! Processed string: %s (length: %d)\n", ctx.buffer, ctx.result);
        return 0;
    } else {
        fprintf(stderr, "Error: Invalid input sequence\n");
        return 1;
    }
}