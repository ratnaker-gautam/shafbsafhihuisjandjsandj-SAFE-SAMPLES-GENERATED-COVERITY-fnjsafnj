//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READ_CHAR,
    COUNT_DIGITS,
    COUNT_LETTERS,
    FINISH,
    ERROR
};

struct Context {
    enum State current_state;
    char input[256];
    size_t position;
    int digit_count;
    int letter_count;
    int result;
};

void process_state(struct Context *ctx);

void start_state(struct Context *ctx) {
    if (ctx->position >= sizeof(ctx->input) - 1) {
        ctx->current_state = ERROR;
    } else if (ctx->input[ctx->position] == '\0') {
        ctx->current_state = FINISH;
    } else {
        ctx->current_state = READ_CHAR;
    }
    process_state(ctx);
}

void read_char_state(struct Context *ctx) {
    char c = ctx->input[ctx->position];
    if (c >= '0' && c <= '9') {
        ctx->current_state = COUNT_DIGITS;
    } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        ctx->current_state = COUNT_LETTERS;
    } else {
        ctx->position++;
        ctx->current_state = START;
    }
    process_state(ctx);
}

void count_digits_state(struct Context *ctx) {
    if (ctx->digit_count < 1000) {
        ctx->digit_count++;
    }
    ctx->position++;
    ctx->current_state = START;
    process_state(ctx);
}

void count_letters_state(struct Context *ctx) {
    if (ctx->letter_count < 1000) {
        ctx->letter_count++;
    }
    ctx->position++;
    ctx->current_state = START;
    process_state(ctx);
}

void finish_state(struct Context *ctx) {
    if (ctx->digit_count > 0 && ctx->letter_count > 0) {
        ctx->result = 1;
    } else {
        ctx->result = 0;
    }
}

void error_state(struct Context *ctx) {
    ctx->result = -1;
}

void process_state(struct Context *ctx) {
    switch (ctx->current_state) {
        case START:
            start_state(ctx);
            break;
        case READ_CHAR:
            read_char_state(ctx);
            break;
        case COUNT_DIGITS:
            count_digits_state(ctx);
            break;
        case COUNT_LETTERS:
            count_letters_state(ctx);
            break;
        case FINISH:
            finish_state(ctx);
            break;
        case ERROR:
            error_state(ctx);
            break;
    }
}

void initialize_context(struct Context *ctx, const char *input) {
    ctx->current_state = START;
    strncpy(ctx->input, input, sizeof(ctx->input) - 1);
    ctx->input[sizeof(ctx->input) - 1] = '\0';
    ctx->position = 0;
    ctx->digit_count = 0;
    ctx->letter_count = 0;
    ctx->result = 0;
}

int analyze_string(const char *input) {
    if (input == NULL) {
        return -1;
    }
    if (strlen(input) >= 256) {
        return -1;
    }
    
    struct Context ctx;
    initialize_context(&ctx, input);
    process_state(&ctx);
    return ctx.result;
}

int main() {
    char input[256];
    
    printf("Enter a string to analyze (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    int result = analyze_string(input);
    
    switch (result) {
        case 1:
            printf("String contains both digits and letters\n");
            break;
        case 0:
            printf("String does not contain both digits and letters\n");
            break;
        case -1:
            printf("Error processing string\n");
            break;
    }
    
    return 0;
}