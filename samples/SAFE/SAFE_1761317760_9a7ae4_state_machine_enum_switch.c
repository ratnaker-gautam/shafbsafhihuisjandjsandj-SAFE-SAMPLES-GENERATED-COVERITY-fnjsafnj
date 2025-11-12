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
    int stock[5];
};

void init_machine(struct vending_machine *machine) {
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    for (int i = 0; i < 5; i++) {
        machine->stock[i] = 3;
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
    if (input >= 1 && input <= 5) {
        machine->selection = input - 1;
        return 1;
    }
    if (input == 9) {
        if (machine->selection >= 0 && machine->selection < 5) {
            if (machine->stock[machine->selection] > 0) {
                machine->state = PROCESSING;
                return 1;
            }
        }
    }
    return 0;
}

int handle_processing(struct vending_machine *machine, int input) {
    if (input >= 1 && input <= 100) {
        machine->balance += input;
        if (machine->balance >= 50) {
            machine->state = DISPENSING;
            return 1;
        }
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
    if (machine->selection >= 0 && machine->selection < 5) {
        if (machine->stock[machine->selection] > 0) {
            machine->stock[machine->selection]--;
            int change = machine->balance - 50;
            machine->balance = 0;
            machine->selection = -1;
            machine->state = IDLE;
            printf("Item dispensed. Change: %d\n", change);
            return 1;
        }
    }
    machine->state = ERROR;
    return 0;
}

void handle_error(struct vending_machine *machine) {
    printf("Error occurred. Resetting.\n");
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
    if (machine->selection != -1) {
        printf("Selection: %d\n", machine->selection + 1);
    }
    printf("Stock: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", machine->stock[i]);
    }
    printf("\n");
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    
    printf("Vending Machine Simulator\n");
    printf("Items: 1-5 (Price: 50 each)\n");
    printf("Commands: 1-5=select, 9=confirm, 1-100=insert money, 0=cancel\n");
    
    while (1) {
        print_state(&machine);
        printf("Enter command: ");
        
        int input;
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        int valid = 0;
        switch (machine.state) {
            case IDLE:
                valid = handle_idle(&machine, input);
                break;
            case SELECTING:
                valid = handle_selecting(&machine, input);
                break;
            case PROCESSING:
                valid = handle_processing(&machine, input);
                break;
            case DISPENSING:
                handle_dispensing(&machine);
                valid = 1;
                break;
            case ERROR:
                handle_error(&machine);
                valid = 1;
                break;
        }
        
        if (!valid) {
            printf("Invalid command for current state.\n");
        }
        
        if (machine.state == DISPENSING) {
            machine.state = IDLE;
        }
    }
    
    return 0;
}