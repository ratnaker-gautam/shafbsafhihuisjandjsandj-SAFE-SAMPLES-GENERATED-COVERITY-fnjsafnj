//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READ_CHAR,
    COUNT_DIGITS,
    PROCESS_WORD,
    FINISHED,
    ERROR
};

struct Context {
    enum State current_state;
    char input_buffer[256];
    int buffer_index;
    int digit_count;
    int word_count;
    int position;
};

void initialize_context(struct Context *ctx) {
    ctx->current_state = START;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
    ctx->buffer_index = 0;
    ctx->digit_count = 0;
    ctx->word_count = 0;
    ctx->position = 0;
}

enum State handle_start(struct Context *ctx) {
    printf("Enter text (max 255 chars): ");
    if (fgets(ctx->input_buffer, sizeof(ctx->input_buffer), stdin) == NULL) {
        return ERROR;
    }
    size_t len = strlen(ctx->input_buffer);
    if (len > 0 && ctx->input_buffer[len - 1] == '\n') {
        ctx->input_buffer[len - 1] = '\0';
    }
    ctx->position = 0;
    return READ_CHAR;
}

enum State handle_read_char(struct Context *ctx) {
    if (ctx->position >= 255 || ctx->input_buffer[ctx->position] == '\0') {
        return FINISHED;
    }
    
    char c = ctx->input_buffer[ctx->position];
    if (c >= '0' && c <= '9') {
        return COUNT_DIGITS;
    } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        return PROCESS_WORD;
    }
    ctx->position++;
    return READ_CHAR;
}

enum State handle_count_digits(struct Context *ctx) {
    int count = 0;
    while (ctx->position < 255 && ctx->input_buffer[ctx->position] != '\0') {
        char c = ctx->input_buffer[ctx->position];
        if (c >= '0' && c <= '9') {
            count++;
            ctx->position++;
        } else {
            break;
        }
    }
    ctx->digit_count += count;
    return READ_CHAR;
}

enum State handle_process_word(struct Context *ctx) {
    while (ctx->position < 255 && ctx->input_buffer[ctx->position] != '\0') {
        char c = ctx->input_buffer[ctx->position];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            ctx->position++;
        } else {
            break;
        }
    }
    ctx->word_count++;
    return READ_CHAR;
}

enum State handle_finished(struct Context *ctx) {
    printf("Analysis complete:\n");
    printf("Words found: %d\n", ctx->word_count);
    printf("Digits found: %d\n", ctx->digit_count);
    return FINISHED;
}

enum State handle_error(struct Context *ctx) {
    printf("Error occurred during processing.\n");
    return ERROR;
}

enum State process_state(struct Context *ctx) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx);
        case READ_CHAR:
            return handle_read_char(ctx);
        case COUNT_DIGITS:
            return handle_count_digits(ctx);
        case PROCESS_WORD:
            return handle_process_word(ctx);
        case FINISHED:
            return handle_finished(ctx);
        case ERROR:
            return handle_error(ctx);
        default:
            return ERROR;
    }
}

void state_machine_recursive(struct Context *ctx, int depth) {
    if (depth > 1000) {
        ctx->current_state = ERROR;
        return;
    }
    
    enum State next_state = process_state(ctx);
    if (next_state == ctx->current_state && next_state != FINISHED && next_state != ERROR) {
        return;
    }
    
    ctx->current_state = next_state;
    
    if (ctx->current_state != FINISHED && ctx->current_state != ERROR) {
        state_machine_recursive(ctx, depth + 1);
    }
}

int main(void) {
    struct Context ctx;
    initialize_context(&ctx);
    state_machine_recursive(&ctx, 0);
    return 0;
}