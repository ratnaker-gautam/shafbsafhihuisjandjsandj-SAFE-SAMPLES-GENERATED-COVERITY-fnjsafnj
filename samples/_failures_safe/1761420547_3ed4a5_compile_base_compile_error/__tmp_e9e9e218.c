//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum State { IDLE, READY, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data_count;
    int max_data;
    int *data_buffer;
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
        if (val > (LONG_MAX - (str[i] - '0')) / 10) return 0;
        val = val * 10 + (str[i] - '0');
    }
    
    long final_val = val * sign;
    if (final_val > INT_MAX || final_val < INT_MIN) return 0;
    
    *result = (int)final_val;
    return 1;
}

void initialize_context(struct Context *ctx, int max_items) {
    if (ctx == NULL || max_items <= 0) return;
    ctx->current_state = IDLE;
    ctx->data_count = 0;
    ctx->max_data = max_items;
    ctx->processed_count = 0;
    ctx->data_buffer = malloc(sizeof(int) * max_items);
    if (ctx->data_buffer == NULL) {
        ctx->current_state = ERROR;
    }
}

void cleanup_context(struct Context *ctx) {
    if (ctx != NULL) {
        free(ctx->data_buffer);
    }
}

enum State handle_idle(struct Context *ctx) {
    printf("Enter number of data items to process (1-100): ");
    char input[12];
    if (fgets(input, sizeof(input), stdin) == NULL) return ERROR;
    input[strcspn(input, "\n")] = '\0';
    
    if (!validate_input(input)) return ERROR;
    
    int count;
    if (!safe_atoi(input, &count)) return ERROR;
    if (count < 1 || count > 100) return ERROR;
    
    initialize_context(ctx, count);
    if (ctx->current_state == ERROR) return ERROR;
    
    return READY;
}

enum State handle_ready(struct Context *ctx) {
    if (ctx == NULL || ctx->data_buffer == NULL) return ERROR;
    
    printf("Enter %d data values:\n", ctx->max_data);
    
    int i = 0;
    while (i < ctx->max_data) {
        printf("Value %d: ", i + 1);
        char input[12];
        if (fgets(input, sizeof(input), stdin) == NULL) return ERROR;
        input[strcspn(input, "\n")] = '\0';
        
        if (!validate_input(input)) {
            printf("Invalid input. Please enter numbers only.\n");
            continue;
        }
        
        int value;
        if (!safe_atoi(input, &value)) {
            printf("Invalid number format.\n");
            continue;
        }
        
        ctx->data_buffer[i] = value;
        i++;
    }
    
    ctx->data_count = ctx->max_data;
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx == NULL || ctx->data_buffer == NULL || ctx->data_count <= 0) return ERROR;
    
    int sum = 0;
    int i = 0;
    
    do {
        if (i >= ctx->data_count) break;
        sum += ctx->data_buffer[i];
        i++;
    } while (i < ctx->data_count);
    
    int j = 0;
    for (; j < ctx->data_count; j++) {
        if (ctx->data_buffer[j] > INT_MAX / 2 || ctx->data_buffer[j] < INT_MIN / 2) {
            return ERROR;
        }
        ctx->data_buffer[j] *= 2;
    }
    
    int k = 0;
    while (k < ctx->data_count) {
        printf("Processed value %d: %d\n", k + 1, ctx->data_buffer[k]);
        k++;
    }
    
    printf("Sum of original values: %d\n", sum);
    ctx->processed_count = ctx->data_count;
    
    return FINISHED;
}

enum State handle_finished(struct Context *ctx) {
    printf("Processing complete. %d items processed.\n", ctx->processed_count);
    return FINISHED;
}

enum State handle_error(struct Context *ctx) {
    printf("An error occurred during processing.\n");
    return ERROR;
}

int main(void) {
    struct Context ctx;
    ctx.current_state = IDLE;
    ctx.data_count = 0;
    ctx.max_data = 0;
    ctx.data_buffer = NULL;
    ctx.process