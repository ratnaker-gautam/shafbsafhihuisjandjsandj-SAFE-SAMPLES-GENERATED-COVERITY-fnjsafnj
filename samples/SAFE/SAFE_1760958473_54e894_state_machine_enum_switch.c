//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READ_CHAR,
    PROCESS_WORD,
    COUNT_WORD,
    FINISH,
    ERROR
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_pos;
    unsigned int word_count;
    unsigned int char_count;
};

void init_context(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = START;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
}

enum state handle_start(struct context *ctx, int c) {
    if (c == EOF) {
        return FINISH;
    } else if (isspace(c)) {
        return START;
    } else if (isprint(c)) {
        ctx->char_count++;
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)c;
        }
        return READ_CHAR;
    } else {
        return ERROR;
    }
}

enum state handle_read_char(struct context *ctx, int c) {
    if (c == EOF) {
        return PROCESS_WORD;
    } else if (isspace(c)) {
        return PROCESS_WORD;
    } else if (isprint(c)) {
        ctx->char_count++;
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)c;
        }
        return READ_CHAR;
    } else {
        return ERROR;
    }
}

enum state handle_process_word(struct context *ctx) {
    if (ctx->buffer_pos > 0) {
        ctx->buffer[ctx->buffer_pos] = '\0';
        return COUNT_WORD;
    }
    return START;
}

enum state handle_count_word(struct context *ctx) {
    ctx->word_count++;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    return START;
}

enum state handle_error(struct context *ctx) {
    fprintf(stderr, "Error: Invalid input character encountered\n");
    return FINISH;
}

void process_input(struct context *ctx) {
    int c;
    while (ctx->current_state != FINISH && ctx->current_state != ERROR) {
        if (ctx->current_state == START || 
            ctx->current_state == READ_CHAR) {
            c = getchar();
            if (c == EOF && ferror(stdin)) {
                ctx->current_state = ERROR;
                continue;
            }
        } else {
            c = 0;
        }

        switch (ctx->current_state) {
            case START:
                ctx->current_state = handle_start(ctx, c);
                break;
            case READ_CHAR:
                ctx->current_state = handle_read_char(ctx, c);
                break;
            case PROCESS_WORD:
                ctx->current_state = handle_process_word(ctx);
                break;
            case COUNT_WORD:
                ctx->current_state = handle_count_word(ctx);
                break;
            case ERROR:
                ctx->current_state = handle_error(ctx);
                break;
            case FINISH:
                break;
        }
    }

    if (ctx->current_state == PROCESS_WORD) {
        ctx->current_state = handle_process_word(ctx);
    }
    if (ctx->current_state == COUNT_WORD) {
        ctx->current_state = handle_count_word(ctx);
    }
}

int main(void) {
    struct context ctx;
    init_context(&ctx);
    
    printf("Enter text (Ctrl+D to end):\n");
    process_input(&ctx);
    
    if (ctx.current_state == ERROR) {
        return EXIT_FAILURE;
    }
    
    printf("Words: %u\n", ctx.word_count);
    printf("Characters: %u\n", ctx.char_count);
    
    return EXIT_SUCCESS;
}