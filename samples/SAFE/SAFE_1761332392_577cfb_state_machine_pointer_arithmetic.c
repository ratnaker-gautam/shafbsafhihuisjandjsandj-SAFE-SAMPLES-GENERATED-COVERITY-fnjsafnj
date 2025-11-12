//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    char *pos;
    int number_sum;
    int word_count;
} Context;

void process_state(Context *ctx) {
    switch (ctx->current) {
        case START:
            if (*(ctx->pos) == '\0') {
                ctx->current = FINISHED;
            } else if (isdigit(*(ctx->pos))) {
                ctx->current = READING_NUMBER;
            } else if (isalpha(*(ctx->pos))) {
                ctx->current = READING_WORD;
            } else {
                ctx->current = SKIPPING;
            }
            break;
            
        case READING_NUMBER: {
            char *end;
            long num = strtol(ctx->pos, &end, 10);
            if (end > ctx->pos && num >= 0 && num <= 1000000) {
                ctx->number_sum += (int)num;
                ctx->pos = end - 1;
            }
            ctx->current = SKIPPING;
            break;
        }
            
        case READING_WORD:
            while (isalpha(*(ctx->pos))) {
                ctx->pos++;
            }
            ctx->word_count++;
            ctx->pos--;
            ctx->current = SKIPPING;
            break;
            
        case SKIPPING:
            if (*(ctx->pos) == '\0') {
                ctx->current = FINISHED;
            } else if (!isspace(*(ctx->pos))) {
                ctx->current = START;
                ctx->pos--;
            }
            break;
            
        case FINISHED:
            break;
    }
}

int main(void) {
    char input_buffer[MAX_INPUT + 1];
    
    printf("Enter text to analyze: ");
    if (fgets(input_buffer, MAX_INPUT, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input_buffer[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    Context ctx;
    ctx.current = START;
    ctx.input = input_buffer;
    ctx.pos = input_buffer;
    ctx.number_sum = 0;
    ctx.word_count = 0;
    
    int max_iterations = len * 2 + 100;
    int iterations = 0;
    
    while (ctx.current != FINISHED && iterations < max_iterations) {
        process_state(&ctx);
        ctx.pos++;
        iterations++;
        
        if (ctx.pos - input_buffer > (long)len) {
            break;
        }
    }
    
    printf("Word count: %d\n", ctx.word_count);
    printf("Number sum: %d\n", ctx.number_sum);
    
    return 0;
}