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
    char *input_ptr;
    char *input_end;
} parser_context_t;

static void state_start(parser_context_t *ctx);
static void state_reading_word(parser_context_t *ctx);
static void state_reading_number(parser_context_t *ctx);
static void state_error(parser_context_t *ctx);

static void (*state_handlers[])(parser_context_t *) = {
    state_start,
    state_reading_word,
    state_reading_number,
    state_error
};

static void state_start(parser_context_t *ctx) {
    if (ctx->input_ptr >= ctx->input_end) {
        ctx->current = STATE_END;
        return;
    }
    
    char c = *ctx->input_ptr;
    if (isspace(c)) {
        ctx->input_ptr++;
    } else if (isalpha(c)) {
        ctx->current = STATE_READING_WORD;
        ctx->word_count++;
    } else if (isdigit(c)) {
        ctx->current = STATE_READING_NUMBER;
        ctx->number_count++;
    } else {
        ctx->current = STATE_ERROR;
    }
}

static void state_reading_word(parser_context_t *ctx) {
    while (ctx->input_ptr < ctx->input_end && isalpha(*ctx->input_ptr)) {
        ctx->input_ptr++;
    }
    ctx->current = STATE_START;
}

static void state_reading_number(parser_context_t *ctx) {
    while (ctx->input_ptr < ctx->input_end && isdigit(*ctx->input_ptr)) {
        ctx->input_ptr++;
    }
    ctx->current = STATE_START;
}

static void state_error(parser_context_t *ctx) {
    ctx->input_ptr = ctx->input_end;
}

static int run_state_machine(char *input, int *words, int *numbers) {
    if (!input || !words || !numbers) return -1;
    
    parser_context_t ctx = {
        .current = STATE_START,
        .word_count = 0,
        .number_count = 0,
        .input_ptr = input,
        .input_end = input + strlen(input)
    };
    
    int transitions = 0;
    while (ctx.current != STATE_END && ctx.current != STATE_ERROR && transitions < MAX_STATES * 100) {
        if (ctx.current < 0 || ctx.current >= (sizeof(state_handlers) / sizeof(state_handlers[0]))) {
            return -1;
        }
        state_handlers[ctx.current](&ctx);
        transitions++;
    }
    
    *words = ctx.word_count;
    *numbers = ctx.number_count;
    
    return (ctx.current == STATE_END) ? 0 : -1;
}

int main(void) {
    char input_buffer[MAX_INPUT + 1] = {0};
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT);
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input_buffer[0] == '\0')) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    int word_count = 0;
    int number_count = 0;
    
    if (run_state_machine(input_buffer, &word_count, &number_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    
    return EXIT_SUCCESS;
}