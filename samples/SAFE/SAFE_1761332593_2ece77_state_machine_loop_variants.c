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
    char buffer[256];
    size_t buffer_pos;
};

void state_machine_reset(struct Context *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_sum = 0;
    ctx->word_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_input_char(char c) {
    return isalnum(c) || isspace(c) || c == '+' || c == '-';
}

int parse_number(const char *str, size_t len) {
    if (len == 0 || len > 10) return 0;
    
    char temp[32];
    if (len >= sizeof(temp)) return 0;
    memcpy(temp, str, len);
    temp[len] = '\0';
    
    char *endptr;
    long val = strtol(temp, &endptr, 10);
    if (endptr != temp + len) return 0;
    if (val < -1000000 || val > 1000000) return 0;
    
    return (int)val;
}

void process_buffer(struct Context *ctx) {
    if (ctx->buffer_pos == 0) return;
    
    ctx->buffer[ctx->buffer_pos] = '\0';
    
    if (isdigit(ctx->buffer[0]) || 
        (ctx->buffer[0] == '-' && isdigit(ctx->buffer[1])) ||
        (ctx->buffer[0] == '+' && isdigit(ctx->buffer[1]))) {
        int num = parse_number(ctx->buffer, ctx->buffer_pos);
        if (num != 0 || (ctx->buffer_pos == 1 && ctx->buffer[0] == '0')) {
            ctx->number_sum += num;
            ctx->current_state = STATE_READING_NUMBER;
        } else {
            ctx->word_count++;
            ctx->current_state = STATE_READING_WORD;
        }
    } else if (isalpha(ctx->buffer[0])) {
        ctx->word_count++;
        ctx->current_state = STATE_READING_WORD;
    } else {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int main(void) {
    struct Context ctx;
    state_machine_reset(&ctx);
    
    printf("Enter text (numbers and words, max 1000 chars): ");
    fflush(stdout);
    
    char input[1002];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[--input_len] = '\0';
    }
    
    if (input_len >= sizeof(input) - 1) {
        printf("Input too long\n");
        return 1;
    }
    
    size_t i = 0;
    while (i < input_len) {
        char c = input[i];
        
        if (!is_valid_input_char(c)) {
            ctx.current_state = STATE_ERROR;
            break;
        }
        
        switch (ctx.current_state) {
            case STATE_START:
            case STATE_READING_NUMBER:
            case STATE_READING_WORD:
                if (isspace(c)) {
                    if (ctx.buffer_pos > 0) {
                        process_buffer(&ctx);
                        if (ctx.current_state == STATE_ERROR) break;
                    }
                } else {
                    if (ctx.buffer_pos >= sizeof(ctx.buffer) - 1) {
                        ctx.current_state = STATE_ERROR;
                        break;
                    }
                    ctx.buffer[ctx.buffer_pos++] = c;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_DONE:
                break;
        }
        
        if (ctx.current_state == STATE_ERROR) break;
        i++;
    }
    
    if (ctx.current_state != STATE_ERROR && ctx.buffer_pos > 0) {
        process_buffer(&ctx);
    }
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Sum of numbers: %d\n", ctx.number_sum);
    printf("Word count: %d\n", ctx.word_count);
    
    return 0;
}