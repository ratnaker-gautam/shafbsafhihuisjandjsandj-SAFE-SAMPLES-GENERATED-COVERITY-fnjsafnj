//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

struct Context {
    enum State current_state;
    int number_count;
    int word_count;
    char buffer[256];
    size_t buffer_pos;
};

void context_init(struct Context *ctx) {
    ctx->current_state = START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_input_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

void process_char(struct Context *ctx, char c) {
    switch (ctx->current_state) {
        case START:
            if (isdigit((unsigned char)c)) {
                ctx->current_state = READING_NUMBER;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (isalpha((unsigned char)c)) {
                ctx->current_state = READING_WORD;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (isspace((unsigned char)c)) {
                ctx->current_state = SKIPPING_SPACE;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_NUMBER:
            if (isdigit((unsigned char)c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace((unsigned char)c)) {
                ctx->number_count++;
                ctx->buffer_pos = 0;
                ctx->current_state = SKIPPING_SPACE;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_WORD:
            if (isalpha((unsigned char)c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace((unsigned char)c)) {
                ctx->word_count++;
                ctx->buffer_pos = 0;
                ctx->current_state = SKIPPING_SPACE;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case SKIPPING_SPACE:
            if (isdigit((unsigned char)c)) {
                ctx->current_state = READING_NUMBER;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (isalpha((unsigned char)c)) {
                ctx->current_state = READING_WORD;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (!isspace((unsigned char)c)) {
                ctx->current_state = ERROR;
            }
            break;
            
        case FINISHED:
        case ERROR:
            break;
    }
}

void finalize_processing(struct Context *ctx) {
    if (ctx->current_state == READING_NUMBER) {
        ctx->number_count++;
    } else if (ctx->current_state == READING_WORD) {
        ctx->word_count++;
    }
    ctx->current_state = FINISHED;
}

int main(void) {
    struct Context ctx;
    context_init(&ctx);
    
    printf("Enter text (alphanumeric characters and spaces only): ");
    fflush(stdout);
    
    char input_buffer[1024];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input_buffer[i];
        if (!is_valid_input_char(c)) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
        process_char(&ctx, c);
        if (ctx.current_state == ERROR) {
            fprintf(stderr, "Invalid input format\n");
            return 1;
        }
    }
    
    finalize_processing(&ctx);
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}