//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READ_COMMAND,
    PROCESS_NUMBER,
    PROCESS_TEXT,
    EXIT_STATE,
    ERROR_STATE
};

struct context {
    enum state current_state;
    int number_value;
    char text_buffer[64];
    int text_len;
};

void state_machine_init(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = INIT;
    ctx->number_value = 0;
    memset(ctx->text_buffer, 0, sizeof(ctx->text_buffer));
    ctx->text_len = 0;
}

enum state handle_init(struct context *ctx) {
    printf("State Machine Demo\n");
    printf("Commands: number <value>, text <string>, exit\n");
    printf("Enter command: ");
    return READ_COMMAND;
}

enum state handle_read_command(struct context *ctx) {
    char input[128];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return ERROR_STATE;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strncmp(input, "number", 6) == 0) {
        char *num_str = input + 6;
        while (*num_str == ' ') num_str++;
        
        if (*num_str == '\0') {
            printf("Error: Missing number value\n");
            return READ_COMMAND;
        }
        
        char *endptr;
        long val = strtol(num_str, &endptr, 10);
        if (*endptr != '\0' || val < -1000000 || val > 1000000) {
            printf("Error: Invalid number format or range\n");
            return READ_COMMAND;
        }
        
        ctx->number_value = (int)val;
        return PROCESS_NUMBER;
    }
    else if (strncmp(input, "text", 4) == 0) {
        char *text_str = input + 4;
        while (*text_str == ' ') text_str++;
        
        if (*text_str == '\0') {
            printf("Error: Missing text value\n");
            return READ_COMMAND;
        }
        
        size_t len = strlen(text_str);
        if (len >= sizeof(ctx->text_buffer)) {
            printf("Error: Text too long\n");
            return READ_COMMAND;
        }
        
        strncpy(ctx->text_buffer, text_str, sizeof(ctx->text_buffer) - 1);
        ctx->text_buffer[sizeof(ctx->text_buffer) - 1] = '\0';
        ctx->text_len = (int)len;
        return PROCESS_TEXT;
    }
    else if (strcmp(input, "exit") == 0) {
        return EXIT_STATE;
    }
    else {
        printf("Error: Unknown command\n");
        return READ_COMMAND;
    }
}

enum state handle_process_number(struct context *ctx) {
    printf("Processing number: %d\n", ctx->number_value);
    printf("Squared: %ld\n", (long)ctx->number_value * (long)ctx->number_value);
    printf("Enter command: ");
    return READ_COMMAND;
}

enum state handle_process_text(struct context *ctx) {
    printf("Processing text: %s\n", ctx->text_buffer);
    printf("Text length: %d\n", ctx->text_len);
    
    int upper_count = 0;
    for (int i = 0; i < ctx->text_len; i++) {
        if (isupper(ctx->text_buffer[i])) {
            upper_count++;
        }
    }
    printf("Uppercase letters: %d\n", upper_count);
    printf("Enter command: ");
    return READ_COMMAND;
}

int main(void) {
    struct context ctx;
    state_machine_init(&ctx);
    
    while (1) {
        switch (ctx.current_state) {
            case INIT:
                ctx.current_state = handle_init(&ctx);
                break;
                
            case READ_COMMAND:
                ctx.current_state = handle_read_command(&ctx);
                break;
                
            case PROCESS_NUMBER:
                ctx.current_state = handle_process_number(&ctx);
                break;
                
            case PROCESS_TEXT:
                ctx.current_state = handle_process_text(&ctx);
                break;
                
            case EXIT_STATE:
                printf("Goodbye!\n");
                return 0;
                
            case ERROR_STATE:
                printf("Fatal error occurred\n");
                return 1;
                
            default:
                printf("Invalid state\n");
                return 1;
        }
    }
    
    return 0;
}