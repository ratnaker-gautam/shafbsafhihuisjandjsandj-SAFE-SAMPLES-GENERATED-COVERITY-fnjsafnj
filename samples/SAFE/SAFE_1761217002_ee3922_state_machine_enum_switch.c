//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
};

struct context {
    enum state current_state;
    int number_count;
    int word_count;
    char buffer[256];
    size_t buffer_pos;
};

void context_init(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

void process_input(struct context *ctx, const char *input) {
    if (ctx == NULL || input == NULL) return;
    
    size_t input_len = strlen(input);
    if (input_len == 0) return;
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        
        if (!is_valid_char(c)) {
            ctx->current_state = STATE_ERROR;
            break;
        }
        
        switch (ctx->current_state) {
            case STATE_START:
                if (isdigit((unsigned char)c)) {
                    ctx->current_state = STATE_READING_NUMBER;
                    ctx->buffer[0] = c;
                    ctx->buffer_pos = 1;
                } else if (isalpha((unsigned char)c)) {
                    ctx->current_state = STATE_READING_WORD;
                    ctx->buffer[0] = c;
                    ctx->buffer_pos = 1;
                } else if (!isspace((unsigned char)c)) {
                    ctx->current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit((unsigned char)c)) {
                    if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                        ctx->buffer[ctx->buffer_pos++] = c;
                    } else {
                        ctx->current_state = STATE_ERROR;
                    }
                } else if (isspace((unsigned char)c)) {
                    ctx->number_count++;
                    ctx->current_state = STATE_START;
                    ctx->buffer_pos = 0;
                    memset(ctx->buffer, 0, sizeof(ctx->buffer));
                } else if (isalpha((unsigned char)c)) {
                    ctx->current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha((unsigned char)c)) {
                    if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                        ctx->buffer[ctx->buffer_pos++] = c;
                    } else {
                        ctx->current_state = STATE_ERROR;
                    }
                } else if (isspace((unsigned char)c)) {
                    ctx->word_count++;
                    ctx->current_state = STATE_START;
                    ctx->buffer_pos = 0;
                    memset(ctx->buffer, 0, sizeof(ctx->buffer));
                } else if (isdigit((unsigned char)c)) {
                    ctx->current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_DONE:
                break;
        }
        
        if (ctx->current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (ctx->current_state != STATE_ERROR) {
        switch (ctx->current_state) {
            case STATE_READING_NUMBER:
                ctx->number_count++;
                break;
            case STATE_READING_WORD:
                ctx->word_count++;
                break;
            default:
                break;
        }
        ctx->current_state = STATE_DONE;
    }
}

int main(void) {
    struct context ctx;
    char input[512];
    
    context_init(&ctx);
    
    printf("Enter text (alphanumeric characters and spaces only): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    process_input(&ctx, input);
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return EXIT_SUCCESS;
}