//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    IDLE,
    READING,
    PROCESSING,
    WRITING,
    ERROR,
    DONE
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_len;
    int data_value;
    int error_code;
};

struct StateMachine {
    struct Context ctx;
    int cycle_count;
};

void state_idle(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Enter data value (0-100): ");
    int result = scanf("%d", &sm->ctx.data_value);
    if (result != 1) {
        sm->ctx.current_state = ERROR;
        sm->ctx.error_code = 1;
        return;
    }
    
    if (sm->ctx.data_value < 0 || sm->ctx.data_value > 100) {
        sm->ctx.current_state = ERROR;
        sm->ctx.error_code = 2;
        return;
    }
    
    sm->ctx.current_state = READING;
}

void state_reading(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Enter text (max 255 chars): ");
    getchar();
    
    if (fgets(sm->ctx.buffer, sizeof(sm->ctx.buffer), stdin) == NULL) {
        sm->ctx.current_state = ERROR;
        sm->ctx.error_code = 3;
        return;
    }
    
    sm->ctx.buffer_len = strlen(sm->ctx.buffer);
    if (sm->ctx.buffer_len > 0 && sm->ctx.buffer[sm->ctx.buffer_len - 1] == '\n') {
        sm->ctx.buffer[sm->ctx.buffer_len - 1] = '\0';
        sm->ctx.buffer_len--;
    }
    
    if (sm->ctx.buffer_len == 0) {
        sm->ctx.current_state = ERROR;
        sm->ctx.error_code = 4;
        return;
    }
    
    sm->ctx.current_state = PROCESSING;
}

void state_processing(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    for (size_t i = 0; i < sm->ctx.buffer_len; i++) {
        if (sm->ctx.buffer[i] >= 'a' && sm->ctx.buffer[i] <= 'z') {
            sm->ctx.buffer[i] = sm->ctx.buffer[i] - 'a' + 'A';
        }
    }
    
    sm->ctx.data_value *= 2;
    if (sm->ctx.data_value > 1000) {
        sm->ctx.data_value = 1000;
    }
    
    sm->ctx.current_state = WRITING;
}

void state_writing(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Processed data: %d\n", sm->ctx.data_value);
    printf("Processed text: %s\n", sm->ctx.buffer);
    printf("Buffer length: %zu\n", sm->ctx.buffer_len);
    
    sm->ctx.current_state = DONE;
}

void state_error(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Error %d occurred\n", sm->ctx.error_code);
    sm->ctx.current_state = DONE;
}

void state_done(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("State machine completed in %d cycles\n", sm->cycle_count);
}

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    memset(sm, 0, sizeof(struct StateMachine));
    sm->ctx.current_state = IDLE;
    sm->ctx.buffer_len = 0;
    sm->ctx.data_value = 0;
    sm->ctx.error_code = 0;
    sm->cycle_count = 0;
}

void state_machine_run(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    while (sm->ctx.current_state != DONE && sm->cycle_count < 100) {
        sm->cycle_count++;
        
        switch (sm->ctx.current_state) {
            case IDLE:
                state_idle(sm);
                break;
            case READING:
                state_reading(sm);
                break;
            case PROCESSING:
                state_processing(sm);
                break;
            case WRITING:
                state_writing(sm);
                break;
            case ERROR:
                state_error(sm);
                break;
            case DONE:
                break;
        }
    }
    
    if (sm->cycle_count >= 100) {
        printf("State machine timeout\n");
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    state_machine_run(&sm);
    return 0;
}