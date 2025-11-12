//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    INIT,
    READY,
    PROCESSING,
    FINISHED,
    ERROR,
    NUM_STATES
};

enum Event {
    START,
    DATA_RECEIVED,
    PROCESS_COMPLETE,
    ERROR_OCCURRED,
    RESET,
    NUM_EVENTS
};

typedef void (*StateHandler)(enum Event);

void init_handler(enum Event event);
void ready_handler(enum Event event);
void processing_handler(enum Event event);
void finished_handler(enum Event event);
void error_handler(enum Event event);

StateHandler state_table[NUM_STATES] = {
    init_handler,
    ready_handler,
    processing_handler,
    finished_handler,
    error_handler
};

struct StateMachine {
    enum State current_state;
    int data_count;
    int max_data;
};

struct StateMachine machine = {INIT, 0, 10};

void init_handler(enum Event event) {
    switch (event) {
        case START:
            printf("State: INIT -> READY\n");
            machine.current_state = READY;
            break;
        case RESET:
            printf("Already in INIT state\n");
            break;
        default:
            printf("Invalid event for INIT state\n");
            break;
    }
}

void ready_handler(enum Event event) {
    switch (event) {
        case DATA_RECEIVED:
            if (machine.data_count < machine.max_data) {
                printf("State: READY -> PROCESSING\n");
                machine.current_state = PROCESSING;
                machine.data_count++;
            } else {
                printf("Maximum data limit reached\n");
                machine.current_state = ERROR;
            }
            break;
        case RESET:
            printf("State: READY -> INIT\n");
            machine.current_state = INIT;
            machine.data_count = 0;
            break;
        default:
            printf("Invalid event for READY state\n");
            break;
    }
}

void processing_handler(enum Event event) {
    switch (event) {
        case PROCESS_COMPLETE:
            printf("State: PROCESSING -> FINISHED\n");
            machine.current_state = FINISHED;
            break;
        case ERROR_OCCURRED:
            printf("State: PROCESSING -> ERROR\n");
            machine.current_state = ERROR;
            break;
        case RESET:
            printf("State: PROCESSING -> INIT\n");
            machine.current_state = INIT;
            machine.data_count = 0;
            break;
        default:
            printf("Invalid event for PROCESSING state\n");
            break;
    }
}

void finished_handler(enum Event event) {
    switch (event) {
        case RESET:
            printf("State: FINISHED -> INIT\n");
            machine.current_state = INIT;
            machine.data_count = 0;
            break;
        default:
            printf("Invalid event for FINISHED state\n");
            break;
    }
}

void error_handler(enum Event event) {
    switch (event) {
        case RESET:
            printf("State: ERROR -> INIT\n");
            machine.current_state = INIT;
            machine.data_count = 0;
            break;
        default:
            printf("Invalid event for ERROR state\n");
            break;
    }
}

void process_event(enum Event event) {
    if (event >= 0 && event < NUM_EVENTS && machine.current_state >= 0 && machine.current_state < NUM_STATES) {
        state_table[machine.current_state](event);
    } else {
        printf("Invalid event or state\n");
    }
}

void print_menu(void) {
    printf("\nCurrent State: ");
    switch (machine.current_state) {
        case INIT: printf("INIT"); break;
        case READY: printf("READY"); break;
        case PROCESSING: printf("PROCESSING"); break;
        case FINISHED: printf("FINISHED"); break;
        case ERROR: printf("ERROR"); break;
        default: printf("UNKNOWN"); break;
    }
    printf(" | Data processed: %d/%d\n", machine.data_count, machine.max_data);
    printf("Events: 0=START, 1=DATA_RECEIVED, 2=PROCESS_COMPLETE, 3=ERROR_OCCURRED, 4=RESET, 5=EXIT\n");
    printf("Choose event: ");
}

int main(void) {
    int choice;
    
    printf("State Machine Demo Started\n");
    
    while (1) {
        print_menu();
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (choice == 5) {
            printf("Exiting state machine\n");
            break;
        }
        
        if (choice >= 0 && choice < 5) {
            process_event((enum Event)choice);
        } else {
            printf("Invalid event choice\n");
        }
    }
    
    return 0;
}