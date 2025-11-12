//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READ_CHAR,
    PROCESS_WORD,
    COUNT_WORD,
    FINISH,
    ERROR
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_pos;
    unsigned int word_count;
    unsigned int char_count;
};

void state_machine_reset(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = START;
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int process_input(struct context *ctx, const char *input) {
    if (ctx == NULL || input == NULL) return -1;
    
    size_t input_len = strlen(input);
    if (input_len >= 256) return -1;
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        
        switch (ctx->current_state) {
            case START:
                if (isalpha(c)) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                    ctx->char_count++;
                    ctx->current_state = READ_CHAR;
                } else if (c == ' ' || c == '\t') {
                    ctx->current_state = START;
                } else if (c == '\0') {
                    ctx->current_state = FINISH;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            case READ_CHAR:
                if (isalpha(c)) {
                    if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                        ctx->buffer[ctx->buffer_pos++] = c;
                        ctx->char_count++;
                    } else {
                        ctx->current_state = ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    ctx->buffer[ctx->buffer_pos] = '\0';
                    ctx->current_state = PROCESS_WORD;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            case PROCESS_WORD:
                if (ctx->buffer_pos > 0) {
                    ctx->word_count++;
                    printf("Word %u: %s\n", ctx->word_count, ctx->buffer);
                    ctx->buffer_pos = 0;
                    memset(ctx->buffer, 0, sizeof(ctx->buffer));
                }
                
                if (c == ' ' || c == '\t') {
                    ctx->current_state = START;
                } else if (c == '\0') {
                    ctx->current_state = FINISH;
                } else if (isalpha(c)) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                    ctx->char_count++;
                    ctx->current_state = READ_CHAR;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            case COUNT_WORD:
                if (ctx->buffer_pos > 0) {
                    ctx->word_count++;
                    ctx->buffer_pos = 0;
                }
                ctx->current_state = FINISH;
                break;
                
            case FINISH:
                break;
                
            case ERROR:
                return -1;
        }
        
        if (ctx->current_state == ERROR) {
            return -1;
        }
    }
    
    if (ctx->current_state == READ_CHAR) {
        ctx->current_state = PROCESS_WORD;
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
            printf("Word %u: %s\n", ctx->word_count, ctx->buffer);
        }
    }
    
    return 0;
}

int main(void) {
    struct context ctx;
    state_machine_reset(&ctx);
    
    char input[512];
    printf("Enter text to process (max 511 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (process_input(&ctx, input) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Total words: %u\n", ctx.word_count);
    printf("Total characters: %u\n", ctx.char_count);
    
    return 0;
}