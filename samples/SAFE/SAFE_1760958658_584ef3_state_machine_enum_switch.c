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
    if (input >= 0 && input <= 4) {
        machine->selection = input;
        machine->state = SELECTING;
        return 1;
    }
    return 0;
}

int handle_selecting(struct vending_machine *machine, int input) {
    if (input == 1) {
        if (machine->inventory[machine->selection] > 0) {
            machine->state = PROCESSING;
            return 1;
        } else {
            machine->state = ERROR;
            return 0;
        }
    } else if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        return 1;
    }
    return 0;
}

int handle_processing(struct vending_machine *machine, int input) {
    if (input == 50 || input == 100) {
        if (machine->balance + input <= 200) {
            machine->balance += input;
            if (machine->balance >= 100) {
                machine->state = DISPENSING;
                return 1;
            }
            return 1;
        }
    } else if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
        return 1;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *machine) {
    if (machine->inventory[machine->selection] > 0) {
        machine->inventory[machine->selection]--;
        int change = machine->balance - 100;
        machine->balance = 0;
        machine->selection = -1;
        machine->state = IDLE;
        printf("Item dispensed. Change: %d\n", change);
        return 1;
    }
    machine->state = ERROR;
    return 0;
}

void handle_error(struct vending_machine *machine) {
    printf("Error state. Resetting.\n");
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
}

void print_state(struct vending_machine *machine) {
    const char *state_names[] = {
        "IDLE", "SELECTING", "PROCESSING", "DISPENSING", "ERROR"
    };
    printf("Current state: %s\n", state_names[machine->state]);
    printf("Balance: %d\n", machine->balance);
    printf("Selection: %d\n", machine->selection);
    printf("Inventory: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", machine->inventory[i]);
    }
    printf("\n");
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
    
    if (value < -1 || value > 1000) {
        return -1;
    }
    
    return (int)value;
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    
    printf("Vending Machine Simulator\n");
    printf("Items: 0-4 (Price: 100 each)\n");
    printf("Inputs: Item(0-4), Cancel(0), Coin(50,100), Confirm(1)\n");
    printf("Enter -1 to exit\n\n");
    
    while (1) {
        print_state(&machine);
        printf("Enter input: ");
        
        int input = get_valid_input();
        if (input == -1) {
            break;
        }
        
        int result = 0;
        switch (machine.state) {
            case IDLE:
                result = handle_idle(&machine, input);
                break;
            case SELECTING:
                result = handle_selecting(&machine, input);
                break;
            case PROCESSING:
                result = handle_processing(&machine, input);
                break;
            case DISPENSING:
                result = handle_dispensing(&machine);
                break;
            case ERROR:
                handle_error(&machine);
                result = 1;
                break;
        }
        
        if (!result) {
            printf("Invalid input for current state\n");
        }
        
        printf("\n");
    }
    
    return 0;
}