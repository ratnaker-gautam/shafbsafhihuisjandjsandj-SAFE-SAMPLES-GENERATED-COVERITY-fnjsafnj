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
    ERROR,
    DONE
};

struct Context {
    enum State current_state;
    int data_count;
    int values[10];
    int processed_count;
    int error_code;
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 10) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int safe_atoi(const char* str, int* result) {
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

void state_machine_step(struct Context* ctx, const char* input) {
    switch (ctx->current_state) {
        case INIT:
            ctx->data_count = 0;
            ctx->processed_count = 0;
            ctx->error_code = 0;
            ctx->current_state = READING;
            break;
            
        case READING:
            if (input == NULL || strlen(input) == 0) {
                ctx->current_state = DONE;
            } else if (!validate_input(input)) {
                ctx->error_code = 1;
                ctx->current_state = ERROR;
            } else if (ctx->data_count >= 10) {
                ctx->error_code = 2;
                ctx->current_state = ERROR;
            } else {
                int value;
                if (safe_atoi(input, &value)) {
                    ctx->values[ctx->data_count] = value;
                    ctx->data_count++;
                    ctx->current_state = PROCESSING;
                } else {
                    ctx->error_code = 3;
                    ctx->current_state = ERROR;
                }
            }
            break;
            
        case PROCESSING:
            if (ctx->processed_count < ctx->data_count) {
                ctx->values[ctx->processed_count] *= 2;
                ctx->processed_count++;
                
                if (ctx->processed_count >= ctx->data_count) {
                    ctx->current_state = OUTPUT;
                }
            }
            break;
            
        case OUTPUT:
            for (int i = 0; i < ctx->data_count; i++) {
                printf("Value %d: %d\n", i + 1, ctx->values[i]);
            }
            ctx->current_state = READING;
            break;
            
        case ERROR:
            printf("Error %d occurred\n", ctx->error_code);
            ctx->current_state = DONE;
            break;
            
        case DONE:
            break;
    }
}

int main(void) {
    struct Context ctx = {INIT, 0, {0}, 0, 0};
    char input_buffer[32];
    
    state_machine_step(&ctx, NULL);
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        printf("Enter a number (empty to finish): ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        state_machine_step(&ctx, input_buffer);
        
        int i = 0;
        do {
            if (ctx.current_state == PROCESSING) {
                state_machine_step(&ctx, NULL);
            }
            i++;
        } while (ctx.current_state == PROCESSING && i < 10);
    }
    
    if (ctx.current_state == ERROR) {
        state_machine_step(&ctx, NULL);
    }
    
    printf("Processing complete. Total values: %d\n", ctx.data_count);
    return 0;
}