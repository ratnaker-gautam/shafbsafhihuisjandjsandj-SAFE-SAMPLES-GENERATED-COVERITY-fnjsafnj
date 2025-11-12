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
    if (!ctx) return;
    ctx->current = STATE_START;
    ctx->word_count = 0;
    ctx->number_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->ptr = ctx->buffer;
}

static int is_valid_input(const char *input) {
    if (!input) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT) return 0;
    for (const char *p = input; *p != '\0'; p++) {
        if (!isprint(*p)) return 0;
    }
    return 1;
}

static void process_start_state(parser_context_t *ctx) {
    if (!ctx || !ctx->ptr) return;
    if (isalpha(*ctx->ptr)) {
        ctx->current = STATE_READING_WORD;
    } else if (isdigit(*ctx->ptr)) {
        ctx->current = STATE_READING_NUMBER;
    } else if (*ctx->ptr == '\0') {
        ctx->current = STATE_END;
    } else {
        ctx->current = STATE_ERROR;
    }
}

static void process_reading_word(parser_context_t *ctx) {
    if (!ctx || !ctx->ptr) return;
    while (isalpha(*ctx->ptr)) {
        ctx->ptr++;
    }
    ctx->word_count++;
    ctx->current = STATE_START;
}

static void process_reading_number(parser_context_t *ctx) {
    if (!ctx || !ctx->ptr) return;
    while (isdigit(*ctx->ptr)) {
        ctx->ptr++;
    }
    ctx->number_count++;
    ctx->current = STATE_START;
}

static int run_state_machine(parser_context_t *ctx) {
    if (!ctx) return -1;
    
    while (ctx->current != STATE_END && ctx->current != STATE_ERROR) {
        switch (ctx->current) {
            case STATE_START:
                process_start_state(ctx);
                break;
            case STATE_READING_WORD:
                process_reading_word(ctx);
                break;
            case STATE_READING_NUMBER:
                process_reading_number(ctx);
                break;
            default:
                ctx->current = STATE_ERROR;
                break;
        }
        
        if (ctx->ptr - ctx->buffer >= MAX_INPUT) {
            ctx->current = STATE_ERROR;
            break;
        }
    }
    
    return ctx->current == STATE_END ? 0 : -1;
}

int main(void) {
    char input[MAX_INPUT];
    parser_context_t ctx;
    
    printf("Enter text to analyze (words and numbers): ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    state_machine_init(&ctx);
    strncpy(ctx.buffer, input, sizeof(ctx.buffer) - 1);
    ctx.buffer[sizeof(ctx.buffer) - 1] = '\0';
    ctx.ptr = ctx.buffer;
    
    if (run_state_machine(&ctx) == 0) {
        printf("Analysis complete:\n");
        printf("Words: %d\n", ctx.word_count);
        printf("Numbers: %d\n", ctx.number_count);
    } else {
        fprintf(stderr, "Error during analysis\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}