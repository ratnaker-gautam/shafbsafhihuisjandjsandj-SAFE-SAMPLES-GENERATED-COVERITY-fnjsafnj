//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    END
};

struct Context {
    enum State current_state;
    int number_sum;
    int word_count;
    char buffer[256];
    int buffer_pos;
};

void init_context(struct Context *ctx) {
    ctx->current_state = START;
    ctx->number_sum = 0;
    ctx->word_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_digit_char(char c) {
    return (c >= '0' && c <= '9');
}

int is_alpha_char(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int is_space_char(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

void process_char(struct Context *ctx, char c) {
    switch (ctx->current_state) {
        case START:
            if (is_digit_char(c)) {
                ctx->buffer[ctx->buffer_pos++] = c;
                ctx->current_state = READING_NUMBER;
            } else if (is_alpha_char(c)) {
                ctx->buffer[ctx->buffer_pos++] = c;
                ctx->current_state = READING_WORD;
            } else if (is_space_char(c)) {
                ctx->current_state = SKIPPING_SPACE;
            } else {
                ctx->current_state = END;
            }
            break;
            
        case READING_NUMBER:
            if (is_digit_char(c)) {
                if (ctx->buffer_pos < 255) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                }
            } else {
                ctx->buffer[ctx->buffer_pos] = '\0';
                int num = atoi(ctx->buffer);
                ctx->number_sum += num;
                ctx->buffer_pos = 0;
                
                if (is_space_char(c)) {
                    ctx->current_state = SKIPPING_SPACE;
                } else if (is_alpha_char(c)) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                    ctx->current_state = READING_WORD;
                } else {
                    ctx->current_state = END;
                }
            }
            break;
            
        case READING_WORD:
            if (is_alpha_char(c)) {
                if (ctx->buffer_pos < 255) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                }
            } else {
                ctx->word_count++;
                ctx->buffer_pos = 0;
                
                if (is_space_char(c)) {
                    ctx->current_state = SKIPPING_SPACE;
                } else if (is_digit_char(c)) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                    ctx->current_state = READING_NUMBER;
                } else {
                    ctx->current_state = END;
                }
            }
            break;
            
        case SKIPPING_SPACE:
            if (is_digit_char(c)) {
                ctx->buffer[ctx->buffer_pos++] = c;
                ctx->current_state = READING_NUMBER;
            } else if (is_alpha_char(c)) {
                ctx->buffer[ctx->buffer_pos++] = c;
                ctx->current_state = READING_WORD;
            } else if (!is_space_char(c)) {
                ctx->current_state = END;
            }
            break;
            
        case END:
            break;
    }
}

void finalize_processing(struct Context *ctx) {
    switch (ctx->current_state) {
        case READING_NUMBER:
            ctx->buffer[ctx->buffer_pos] = '\0';
            ctx->number_sum += atoi(ctx->buffer);
            break;
            
        case READING_WORD:
            ctx->word_count++;
            break;
            
        default:
            break;
    }
}

int main(void) {
    struct Context ctx;
    init_context(&ctx);
    
    printf("Enter text (max 1000 characters): ");
    
    char input[1001];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    for (size_t i = 0; i < len && ctx.current_state != END; i++) {
        process_char(&ctx, input[i]);
    }
    
    finalize_processing(&ctx);
    
    printf("Sum of numbers: %d\n", ctx.number_sum);
    printf("Word count: %d\n", ctx.word_count);
    
    return 0;
}