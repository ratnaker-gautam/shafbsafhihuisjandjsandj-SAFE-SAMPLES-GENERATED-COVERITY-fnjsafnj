//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_NUM,
    STATE_READING_ALPHA,
    STATE_READING_OTHER,
    STATE_END
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int number_count;
    int alpha_count;
    int other_count;
    char input_buffer[MAX_INPUT_LEN + 1];
} parser_context_t;

static void initialize_context(parser_context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->alpha_count = 0;
    ctx->other_count = 0;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
}

static parser_state_t handle_start_state(char c) {
    if (isdigit(c)) return STATE_READING_NUM;
    if (isalpha(c)) return STATE_READING_ALPHA;
    if (c != '\0' && c != '\n') return STATE_READING_OTHER;
    return STATE_START;
}

static parser_state_t handle_reading_num(char c) {
    if (isdigit(c)) return STATE_READING_NUM;
    if (isalpha(c)) return STATE_READING_ALPHA;
    if (c != '\0' && c != '\n') return STATE_READING_OTHER;
    return STATE_END;
}

static parser_state_t handle_reading_alpha(char c) {
    if (isalpha(c)) return STATE_READING_ALPHA;
    if (isdigit(c)) return STATE_READING_NUM;
    if (c != '\0' && c != '\n') return STATE_READING_OTHER;
    return STATE_END;
}

static parser_state_t handle_reading_other(char c) {
    if (isdigit(c)) return STATE_READING_NUM;
    if (isalpha(c)) return STATE_READING_ALPHA;
    if (c != '\0' && c != '\n') return STATE_READING_OTHER;
    return STATE_END;
}

static void process_character(parser_context_t *ctx, char c) {
    if (ctx == NULL) return;
    
    switch (ctx->current_state) {
        case STATE_START:
            ctx->current_state = handle_start_state(c);
            break;
        case STATE_READING_NUM:
            if (!isdigit(c)) ctx->number_count++;
            ctx->current_state = handle_reading_num(c);
            break;
        case STATE_READING_ALPHA:
            if (!isalpha(c)) ctx->alpha_count++;
            ctx->current_state = handle_reading_alpha(c);
            break;
        case STATE_READING_OTHER:
            if (isdigit(c) || isalpha(c) || c == '\0' || c == '\n') ctx->other_count++;
            ctx->current_state = handle_reading_other(c);
            break;
        case STATE_END:
            break;
    }
}

static void finalize_counts(parser_context_t *ctx) {
    if (ctx == NULL) return;
    
    switch (ctx->current_state) {
        case STATE_READING_NUM:
            ctx->number_count++;
            break;
        case STATE_READING_ALPHA:
            ctx->alpha_count++;
            break;
        case STATE_READING_OTHER:
            ctx->other_count++;
            break;
        default:
            break;
    }
}

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 0;
    }
    return 1;
}

int main(void) {
    parser_context_t ctx;
    char user_input[MAX_INPUT_LEN + 2];
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
    }
    
    if (!validate_input(user_input)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    initialize_context(&ctx);
    strncpy(ctx.input_buffer, user_input, MAX_INPUT_LEN);
    ctx.input_buffer[MAX_INPUT_LEN] = '\0';
    
    for (size_t i = 0; i < strlen(ctx.input_buffer); i++) {
        process_character(&ctx, ctx.input_buffer[i]);
    }
    
    process_character(&ctx, '\0');
    finalize_counts(&ctx);
    
    printf("Analysis results:\n");
    printf("Number sequences: %d\n", ctx.number_count);
    printf("Alpha sequences: %d\n", ctx.alpha_count);
    printf("Other sequences: %d\n", ctx.other_count);
    
    return EXIT_SUCCESS;
}