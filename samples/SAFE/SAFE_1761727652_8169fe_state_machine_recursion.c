//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READ_CHAR,
    PROCESS_DIGIT,
    PROCESS_LETTER,
    END,
    ERROR
};

struct Context {
    const char *input;
    size_t position;
    int digit_sum;
    int letter_count;
    enum State current_state;
};

enum State handle_start(struct Context *ctx) {
    if (ctx->position >= strlen(ctx->input)) {
        return END;
    }
    return READ_CHAR;
}

enum State handle_read_char(struct Context *ctx) {
    if (ctx->position >= strlen(ctx->input)) {
        return END;
    }
    
    char c = ctx->input[ctx->position];
    if (c >= '0' && c <= '9') {
        return PROCESS_DIGIT;
    } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return PROCESS_LETTER;
    } else {
        return ERROR;
    }
}

enum State handle_process_digit(struct Context *ctx) {
    if (ctx->position >= strlen(ctx->input)) {
        return ERROR;
    }
    
    char c = ctx->input[ctx->position];
    if (c >= '0' && c <= '9') {
        ctx->digit_sum += (c - '0');
        ctx->position++;
        return READ_CHAR;
    }
    return ERROR;
}

enum State handle_process_letter(struct Context *ctx) {
    if (ctx->position >= strlen(ctx->input)) {
        return ERROR;
    }
    
    char c = ctx->input[ctx->position];
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        ctx->letter_count++;
        ctx->position++;
        return READ_CHAR;
    }
    return ERROR;
}

void process_state_machine(struct Context *ctx) {
    if (ctx->current_state == END || ctx->current_state == ERROR) {
        return;
    }
    
    enum State next_state;
    switch (ctx->current_state) {
        case START:
            next_state = handle_start(ctx);
            break;
        case READ_CHAR:
            next_state = handle_read_char(ctx);
            break;
        case PROCESS_DIGIT:
            next_state = handle_process_digit(ctx);
            break;
        case PROCESS_LETTER:
            next_state = handle_process_letter(ctx);
            break;
        default:
            next_state = ERROR;
            break;
    }
    
    ctx->current_state = next_state;
    process_state_machine(ctx);
}

int main(void) {
    char input_buffer[1024];
    
    printf("Enter a string (letters and digits only): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    for (size_t i = 0; i < strlen(input_buffer); i++) {
        char c = input_buffer[i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    struct Context ctx = {
        .input = input_buffer,
        .position = 0,
        .digit_sum = 0,
        .letter_count = 0,
        .current_state = START
    };
    
    process_state_machine(&ctx);
    
    if (ctx.current_state == ERROR) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Digit sum: %d\n", ctx.digit_sum);
    printf("Letter count: %d\n", ctx.letter_count);
    
    return 0;
}