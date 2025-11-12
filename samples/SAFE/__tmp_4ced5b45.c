//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum vending_state {
    IDLE,
    SELECTING,
    PROCESSING,
    DISPENSING,
    ERROR
};

struct vending_machine {
    enum vending_state state;
    int selection;
    int balance;
    int item_count;
    int max_items;
};

void init_machine(struct vending_machine *machine) {
    if (machine == NULL) return;
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    machine->item_count = 5;
    machine->max_items = 10;
}

int handle_idle(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    
    if (input == 1) {
        machine->state = SELECTING;
        return 1;
    } else if (input == 2) {
        machine->balance += 1;
        return 1;
    }
    return 0;
}

int handle_selecting(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    
    if (input >= 0 && input <= 4) {
        machine->selection = input;
        machine->state = PROCESSING;
        return 1;
    } else if (input == 9) {
        machine->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_processing(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    
    if (input == 1) {
        if (machine->balance >= 2 && machine->item_count > 0) {
            machine->balance -= 2;
            machine->item_count--;
            machine->state = DISPENSING;
        } else {
            machine->state = ERROR;
        }
        return 1;
    } else if (input == 9) {
        machine->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    
    if (input == 1) {
        machine->selection = -1;
        machine->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_error(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    
    if (input == 1) {
        machine->state = IDLE;
        return 1;
    }
    return 0;
}

void print_state(struct vending_machine *machine) {
    if (machine == NULL) return;
    
    printf("\n=== VENDING MACHINE ===\n");
    printf("State: ");
    
    switch (machine->state) {
        case IDLE:
            printf("IDLE");
            break;
        case SELECTING:
            printf("SELECTING");
            break;
        case PROCESSING:
            printf("PROCESSING");
            break;
        case DISPENSING:
            printf("DISPENSING");
            break;
        case ERROR:
            printf("ERROR");
            break;
    }
    
    printf("\nBalance: $%d\n", machine->balance);
    printf("Items available: %d\n", machine->item_count);
    
    if (machine->state == SELECTING) {
        printf("Select item (0-4) or 9 to cancel: ");
    } else if (machine->state == PROCESSING) {
        printf("Selection: %d, Cost: $2\n", machine->selection);
        printf("Press 1 to confirm, 9 to cancel: ");
    } else if (machine->state == DISPENSING) {
        printf("Item %d dispensed! Press 1 to continue: ", machine->selection);
    } else if (machine->state == ERROR) {
        printf("Error: insufficient funds or out of stock\n");
        printf("Press 1 to continue: ");
    } else {
        printf("Press 1 to select item, 2 to insert $1: ");
    }
}

int get_valid_input(void) {
    char buffer[16];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    char *endptr;
    long value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    
    if (value < 0 || value > 9) {
        return -1;
    }
    
    return (int)value;
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    int running = 1;
    
    while (running) {
        print_state(&machine);
        int input = get_valid_input();
        
        if (input == -1) {
            continue;
        }
        
        int handled = 0;
        switch (machine.state) {
            case IDLE:
                handled = handle_idle(&machine, input);
                break;
            case SELECTING:
                handled = handle_selecting(&machine, input);
                break;
            case PROCESSING:
                handled = handle_processing(&machine, input);
                break;
            case DISPENSING:
                handled = handle_dispensing(&machine, input);
                break;
            case ERROR:
                handled = handle_error(&machine, input);
                break;
        }
        
        if (input == 0 && machine