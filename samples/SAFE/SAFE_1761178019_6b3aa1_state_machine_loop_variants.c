//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    INIT,
    READING,
    PROCESSING,
    OUTPUT,
    ERROR,
    DONE
};

struct Context {
    enum State current_state;
    int data_count;
    int values[10];
    int processed[10];
    int error_code;
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

int safe_atoi(const char* str) {
    if (str == NULL) return -1;
    long result = 0;
    int sign = 1;
    size_t i = 0;
    
    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }
    
    for (; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') return -1;
        result = result * 10 + (str[i] - '0');
        if (result > INT32_MAX) return -1;
    }
    
    return (int)(result * sign);
}

void state_machine_loop(struct Context* ctx) {
    int running = 1;
    
    while (running) {
        switch (ctx->current_state) {
            case INIT:
                ctx->data_count = 0;
                ctx->error_code = 0;
                memset(ctx->values, 0, sizeof(ctx->values));
                memset(ctx->processed, 0, sizeof(ctx->processed));
                ctx->current_state = READING;
                break;
                
            case READING: {
                char input[32];
                printf("Enter number (or 'done' to finish): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx->current_state = ERROR;
                    ctx->error_code = 1;
                    break;
                }
                
                size_t len = strlen(input);
                if (len > 0 && input[len-1] == '\n') {
                    input[len-1] = '\0';
                }
                
                if (strcmp(input, "done") == 0) {
                    if (ctx->data_count == 0) {
                        printf("No data entered. Exiting.\n");
                        ctx->current_state = DONE;
                    } else {
                        ctx->current_state = PROCESSING;
                    }
                    break;
                }
                
                if (!validate_input(input)) {
                    printf("Invalid input. Please enter numbers only.\n");
                    break;
                }
                
                int value = safe_atoi(input);
                if (value == -1) {
                    printf("Invalid number format.\n");
                    break;
                }
                
                if (ctx->data_count >= 10) {
                    printf("Maximum 10 numbers allowed.\n");
                    ctx->current_state = PROCESSING;
                    break;
                }
                
                ctx->values[ctx->data_count] = value;
                ctx->data_count++;
                break;
            }
            
            case PROCESSING: {
                int i = 0;
                do {
                    if (i >= ctx->data_count) break;
                    ctx->processed[i] = ctx->values[i] * 2;
                    i++;
                } while (i < ctx->data_count);
                
                ctx->current_state = OUTPUT;
                break;
            }
            
            case OUTPUT: {
                printf("Processed results:\n");
                for (int i = 0; i < ctx->data_count; i++) {
                    printf("Value %d: %d -> %d\n", i+1, ctx->values[i], ctx->processed[i]);
                }
                ctx->current_state = DONE;
                break;
            }
            
            case ERROR:
                printf("Error occurred: %d\n", ctx->error_code);
                ctx->current_state = DONE;
                break;
                
            case DONE:
                running = 0;
                break;
        }
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = INIT;
    state_machine_loop(&ctx);
    return 0;
}