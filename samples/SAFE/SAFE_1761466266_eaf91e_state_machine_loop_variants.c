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
    
    char input_buffer[64];
    int input_value;
    
    while (1) {
        switch (ctx.current_state) {
            case INIT: {
                printf("Enter maximum data count (1-100): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (!validate_input(input_buffer, &ctx.max_data)) {
                    printf("Invalid input. Must be integer 1-100.\n");
                    break;
                }
                
                if (ctx.max_data < 1 || ctx.max_data > 100) {
                    printf("Invalid range. Must be 1-100.\n");
                    break;
                }
                
                ctx.data_array = malloc(ctx.max_data * sizeof(int));
                if (ctx.data_array == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                ctx.current_state = READY;
                break;
            }
            
            case READY: {
                if (ctx.data_count >= ctx.max_data) {
                    ctx.current_state = PROCESSING;
                    break;
                }
                
                printf("Enter data value %d/%d: ", ctx.data_count + 1, ctx.max_data);
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (strlen(input_buffer) == 0) {
                    ctx.current_state = PROCESSING;
                    break;
                }
                
                if (!validate_input(input_buffer, &input_value)) {
                    printf("Invalid input. Must be integer.\n");
                    break;
                }
                
                ctx.data_array[ctx.data_count] = input_value;
                ctx.data_count++;
                
                if (ctx.data_count >= ctx.max_data) {
                    ctx.current_state = PROCESSING;
                }
                break;
            }
            
            case PROCESSING: {
                int i = 0;
                int sum = 0;
                
                do {
                    if (i < ctx.data_count) {
                        sum += ctx.data_array[i];
                        i++;
                    }
                } while (i < ctx.data_count);
                
                int j;
                for (j = 0; j < ctx.data_count; j++) {
                    if (ctx.data_array[j] > 0) {
                        ctx.processed_count++;
                    }
                }
                
                printf("Data processing complete.\n");
                printf("Sum: %d, Positive values: %d\n", sum, ctx.processed_count);
                ctx.current_state = FINISHED;
                break;
            }
            
            case FINISHED: {
                printf("Processing finished. Exiting.\n");
                cleanup(&ctx);
                return 0;
            }
            
            case ERROR: {
                printf("Error occurred. Cleaning up.\n");
                cleanup(&ctx);
                return 1;
            }
        }
    }
    
    return 0;
}