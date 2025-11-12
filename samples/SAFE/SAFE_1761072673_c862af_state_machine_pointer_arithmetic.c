//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING,
    FINISHED
} State;

typedef struct {
    State current;
    char *input;
    char *position;
    int number_sum;
    int word_count;
} Context;

void process_input(Context *ctx);
int is_digit_char(char c);
int is_alpha_char(char c);

int main(void) {
    char user_input[MAX_INPUT + 1];
    Context ctx = {START, NULL, NULL, 0, 0};
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(user_input);
    if (len > 0 && user_input[len - 1] == '\n') {
        user_input[len - 1] = '\0';
    }
    
    if (len >= sizeof(user_input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    ctx.input = user_input;
    ctx.position = user_input;
    
    process_input(&ctx);
    
    printf("Number sum: %d\n", ctx.number_sum);
    printf("Word count: %d\n", ctx.word_count);
    
    return 0;
}

void process_input(Context *ctx) {
    if (ctx == NULL || ctx->input == NULL || ctx->position == NULL) {
        return;
    }
    
    char *end = ctx->input + strlen(ctx->input);
    
    while (ctx->current != FINISHED && ctx->position < end) {
        char current_char = *ctx->position;
        
        switch (ctx->current) {
            case START:
                if (is_digit_char(current_char)) {
                    ctx->current = READING_NUMBER;
                } else if (is_alpha_char(current_char)) {
                    ctx->current = READING_WORD;
                } else {
                    ctx->current = SKIPPING;
                }
                break;
                
            case READING_NUMBER: {
                if (!is_digit_char(current_char)) {
                    char *num_start = ctx->position;
                    while (num_start > ctx->input && is_digit_char(*(num_start - 1))) {
                        num_start--;
                    }
                    
                    char num_buf[16];
                    size_t num_len = ctx->position - num_start;
                    if (num_len >= sizeof(num_buf)) {
                        num_len = sizeof(num_buf) - 1;
                    }
                    
                    memcpy(num_buf, num_start, num_len);
                    num_buf[num_len] = '\0';
                    
                    int num = atoi(num_buf);
                    if (num > 0) {
                        if (ctx->number_sum <= INT_MAX - num) {
                            ctx->number_sum += num;
                        }
                    }
                    
                    if (is_alpha_char(current_char)) {
                        ctx->current = READING_WORD;
                    } else {
                        ctx->current = SKIPPING;
                    }
                }
                break;
            }
                
            case READING_WORD:
                if (!is_alpha_char(current_char)) {
                    ctx->word_count++;
                    if (is_digit_char(current_char)) {
                        ctx->current = READING_NUMBER;
                    } else {
                        ctx->current = SKIPPING;
                    }
                }
                break;
                
            case SKIPPING:
                if (is_digit_char(current_char)) {
                    ctx->current = READING_NUMBER;
                } else if (is_alpha_char(current_char)) {
                    ctx->current = READING_WORD;
                }
                break;
                
            case FINISHED:
                return;
        }
        
        ctx->position++;
    }
    
    if (ctx->current == READING_WORD) {
        ctx->word_count++;
    } else if (ctx->current == READING_NUMBER) {
        char *num_start = ctx->position;
        while (num_start > ctx->input && is_digit_char(*(num_start - 1))) {
            num_start--;
        }
        
        char num_buf[16];
        size_t num_len = ctx->position - num_start;
        if (num_len >= sizeof(num_buf)) {
            num_len = sizeof(num_buf) - 1;
        }
        
        memcpy(num_buf, num_start, num_len);
        num_buf[num_len] = '\0';
        
        int num = atoi(num_buf);
        if (num > 0) {
            if (ctx->number_sum <= INT_MAX - num) {
                ctx->number_sum += num;
            }
        }
    }
    
    ctx->current = FINISHED;
}

int is_digit_char(char c) {
    return c >= '0' && c <= '9';
}

int is_alpha_char(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}