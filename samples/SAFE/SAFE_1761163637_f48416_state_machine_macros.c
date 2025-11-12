//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define EVENT_START 0
#define EVENT_DATA 1
#define EVENT_END 2
#define EVENT_INVALID 3

#define MAX_INPUT_LEN 100
#define MAX_WORDS 20
#define MAX_WORD_LEN 20

typedef struct {
    int state;
    char input_buffer[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count;
    int buffer_pos;
} state_machine_t;

void initialize_machine(state_machine_t *machine) {
    machine->state = STATE_IDLE;
    machine->buffer_pos = 0;
    machine->word_count = 0;
    memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(machine->words[i], 0, sizeof(machine->words[i]));
    }
}

int get_event(char c) {
    if (c == 'S') return EVENT_START;
    if (c == 'E') return EVENT_END;
    if (isalnum(c) || c == ' ') return EVENT_DATA;
    return EVENT_INVALID;
}

int process_input(state_machine_t *machine, char input) {
    int event = get_event(input);
    
    switch (machine->state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                machine->state = STATE_READING;
                initialize_machine(machine);
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (machine->buffer_pos < MAX_INPUT_LEN) {
                    machine->input_buffer[machine->buffer_pos++] = input;
                }
            } else if (event == EVENT_END) {
                machine->state = STATE_PROCESSING;
            } else if (event == EVENT_INVALID) {
                machine->state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (machine->buffer_pos > 0) {
                char *token = strtok(machine->input_buffer, " ");
                while (token != NULL && machine->word_count < MAX_WORDS) {
                    size_t len = strlen(token);
                    if (len > 0 && len <= MAX_WORD_LEN) {
                        strncpy(machine->words[machine->word_count], token, MAX_WORD_LEN);
                        machine->words[machine->word_count][MAX_WORD_LEN] = '\0';
                        machine->word_count++;
                    }
                    token = strtok(NULL, " ");
                }
                machine->state = STATE_DONE;
            } else {
                machine->state = STATE_ERROR;
            }
            break;
            
        case STATE_DONE:
        case STATE_ERROR:
            break;
    }
    
    return machine->state;
}

void print_results(const state_machine_t *machine) {
    switch (machine->state) {
        case STATE_DONE:
            printf("Processing complete. Found %d words:\n", machine->word_count);
            for (int i = 0; i < machine->word_count; i++) {
                printf("  %d: %s\n", i + 1, machine->words[i]);
            }
            break;
            
        case STATE_ERROR:
            printf("Error: Invalid input sequence detected.\n");
            break;
            
        case STATE_IDLE:
            printf("State: Waiting for start signal.\n");
            break;
            
        case STATE_READING:
            printf("State: Reading input data.\n");
            break;
            
        case STATE_PROCESSING:
            printf("State: Processing data.\n");
            break;
    }
}

int main(void) {
    state_machine_t machine;
    char input[MAX_INPUT_LEN * 2];
    char *line = NULL;
    size_t len = 0;
    
    initialize_machine(&machine);
    
    printf("Text Processor State Machine\n");
    printf("Commands: S=Start, E=End, alphanumeric=data\n");
    printf("Enter input sequence: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    for (size_t i = 0; i < strlen(input); i++) {
        int result = process_input(&machine, input[i]);
        if (result == STATE_ERROR || result == STATE_DONE) {
            break;
        }
    }
    
    print_results(&machine);
    
    return 0;
}