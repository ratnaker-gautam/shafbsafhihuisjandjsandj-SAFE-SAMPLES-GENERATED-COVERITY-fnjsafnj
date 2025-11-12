//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUMBER,
    READING_STRING,
    ERROR,
    DONE
};

struct Context {
    enum State current_state;
    int number_count;
    int string_count;
    char buffer[256];
    int buffer_pos;
};

void init_context(struct Context *ctx) {
    ctx->current_state = INIT;
    ctx->number_count = 0;
    ctx->string_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_number(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int is_valid_string(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i])) return 0;
    }
    return 1;
}

void process_input(struct Context *ctx, const char *input) {
    if (input == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    size_t len = strlen(input);
    if (len == 0 || len >= 255) {
        ctx->current_state = ERROR;
        return;
    }
    
    if (strcmp(input, "END") == 0) {
        ctx->current_state = DONE;
        return;
    }
    
    if (is_valid_number(input)) {
        ctx->current_state = READING_NUMBER;
        ctx->number_count++;
    } else if (is_valid_string(input)) {
        ctx->current_state = READING_STRING;
        ctx->string_count++;
    } else {
        ctx->current_state = ERROR;
    }
}

void print_state_info(enum State state) {
    switch (state) {
        case INIT:
            printf("Ready for input\n");
            break;
        case READING_NUMBER:
            printf("Processing number\n");
            break;
        case READING_STRING:
            printf("Processing string\n");
            break;
        case ERROR:
            printf("Invalid input detected\n");
            break;
        case DONE:
            printf("Processing complete\n");
            break;
    }
}

int main() {
    struct Context ctx;
    init_context(&ctx);
    
    char input[256];
    int max_attempts = 100;
    int attempts = 0;
    
    printf("Enter inputs (numbers, strings, or 'END' to finish):\n");
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR && attempts < max_attempts) {
        print_state_info(ctx.current_state);
        printf("> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            ctx.current_state = ERROR;
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        process_input(&ctx, input);
        attempts++;
    }
    
    if (ctx.current_state == DONE) {
        printf("\nProcessing complete!\n");
        printf("Numbers processed: %d\n", ctx.number_count);
        printf("Strings processed: %d\n", ctx.string_count);
    } else if (ctx.current_state == ERROR) {
        printf("\nError occurred during processing\n");
    } else {
        printf("\nMaximum attempts reached\n");
    }
    
    return ctx.current_state == DONE ? 0 : 1;
}