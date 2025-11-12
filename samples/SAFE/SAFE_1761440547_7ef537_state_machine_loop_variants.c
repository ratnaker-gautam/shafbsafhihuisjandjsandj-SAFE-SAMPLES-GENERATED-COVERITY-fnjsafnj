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
    DONE,
    ERROR
};

struct Context {
    enum State current_state;
    int data_count;
    int max_data;
    int *data_buffer;
    int processed_sum;
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

int safe_atoi(const char *str, int *result) {
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
        if (val * sign > INT32_MAX || val * sign < INT32_MIN) return 0;
    }
    
    *result = (int)(val * sign);
    return 1;
}

void state_machine_loop(struct Context *ctx) {
    char input_buffer[32];
    int input_value;
    
    while (ctx->current_state != DONE && ctx->current_state != ERROR) {
        switch (ctx->current_state) {
            case INIT:
                printf("Enter maximum number of data points (1-10): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    ctx->current_state = ERROR;
                    break;
                }
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (!validate_input(input_buffer) || !safe_atoi(input_buffer, &ctx->max_data)) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                if (ctx->max_data < 1 || ctx->max_data > 10) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                ctx->data_buffer = malloc(ctx->max_data * sizeof(int));
                if (ctx->data_buffer == NULL) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                ctx->data_count = 0;
                ctx->processed_sum = 0;
                ctx->current_state = READING;
                break;
                
            case READING:
                if (ctx->data_count >= ctx->max_data) {
                    ctx->current_state = PROCESSING;
                    break;
                }
                
                printf("Enter data point %d: ", ctx->data_count + 1);
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    ctx->current_state = ERROR;
                    break;
                }
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (!validate_input(input_buffer) || !safe_atoi(input_buffer, &input_value)) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                ctx->data_buffer[ctx->data_count] = input_value;
                ctx->data_count++;
                
                if (ctx->data_count >= ctx->max_data) {
                    ctx->current_state = PROCESSING;
                }
                break;
                
            case PROCESSING:
                ctx->processed_sum = 0;
                for (int i = 0; i < ctx->data_count; i++) {
                    if (ctx->processed_sum > INT32_MAX - ctx->data_buffer[i]) {
                        ctx->current_state = ERROR;
                        break;
                    }
                    ctx->processed_sum += ctx->data_buffer[i];
                }
                if (ctx->current_state != ERROR) {
                    ctx->current_state = OUTPUT;
                }
                break;
                
            case OUTPUT:
                printf("Processed %d data points\n", ctx->data_count);
                printf("Sum of all values: %d\n", ctx->processed_sum);
                printf("Individual values: ");
                for (int i = 0; i < ctx->data_count; i++) {
                    printf("%d", ctx->data_buffer[i]);
                    if (i < ctx->data_count - 1) printf(", ");
                }
                printf("\n");
                ctx->current_state = DONE;
                break;
                
            default:
                ctx->current_state = ERROR;
                break;
        }
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = INIT;
    ctx.data_buffer = NULL;
    
    state_machine_loop(&ctx);
    
    if (ctx.data_buffer != NULL) {
        free(ctx.data_buffer);
    }
    
    if (ctx.current_state == ERROR) {
        printf("Error occurred during processing\n");
        return 1;
    }
    
    return 0;
}