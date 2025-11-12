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
    char buffer[256];
    size_t buffer_pos;
    int number_count;
    int word_count;
};

void init_context(struct context *ctx) {
    ctx->current_state = START;
    ctx->buffer_pos = 0;
    ctx->number_count = 0;
    ctx->word_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_char(char c) {
    return isalnum(c) || isspace(c);
}

void process_buffer(struct context *ctx) {
    if (ctx->buffer_pos > 0) {
        ctx->buffer[ctx->buffer_pos] = '\0';
        if (isdigit(ctx->buffer[0])) {
            ctx->number_count++;
        } else {
            ctx->word_count++;
        }
        ctx->buffer_pos = 0;
    }
}

int main(void) {
    struct context ctx;
    init_context(&ctx);
    
    printf("Enter text (max 1024 chars): ");
    char input[1025];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= input_len && ctx.current_state != ERROR && ctx.current_state != FINISHED; i++) {
        char c = (i < input_len) ? input[i] : ' ';
        
        if (!is_valid_char(c) && c != '\0') {
            ctx.current_state = ERROR;
            break;
        }
        
        switch (ctx.current_state) {
            case START:
                if (isalnum(c)) {
                    if (isdigit(c)) {
                        ctx.current_state = READING_NUMBER;
                    } else {
                        ctx.current_state = READING_WORD;
                    }
                    if (ctx.buffer_pos < sizeof(ctx.buffer) - 1) {
                        ctx.buffer[ctx.buffer_pos++] = c;
                    } else {
                        ctx.current_state = ERROR;
                    }
                } else if (isspace(c)) {
                    ctx.current_state = SKIPPING_SPACE;
                } else if (c == '\0') {
                    ctx.current_state = FINISHED;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (ctx.buffer_pos < sizeof(ctx.buffer) - 1) {
                        ctx.buffer[ctx.buffer_pos++] = c;
                    } else {
                        ctx.current_state = ERROR;
                    }
                } else if (isspace(c) || c == '\0') {
                    process_buffer(&ctx);
                    ctx.current_state = (c == '\0') ? FINISHED : SKIPPING_SPACE;
                } else if (isalpha(c)) {
                    ctx.current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (ctx.buffer_pos < sizeof(ctx.buffer) - 1) {
                        ctx.buffer[ctx.buffer_pos++] = c;
                    } else {
                        ctx.current_state = ERROR;
                    }
                } else if (isspace(c) || c == '\0') {
                    process_buffer(&ctx);
                    ctx.current_state = (c == '\0') ? FINISHED : SKIPPING_SPACE;
                } else if (isdigit(c)) {
                    ctx.current_state = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                if (isalnum(c)) {
                    if (isdigit(c)) {
                        ctx.current_state = READING_NUMBER;
                    } else {
                        ctx.current_state = READING_WORD;
                    }
                    if (ctx.buffer_pos < sizeof(ctx.buffer) - 1) {
                        ctx.buffer[ctx.buffer_pos++] = c;
                    } else {
                        ctx.current_state = ERROR;
                    }
                } else if (c == '\0') {
                    ctx.current_state = FINISHED;
                }
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                break;
        }
    }
    
    if (ctx.current_state == ERROR) {
        fprintf(stderr, "Error: Invalid input format\n");
        return 1;
    }
    
    process_buffer(&ctx);
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}