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
    struct context ctx = {INIT, 0, 0, NULL, 0};
    char input_buffer[64];
    int input_value;
    
    printf("Enter maximum data count (1-100): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
    
    if (!validate_input(input_buffer, &input_value)) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (input_value < 1 || input_value > 100) {
        printf("Value out of range\n");
        return 1;
    }
    
    ctx.max_data = input_value;
    ctx.data_array = malloc(ctx.max_data * sizeof(int));
    if (ctx.data_array == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    ctx.current_state = READY;
    
    while (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case READY:
                printf("Enter data value %d/%d (or 'done' to finish): ", 
                       ctx.data_count + 1, ctx.max_data);
                
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (strcmp(input_buffer, "done") == 0) {
                    if (ctx.data_count > 0) {
                        ctx.current_state = PROCESSING;
                    } else {
                        printf("No data to process\n");
                        ctx.current_state = FINISHED;
                    }
                    break;
                }
                
                if (!validate_input(input_buffer, &input_value)) {
                    printf("Invalid data value\n");
                    continue;
                }
                
                if (ctx.data_count >= ctx.max_data) {
                    printf("Maximum data count reached\n");
                    ctx.current_state = PROCESSING;
                    break;
                }
                
                ctx.data_array[ctx.data_count] = input_value;
                ctx.data_count++;
                break;
                
            case PROCESSING:
                if (ctx.processed_count < ctx.data_count) {
                    int result = ctx.data_array[ctx.processed_count] * 2;
                    if (ctx.data_array[ctx.processed_count] > 0 && 
                        result < ctx.data_array[ctx.processed_count]) {
                        printf("Integer overflow detected\n");
                        ctx.current_state = ERROR;
                        break;
                    }
                    printf("Processed data[%d]: %d -> %d\n", 
                           ctx.processed_count, 
                           ctx.data_array[ctx.processed_count], 
                           result);
                    ctx.processed_count++;
                } else {
                    ctx.current_state = FINISHED;
                }
                break;
                
            default:
                ctx.current_state = ERROR;
                break;
        }
    }
    
    if (ctx.current_state == FINISHED) {
        printf("Processing completed successfully\n");
        printf("Total data processed: %d\n", ctx.processed_count);
    } else if (ctx.current_state == ERROR) {
        printf("Processing terminated with error\n");
    }
    
    cleanup_context(&ctx);
    return ctx.current_state == FINISHED ? 0 : 1;
}