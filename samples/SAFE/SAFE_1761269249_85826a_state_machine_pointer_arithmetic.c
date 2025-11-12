//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define MAX_STATES 8

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_END,
    STATE_ERROR
} parser_state_t;

typedef struct {
    parser_state_t current;
    int word_count;
    int number_count;
    char buffer[MAX_INPUT];
    char *ptr;
} parser_context_t;

static void state_machine_init(parser_context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current = STATE_START;
    ctx->word_count = 0;
    ctx->number_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->ptr = ctx->buffer;
}

static parser_state_t handle_start(parser_context_t *ctx, char c) {
    if (isspace(c)) return STATE_START;
    if (isalpha(c)) {
        *(ctx->ptr) = c;
        ctx->ptr++;
        return STATE_READING_WORD;
    }
    if (isdigit(c)) {
        *(ctx->ptr) = c;
        ctx->ptr++;
        return STATE_READING_NUMBER;
    }
    return STATE_ERROR;
}

static parser_state_t handle_reading_word(parser_context_t *ctx, char c) {
    if (isalpha(c)) {
        if ((ctx->ptr - ctx->buffer) < (MAX_INPUT - 1)) {
            *(ctx->ptr) = c;
            ctx->ptr++;
        }
        return STATE_READING_WORD;
    }
    if (isspace(c)) {
        ctx->word_count++;
        ctx->ptr = ctx->buffer;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_START;
    }
    return STATE_ERROR;
}

static parser_state_t handle_reading_number(parser_context_t *ctx, char c) {
    if (isdigit(c)) {
        if ((ctx->ptr - ctx->buffer) < (MAX_INPUT - 1)) {
            *(ctx->ptr) = c;
            ctx->ptr++;
        }
        return STATE_READING_NUMBER;
    }
    if (isspace(c)) {
        ctx->number_count++;
        ctx->ptr = ctx->buffer;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_START;
    }
    return STATE_ERROR;
}

static void process_input(parser_context_t *ctx, const char *input) {
    if (ctx == NULL || input == NULL) return;
    
    size_t len = strlen(input);
    if (len >= MAX_INPUT) {
        ctx->current = STATE_ERROR;
        return;
    }
    
    for (size_t i = 0; i < len && ctx->current != STATE_ERROR; i++) {
        char c = input[i];
        
        switch (ctx->current) {
            case STATE_START:
                ctx->current = handle_start(ctx, c);
                break;
            case STATE_READING_WORD:
                ctx->current = handle_reading_word(ctx, c);
                break;
            case STATE_READING_NUMBER:
                ctx->current = handle_reading_number(ctx, c);
                break;
            default:
                ctx->current = STATE_ERROR;
                break;
        }
    }
    
    if (ctx->current == STATE_READING_WORD && *(ctx->buffer) != '\0') {
        ctx->word_count++;
    } else if (ctx->current == STATE_READING_NUMBER && *(ctx->buffer) != '\0') {
        ctx->number_count++;
    }
    
    if (ctx->current != STATE_ERROR) {
        ctx->current = STATE_END;
    }
}

int main(void) {
    char input[MAX_INPUT];
    parser_context_t ctx;
    
    printf("Enter text to analyze (words and numbers): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    state_machine_init(&ctx);
    process_input(&ctx, input);
    
    if (ctx.current == STATE_ERROR) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    printf("Words: %d\n", ctx.word_count);
    printf("Numbers: %d\n", ctx.number_count);
    
    return EXIT_SUCCESS;
}