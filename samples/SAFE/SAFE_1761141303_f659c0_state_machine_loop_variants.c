//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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

int validate_input(const char *input, int *value) {
    if (input == NULL) return 0;
    char *endptr;
    long temp = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\0') return 0;
    if (temp < INT32_MIN || temp > INT32_MAX) return 0;
    *value = (int)temp;
    return 1;
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
    
    char buffer[64];
    int input_value;
    
    while (1) {
        switch (ctx.current_state) {
            case INIT:
                printf("Enter maximum data count (1-100): ");
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                buffer[strcspn(buffer, "\n")] = '\0';
                if (!validate_input(buffer, &ctx.max_data)) {
                    ctx.current_state = ERROR;
                    break;
                }
                if (ctx.max_data < 1 || ctx.max_data > 100) {
                    ctx.current_state = ERROR;
                    break;
                }
                ctx.data_array = malloc(ctx.max_data * sizeof(int));
                if (ctx.data_array == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                ctx.current_state = READY;
                break;
                
            case READY:
                printf("Enter data value %d/%d (or 'process' to start): ", 
                       ctx.data_count + 1, ctx.max_data);
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                buffer[strcspn(buffer, "\n")] = '\0';
                if (strcmp(buffer, "process") == 0) {
                    if (ctx.data_count > 0) {
                        ctx.current_state = PROCESSING;
                    } else {
                        printf("No data to process\n");
                    }
                    break;
                }
                if (!validate_input(buffer, &input_value)) {
                    printf("Invalid input\n");
                    break;
                }
                if (ctx.data_count >= ctx.max_data) {
                    printf("Maximum data reached\n");
                    break;
                }
                ctx.data_array[ctx.data_count] = input_value;
                ctx.data_count++;
                break;
                
            case PROCESSING:
                {
                    int i = 0;
                    while (i < ctx.data_count) {
                        ctx.data_array[i] *= 2;
                        i++;
                    }
                    
                    int j = 0;
                    do {
                        ctx.data_array[j] += 1;
                        j++;
                    } while (j < ctx.data_count);
                    
                    for (int k = 0; k < ctx.data_count; k++) {
                        ctx.data_array[k] -= 1;
                    }
                }
                ctx.processed_count = ctx.data_count;
                ctx.current_state = FINISHED;
                break;
                
            case FINISHED:
                printf("Processing complete. Results:\n");
                for (int i = 0; i < ctx.processed_count; i++) {
                    printf("Data[%d] = %d\n", i, ctx.data_array[i]);
                }
                printf("Enter 'quit' to exit or 'reset' to start over: ");
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    cleanup(&ctx);
                    return 1;
                }
                buffer[strcspn(buffer, "\n")] = '\0';
                if (strcmp(buffer, "quit") == 0) {
                    cleanup(&ctx);
                    return 0;
                } else if (strcmp(buffer, "reset") == 0) {
                    cleanup(&ctx);
                    ctx.current_state = INIT;
                    ctx.data_count = 0;
                    ctx.max_data = 0;
                    ctx.data_array = NULL;
                    ctx.processed_count = 0;
                }
                break;
                
            case ERROR:
                printf("Error occurred. Cleaning up.\n");
                cleanup(&ctx);
                return 1;
        }
    }
    
    return 0;
}