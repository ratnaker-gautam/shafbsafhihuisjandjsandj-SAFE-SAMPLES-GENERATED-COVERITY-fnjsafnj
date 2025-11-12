//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_END,
    STATE_ERROR
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int word_count;
    int number_count;
    char *input_ptr;
    char *input_end;
} parser_context_t;

static void initialize_parser(parser_context_t *ctx, char *input) {
    ctx->current_state = STATE_START;
    ctx->word_count = 0;
    ctx->number_count = 0;
    ctx->input_ptr = input;
    ctx->input_end = input + strlen(input);
}

static int is_valid_input_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

static int validate_input_string(const char *input) {
    if (input == NULL) return 0;
    
    const char *ptr = input;
    while (*ptr != '\0') {
        if (!is_valid_input_char(*ptr)) return 0;
        ptr++;
    }
    return 1;
}

static void process_state_start(parser_context_t *ctx) {
    if (ctx->input_ptr >= ctx->input_end) {
        ctx->current_state = STATE_END;
        return;
    }
    
    char current_char = *ctx->input_ptr;
    if (isalpha((unsigned char)current_char)) {
        ctx->current_state = STATE_READING_WORD;
    } else if (isdigit((unsigned char)current_char)) {
        ctx->current_state = STATE_READING_NUMBER;
    } else if (isspace((unsigned char)current_char)) {
        ctx->input_ptr++;
    } else {
        ctx->current_state = STATE_ERROR;
    }
}

static void process_state_reading_word(parser_context_t *ctx) {
    ctx->word_count++;
    
    while (ctx->input_ptr < ctx->input_end && isalpha((unsigned char)*ctx->input_ptr)) {
        ctx->input_ptr++;
    }
    
    ctx->current_state = STATE_START;
}

static void process_state_reading_number(parser_context_t *ctx) {
    ctx->number_count++;
    
    while (ctx->input_ptr < ctx->input_end && isdigit((unsigned char)*ctx->input_ptr)) {
        ctx->input_ptr++;
    }
    
    ctx->current_state = STATE_START;
}

static void run_state_machine(parser_context_t *ctx) {
    while (ctx->current_state != STATE_END && ctx->current_state != STATE_ERROR) {
        switch (ctx->current_state) {
            case STATE_START:
                process_state_start(ctx);
                break;
            case STATE_READING_WORD:
                process_state_reading_word(ctx);
                break;
            case STATE_READING_NUMBER:
                process_state_reading_number(ctx);
                break;
            default:
                ctx->current_state = STATE_ERROR;
                break;
        }
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    
    printf("Enter text (alphanumeric and spaces only): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (!validate_input_string(input_buffer)) {
        fprintf(stderr, "Invalid input: only alphanumeric characters and spaces allowed\n");
        return EXIT_FAILURE;
    }
    
    parser_context_t parser;
    initialize_parser(&parser, input_buffer);
    run_state_machine(&parser);
    
    if (parser.current_state == STATE_ERROR) {
        fprintf(stderr, "Error during parsing\n");
        return EXIT_FAILURE;
    }
    
    printf("Words: %d\n", parser.word_count);
    printf("Numbers: %d\n", parser.number_count);
    
    return EXIT_SUCCESS;
}