//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

struct context {
    enum state current_state;
    int number_count;
    int word_count;
    int position;
    char current_token[64];
};

void initialize_context(struct context *ctx) {
    ctx->current_state = START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->position = 0;
    memset(ctx->current_token, 0, sizeof(ctx->current_token));
}

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 1024) return 0;
    return 1;
}

void process_character(struct context *ctx, char c) {
    switch (ctx->current_state) {
        case START:
            if (isspace(c)) {
                ctx->current_state = SKIPPING_SPACE;
            } else if (isdigit(c)) {
                ctx->current_token[ctx->position++] = c;
                ctx->current_state = READING_NUMBER;
            } else if (isalpha(c)) {
                ctx->current_token[ctx->position++] = c;
                ctx->current_state = READING_WORD;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_NUMBER:
            if (isdigit(c)) {
                if (ctx->position < 63) {
                    ctx->current_token[ctx->position++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace(c)) {
                ctx->number_count++;
                ctx->position = 0;
                memset(ctx->current_token, 0, sizeof(ctx->current_token));
                ctx->current_state = SKIPPING_SPACE;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_WORD:
            if (isalnum(c)) {
                if (ctx->position < 63) {
                    ctx->current_token[ctx->position++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace(c)) {
                ctx->word_count++;
                ctx->position = 0;
                memset(ctx->current_token, 0, sizeof(ctx->current_token));
                ctx->current_state = SKIPPING_SPACE;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case SKIPPING_SPACE:
            if (isspace(c)) {
                break;
            } else if (isdigit(c)) {
                ctx->current_token[ctx->position++] = c;
                ctx->current_state = READING_NUMBER;
            } else if (isalpha(c)) {
                ctx->current_token[ctx->position++] = c;
                ctx->current_state = READING_WORD;
            } else if (c == '\0') {
                ctx->current_state = FINISHED;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case FINISHED:
        case ERROR:
            break;
    }
}

void finalize_processing(struct context *ctx) {
    switch (ctx->current_state) {
        case READING_NUMBER:
            ctx->number_count++;
            break;
        case READING_WORD:
            ctx->word_count++;
            break;
        case SKIPPING_SPACE:
            ctx->current_state = FINISHED;
            break;
        default:
            break;
    }
}

int main(void) {
    char input_buffer[1025];
    struct context ctx;
    
    printf("Enter text to analyze (max 1024 characters): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input_buffer)) {
        printf("Invalid input\n");
        return 1;
    }
    
    initialize_context(&ctx);
    
    for (size_t i = 0; i <= input_len; i++) {
        char current_char = input_buffer[i];
        process_character(&ctx, current_char);
        
        if (ctx.current_state == ERROR) {
            printf("Error: Invalid input format at position %zu\n", i);
            return 1;
        }
    }
    
    finalize_processing(&ctx);
    
    if (ctx.current_state != FINISHED) {
        printf("Error: Incomplete processing\n");
        return 1;
    }
    
    printf("Analysis complete:\n");
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}