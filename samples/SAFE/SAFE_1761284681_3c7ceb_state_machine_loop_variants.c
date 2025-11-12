//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state { IDLE, READING, PROCESSING, WRITING, ERROR, DONE };

struct context {
    enum state current;
    char buffer[256];
    size_t pos;
    int word_count;
    int char_count;
};

void state_idle(struct context *ctx, int input) {
    if (input == EOF) {
        ctx->current = DONE;
    } else if (isalpha(input)) {
        ctx->buffer[0] = (char)input;
        ctx->pos = 1;
        ctx->char_count = 1;
        ctx->current = READING;
    } else if (!isspace(input)) {
        ctx->current = ERROR;
    }
}

void state_reading(struct context *ctx, int input) {
    if (input == EOF || isspace(input)) {
        if (ctx->pos > 0 && ctx->pos < sizeof(ctx->buffer)) {
            ctx->buffer[ctx->pos] = '\0';
            ctx->current = PROCESSING;
        } else {
            ctx->current = ERROR;
        }
    } else if (isalpha(input)) {
        if (ctx->pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->pos++] = (char)input;
            ctx->char_count++;
        } else {
            ctx->current = ERROR;
        }
    } else {
        ctx->current = ERROR;
    }
}

void state_processing(struct context *ctx) {
    if (ctx->pos > 0) {
        for (size_t i = 0; i < ctx->pos; i++) {
            if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                ctx->buffer[i] = (char)(ctx->buffer[i] - 32);
            }
        }
        ctx->word_count++;
        ctx->current = WRITING;
    } else {
        ctx->current = ERROR;
    }
}

void state_writing(struct context *ctx) {
    if (ctx->word_count == 1) {
        printf("WORDS: ");
    }
    printf("%s", ctx->buffer);
    if (ctx->current != DONE) {
        printf(" ");
    }
    ctx->pos = 0;
    ctx->current = IDLE;
}

void state_error(struct context *ctx) {
    fprintf(stderr, "Error: Invalid input encountered\n");
    ctx->current = DONE;
}

int main(void) {
    struct context ctx = {IDLE, {0}, 0, 0, 0};
    int input;
    
    while (ctx.current != DONE && ctx.current != ERROR) {
        input = getchar();
        
        switch (ctx.current) {
            case IDLE:
                state_idle(&ctx, input);
                break;
            case READING:
                state_reading(&ctx, input);
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
            case DONE:
                break;
        }
    }
    
    if (ctx.current == ERROR) {
        state_error(&ctx);
    }
    
    if (ctx.word_count > 0) {
        printf("\n");
        printf("Total words: %d\n", ctx.word_count);
        printf("Total characters: %d\n", ctx.char_count);
    }
    
    return ctx.current == ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}