//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
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
    if (ctx->position >= 255) return ERROR;
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
    int sum = 0;
    int count = 0;
    
    while (token != NULL) {
        char* endptr;
        long num = strtol(token, &endptr, 10);
        if (*endptr != '\0' || num < 0 || num > 1000) {
            return ERROR;
        }
        sum += (int)num;
        count++;
        if (sum < 0) return ERROR;
        token = strtok(NULL, ",");
    }
    
    if (count == 0) return ERROR;
    ctx->value = sum / count;
    return FINISHED;
}

void process_state_machine(struct Context* ctx, const char* input, size_t index) {
    if (ctx->current_state == ERROR || ctx->current_state == FINISHED) return;
    
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
    
    process_state_machine(ctx, input, index + 1);
}

int main() {
    char input[512];
    
    printf("Enter data sequence (format: Bnum1,num2,...E): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Context ctx = { START, "", 0, 0 };
    process_state_machine(&ctx, input, 0);
    
    switch (ctx.current_state) {
        case FINISHED:
            printf("Success! Average value: %d\n", ctx.value);
            break;
        case ERROR:
            fprintf(stderr, "Error: Invalid input format\n");
            return 1;
        default:
            fprintf(stderr, "Error: Incomplete input\n");
            return 1;
    }
    
    return 0;
}