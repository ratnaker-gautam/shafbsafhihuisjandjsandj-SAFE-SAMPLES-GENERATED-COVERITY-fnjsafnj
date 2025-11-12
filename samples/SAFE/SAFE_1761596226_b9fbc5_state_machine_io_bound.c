//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUMBERS,
    PROCESSING,
    FINISHED,
    ERROR
};

struct StateMachine {
    enum State current_state;
    int number_count;
    int numbers[100];
    int sum;
    int valid_inputs;
};

void init_state_machine(struct StateMachine *sm) {
    sm->current_state = INIT;
    sm->number_count = 0;
    sm->sum = 0;
    sm->valid_inputs = 0;
    memset(sm->numbers, 0, sizeof(sm->numbers));
}

int is_valid_number(const char *input) {
    if (input == NULL || strlen(input) == 0) {
        return 0;
    }
    
    for (int i = 0; input[i] != '\0'; i++) {
        if (i == 0 && input[i] == '-') {
            continue;
        }
        if (!isdigit(input[i])) {
            return 0;
        }
    }
    
    return 1;
}

int parse_number(const char *input) {
    long value = strtol(input, NULL, 10);
    if (value < -1000000 || value > 1000000) {
        return 0;
    }
    return (int)value;
}

void process_transition(struct StateMachine *sm, const char *input) {
    switch (sm->current_state) {
        case INIT:
            printf("Enter numbers one per line. Type 'done' to finish:\n");
            sm->current_state = READING_NUMBERS;
            break;
            
        case READING_NUMBERS:
            if (strcmp(input, "done") == 0) {
                if (sm->number_count > 0) {
                    sm->current_state = PROCESSING;
                } else {
                    printf("No numbers entered. Exiting.\n");
                    sm->current_state = FINISHED;
                }
            } else if (is_valid_number(input)) {
                if (sm->number_count < 100) {
                    int num = parse_number(input);
                    sm->numbers[sm->number_count] = num;
                    sm->number_count++;
                    sm->valid_inputs++;
                    printf("Added number: %d\n", num);
                } else {
                    printf("Maximum number count reached (100). Processing...\n");
                    sm->current_state = PROCESSING;
                }
            } else {
                printf("Invalid input. Please enter a valid integer or 'done'.\n");
            }
            break;
            
        case PROCESSING:
            for (int i = 0; i < sm->number_count; i++) {
                if (sm->sum + sm->numbers[i] > 1000000 || sm->sum + sm->numbers[i] < -1000000) {
                    printf("Sum overflow detected. Resetting calculation.\n");
                    sm->sum = 0;
                }
                sm->sum += sm->numbers[i];
            }
            
            printf("Processing complete.\n");
            printf("Numbers entered: %d\n", sm->valid_inputs);
            printf("Sum of numbers: %d\n", sm->sum);
            printf("Average: %.2f\n", sm->valid_inputs > 0 ? (double)sm->sum / sm->valid_inputs : 0.0);
            sm->current_state = FINISHED;
            break;
            
        case FINISHED:
            break;
            
        case ERROR:
            printf("An error occurred in the state machine.\n");
            break;
    }
}

int main(void) {
    struct StateMachine sm;
    char input[256];
    
    init_state_machine(&sm);
    
    while (sm.current_state != FINISHED && sm.current_state != ERROR) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            if (feof(stdin)) {
                printf("EOF detected. Exiting.\n");
                break;
            } else {
                printf("Input error occurred.\n");
                sm.current_state = ERROR;
                break;
            }
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (len == 0) {
            continue;
        }
        
        process_transition(&sm, input);
    }
    
    return sm.current_state == ERROR ? 1 : 0;
}