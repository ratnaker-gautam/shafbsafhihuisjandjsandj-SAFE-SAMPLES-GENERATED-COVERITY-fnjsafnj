//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR, DONE };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
};

void state_idle(struct Context *ctx, int input_char) {
    if (input_char == EOF) {
        ctx->current_state = DONE;
    } else if (isalpha(input_char)) {
        ctx->buffer[0] = (char)input_char;
        ctx->buffer_pos = 1;
        ctx->char_count++;
        ctx->current_state = READING;
    } else if (isspace(input_char)) {
        ctx->char_count++;
    } else {
        ctx->current_state = ERROR;
    }
}

void state_reading(struct Context *ctx, int input_char) {
    if (input_char == EOF) {
        if (ctx->buffer_pos > 0) {
            ctx->current_state = PROCESSING;
        } else {
            ctx->current_state = DONE;
        }
    } else if (isalpha(input_char)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)input_char;
            ctx->char_count++;
        } else {
            ctx->current_state = ERROR;
        }
    } else if (isspace(input_char)) {
        ctx->buffer[ctx->buffer_pos] = '\0';
        ctx->char_count++;
        ctx->current_state = PROCESSING;
    } else {
        ctx->current_state = ERROR;
    }
}

void state_processing(struct Context *ctx) {
    if (ctx->buffer_pos > 0) {
        ctx->word_count++;
    }
    ctx->buffer_pos = 0;
    ctx->current_state = WRITING;
}

void state_writing(struct Context *ctx) {
    if (ctx->word_count > 0) {
        printf("Word %d processed\n", ctx->word_count);
    }
    ctx->current_state = IDLE;
}

void state_error(struct Context *ctx) {
    fprintf(stderr, "Error: Invalid input character encountered\n");
    ctx->current_state = DONE;
}

int main(void) {
    struct Context ctx = {IDLE, {0}, 0, 0, 0};
    int input_char;
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case IDLE:
                input_char = getchar();
                state_idle(&ctx, input_char);
                break;
            case READING:
                input_char = getchar();
                state_reading(&ctx, input_char);
                break;
            case PROCESSING:
                state_processing(&ctx);
                break;
            case WRITING:
                state_writing(&ctx);
                break;
            case ERROR:
                state_error(&ctx);
                break;
            default:
                ctx.current_state = ERROR;
                break;
        }
    }
    
    if (ctx.current_state == DONE) {
        printf("Processing complete. Words: %d, Characters: %d\n", ctx.word_count, ctx.char_count);
    }
    
    return ctx.current_state == DONE ? EXIT_SUCCESS : EXIT_FAILURE;
}