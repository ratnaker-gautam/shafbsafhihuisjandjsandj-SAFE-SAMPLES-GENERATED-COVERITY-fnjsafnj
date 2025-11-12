//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t position;
    int result;
};

int state_machine(struct Context* ctx, const char* input, size_t input_len, size_t index);

int process_character(struct Context* ctx, char c) {
    if (ctx->position >= sizeof(ctx->buffer) - 1) {
        ctx->current_state = ERROR;
        return -1;
    }
    
    if (c >= '0' && c <= '9') {
        ctx->buffer[ctx->position++] = c;
        ctx->current_state = READING;
        return 1;
    } else if (c == ' ' || c == '\t') {
        if (ctx->current_state == READING) {
            ctx->buffer[ctx->position] = '\0';
            ctx->current_state = PROCESSING;
        }
        return 1;
    } else if (c == '\0' || c == '\n') {
        if (ctx->current_state == READING) {
            ctx->buffer[ctx->position] = '\0';
            ctx->current_state = PROCESSING;
        }
        return 0;
    } else {
        ctx->current_state = ERROR;
        return -1;
    }
}

int process_buffer(struct Context* ctx) {
    if (ctx->position == 0) {
        ctx->current_state = ERROR;
        return -1;
    }
    
    long value = 0;
    for (size_t i = 0; i < ctx->position; i++) {
        if (ctx->buffer[i] < '0' || ctx->buffer[i] > '9') {
            ctx->current_state = ERROR;
            return -1;
        }
        if (value > (LONG_MAX - (ctx->buffer[i] - '0')) / 10) {
            ctx->current_state = ERROR;
            return -1;
        }
        value = value * 10 + (ctx->buffer[i] - '0');
    }
    
    if (value > 1000000) {
        ctx->current_state = ERROR;
        return -1;
    }
    
    ctx->result += (int)value;
    ctx->position = 0;
    ctx->current_state = FINISHED;
    return 1;
}

int state_machine(struct Context* ctx, const char* input, size_t input_len, size_t index) {
    if (ctx == NULL || input == NULL) {
        return -1;
    }
    
    if (index >= input_len) {
        if (ctx->current_state == READING) {
            ctx->buffer[ctx->position] = '\0';
            ctx->current_state = PROCESSING;
        }
        
        if (ctx->current_state == PROCESSING) {
            if (process_buffer(ctx) < 0) {
                return -1;
            }
        }
        
        return (ctx->current_state == FINISHED || ctx->current_state == START) ? 0 : -1;
    }
    
    if (ctx->current_state == ERROR) {
        return -1;
    }
    
    switch (ctx->current_state) {
        case START:
        case FINISHED:
            ctx->current_state = START;
            ctx->position = 0;
            if (input[index] != ' ' && input[index] != '\t' && input[index] != '\n') {
                return state_machine(ctx, input, input_len, index);
            }
            break;
            
        case READING:
        case PROCESSING:
            break;
            
        case ERROR:
            return -1;
    }
    
    int char_result = process_character(ctx, input[index]);
    if (char_result < 0) {
        return -1;
    }
    
    if (ctx->current_state == PROCESSING) {
        if (process_buffer(ctx) < 0) {
            return -1;
        }
    }
    
    if (char_result > 0) {
        return state_machine(ctx, input, input_len, index + 1);
    }
    
    return 0;
}

int main(void) {
    char input[512];
    struct Context ctx = {START, {0}, 0, 0};
    
    printf("Enter numbers separated by spaces: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= sizeof(input)) {
        printf("Input too long\n");
        return 1;
    }
    
    int result = state_machine(&ctx, input, input_len, 0);
    
    if (result < 0 || ctx.current_state == ERROR) {
        printf("Error processing input\n");
        return 1;
    }
    
    printf("Sum: %d\n", ctx.result);
    return 0;
}