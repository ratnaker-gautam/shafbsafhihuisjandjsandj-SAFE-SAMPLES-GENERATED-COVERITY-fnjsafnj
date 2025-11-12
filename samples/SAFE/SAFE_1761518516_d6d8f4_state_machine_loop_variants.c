//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR, DONE };

struct Context {
    enum State current;
    char buffer[256];
    size_t pos;
    int word_count;
    int char_count;
};

void state_idle(struct Context *ctx, char input) {
    if (isspace(input)) {
        return;
    } else if (isalpha(input)) {
        ctx->current = READING;
        ctx->buffer[ctx->pos++] = input;
        ctx->char_count++;
    } else {
        ctx->current = ERROR;
    }
}

void state_reading(struct Context *ctx, char input) {
    if (isalpha(input)) {
        if (ctx->pos < 255) {
            ctx->buffer[ctx->pos++] = input;
            ctx->char_count++;
        } else {
            ctx->current = ERROR;
        }
    } else if (isspace(input)) {
        ctx->buffer[ctx->pos] = '\0';
        ctx->current = PROCESSING;
    } else {
        ctx->current = ERROR;
    }
}

void state_processing(struct Context *ctx, char input) {
    if (ctx->pos > 0) {
        ctx->word_count++;
    }
    ctx->pos = 0;
    if (input == '\0') {
        ctx->current = DONE;
    } else if (isspace(input)) {
        ctx->current = IDLE;
    } else if (isalpha(input)) {
        ctx->current = READING;
        ctx->buffer[ctx->pos++] = input;
        ctx->char_count++;
    } else {
        ctx->current = ERROR;
    }
}

void state_writing(struct Context *ctx) {
    printf("Words: %d, Characters: %d\n", ctx->word_count, ctx->char_count);
    ctx->current = DONE;
}

void state_error(struct Context *ctx) {
    printf("Error: Invalid input encountered\n");
    ctx->current = DONE;
}

int main(void) {
    struct Context ctx = {IDLE, {0}, 0, 0, 0};
    char input[512];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    size_t i = 0;
    while (ctx.current != DONE && ctx.current != ERROR && i <= len) {
        char c = (i < len) ? input[i] : '\0';
        
        switch (ctx.current) {
            case IDLE:
                state_idle(&ctx, c);
                break;
            case READING:
                state_reading(&ctx, c);
                break;
            case PROCESSING:
                state_processing(&ctx, c);
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
        
        if (ctx.current != PROCESSING && ctx.current != WRITING) {
            i++;
        }
    }
    
    if (ctx.current == PROCESSING) {
        state_processing(&ctx, '\0');
    }
    
    if (ctx.current != ERROR) {
        state_writing(&ctx);
    }
    
    return 0;
}