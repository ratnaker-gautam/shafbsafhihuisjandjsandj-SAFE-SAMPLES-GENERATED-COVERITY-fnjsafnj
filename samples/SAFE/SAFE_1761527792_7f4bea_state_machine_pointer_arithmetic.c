//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_SKIPPING,
    STATE_END
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    char *input_ptr;
    char *input_end;
    int word_count;
    int number_count;
} parser_context_t;

void init_parser(parser_context_t *ctx, char *input) {
    if (!ctx || !input) return;
    
    ctx->current_state = STATE_START;
    ctx->input_ptr = input;
    ctx->input_end = input + strnlen(input, MAX_INPUT_LEN);
    ctx->word_count = 0;
    ctx->number_count = 0;
}

int is_valid_input_char(char c) {
    return isalnum(c) || isspace(c) || c == '\0';
}

int is_digit_char(char c) {
    return c >= '0' && c <= '9';
}

int is_alpha_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void process_state_machine(parser_context_t *ctx) {
    if (!ctx || !ctx->input_ptr || !ctx->input_end) return;
    
    while (ctx->current_state != STATE_END && ctx->input_ptr < ctx->input_end) {
        char current_char = *ctx->input_ptr;
        
        if (!is_valid_input_char(current_char)) {
            ctx->current_state = STATE_SKIPPING;
        }
        
        switch (ctx->current_state) {
            case STATE_START:
                if (is_alpha_char(current_char)) {
                    ctx->current_state = STATE_READING_WORD;
                    ctx->word_count++;
                } else if (is_digit_char(current_char)) {
                    ctx->current_state = STATE_READING_NUMBER;
                    ctx->number_count++;
                } else if (!isspace(current_char)) {
                    ctx->current_state = STATE_SKIPPING;
                }
                ctx->input_ptr++;
                break;
                
            case STATE_READING_WORD:
                if (isspace(current_char)) {
                    ctx->current_state = STATE_START;
                } else if (!is_alpha_char(current_char)) {
                    ctx->current_state = STATE_SKIPPING;
                }
                ctx->input_ptr++;
                break;
                
            case STATE_READING_NUMBER:
                if (isspace(current_char)) {
                    ctx->current_state = STATE_START;
                } else if (!is_digit_char(current_char)) {
                    ctx->current_state = STATE_SKIPPING;
                }
                ctx->input_ptr++;
                break;
                
            case STATE_SKIPPING:
                if (isspace(current_char)) {
                    ctx->current_state = STATE_START;
                }
                ctx->input_ptr++;
                break;
                
            case STATE_END:
                break;
        }
        
        if (ctx->input_ptr >= ctx->input_end) {
            ctx->current_state = STATE_END;
        }
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    parser_context_t parser;
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strnlen(input_buffer, sizeof(input_buffer));
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    init_parser(&parser, input_buffer);
    process_state_machine(&parser);
    
    printf("Words found: %d\n", parser.word_count);
    printf("Numbers found: %d\n", parser.number_count);
    
    return 0;
}