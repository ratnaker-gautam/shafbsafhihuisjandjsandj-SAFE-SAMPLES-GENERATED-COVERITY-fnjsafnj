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

void state_machine_step(struct context *ctx) {
    switch (ctx->current_state) {
        case INIT:
            ctx->data_count = 0;
            ctx->processed_count = 0;
            ctx->current_state = READY;
            break;
            
        case READY: {
            char input[32];
            printf("Enter max data items (1-100): ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                ctx->current_state = ERROR;
                break;
            }
            input[strcspn(input, "\n")] = '\0';
            
            int max_data;
            if (!validate_input(input) || !safe_atoi(input, &max_data)) {
                ctx->current_state = ERROR;
                break;
            }
            
            if (max_data < 1 || max_data > 100) {
                ctx->current_state = ERROR;
                break;
            }
            
            ctx->max_data = max_data;
            ctx->data_array = malloc(sizeof(int) * max_data);
            if (ctx->data_array == NULL) {
                ctx->current_state = ERROR;
                break;
            }
            ctx->current_state = PROCESSING;
            break;
        }
            
        case PROCESSING: {
            char input[32];
            printf("Enter data value %d/%d (or 'done' to finish): ", 
                   ctx->data_count + 1, ctx->max_data);
            if (fgets(input, sizeof(input), stdin) == NULL) {
                ctx->current_state = ERROR;
                break;
            }
            input[strcspn(input, "\n")] = '\0';
            
            if (strcmp(input, "done") == 0) {
                ctx->current_state = FINISHED;
                break;
            }
            
            int value;
            if (!validate_input(input) || !safe_atoi(input, &value)) {
                printf("Invalid input. Use numbers only.\n");
                break;
            }
            
            if (ctx->data_count >= ctx->max_data) {
                ctx->current_state = FINISHED;
                break;
            }
            
            ctx->data_array[ctx->data_count] = value;
            ctx->data_count++;
            break;
        }
            
        case FINISHED: {
            int sum = 0;
            int i = 0;
            do {
                if (i < ctx->data_count) {
                    sum += ctx->data_array[i];
                }
                i++;
            } while (i < ctx->data_count);
            
            printf("Processed %d items. Sum: %d\n", ctx->data_count, sum);
            ctx->current_state = INIT;
            break;
        }
            
        case ERROR:
            printf("Error occurred. Resetting state machine.\n");
            if (ctx->data_array != NULL) {
                free(ctx->data_array);
                ctx->data_array = NULL;
            }
            ctx->current_state = INIT;
            break;
    }
}

int main(void) {
    struct context ctx = {INIT, 0, 0, NULL, 0};
    
    int iteration = 0;
    while (iteration < 100) {
        state_machine_step(&ctx);
        
        if (ctx.current_state == FINISHED) {
            if (ctx.data_array != NULL) {
                free(ctx.data_array);
                ctx.data_array = NULL;
            }
        }
        
        iteration++;
        
        int count = 0;
        for (int i = 0; i < 5; i++) {
            if (i % 2 == 0) {
                count++;
            }
        }
        
        if (iteration >= 10 && ctx.current_state == INIT) {
            break;
        }
    }
    
    if (ctx.data_array != NULL) {
        free(ctx.data_array);
    }
    
    return 0;
}