//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int number_count;
    int word_count;
    int total_chars;
} parser_context_t;

static void initialize_context(parser_context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->total_chars = 0;
}

static int is_valid_input_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

static parser_state_t handle_start_state(char c, parser_context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (isdigit((unsigned char)c)) {
        ctx->number_count++;
        return STATE_READING_NUMBER;
    } else if (isalpha((unsigned char)c)) {
        ctx->word_count++;
        return STATE_READING_WORD;
    } else if (isspace((unsigned char)c)) {
        return STATE_START;
    }
    return STATE_ERROR;
}

static parser_state_t handle_reading_number(char c, parser_context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (isdigit((unsigned char)c)) {
        return STATE_READING_NUMBER;
    } else if (isspace((unsigned char)c)) {
        return STATE_START;
    } else if (isalpha((unsigned char)c)) {
        return STATE_ERROR;
    }
    return STATE_ERROR;
}

static parser_state_t handle_reading_word(char c, parser_context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (isalpha((unsigned char)c)) {
        return STATE_READING_WORD;
    } else if (isspace((unsigned char)c)) {
        return STATE_START;
    } else if (isdigit((unsigned char)c)) {
        return STATE_ERROR;
    }
    return STATE_ERROR;
}

static int process_input(const char *input, parser_context_t *ctx) {
    if (input == NULL || ctx == NULL) return 0;
    
    size_t len = strnlen(input, MAX_INPUT_LEN);
    if (len >= MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        ctx->total_chars++;
        
        if (!is_valid_input_char(c)) {
            ctx->current_state = STATE_ERROR;
            return 0;
        }
        
        switch (ctx->current_state) {
            case STATE_START:
                ctx->current_state = handle_start_state(c, ctx);
                break;
            case STATE_READING_NUMBER:
                ctx->current_state = handle_reading_number(c, ctx);
                break;
            case STATE_READING_WORD:
                ctx->current_state = handle_reading_word(c, ctx);
                break;
            case STATE_ERROR:
                return 0;
            case STATE_END:
                return 1;
        }
        
        if (ctx->current_state == STATE_ERROR) {
            return 0;
        }
    }
    
    ctx->current_state = STATE_END;
    return 1;
}

static void perform_cpu_bound_work(parser_context_t *ctx) {
    if (ctx == NULL) return;
    
    int iterations = (ctx->number_count * 1000) + (ctx->word_count * 500);
    if (iterations > 1000000) iterations = 1000000;
    
    volatile int result = 0;
    for (int i = 0; i < iterations; i++) {
        result += (i * i) % 1000;
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    parser_context_t context;
    
    initialize_context(&context);
    
    printf("Enter text (alphanumeric with spaces): ");
    
    if (fgets(input_buffer, MAX_INPUT_LEN, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strnlen(input_buffer, MAX_INPUT_LEN);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input_buffer[0] == '\0')) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    int success = process_input(input_buffer, &context);
    
    if (!success || context.current_state == STATE_ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    perform_cpu_bound_work(&context);
    
    printf("Parsing completed successfully\n");
    printf("Numbers found: %d\n", context.number_count);
    printf("Words found: %d\n", context.word_count);
    printf("Total characters processed: %d\n", context.total_chars);
    
    return EXIT_SUCCESS;
}