//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READ_CHAR,
    PROCESS_CHAR,
    COUNT_DIGITS,
    COUNT_LETTERS,
    FINISH,
    ERROR
};

struct Context {
    enum State current_state;
    char input_buffer[256];
    size_t input_len;
    size_t position;
    int digit_count;
    int letter_count;
    int result;
};

void initialize_context(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = START;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
    ctx->input_len = 0;
    ctx->position = 0;
    ctx->digit_count = 0;
    ctx->letter_count = 0;
    ctx->result = 0;
}

enum State handle_start(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    printf("Enter a string (max 255 chars): ");
    if (fgets(ctx->input_buffer, sizeof(ctx->input_buffer), stdin) == NULL) {
        return ERROR;
    }
    ctx->input_len = strlen(ctx->input_buffer);
    if (ctx->input_len > 0 && ctx->input_buffer[ctx->input_len - 1] == '\n') {
        ctx->input_buffer[ctx->input_len - 1] = '\0';
        ctx->input_len--;
    }
    if (ctx->input_len == 0) {
        return ERROR;
    }
    return READ_CHAR;
}

enum State handle_read_char(struct Context *ctx) {
    if (ctx == NULL || ctx->position >= ctx->input_len) {
        return FINISH;
    }
    return PROCESS_CHAR;
}

enum State handle_process_char(struct Context *ctx) {
    if (ctx == NULL || ctx->position >= ctx->input_len) {
        return ERROR;
    }
    char c = ctx->input_buffer[ctx->position];
    if (c >= '0' && c <= '9') {
        return COUNT_DIGITS;
    } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        return COUNT_LETTERS;
    }
    ctx->position++;
    return READ_CHAR;
}

enum State handle_count_digits(struct Context *ctx) {
    if (ctx == NULL || ctx->position >= ctx->input_len) {
        return ERROR;
    }
    ctx->digit_count++;
    ctx->position++;
    return READ_CHAR;
}

enum State handle_count_letters(struct Context *ctx) {
    if (ctx == NULL || ctx->position >= ctx->input_len) {
        return ERROR;
    }
    ctx->letter_count++;
    ctx->position++;
    return READ_CHAR;
}

enum State handle_finish(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    ctx->result = ctx->digit_count + ctx->letter_count;
    printf("Digits: %d, Letters: %d, Total alphanumeric: %d\n",
           ctx->digit_count, ctx->letter_count, ctx->result);
    return FINISH;
}

enum State handle_error(struct Context *ctx) {
    if (ctx == NULL) return ERROR;
    printf("Error processing input\n");
    return ERROR;
}

void state_machine(struct Context *ctx) {
    if (ctx == NULL) return;
    
    switch (ctx->current_state) {
        case START:
            ctx->current_state = handle_start(ctx);
            break;
        case READ_CHAR:
            ctx->current_state = handle_read_char(ctx);
            break;
        case PROCESS_CHAR:
            ctx->current_state = handle_process_char(ctx);
            break;
        case COUNT_DIGITS:
            ctx->current_state = handle_count_digits(ctx);
            break;
        case COUNT_LETTERS:
            ctx->current_state = handle_count_letters(ctx);
            break;
        case FINISH:
            return;
        case ERROR:
            handle_error(ctx);
            return;
        default:
            ctx->current_state = ERROR;
            break;
    }
    
    if (ctx->current_state != FINISH && ctx->current_state != ERROR) {
        state_machine(ctx);
    }
}

int main(void) {
    struct Context ctx;
    initialize_context(&ctx);
    state_machine(&ctx);
    return ctx.current_state == FINISH ? EXIT_SUCCESS : EXIT_FAILURE;
}