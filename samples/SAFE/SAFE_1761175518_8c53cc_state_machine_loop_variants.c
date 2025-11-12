//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUMBERS,
    PROCESSING_TEXT,
    FINISHED,
    ERROR
};

struct Context {
    enum State current_state;
    int number_count;
    int total_sum;
    char buffer[256];
    int buffer_pos;
};

void state_machine_init(struct Context *ctx) {
    ctx->current_state = INIT;
    ctx->number_count = 0;
    ctx->total_sum = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_number(const char *str, int len) {
    if (len <= 0 || len > 10) return 0;
    
    int i = 0;
    if (str[0] == '-' || str[0] == '+') {
        i = 1;
        if (len == 1) return 0;
    }
    
    for (; i < len; i++) {
        if (!isdigit(str[i])) return 0;
    }
    
    long val = strtol(str, NULL, 10);
    if (val < -1000000 || val > 1000000) return 0;
    
    return 1;
}

void process_buffer(struct Context *ctx) {
    if (ctx->buffer_pos == 0) return;
    
    ctx->buffer[ctx->buffer_pos] = '\0';
    
    if (is_valid_number(ctx->buffer, ctx->buffer_pos)) {
        int num = atoi(ctx->buffer);
        ctx->total_sum += num;
        ctx->number_count++;
        printf("Processed number: %d\n", num);
    } else {
        printf("Processed text: %s\n", ctx->buffer);
    }
    
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

enum State handle_init(struct Context *ctx, char c) {
    if (c == 'N') {
        printf("Starting number processing mode\n");
        return READING_NUMBERS;
    } else if (c == 'T') {
        printf("Starting text processing mode\n");
        return PROCESSING_TEXT;
    } else if (c == 'Q') {
        return FINISHED;
    }
    return ERROR;
}

enum State handle_reading_numbers(struct Context *ctx, char c) {
    if (c == ';') {
        process_buffer(ctx);
        return READING_NUMBERS;
    } else if (c == 'S') {
        process_buffer(ctx);
        return PROCESSING_TEXT;
    } else if (c == 'Q') {
        process_buffer(ctx);
        return FINISHED;
    } else if (isdigit(c) || c == '-' || c == '+') {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = c;
        }
        return READING_NUMBERS;
    }
    return ERROR;
}

enum State handle_processing_text(struct Context *ctx, char c) {
    if (c == ';') {
        process_buffer(ctx);
        return PROCESSING_TEXT;
    } else if (c == 'N') {
        process_buffer(ctx);
        return READING_NUMBERS;
    } else if (c == 'Q') {
        process_buffer(ctx);
        return FINISHED;
    } else if (isprint(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = c;
        }
        return PROCESSING_TEXT;
    }
    return ERROR;
}

int main(void) {
    struct Context ctx;
    state_machine_init(&ctx);
    
    printf("Enter commands: N=numbers, T=text, Q=quit, ;=separator\n");
    printf("Input: ");
    
    int c;
    while ((c = getchar()) != EOF && ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        if (c == '\n') continue;
        
        switch (ctx.current_state) {
            case INIT:
                ctx.current_state = handle_init(&ctx, c);
                break;
            case READING_NUMBERS:
                ctx.current_state = handle_reading_numbers(&ctx, c);
                break;
            case PROCESSING_TEXT:
                ctx.current_state = handle_processing_text(&ctx, c);
                break;
            case FINISHED:
            case ERROR:
                break;
        }
        
        if (ctx.current_state == ERROR) {
            printf("Error: Invalid input sequence\n");
            break;
        }
    }
    
    if (ctx.current_state != ERROR) {
        process_buffer(&ctx);
    }
    
    if (ctx.current_state == FINISHED || ctx.current_state == ERROR) {
        printf("\nFinal statistics:\n");
        printf("Numbers processed: %d\n", ctx.number_count);
        printf("Total sum: %d\n", ctx.total_sum);
    }
    
    return ctx.current_state == ERROR ? 1 : 0;
}