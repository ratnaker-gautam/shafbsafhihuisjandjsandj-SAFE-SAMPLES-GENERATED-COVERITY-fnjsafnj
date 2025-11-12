//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum state {
    IDLE,
    READING,
    PROCESSING,
    WRITING,
    ERROR,
    EXIT
};

typedef enum state (*state_handler)(void);

struct context {
    char buffer[256];
    size_t buffer_pos;
    int data_value;
};

static struct context ctx;

enum state idle_state(void) {
    printf("Enter command (read, process, write, exit): ");
    if (fgets(ctx.buffer, sizeof(ctx.buffer), stdin) == NULL) {
        return ERROR;
    }
    
    ctx.buffer[strcspn(ctx.buffer, "\n")] = '\0';
    
    if (strcmp(ctx.buffer, "read") == 0) {
        return READING;
    } else if (strcmp(ctx.buffer, "process") == 0) {
        return PROCESSING;
    } else if (strcmp(ctx.buffer, "write") == 0) {
        return WRITING;
    } else if (strcmp(ctx.buffer, "exit") == 0) {
        return EXIT;
    } else {
        printf("Unknown command\n");
        return IDLE;
    }
}

enum state reading_state(void) {
    printf("Enter data value (0-100): ");
    if (fgets(ctx.buffer, sizeof(ctx.buffer), stdin) == NULL) {
        return ERROR;
    }
    
    char *endptr;
    long value = strtol(ctx.buffer, &endptr, 10);
    if (endptr == ctx.buffer || *endptr != '\n') {
        printf("Invalid input\n");
        return IDLE;
    }
    
    if (value < 0 || value > 100) {
        printf("Value out of range\n");
        return IDLE;
    }
    
    ctx.data_value = (int)value;
    printf("Read value: %d\n", ctx.data_value);
    return IDLE;
}

enum state processing_state(void) {
    if (ctx.data_value < 0) {
        printf("No data to process\n");
        return IDLE;
    }
    
    int result = ctx.data_value * 2;
    if (result > 200) {
        result = 200;
    }
    
    ctx.data_value = result;
    printf("Processed value: %d\n", ctx.data_value);
    return IDLE;
}

enum state writing_state(void) {
    if (ctx.data_value < 0) {
        printf("No data to write\n");
        return IDLE;
    }
    
    printf("Writing value: %d\n", ctx.data_value);
    ctx.data_value = -1;
    return IDLE;
}

enum state error_state(void) {
    printf("Error occurred\n");
    return EXIT;
}

enum state exit_state(void) {
    printf("Goodbye\n");
    return EXIT;
}

int main(void) {
    state_handler handlers[] = {
        idle_state,
        reading_state,
        processing_state,
        writing_state,
        error_state,
        exit_state
    };
    
    enum state current_state = IDLE;
    
    while (current_state != EXIT) {
        if (current_state < 0 || current_state >= (int)(sizeof(handlers)/sizeof(handlers[0]))) {
            current_state = ERROR;
        }
        current_state = handlers[current_state]();
    }
    
    return 0;
}