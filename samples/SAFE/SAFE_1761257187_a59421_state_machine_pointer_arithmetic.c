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

static int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (const char *p = input; *p != '\0'; p++) {
        if (!isprint(*p)) return 0;
    }
    return 1;
}

static void process_state_machine(parser_context_t *ctx) {
    while (ctx->current_state != STATE_END && ctx->current_state != STATE_ERROR) {
        if (ctx->input_ptr >= ctx->input_end) {
            ctx->current_state = STATE_END;
            break;
        }

        char current_char = *ctx->input_ptr;
        
        switch (ctx->current_state) {
            case STATE_START:
                if (isalpha(current_char)) {
                    ctx->current_state = STATE_READING_WORD;
                    ctx->word_count++;
                } else if (isdigit(current_char)) {
                    ctx->current_state = STATE_READING_NUMBER;
                    ctx->number_count++;
                } else if (!isspace(current_char)) {
                    ctx->current_state = STATE_ERROR;
                }
                ctx->input_ptr++;
                break;
                
            case STATE_READING_WORD:
                if (isalpha(current_char)) {
                    ctx->input_ptr++;
                } else if (isspace(current_char)) {
                    ctx->current_state = STATE_START;
                    ctx->input_ptr++;
                } else if (isdigit(current_char) || !isprint(current_char)) {
                    ctx->current_state = STATE_ERROR;
                } else {
                    ctx->current_state = STATE_START;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(current_char)) {
                    ctx->input_ptr++;
                } else if (isspace(current_char)) {
                    ctx->current_state = STATE_START;
                    ctx->input_ptr++;
                } else if (isalpha(current_char) || !isprint(current_char)) {
                    ctx->current_state = STATE_ERROR;
                } else {
                    ctx->current_state = STATE_START;
                }
                break;
                
            default:
                ctx->current_state = STATE_ERROR;
                break;
        }
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 2];
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (!is_valid_input(input_buffer)) {
        fprintf(stderr, "Invalid input: must be printable characters, 1-%d chars\n", MAX_INPUT_LEN);
        return EXIT_FAILURE;
    }
    
    parser_context_t parser;
    initialize_parser(&parser, input_buffer);
    process_state_machine(&parser);
    
    if (parser.current_state == STATE_ERROR) {
        fprintf(stderr, "Error: Invalid character sequence detected\n");
        return EXIT_FAILURE;
    }
    
    printf("Analysis complete:\n");
    printf("Words found: %d\n", parser.word_count);
    printf("Numbers found: %d\n", parser.number_count);
    
    return EXIT_SUCCESS;
}