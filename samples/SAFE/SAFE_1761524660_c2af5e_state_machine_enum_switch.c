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
    if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        return 1;
    } else if (input == 1) {
        if (machine->inventory[machine->selection] > 0) {
            machine->state = PROCESSING;
            return 1;
        } else {
            machine->state = ERROR;
            return 0;
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
        machine->balance = 0;
        machine->selection = -1;
        machine->state = IDLE;
        return 1;
    }
    machine->state = ERROR;
    return 0;
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

int main() {
    struct vending_machine machine;
    init_machine(&machine);
    int input;
    int result;
    
    printf("Vending Machine Simulator\n");
    printf("Items: 0-4, Cost: 2 coins each\n");
    printf("Commands: item(0-4), pay(1-5), confirm(1), cancel(0)\n");
    
    while (1) {
        print_state(&machine);
        printf("Enter command: ");
        
        result = scanf("%d", &input);
        if (result != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (input == -1) {
            break;
        }
        
        switch (machine.state) {
            case IDLE:
                if (!handle_idle(&machine, input)) {
                    printf("Invalid selection\n");
                }
                break;
                
            case SELECTING:
                if (!handle_selecting(&machine, input)) {
                    printf("Invalid command\n");
                }
                break;
                
            case PROCESSING:
                if (!handle_processing(&machine, input)) {
                    printf("Invalid payment\n");
                }
                break;
                
            case DISPENSING:
                if (handle_dispensing(&machine)) {
                    printf("Item dispensed!\n");
                } else {
                    printf("Dispensing error\n");
                }
                break;
                
            case ERROR:
                if (!handle_error(&machine, input)) {
                    printf("Press 0 to reset\n");
                }
                break;
        }
    }
    
    return 0;
}