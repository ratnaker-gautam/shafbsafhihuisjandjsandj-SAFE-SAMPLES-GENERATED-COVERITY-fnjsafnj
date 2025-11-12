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
    STATE_READING_OTHER,
    STATE_END
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int number_count;
    int alpha_count;
    int other_count;
} parser_context_t;

static void initialize_parser(parser_context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->alpha_count = 0;
    ctx->other_count = 0;
}

static parser_state_t transition_state(parser_state_t current, char input) {
    switch (current) {
        case STATE_START:
            if (isdigit(input)) return STATE_READING_NUM;
            if (isalpha(input)) return STATE_READING_ALPHA;
            if (input != '\0') return STATE_READING_OTHER;
            return STATE_END;
            
        case STATE_READING_NUM:
            if (isdigit(input)) return STATE_READING_NUM;
            if (isalpha(input)) return STATE_READING_ALPHA;
            if (input == '\0') return STATE_END;
            return STATE_READING_OTHER;
            
        case STATE_READING_ALPHA:
            if (isalpha(input)) return STATE_READING_ALPHA;
            if (isdigit(input)) return STATE_READING_NUM;
            if (input == '\0') return STATE_END;
            return STATE_READING_OTHER;
            
        case STATE_READING_OTHER:
            if (isdigit(input)) return STATE_READING_NUM;
            if (isalpha(input)) return STATE_READING_ALPHA;
            if (input == '\0') return STATE_END;
            return STATE_READING_OTHER;
            
        case STATE_END:
            return STATE_END;
            
        default:
            return STATE_START;
    }
}

static void process_character(parser_context_t *ctx, char input) {
    if (ctx == NULL) return;
    
    parser_state_t previous_state = ctx->current_state;
    ctx->current_state = transition_state(previous_state, input);
    
    if (previous_state != ctx->current_state) {
        switch (previous_state) {
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
}

static void finalize_counting(parser_context_t *ctx) {
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
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 2];
    parser_context_t parser;
    
    printf("Enter a string to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_buffer)) {
        fprintf(stderr, "Invalid input: empty or too long\n");
        return EXIT_FAILURE;
    }
    
    initialize_parser(&parser);
    
    for (size_t i = 0; i <= input_len; i++) {
        process_character(&parser, input_buffer[i]);
    }
    
    finalize_counting(&parser);
    
    printf("Analysis results:\n");
    printf("Number sequences: %d\n", parser.number_count);
    printf("Alpha sequences: %d\n", parser.alpha_count);
    printf("Other sequences: %d\n", parser.other_count);
    
    return EXIT_SUCCESS;
}