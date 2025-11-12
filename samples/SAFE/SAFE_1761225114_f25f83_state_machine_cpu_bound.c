//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    STATE_READING_SPECIAL,
    STATE_ERROR
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int number_count;
    int alpha_count;
    int special_count;
    int total_chars;
} parser_context_t;

static void initialize_context(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->alpha_count = 0;
    ctx->special_count = 0;
    ctx->total_chars = 0;
}

static parser_state_t transition_state(parser_state_t current, char c) {
    switch (current) {
        case STATE_START:
            if (isdigit(c)) return STATE_READING_NUM;
            if (isalpha(c)) return STATE_READING_ALPHA;
            if (c != ' ' && c != '\t' && c != '\n') return STATE_READING_SPECIAL;
            return STATE_START;
        
        case STATE_READING_NUM:
            if (isdigit(c)) return STATE_READING_NUM;
            if (isalpha(c)) return STATE_READING_ALPHA;
            if (c == ' ' || c == '\t' || c == '\n') return STATE_START;
            return STATE_READING_SPECIAL;
        
        case STATE_READING_ALPHA:
            if (isalpha(c)) return STATE_READING_ALPHA;
            if (isdigit(c)) return STATE_READING_NUM;
            if (c == ' ' || c == '\t' || c == '\n') return STATE_START;
            return STATE_READING_SPECIAL;
        
        case STATE_READING_SPECIAL:
            if (isdigit(c)) return STATE_READING_NUM;
            if (isalpha(c)) return STATE_READING_ALPHA;
            if (c == ' ' || c == '\t' || c == '\n') return STATE_START;
            return STATE_READING_SPECIAL;
        
        default:
            return STATE_ERROR;
    }
}

static void process_character(parser_context_t *ctx, char c) {
    parser_state_t new_state = transition_state(ctx->current_state, c);
    
    if (ctx->current_state == STATE_START && new_state != STATE_START) {
        switch (new_state) {
            case STATE_READING_NUM: ctx->number_count++; break;
            case STATE_READING_ALPHA: ctx->alpha_count++; break;
            case STATE_READING_SPECIAL: ctx->special_count++; break;
            default: break;
        }
    }
    
    ctx->current_state = new_state;
    if (c != ' ' && c != '\t' && c != '\n') {
        ctx->total_chars++;
    }
}

static int validate_input(const char *input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 0;
    }
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    parser_context_t context;
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    initialize_context(&context);
    
    for (size_t i = 0; i < input_len; i++) {
        process_character(&context, input_buffer[i]);
    }
    
    printf("Analysis results:\n");
    printf("Total characters: %d\n", context.total_chars);
    printf("Number sequences: %d\n", context.number_count);
    printf("Alpha sequences: %d\n", context.alpha_count);
    printf("Special sequences: %d\n", context.special_count);
    
    return EXIT_SUCCESS;
}