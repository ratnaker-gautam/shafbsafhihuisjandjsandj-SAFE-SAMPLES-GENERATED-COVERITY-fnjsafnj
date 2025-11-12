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
    
    while (str[i] != '\0') {
        if (str[i] < '0' || str[i] > '9') return 0;
        val = val * 10 + (str[i] - '0');
        if (val > INT32_MAX) return 0;
        i++;
    }
    
    val *= sign;
    if (val < INT32_MIN || val > INT32_MAX) return 0;
    *result = (int)val;
    return 1;
}

void state_machine(struct context* ctx) {
    int running = 1;
    
    while (running) {
        switch (ctx->current_state) {
            case INIT: {
                ctx->data_count = 0;
                ctx->output_count = 0;
                memset(ctx->values, 0, sizeof(ctx->values));
                memset(ctx->processed, 0, sizeof(ctx->processed));
                ctx->current_state = READING;
                break;
            }
            
            case READING: {
                char input[20];
                printf("Enter number (or 'done' to finish): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                size_t len = strlen(input);
                if (len > 0 && input[len-1] == '\n') {
                    input[len-1] = '\0';
                }
                
                if (strcmp(input, "done") == 0) {
                    if (ctx->data_count > 0) {
                        ctx->current_state = PROCESSING;
                    } else {
                        ctx->current_state = DONE;
                    }
                    break;
                }
                
                if (!validate_input(input)) {
                    printf("Invalid input. Please enter digits only.\n");
                    break;
                }
                
                int value;
                if (!safe_atoi(input, &value)) {
                    printf("Number too large.\n");
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
                while (i < ctx->data_count) {
                    ctx->processed[i] = ctx->values[i] * 2;
                    if (ctx->processed[i] < 0) {
                        ctx->current_state = ERROR;
                        break;
                    }
                    i++;
                }
                if (ctx->current_state != ERROR) {
                    ctx->current_state = OUTPUT;
                }
                break;
            }
            
            case OUTPUT: {
                int j = 0;
                do {
                    printf("Processed value %d: %d\n", j+1, ctx->processed[j]);
                    ctx->output_count++;
                    j++;
                } while (j < ctx->data_count);
                ctx->current_state = DONE;
                break;
            }
            
            case ERROR: {
                printf("Error occurred during processing.\n");
                ctx->current_state = DONE;
                break;
            }
            
            case DONE: {
                printf("Processing complete. %d values processed.\n", ctx->output_count);
                running = 0;
                break;
            }
        }
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    state_machine(&ctx);
    return 0;
}