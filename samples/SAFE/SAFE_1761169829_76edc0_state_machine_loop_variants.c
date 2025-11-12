//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
};

struct Context {
    enum State current_state;
    int number_sum;
    int word_count;
    char current_word[64];
    int word_pos;
};

void state_machine_init(struct Context *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_sum = 0;
    ctx->word_count = 0;
    ctx->word_pos = 0;
    memset(ctx->current_word, 0, sizeof(ctx->current_word));
}

int is_valid_input_char(char c) {
    return isalnum(c) || isspace(c) || c == '\n' || c == '\t';
}

void process_char(struct Context *ctx, char c) {
    switch (ctx->current_state) {
        case STATE_START:
            if (isdigit(c)) {
                ctx->current_state = STATE_READING_NUMBER;
                ctx->number_sum += (c - '0');
            } else if (isalpha(c)) {
                ctx->current_state = STATE_READING_WORD;
                ctx->current_word[0] = c;
                ctx->word_pos = 1;
            } else if (!isspace(c)) {
                ctx->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_READING_NUMBER:
            if (isdigit(c)) {
                ctx->number_sum += (c - '0');
            } else if (isalpha(c)) {
                ctx->current_state = STATE_READING_WORD;
                ctx->current_word[0] = c;
                ctx->word_pos = 1;
            } else if (isspace(c)) {
                ctx->current_state = STATE_START;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_READING_WORD:
            if (isalpha(c)) {
                if (ctx->word_pos < 63) {
                    ctx->current_word[ctx->word_pos++] = c;
                }
            } else if (isdigit(c)) {
                ctx->current_state = STATE_READING_NUMBER;
                ctx->number_sum += (c - '0');
                ctx->word_count++;
                ctx->word_pos = 0;
                memset(ctx->current_word, 0, sizeof(ctx->current_word));
            } else if (isspace(c)) {
                ctx->current_state = STATE_START;
                ctx->word_count++;
                ctx->word_pos = 0;
                memset(ctx->current_word, 0, sizeof(ctx->current_word));
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            break;
            
        case STATE_DONE:
            break;
    }
}

int main(void) {
    struct Context ctx;
    state_machine_init(&ctx);
    
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
        if (!is_valid_input_char(input_buffer[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    size_t i = 0;
    while (i < input_len && ctx.current_state != STATE_ERROR) {
        process_char(&ctx, input_buffer[i]);
        i++;
    }
    
    if (ctx.current_state == STATE_READING_WORD) {
        ctx.word_count++;
    }
    
    if (ctx.current_state == STATE_ERROR) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Sum of digits: %d\n", ctx.number_sum);
    printf("Word count: %d\n", ctx.word_count);
    
    return 0;
}