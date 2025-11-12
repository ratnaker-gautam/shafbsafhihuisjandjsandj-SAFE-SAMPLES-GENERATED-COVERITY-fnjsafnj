//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3

typedef struct {
    int current_state;
    char *input_ptr;
    char *output_ptr;
    char input_buffer[MAX_INPUT];
    char output_buffer[MAX_INPUT];
} StateMachine;

void init_state_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->input_ptr = sm->input_buffer;
    sm->output_ptr = sm->output_buffer;
    memset(sm->input_buffer, 0, sizeof(sm->input_buffer));
    memset(sm->output_buffer, 0, sizeof(sm->output_buffer));
}

int read_input(StateMachine *sm) {
    if (sm->current_state != STATE_IDLE) {
        return -1;
    }
    
    printf("Enter text (max %d chars): ", MAX_INPUT - 1);
    if (fgets(sm->input_buffer, sizeof(sm->input_buffer), stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(sm->input_buffer);
    if (len > 0 && *(sm->input_ptr + len - 1) == '\n') {
        *(sm->input_ptr + len - 1) = '\0';
    }
    
    sm->current_state = STATE_READING;
    return 0;
}

int process_text(StateMachine *sm) {
    if (sm->current_state != STATE_READING) {
        return -1;
    }
    
    char *input = sm->input_ptr;
    char *output = sm->output_ptr;
    
    while (*input != '\0' && (output - sm->output_buffer) < (MAX_INPUT - 1)) {
        if (isalpha((unsigned char)*input)) {
            *output = toupper((unsigned char)*input);
            output++;
        }
        input++;
    }
    *output = '\0';
    
    sm->current_state = STATE_PROCESSING;
    return 0;
}

int display_result(StateMachine *sm) {
    if (sm->current_state != STATE_PROCESSING) {
        return -1;
    }
    
    printf("Original: %s\n", sm->input_ptr);
    printf("Processed: %s\n", sm->output_ptr);
    
    sm->current_state = STATE_DONE;
    return 0;
}

int reset_state_machine(StateMachine *sm) {
    init_state_machine(sm);
    return 0;
}

int main(void) {
    StateMachine sm;
    init_state_machine(&sm);
    
    int choice;
    int result;
    
    do {
        printf("\nState Machine Menu:\n");
        printf("1. Read input\n");
        printf("2. Process text\n");
        printf("3. Display result\n");
        printf("4. Reset\n");
        printf("5. Exit\n");
        printf("Choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        while (getchar() != '\n');
        
        switch (choice) {
            case 1:
                result = read_input(&sm);
                if (result == 0) {
                    printf("Input read successfully\n");
                } else {
                    printf("Cannot read input in current state\n");
                }
                break;
                
            case 2:
                result = process_text(&sm);
                if (result == 0) {
                    printf("Text processed successfully\n");
                } else {
                    printf("Cannot process text in current state\n");
                }
                break;
                
            case 3:
                result = display_result(&sm);
                if (result == 0) {
                    printf("Result displayed\n");
                } else {
                    printf("Cannot display result in current state\n");
                }
                break;
                
            case 4:
                reset_state_machine(&sm);
                printf("State machine reset\n");
                break;
                
            case 5:
                printf("Exiting...\n");
                break;
                
            default:
                printf("Invalid choice\n");
                break;
        }
        
    } while (choice != 5);
    
    return 0;
}