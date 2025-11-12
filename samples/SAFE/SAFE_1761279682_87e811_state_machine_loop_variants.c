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
    if (input == NULL || value == NULL) return 0;
    
    char *endptr;
    long temp = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (temp < INT32_MIN || temp > INT32_MAX) return 0;
    
    *value = (int)temp;
    return 1;
}

void cleanup_context(struct context *ctx) {
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
    
    char input_buffer[64];
    int should_continue = 1;
    
    while (should_continue) {
        switch (ctx.current_state) {
            case INIT: {
                printf("Enter maximum data items (1-100): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                int max_data;
                if (!validate_input(input_buffer, &max_data) || max_data < 1 || max_data > 100) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                ctx.max_data = max_data;
                ctx.data_array = malloc(max_data * sizeof(int));
                if (ctx.data_array == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                ctx.current_state = READY;
                break;
            }
            
            case READY: {
                printf("Enter data value %d/%d (or 'done' to finish): ", ctx.data_count + 1, ctx.max_data);
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                if (strncmp(input_buffer, "done", 4) == 0) {
                    ctx.current_state = PROCESSING;
                    break;
                }
                
                int value;
                if (!validate_input(input_buffer, &value)) {
                    printf("Invalid input. Please enter a number or 'done'.\n");
                    break;
                }
                
                if (ctx.data_count >= ctx.max_data) {
                    ctx.current_state = PROCESSING;
                    break;
                }
                
                ctx.data_array[ctx.data_count] = value;
                ctx.data_count++;
                
                if (ctx.data_count >= ctx.max_data) {
                    ctx.current_state = PROCESSING;
                }
                break;
            }
            
            case PROCESSING: {
                int i = 0;
                while (i < ctx.data_count) {
                    if (ctx.data_array[i] % 2 == 0) {
                        ctx.data_array[i] *= 2;
                    } else {
                        ctx.data_array[i] += 1;
                    }
                    i++;
                }
                
                int j = 0;
                do {
                    if (j < ctx.data_count) {
                        ctx.processed_count++;
                    }
                    j++;
                } while (j < ctx.data_count);
                
                ctx.current_state = FINISHED;
                break;
            }
            
            case FINISHED: {
                printf("Processing complete. Results:\n");
                
                for (int k = 0; k < ctx.data_count; k++) {
                    printf("Item %d: %d\n", k + 1, ctx.data_array[k]);
                }
                
                printf("Total items processed: %d\n", ctx.processed_count);
                should_continue = 0;
                break;
            }
            
            case ERROR: {
                printf("An error occurred during processing.\n");
                cleanup_context(&ctx);
                should_continue = 0;
                break;
            }
        }
    }
    
    cleanup_context(&ctx);
    return ctx.current_state == FINISHED ? 0 : 1;
}