//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

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

static void process_char(parser_context_t *ctx) {
    char current = *(ctx->input_ptr);
    
    switch (ctx->current_state) {
        case STATE_START:
            if (isalpha(current)) {
                ctx->current_state = STATE_READING_WORD;
                ctx->word_count++;
            } else if (isdigit(current)) {
                ctx->current_state = STATE_READING_NUMBER;
                ctx->number_count++;
            } else if (!isspace(current)) {
                ctx->current_state = STATE_SKIPPING;
            }
            break;
            
        case STATE_READING_WORD:
            if (!isalpha(current)) {
                if (isdigit(current)) {
                    ctx->current_state = STATE_READING_NUMBER;
                    ctx->number_count++;
                } else if (isspace(current)) {
                    ctx->current_state = STATE_START;
                } else {
                    ctx->current_state = STATE_SKIPPING;
                }
            }
            break;
            
        case STATE_READING_NUMBER:
            if (!isdigit(current)) {
                if (isalpha(current)) {
                    ctx->current_state = STATE_READING_WORD;
                    ctx->word_count++;
                } else if (isspace(current)) {
                    ctx->current_state = STATE_START;
                } else {
                    ctx->current_state = STATE_SKIPPING;
                }
            }
            break;
            
        case STATE_SKIPPING:
            if (isspace(current)) {
                ctx->current_state = STATE_START;
            } else if (isalpha(current)) {
                ctx->current_state = STATE_READING_WORD;
                ctx->word_count++;
            } else if (isdigit(current)) {
                ctx->current_state = STATE_READING_NUMBER;
                ctx->number_count++;
            }
            break;
            
        case STATE_END:
            break;
    }
}

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT) return 0;
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT];
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT - 1);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input_buffer)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    parser_context_t ctx;
    ctx.current_state = STATE_START;
    ctx.input_ptr = input_buffer;
    ctx.input_end = input_buffer + input_len;
    ctx.word_count = 0;
    ctx.number_count = 0;
    
    while (ctx.input_ptr < ctx.input_end && ctx.current_state != STATE_END) {
        process_char(&ctx);
        ctx.input_ptr++;
    }
    
    printf("Analysis complete:\n");
    printf("Words found: %d\n", ctx.word_count);
    printf("Numbers found: %d\n", ctx.number_count);
    
    return 0;
}