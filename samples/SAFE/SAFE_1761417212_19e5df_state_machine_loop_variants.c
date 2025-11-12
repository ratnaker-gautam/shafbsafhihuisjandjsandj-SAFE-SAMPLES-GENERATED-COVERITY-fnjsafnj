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
        if (i == 0 && str[i] == '-') continue;
        if (str[i] < '0' || str[i] > '9') return 0;
    }
    return 1;
}

int safe_str_to_int(const char* str, int* result) {
    if (str == NULL || result == NULL) return 0;
    
    long val = 0;
    int sign = 1;
    int i = 0;
    
    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }
    
    for (; str[i] != '\0'; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            val = val * 10 + (str[i] - '0');
            if (val > INT32_MAX) return 0;
        } else {
            return 0;
        }
    }
    
    val *= sign;
    if (val < INT32_MIN || val > INT32_MAX) return 0;
    
    *result = (int)val;
    return 1;
}

void state_machine_loop(struct context* ctx) {
    char input[32];
    
    while (ctx->current_state != DONE && ctx->current_state != ERROR) {
        switch (ctx->current_state) {
            case INIT:
                ctx->data_count = 0;
                ctx->sum = 0;
                ctx->valid_inputs = 0;
                printf("Enter up to 10 numbers (or 'done' to finish):\n");
                ctx->current_state = READING;
                break;
                
            case READING:
                printf("Enter number %d: ", ctx->data_count + 1);
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                input[strcspn(input, "\n")] = '\0';
                
                if (strcmp(input, "done") == 0) {
                    if (ctx->data_count > 0) {
                        ctx->current_state = PROCESSING;
                    } else {
                        printf("No numbers entered.\n");
                        ctx->current_state = DONE;
                    }
                    break;
                }
                
                if (!is_valid_number(input)) {
                    printf("Invalid input. Please enter a valid integer or 'done'.\n");
                    break;
                }
                
                int num;
                if (!safe_str_to_int(input, &num)) {
                    printf("Number out of range.\n");
                    break;
                }
                
                if (ctx->data_count < 10) {
                    ctx->values[ctx->data_count] = num;
                    ctx->data_count++;
                    ctx->valid_inputs++;
                } else {
                    printf("Maximum of 10 numbers reached.\n");
                    ctx->current_state = PROCESSING;
                }
                break;
                
            case PROCESSING:
                for (int i = 0; i < ctx->data_count; i++) {
                    if (ctx->sum > INT32_MAX - ctx->values[i]) {
                        printf("Sum overflow detected.\n");
                        ctx->current_state = ERROR;
                        break;
                    }
                    ctx->sum += ctx->values[i];
                }
                if (ctx->current_state != ERROR) {
                    ctx->current_state = OUTPUT;
                }
                break;
                
            case OUTPUT:
                printf("You entered %d numbers:\n", ctx->data_count);
                for (int i = 0; i < ctx->data_count; i++) {
                    printf("  %d", ctx->values[i]);
                    if (i < ctx->data_count - 1) printf(",");
                }
                printf("\nSum: %d\n", ctx->sum);
                ctx->current_state = DONE;
                break;
                
            case ERROR:
                printf("An error occurred during processing.\n");
                ctx->current_state = DONE;
                break;
                
            default:
                ctx->current_state = ERROR;
                break;
        }
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    
    state_machine_loop(&ctx);
    
    return ctx.current_state == ERROR ? 1 : 0;
}