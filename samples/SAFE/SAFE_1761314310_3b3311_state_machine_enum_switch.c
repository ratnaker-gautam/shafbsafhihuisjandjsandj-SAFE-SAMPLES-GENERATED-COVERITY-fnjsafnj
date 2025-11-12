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
    if (input >= 0 && input <= 4) {
        machine->selection = input;
        machine->state = SELECTING;
        return 1;
    }
    return 0;
}

int handle_selecting(struct vending_machine *machine, int input) {
    if (input == 1) {
        if (machine->stock[machine->selection] > 0) {
            machine->balance += 100;
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
    if (input == 1) {
        if (machine->balance >= 100) {
            machine->stock[machine->selection]--;
            machine->balance -= 100;
            machine->state = DISPENSING;
            return 1;
        } else {
            machine->state = ERROR;
            return 0;
        }
    } else if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
        return 1;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *machine, int input) {
    if (input == 1) {
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
        return 1;
    }
    return 0;
}

int handle_error(struct vending_machine *machine, int input) {
    if (input == 1) {
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
        return 1;
    }
    return 0;
}

int process_input(struct vending_machine *machine, int input) {
    switch (machine->state) {
        case IDLE:
            return handle_idle(machine, input);
        case SELECTING:
            return handle_selecting(machine, input);
        case PROCESSING:
            return handle_processing(machine, input);
        case DISPENSING:
            return handle_dispensing(machine, input);
        case ERROR:
            return handle_error(machine, input);
        default:
            return 0;
    }
}

void print_state(struct vending_machine *machine) {
    printf("Current state: ");
    switch (machine->state) {
        case IDLE:
            printf("IDLE\n");
            printf("Select product (0-4): ");
            break;
        case SELECTING:
            printf("SELECTING\n");
            printf("Product %d selected. Insert coin (1) or cancel (0): ", machine->selection);
            break;
        case PROCESSING:
            printf("PROCESSING\n");
            printf("Balance: %d. Confirm purchase (1) or cancel (0): ", machine->balance);
            break;
        case DISPENSING:
            printf("DISPENSING\n");
            printf("Product dispensed. Continue (1): ");
            break;
        case ERROR:
            printf("ERROR\n");
            printf("Operation failed. Continue (1): ");
            break;
    }
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    
    printf("Vending Machine Simulator\n");
    printf("Products: 0-4, Price: 100 each\n");
    
    while (1) {
        print_state(&machine);
        
        int input;
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            continue;
        }
        
        if (input < 0 || input > 9) {
            continue;
        }
        
        if (!process_input(&machine, input)) {
            machine.state = ERROR;
        }
        
        printf("\n");
    }
    
    return 0;
}