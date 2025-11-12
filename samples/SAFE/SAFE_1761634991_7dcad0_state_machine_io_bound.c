//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
};

void state_idle(struct Context *ctx);
void state_reading(struct Context *ctx);
void state_processing(struct Context *ctx);
void state_writing(struct Context *ctx);
void state_error(struct Context *ctx);
void state_done(struct Context *ctx);

int main(void) {
    struct Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_pos = 0;
    ctx.word_count = 0;
    ctx.char_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));

    while (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        switch (ctx.current_state) {
            case STATE_IDLE:
                state_idle(&ctx);
                break;
            case STATE_READING:
                state_reading(&ctx);
                break;
            case STATE_PROCESSING:
                state_processing(&ctx);
                break;
            case STATE_WRITING:
                state_writing(&ctx);
                break;
            case STATE_ERROR:
                state_error(&ctx);
                break;
            case STATE_DONE:
                state_done(&ctx);
                break;
        }
    }

    return ctx.current_state == STATE_DONE ? 0 : 1;
}

void state_idle(struct Context *ctx) {
    printf("Enter text (empty line to finish):\n");
    ctx->current_state = STATE_READING;
}

void state_reading(struct Context *ctx) {
    char input[512];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }

    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }

    if (input_len == 0) {
        ctx->current_state = STATE_PROCESSING;
        return;
    }

    if (ctx->buffer_pos + input_len >= sizeof(ctx->buffer) - 1) {
        ctx->current_state = STATE_ERROR;
        return;
    }

    memcpy(ctx->buffer + ctx->buffer_pos, input, input_len);
    ctx->buffer_pos += input_len;
    ctx->buffer[ctx->buffer_pos] = ' ';
    ctx->buffer_pos++;

    if (ctx->buffer_pos >= sizeof(ctx->buffer) - 1) {
        ctx->current_state = STATE_PROCESSING;
    }
}

void state_processing(struct Context *ctx) {
    if (ctx->buffer_pos == 0) {
        ctx->current_state = STATE_DONE;
        return;
    }

    ctx->buffer[ctx->buffer_pos] = '\0';
    ctx->char_count = 0;
    ctx->word_count = 0;

    int in_word = 0;
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (ctx->buffer[i] != ' ' && ctx->buffer[i] != '\t') {
            ctx->char_count++;
            if (!in_word) {
                ctx->word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }

    ctx->current_state = STATE_WRITING;
}

void state_writing(struct Context *ctx) {
    printf("Statistics:\n");
    printf("Characters: %d\n", ctx->char_count);
    printf("Words: %d\n", ctx->word_count);
    printf("Buffer size: %zu\n", ctx->buffer_pos);

    ctx->current_state = STATE_DONE;
}

void state_error(struct Context *ctx) {
    fprintf(stderr, "Error: Invalid input or buffer overflow\n");
}

void state_done(struct Context *ctx) {
}