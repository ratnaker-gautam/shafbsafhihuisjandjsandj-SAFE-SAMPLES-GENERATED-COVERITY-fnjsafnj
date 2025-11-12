//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    char *input_ptr;
    char *output;
    char *output_ptr;
    int number_count;
    int word_count;
} Context;

void init_context(Context *ctx, char *input) {
    ctx->current = START;
    ctx->input_ptr = input;
    ctx->output = malloc(MAX_INPUT * 2);
    ctx->output_ptr = ctx->output;
    ctx->number_count = 0;
    ctx->word_count = 0;
    *(ctx->output_ptr) = '\0';
}

void cleanup_context(Context *ctx) {
    free(ctx->output);
}

State handle_start(Context *ctx) {
    if (*(ctx->input_ptr) == '\0') {
        return FINISHED;
    }
    if (isdigit(*(ctx->input_ptr))) {
        return READING_NUMBER;
    }
    if (isalpha(*(ctx->input_ptr))) {
        return READING_WORD;
    }
    return SKIPPING;
}

State handle_reading_number(Context *ctx) {
    char *start = ctx->input_ptr;
    while (isdigit(*(ctx->input_ptr))) {
        *(ctx->output_ptr) = *(ctx->input_ptr);
        ctx->output_ptr++;
        ctx->input_ptr++;
    }
    *(ctx->output_ptr) = ' ';
    ctx->output_ptr++;
    ctx->number_count++;
    return (*(ctx->input_ptr) == '\0') ? FINISHED : START;
}

State handle_reading_word(Context *ctx) {
    char *start = ctx->input_ptr;
    while (isalpha(*(ctx->input_ptr))) {
        *(ctx->output_ptr) = *(ctx->input_ptr);
        ctx->output_ptr++;
        ctx->input_ptr++;
    }
    *(ctx->output_ptr) = ' ';
    ctx->output_ptr++;
    ctx->word_count++;
    return (*(ctx->input_ptr) == '\0') ? FINISHED : START;
}

State handle_skipping(Context *ctx) {
    while (*(ctx->input_ptr) && !isdigit(*(ctx->input_ptr)) && !isalpha(*(ctx->input_ptr))) {
        ctx->input_ptr++;
    }
    return (*(ctx->input_ptr) == '\0') ? FINISHED : START;
}

void run_state_machine(Context *ctx) {
    while (ctx->current != FINISHED) {
        switch (ctx->current) {
            case START:
                ctx->current = handle_start(ctx);
                break;
            case READING_NUMBER:
                ctx->current = handle_reading_number(ctx);
                break;
            case READING_WORD:
                ctx->current = handle_reading_word(ctx);
                break;
            case SKIPPING:
                ctx->current = handle_skipping(ctx);
                break;
            case FINISHED:
                break;
        }
    }
    *(ctx->output_ptr) = '\0';
}

int main(void) {
    char input[MAX_INPUT];
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1 && input[0] == '\n') {
        printf("Empty input\n");
        return 0;
    }
    
    Context ctx;
    init_context(&ctx, input);
    
    run_state_machine(&ctx);
    
    printf("Extracted: %s\n", ctx.output);
    printf("Numbers: %d, Words: %d\n", ctx.number_count, ctx.word_count);
    
    cleanup_context(&ctx);
    return 0;
}