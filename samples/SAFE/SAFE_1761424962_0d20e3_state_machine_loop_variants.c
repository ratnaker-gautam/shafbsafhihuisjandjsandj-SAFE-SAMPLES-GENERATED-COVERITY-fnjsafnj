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
        if (i == 0 && (str[i] == '-' || str[i] == '+')) continue;
        if (str[i] < '0' || str[i] > '9') return 0;
    }
    return 1;
}

int safe_str_to_int(const char* str, int* result) {
    if (str == NULL || result == NULL) return 0;
    if (!is_valid_number(str)) return 0;
    
    long val = 0;
    int sign = 1;
    int i = 0;
    
    if (str[0] == '-') {
        sign = -1;
        i = 1;
    } else if (str[0] == '+') {
        i = 1;
    }
    
    for (; str[i] != '\0'; i++) {
        if (val > (INT32_MAX - (str[i] - '0')) / 10) return 0;
        val = val * 10 + (str[i] - '0');
    }
    
    val *= sign;
    if (val < INT32_MIN || val > INT32_MAX) return 0;
    
    *result = (int)val;
    return 1;
}

void state_machine_loop(struct context* ctx) {
    char input[32];
    int value;
    
    while (ctx->current_state != DONE && ctx->current_state != ERROR) {
        switch (ctx->current_state) {
            case INIT:
                ctx->data_count = 0;
                ctx->sum = 0;
                ctx->valid_inputs = 0;
                printf("Enter up to 10 numbers (type 'done' to finish):\n");
                ctx->current_state = READING;
                break;
                
            case READING:
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx->current_state = ERROR;
                    break;
                }
                
                size_t len = strlen(input);
                if (len > 0 && input[len-1] == '\n') {
                    input[len-1] = '\0';
                }
                
                if (strcmp(input, "done") == 0) {
                    ctx->current_state = PROCESSING;
                    break;
                }
                
                if (ctx->data_count >= 10) {
                    printf("Maximum 10 numbers allowed.\n");
                    ctx->current_state = PROCESSING;
                    break;
                }
                
                if (safe_str_to_int(input, &value)) {
                    ctx->values[ctx->data_count] = value;
                    ctx->data_count++;
                    ctx->valid_inputs++;
                } else {
                    printf("Invalid number: %s\n", input);
                }
                break;
                
            case PROCESSING:
                for (int i = 0; i < ctx->data_count; i++) {
                    if (ctx->sum > INT32_MAX - ctx->values[i]) {
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
                if (ctx->valid_inputs > 0) {
                    printf("Sum of %d valid numbers: %d\n", ctx->valid_inputs, ctx->sum);
                    printf("Average: %.2f\n", (double)ctx->sum / ctx->valid_inputs);
                } else {
                    printf("No valid numbers entered.\n");
                }
                ctx->current_state = DONE;
                break;
                
            case ERROR:
                printf("Error occurred during processing.\n");
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
    
    return ctx.current_state == DONE ? 0 : 1;
}