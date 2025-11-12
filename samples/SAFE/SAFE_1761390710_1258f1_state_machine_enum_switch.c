//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
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
        } else {
            machine->state = ERROR;
            return 1;
        }
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
    machine->balance = 0;
    machine->selection = -1;
    machine->state = IDLE;
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
    printf("Current state: ");
    switch (machine->state) {
        case IDLE:
            printf("IDLE\n");
            printf("Select item (1-5): ");
            break;
        case SELECTING:
            printf("SELECTING\n");
            printf("Item %d selected. Confirm (1) or cancel (0): ", machine->selection + 1);
            break;
        case PROCESSING:
            printf("PROCESSING\n");
            printf("Balance: %d. Insert coins (1-5) or cancel (0): ", machine->balance);
            break;
        case DISPENSING:
            printf("DISPENSING\n");
            printf("Dispensing item...\n");
            break;
        case ERROR:
            printf("ERROR\n");
            printf("Item out of stock. Cancel (0): ");
            break;
    }
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    int input;
    int result;
    
    while (1) {
        print_state(&machine);
        
        if (machine.state == DISPENSING) {
            handle_dispensing(&machine);
            continue;
        }
        
        result = scanf("%d", &input);
        if (result != 1) {
            while (getchar() != '\n');
            continue;
        }
        
        switch (machine.state) {
            case IDLE:
                if (!handle_idle(&machine, input)) {
                    printf("Invalid selection.\n");
                }
                break;
            case SELECTING:
                if (!handle_selecting(&machine, input)) {
                    printf("Invalid input.\n");
                }
                break;
            case PROCESSING:
                if (!handle_processing(&machine, input)) {
                    printf("Invalid coin.\n");
                }
                break;
            case ERROR:
                if (!handle_error(&machine, input)) {
                    printf("Invalid input.\n");
                }
                break;
            default:
                break;
        }
        
        printf("\n");
    }
    
    return 0;
}