//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    INIT,
    READING,
    PROCESSING,
    OUTPUT,
    ERROR,
    DONE
};

struct context {
    enum state current_state;
    int data_count;
    int values[10];
    int processed[10];
    int output_count;
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 10) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

int safe_atoi(const char* str, int* result) {
    if (str == NULL || result == NULL) return 0;
    long val = 0;
    int sign = 1;
    size_t i = 0;
    
    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }
    
    for (; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') return 0;
        val = val * 10 + (str[i] - '0');
        if (val > INT32_MAX) return 0;
    }
    
    val *= sign;
    if (val < INT32_MIN || val > INT32_MAX) return 0;
    *result = (int)val;
    return 1;
}

void state_machine_loop(struct context* ctx) {
    char input[20];
    int temp_value;
    
    while (ctx->current_state != DONE && ctx->current_state != ERROR) {
        switch (ctx->current_state) {
            case INIT:
                printf("Enter up to 10 numbers (one per line, 'done' to finish):\n");
                ctx->data_count = 0;
                ctx->current_state = READING;
                break;
                
            case READING:
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                input[strcspn(input, "\n")] = '\0';
                
                if (strcmp(input, "done") == 0) {
                    if (ctx->data_count == 0) {
                        printf("No data entered\n");
                        ctx->current_state = ERROR;
                    } else {
                        ctx->current_state = PROCESSING;
                    }
                    break;
                }
                
                if (!validate_input(input)) {
                    printf("Invalid input: %s\n", input);
                    ctx->current_state = ERROR;
                    break;
                }
                
                if (!safe_atoi(input, &temp_value)) {
                    printf("Conversion failed: %s\n", input);
                    ctx->current_state = ERROR;
                    break;
                }
                
                if (ctx->data_count >= 10) {
                    printf("Maximum 10 numbers allowed\n");
                    ctx->current_state = ERROR;
                    break;
                }
                
                ctx->values[ctx->data_count] = temp_value;
                ctx->data_count++;
                break;
                
            case PROCESSING:
                for (int i = 0; i < ctx->data_count; i++) {
                    ctx->processed[i] = ctx->values[i] * 2;
                }
                ctx->current_state = OUTPUT;
                break;
                
            case OUTPUT:
                printf("Processed values (doubled):\n");
                for (int i = 0; i < ctx->data_count; i++) {
                    printf("%d -> %d\n", ctx->values[i], ctx->processed[i]);
                }
                ctx->current_state = DONE;
                break;
                
            default:
                ctx->current_state = ERROR;
                break;
        }
    }
    
    if (ctx->current_state == ERROR) {
        printf("State machine terminated with error\n");
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    ctx.data_count = 0;
    ctx.output_count = 0;
    
    state_machine_loop(&ctx);
    
    return ctx.current_state == DONE ? EXIT_SUCCESS : EXIT_FAILURE;
}