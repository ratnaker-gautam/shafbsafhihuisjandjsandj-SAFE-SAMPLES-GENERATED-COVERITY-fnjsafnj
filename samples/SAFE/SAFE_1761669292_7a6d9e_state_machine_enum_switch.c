//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    ERROR,
    DONE
};

struct context {
    enum state current_state;
    int number_count;
    int word_count;
    char buffer[256];
    size_t buffer_pos;
};

void init_context(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = INIT;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

void process_char(struct context *ctx, char c) {
    if (ctx == NULL) return;
    
    if (!is_valid_char(c)) {
        ctx->current_state = ERROR;
        return;
    }
    
    switch (ctx->current_state) {
        case INIT:
            if (isdigit((unsigned char)c)) {
                ctx->current_state = READING_NUMBER;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (isalpha((unsigned char)c)) {
                ctx->current_state = READING_WORD;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (isspace((unsigned char)c)) {
                ctx->current_state = SKIPPING_SPACE;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_NUMBER:
            if (isdigit((unsigned char)c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace((unsigned char)c)) {
                ctx->number_count++;
                ctx->buffer_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current_state = SKIPPING_SPACE;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_WORD:
            if (isalpha((unsigned char)c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace((unsigned char)c)) {
                ctx->word_count++;
                ctx->buffer_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current_state = SKIPPING_SPACE;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case SKIPPING_SPACE:
            if (isdigit((unsigned char)c)) {
                ctx->current_state = READING_NUMBER;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (isalpha((unsigned char)c)) {
                ctx->current_state = READING_WORD;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (!isspace((unsigned char)c)) {
                ctx->current_state = ERROR;
            }
            break;
            
        case ERROR:
        case DONE:
            break;
    }
}

void finalize_processing(struct context *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->current_state == READING_NUMBER) {
        ctx->number_count++;
    } else if (ctx->current_state == READING_WORD) {
        ctx->word_count++;
    }
    
    if (ctx->current_state != ERROR) {
        ctx->current_state = DONE;
    }
}

int main(void) {
    struct context ctx;
    char input[1024];
    
    init_context(&ctx);
    
    printf("Enter text (numbers and words separated by spaces): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    for (size_t i = 0; i < len && ctx.current_state != ERROR; i++) {
        process_char(&ctx, input[i]);
    }
    
    finalize_processing(&ctx);
    
    if (ctx.current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}