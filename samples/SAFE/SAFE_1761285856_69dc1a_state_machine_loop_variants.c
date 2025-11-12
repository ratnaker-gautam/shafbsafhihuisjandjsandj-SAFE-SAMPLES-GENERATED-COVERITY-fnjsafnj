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
    int sum;
    int valid_inputs;
};

int is_valid_number(const char* str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') return 0;
    }
    return 1;
}

void process_state_machine(struct context* ctx) {
    while (ctx->current_state != DONE && ctx->current_state != ERROR) {
        switch (ctx->current_state) {
            case INIT:
                ctx->data_count = 0;
                ctx->sum = 0;
                ctx->valid_inputs = 0;
                ctx->current_state = READING;
                break;
                
            case READING:
                if (ctx->data_count >= 10) {
                    ctx->current_state = PROCESSING;
                    break;
                }
                
                char input[32];
                printf("Enter number %d (or 'done' to finish): ", ctx->data_count + 1);
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                size_t len = strlen(input);
                if (len > 0 && input[len - 1] == '\n') {
                    input[len - 1] = '\0';
                }
                
                if (strcmp(input, "done") == 0) {
                    ctx->current_state = PROCESSING;
                    break;
                }
                
                if (!is_valid_number(input)) {
                    printf("Invalid input. Please enter numbers only.\n");
                    break;
                }
                
                long num = strtol(input, NULL, 10);
                if (num < -1000 || num > 1000) {
                    printf("Number out of range (-1000 to 1000).\n");
                    break;
                }
                
                ctx->values[ctx->data_count] = (int)num;
                ctx->data_count++;
                ctx->valid_inputs++;
                break;
                
            case PROCESSING:
                for (int i = 0; i < ctx->valid_inputs; i++) {
                    ctx->sum += ctx->values[i];
                }
                ctx->current_state = OUTPUT;
                break;
                
            case OUTPUT:
                if (ctx->valid_inputs > 0) {
                    printf("Processed %d numbers:\n", ctx->valid_inputs);
                    for (int i = 0; i < ctx->valid_inputs; i++) {
                        printf("  Number %d: %d\n", i + 1, ctx->values[i]);
                    }
                    printf("Sum: %d\n", ctx->sum);
                    printf("Average: %.2f\n", (double)ctx->sum / ctx->valid_inputs);
                } else {
                    printf("No valid numbers were entered.\n");
                }
                ctx->current_state = DONE;
                break;
                
            default:
                ctx->current_state = ERROR;
                break;
        }
    }
    
    if (ctx->current_state == ERROR) {
        printf("An error occurred during processing.\n");
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    process_state_machine(&ctx);
    return 0;
}