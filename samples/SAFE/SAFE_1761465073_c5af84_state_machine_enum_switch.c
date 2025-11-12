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
    } else if (input == 0) {
        return 0;
    }
    return 1;
}

int handle_selecting(struct vending_machine *machine, int input) {
    if (input >= 1 && input <= 100) {
        machine->balance += input;
        if (machine->balance >= 50) {
            machine->state = PROCESSING;
        }
        return 1;
    } else if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        return 1;
    }
    return 1;
}

int handle_processing(struct vending_machine *machine) {
    if (machine->selection < 0 || machine->selection >= 5) {
        machine->state = ERROR;
        return 1;
    }
    
    if (machine->inventory[machine->selection] <= 0) {
        machine->state = ERROR;
        return 1;
    }
    
    if (machine->balance < 50) {
        machine->state = ERROR;
        return 1;
    }
    
    machine->balance -= 50;
    machine->inventory[machine->selection]--;
    machine->state = DISPENSING;
    return 1;
}

int handle_dispensing(struct vending_machine *machine) {
    printf("Dispensing item %d\n", machine->selection + 1);
    printf("Change: %d\n", machine->balance);
    machine->balance = 0;
    machine->selection = -1;
    machine->state = IDLE;
    return 1;
}

int handle_error(struct vending_machine *machine) {
    printf("Error occurred. Returning balance: %d\n", machine->balance);
    machine->balance = 0;
    machine->selection = -1;
    machine->state = IDLE;
    return 1;
}

int process_input(struct vending_machine *machine, int input) {
    switch (machine->state) {
        case IDLE:
            return handle_idle(machine, input);
        case SELECTING:
            return handle_selecting(machine, input);
        case PROCESSING:
            return handle_processing(machine);
        case DISPENSING:
            return handle_dispensing(machine);
        case ERROR:
            return handle_error(machine);
        default:
            machine->state = ERROR;
            return 1;
    }
}

void print_state(struct vending_machine *machine) {
    printf("Current state: ");
    switch (machine->state) {
        case IDLE:
            printf("IDLE");
            break;
        case SELECTING:
            printf("SELECTING (item %d, balance: %d)", machine->selection + 1, machine->balance);
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
    printf("\n");
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    
    printf("Vending Machine Simulator\n");
    printf("Items: 1-5 (Cost: 50 each)\n");
    printf("Enter item number (1-5) or 0 to exit\n");
    printf("In SELECTING state: enter money amount (1-100) or 0 to cancel\n");
    
    int running = 1;
    while (running) {
        print_state(&machine);
        
        int input = 0;
        char buffer[32];
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (sscanf(buffer, "%d", &input) != 1) {
            printf("Invalid input. Enter a number.\n");
            continue;
        }
        
        if (input < 0 || input > 100) {
            printf("Input out of range (0-100).\n");
            continue;
        }
        
        if (input == 0 && machine.state == IDLE) {
            running = 0;
            continue;
        }
        
        running = process_input(&machine, input);
    }
    
    printf("Goodbye!\n");
    return 0;
}