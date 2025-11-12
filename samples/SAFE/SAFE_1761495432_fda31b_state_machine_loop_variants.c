//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum state {
    IDLE,
    READING,
    PROCESSING,
    WRITING,
    ERROR,
    DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int data_value;
    int processed_count;
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 255) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

int process_data(int value) {
    if (value < 0) return -1;
    int result = 0;
    while (value > 0) {
        int digit = value % 10;
        if (result > INT_MAX / 10) return -1;
        result = result * 10 + digit;
        value /= 10;
    }
    return result;
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_len = 0;
    ctx.data_value = 0;
    ctx.processed_count = 0;
    
    const char* input_data[] = {"123", "456", "789", "999", "0"};
    size_t input_count = sizeof(input_data) / sizeof(input_data[0]);
    size_t input_index = 0;
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case IDLE:
                if (input_index < input_count) {
                    const char* input = input_data[input_index];
                    if (validate_input(input)) {
                        size_t len = strlen(input);
                        if (len < sizeof(ctx.buffer)) {
                            memcpy(ctx.buffer, input, len + 1);
                            ctx.buffer_len = len;
                            ctx.current_state = READING;
                        } else {
                            ctx.current_state = ERROR;
                        }
                    } else {
                        ctx.current_state = ERROR;
                    }
                } else {
                    ctx.current_state = DONE;
                }
                break;
                
            case READING:
                {
                    char* endptr;
                    long value = strtol(ctx.buffer, &endptr, 10);
                    if (endptr == ctx.buffer + ctx.buffer_len && value >= 0 && value <= INT_MAX) {
                        ctx.data_value = (int)value;
                        ctx.current_state = PROCESSING;
                    } else {
                        ctx.current_state = ERROR;
                    }
                }
                break;
                
            case PROCESSING:
                {
                    int result = process_data(ctx.data_value);
                    if (result >= 0) {
                        ctx.data_value = result;
                        ctx.current_state = WRITING;
                    } else {
                        ctx.current_state = ERROR;
                    }
                }
                break;
                
            case WRITING:
                printf("Processed: %d -> %d\n", ctx.processed_count, ctx.data_value);
                ctx.processed_count++;
                input_index++;
                ctx.current_state = IDLE;
                break;
                
            case ERROR:
                printf("Error processing data\n");
                ctx.current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    printf("Total processed: %d\n", ctx.processed_count);
    return ctx.current_state == ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}