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
    int inventory[5];
};

void init_machine(struct vending_machine *machine) {
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    for (int i = 0; i < 5; i++) {
        machine->inventory[i] = 3;
    }
}

int handle_idle(struct vending_machine *machine, int input) {
    if (input >= 1 && input <= 5) {
        machine->selection = input - 1;
        machine->state = SELECTING;
        return 1;
    }
    return 0;
}

int handle_selecting(struct vending_machine *machine, int input) {
    if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        return 1;
    }
    if (input == 1) {
        if (machine->inventory[machine->selection] > 0) {
            machine->state = PROCESSING;
            return 1;
        }
        machine->state = ERROR;
        return 1;
    }
    return 0;
}

int handle_processing(struct vending_machine *machine, int input) {
    if (input >= 1 && input <= 5) {
        machine->balance += input;
        if (machine->balance >= 2) {
            machine->state = DISPENSING;
            return 1;
        }
        return 1;
    }
    if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
        return 1;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *machine) {
    machine->inventory[machine->selection]--;
    machine->balance -= 2;
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    return 1;
}

int handle_error(struct vending_machine *machine, int input) {
    if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
        return 1;
    }
    return 0;
}

void print_state(struct vending_machine *machine) {
    switch (machine->state) {
        case IDLE:
            printf("State: IDLE\n");
            printf("Options: Select item (1-5)\n");
            break;
        case SELECTING:
            printf("State: SELECTING\n");
            printf("Selected: %d\n", machine->selection + 1);
            printf("Options: Confirm (1), Cancel (0)\n");
            break;
        case PROCESSING:
            printf("State: PROCESSING\n");
            printf("Selected: %d\n", machine->selection + 1);
            printf("Balance: %d\n", machine->balance);
            printf("Required: 2\n");
            printf("Options: Insert coin (1-5), Cancel (0)\n");
            break;
        case DISPENSING:
            printf("State: DISPENSING\n");
            printf("Dispensing item %d\n", machine->selection + 1);
            break;
        case ERROR:
            printf("State: ERROR\n");
            printf("Item out of stock\n");
            printf("Options: Cancel (0)\n");
            break;
    }
}

int get_valid_input(void) {
    char buffer[16];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    if (strlen(buffer) == 0 || buffer[0] == '\n') {
        return -1;
    }
    char *endptr;
    long value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    if (value < 0 || value > 5) {
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
        printf("Enter choice: ");
        
        int input = get_valid_input();
        if (input == -1) {
            printf("Invalid input\n");
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
                handled = handle_dispensing(&machine);
                break;
            case ERROR:
                handled = handle_error(&machine, input);
                break;
        }
        
        if (!handled) {
            printf("Invalid action for current state\n");
        }
        
        if (machine.state == DISPENSING) {
            printf("Please take your item\n");
        }
    }
    
    return 0;
}