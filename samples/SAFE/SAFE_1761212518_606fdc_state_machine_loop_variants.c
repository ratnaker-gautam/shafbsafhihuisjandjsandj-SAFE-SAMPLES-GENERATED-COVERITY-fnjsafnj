//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum state {
    INIT,
    READY,
    PROCESSING,
    FINISHED,
    ERROR
};

struct context {
    enum state current_state;
    int data_count;
    int max_data;
    int *data_array;
    int processed_count;
};

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 10) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

int parse_number(const char *input) {
    long value = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (value > LONG_MAX / 10) return -1;
        value = value * 10;
        int digit = input[i] - '0';
        if (value > LONG_MAX - digit) return -1;
        value = value + digit;
        if (value > INT_MAX) return -1;
    }
    return (int)value;
}

void cleanup(struct context *ctx) {
    if (ctx != NULL && ctx->data_array != NULL) {
        free(ctx->data_array);
        ctx->data_array = NULL;
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    ctx.data_count = 0;
    ctx.max_data = 0;
    ctx.data_array = NULL;
    ctx.processed_count = 0;
    
    char input_buffer[32];
    int result;
    
    while (1) {
        switch (ctx.current_state) {
            case INIT:
                printf("Enter maximum number of data items (1-100): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (!validate_input(input_buffer)) {
                    printf("Invalid input\n");
                    break;
                }
                
                result = parse_number(input_buffer);
                if (result < 1 || result > 100) {
                    printf("Value must be between 1 and 100\n");
                    break;
                }
                
                ctx.max_data = result;
                ctx.data_array = malloc(ctx.max_data * sizeof(int));
                if (ctx.data_array == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                ctx.current_state = READY;
                break;
                
            case READY:
                printf("Enter data value %d (or 'done' to finish): ", ctx.data_count + 1);
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (strcmp(input_buffer, "done") == 0) {
                    if (ctx.data_count == 0) {
                        printf("No data entered\n");
                        break;
                    }
                    ctx.current_state = PROCESSING;
                    break;
                }
                
                if (!validate_input(input_buffer)) {
                    printf("Invalid input\n");
                    break;
                }
                
                result = parse_number(input_buffer);
                if (result < 0) {
                    printf("Value too large\n");
                    break;
                }
                
                if (ctx.data_count >= ctx.max_data) {
                    printf("Maximum data items reached\n");
                    ctx.current_state = PROCESSING;
                    break;
                }
                
                ctx.data_array[ctx.data_count] = result;
                ctx.data_count++;
                break;
                
            case PROCESSING:
                printf("Processing data...\n");
                ctx.processed_count = 0;
                int i = 0;
                while (i < ctx.data_count) {
                    if (ctx.data_array[i] > INT_MAX / 2) {
                        printf("Value too large to process\n");
                        ctx.current_state = ERROR;
                        break;
                    }
                    ctx.data_array[i] = ctx.data_array[i] * 2;
                    ctx.processed_count++;
                    i++;
                }
                if (ctx.current_state == ERROR) break;
                
                for (int j = 0; j < ctx.data_count; j++) {
                    printf("Processed value %d: %d\n", j + 1, ctx.data_array[j]);
                }
                
                ctx.current_state = FINISHED;
                break;
                
            case FINISHED:
                printf("Processing complete. %d items processed.\n", ctx.processed_count);
                cleanup(&ctx);
                return 0;
                
            case ERROR:
                printf("An error occurred\n");
                cleanup(&ctx);
                return 1;
        }
    }
    
    cleanup(&ctx);
    return 0;
}