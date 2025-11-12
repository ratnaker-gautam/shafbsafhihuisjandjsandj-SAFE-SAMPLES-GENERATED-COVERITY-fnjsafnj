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
    if (input >= 1 && input <= 5) {
        machine->balance += input;
        if (machine->balance >= 2) {
            machine->state = DISPENSING;
            return 1;
        }
        return 1;
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
        int change = machine->balance - 2;
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
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0) {
        return -1;
    }
    for (size_t i = 0; i < strlen(buffer); i++) {
        if (!isdigit(buffer[i])) {
            return -1;
        }
    }
    long val = strtol(buffer, NULL, 10);
    if (val < 0 || val > 100) {
        return -1;
    }
    return (int)val;
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    int running = 1;
    
    printf("Vending Machine Simulator\n");
    printf("Items: 0-4, Cost: 2 coins each\n");
    printf("Commands: item(0-4), pay(1-5), confirm(1), cancel(0)\n");
    
    while (running) {
        print_state(&machine);
        printf("Enter input: ");
        
        int input = get_valid_input();
        if (input == -1) {
            printf("Invalid input. Try again.\n");
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
                handle_error(&machine);
                handled = 1;
                break;
        }
        
        if (!handled) {
            printf("Invalid operation for current state.\n");
        }
        
        if (machine.state == DISPENSING) {
            handled = handle_dispensing(&machine);
        }
        
        printf("\n");
    }
    
    return 0;
}