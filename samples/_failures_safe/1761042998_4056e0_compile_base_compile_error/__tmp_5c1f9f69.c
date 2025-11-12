//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define EVENT_START 0
#define EVENT_DATA 1
#define EVENT_END 2
#define EVENT_RESET 3
#define EVENT_INVALID 4

#define MAX_INPUT_LEN 100
#define MAX_WORDS 10
#define MAX_WORD_LEN 20

typedef struct {
    int current_state;
    char input_buffer[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count;
    int buffer_pos;
} state_machine_t;

void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(machine->words[i], 0, sizeof(machine->words[i]));
    }
    machine->word_count = 0;
    machine->buffer_pos = 0;
}

int get_event(char input_char) {
    if (input_char == 'S' || input_char == 's') return EVENT_START;
    if (input_char == 'E' || input_char == 'e') return EVENT_END;
    if (input_char == 'R' || input_char == 'r') return EVENT_RESET;
    if (isalnum(input_char) || isspace(input_char)) return EVENT_DATA;
    return EVENT_INVALID;
}

int process_data(state_machine_t *machine, char input_char) {
    if (machine->buffer_pos >= MAX_INPUT_LEN) {
        return STATE_ERROR;
    }
    
    machine->input_buffer[machine->buffer_pos] = input_char;
    machine->buffer_pos++;
    machine->input_buffer[machine->buffer_pos] = '\0';
    
    return STATE_READING;
}

int process_words(state_machine_t *machine) {
    char temp_buffer[MAX_INPUT_LEN + 1];
    strncpy(temp_buffer, machine->input_buffer, sizeof(temp_buffer) - 1);
    temp_buffer[sizeof(temp_buffer) - 1] = '\0';
    
    char *token = strtok(temp_buffer, " \t\n");
    machine->word_count = 0;
    
    while (token != NULL && machine->word_count < MAX_WORDS) {
        if (strlen(token) > MAX_WORD_LEN) {
            return STATE_ERROR;
        }
        strncpy(machine->words[machine->word_count], token, MAX_WORD_LEN);
        machine->words[machine->word_count][MAX_WORD_LEN] = '\0';
        machine->word_count++;
        token = strtok(NULL, " \t\n");
    }
    
    return STATE_PROCESSING;
}

void display_results(state_machine_t *machine) {
    printf("Input processed successfully.\n");
    printf("Input text: %s\n", machine->input_buffer);
    printf("Words found: %d\n", machine->word_count);
    for (int i = 0; i < machine->word_count; i++) {
        printf("Word %d: %s\n", i + 1, machine->words[i]);
    }
}

int state_transition(state_machine_t *machine, int event, char input_char) {
    switch (machine->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) return STATE_READING;
            if (event == EVENT_RESET) return STATE_IDLE;
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) return process_data(machine, input_char);
            if (event == EVENT_END) return process_words(machine);
            if (event == EVENT_RESET) return STATE_IDLE;
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_RESET) return STATE_IDLE;
            if (event == EVENT_START) return STATE_DONE;
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) return STATE_IDLE;
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) return STATE_IDLE;
            break;
    }
    
    return machine->current_state;
}

int main() {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Text Processing State Machine\n");
    printf("Commands: S=Start, E=End, R=Reset, alphanumeric=data\n");
    printf("Enter input sequence:\n");
    
    char input;
    while (machine.current_state != STATE_DONE && machine.current_state != STATE_ERROR) {
        if (scanf("%c", &input) != 1) {
            break;
        }
        
        if (input == '\n') continue;
        
        int event = get_event(input);
        int new_state = state_transition(&machine, event, input);
        
        if (new_state == STATE_ERROR) {
            printf("Error: Invalid operation or buffer overflow\n");
            machine.current_state = STATE_ERROR;
        } else {
            machine.current_state = new_state;
        }
        
        if (machine.current_state == STATE_PROCESSING) {
            display_results(&machine);
            machine.current_state = STATE_DONE;
        }
        
        if (machine.current_state == STATE_DONE) {
            printf("Processing complete. Press R to reset