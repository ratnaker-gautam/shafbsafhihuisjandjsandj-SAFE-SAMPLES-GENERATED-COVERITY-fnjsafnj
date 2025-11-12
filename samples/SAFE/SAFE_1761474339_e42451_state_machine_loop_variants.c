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
        if (val > INT32_MAX) return 0;
    }
    
    val *= sign;
    if (val < INT32_MIN || val > INT32_MAX) return 0;
    *result = (int)val;
    return 1;
}

void cleanup(struct context *ctx) {
    if (ctx != NULL && ctx->data_array != NULL) {
        free(ctx->data_array);
        ctx->data_array = NULL;
    }
}

int main(void) {
    struct context ctx = {INIT, 0, 0, NULL, 0};
    char input_buffer[32];
    int temp_value;
    
    while (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case INIT:
                printf("Enter maximum data count (1-100): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (!validate_input(input_buffer) || !safe_atoi(input_buffer, &temp_value)) {
                    printf("Invalid input\n");
                    ctx.current_state = ERROR;
                    break;
                }
                
                if (temp_value < 1 || temp_value > 100) {
                    printf("Value must be between 1 and 100\n");
                    ctx.current_state = ERROR;
                    break;
                }
                
                ctx.max_data = temp_value;
                ctx.data_array = malloc(ctx.max_data * sizeof(int));
                if (ctx.data_array == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                ctx.current_state = READY;
                break;
                
            case READY:
                printf("Enter data value %d/%d (or 'done' to finish): ", ctx.data_count + 1, ctx.max_data);
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                input_buffer[strcspn(input_buffer, "\n")] = '\0';
                
                if (strcmp(input_buffer, "done") == 0) {
                    if (ctx.data_count == 0) {
                        printf("No data entered\n");
                        ctx.current_state = ERROR;
                    } else {
                        ctx.current_state = PROCESSING;
                    }
                    break;
                }
                
                if (!validate_input(input_buffer) || !safe_atoi(input_buffer, &temp_value)) {
                    printf("Invalid input\n");
                    continue;
                }
                
                if (ctx.data_count >= ctx.max_data) {
                    printf("Maximum data count reached\n");
                    ctx.current_state = PROCESSING;
                    break;
                }
                
                ctx.data_array[ctx.data_count] = temp_value;
                ctx.data_count++;
                break;
                
            case PROCESSING:
                for (int i = 0; i < ctx.data_count; i++) {
                    int j = i;
                    while (j > 0 && ctx.data_array[j-1] > ctx.data_array[j]) {
                        int temp = ctx.data_array[j];
                        ctx.data_array[j] = ctx.data_array[j-1];
                        ctx.data_array[j-1] = temp;
                        j--;
                    }
                }
                
                printf("Sorted data: ");
                for (int i = 0; i < ctx.data_count; i++) {
                    printf("%d ", ctx.data_array[i]);
                }
                printf("\n");
                
                int sum = 0;
                int i = 0;
                do {
                    sum += ctx.data_array[i];
                    i++;
                } while (i < ctx.data_count);
                printf("Sum: %d\n", sum);
                
                ctx.current_state = FINISHED;
                break;
                
            case FINISHED:
            case ERROR:
                break;
        }
    }
    
    if (ctx.current_state == ERROR) {
        printf("Error occurred during processing\n");
    } else {
        printf("Processing completed successfully\n");
    }
    
    cleanup(&ctx);
    return ctx.current_state == FINISHED ? 0 : 1;
}