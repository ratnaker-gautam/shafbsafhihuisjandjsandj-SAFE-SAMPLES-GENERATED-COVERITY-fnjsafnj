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
    return -1;
}

int handle_selecting(struct vending_machine *machine, int input) {
    if (input >= 1 && input <= 100) {
        machine->balance += input;
        if (machine->balance >= 50) {
            machine->state = PROCESSING;
        }
        return 1;
    }
    return -1;
}

int handle_processing(struct vending_machine *machine) {
    if (machine->selection < 0 || machine->selection >= 5) {
        machine->state = ERROR;
        return -1;
    }
    
    if (machine->inventory[machine->selection] <= 0) {
        machine->state = ERROR;
        return -1;
    }
    
    if (machine->balance < 50) {
        machine->state = ERROR;
        return -1;
    }
    
    machine->state = DISPENSING;
    return 1;
}

int handle_dispensing(struct vending_machine *machine) {
    machine->inventory[machine->selection]--;
    machine->balance -= 50;
    machine->selection = -1;
    machine->state = IDLE;
    return 1;
}

int handle_error(struct vending_machine *machine) {
    machine->selection = -1;
    machine->balance = 0;
    machine->state = IDLE;
    return 1;
}

void print_state(struct vending_machine *machine) {
    printf("Current state: ");
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
    printf("\nBalance: %d\n", machine->balance);
    printf("Inventory: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", machine->inventory[i]);
    }
    printf("\n");
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    int running = 1;
    int input;
    char buffer[32];
    
    printf("Vending Machine Simulator\n");
    printf("Items: 1-5 (Cost: 50 each)\n");
    printf("Enter item number (1-5) or 0 to exit\n");
    
    while (running) {
        print_state(&machine);
        
        if (machine.state == IDLE) {
            printf("Select item (1-5) or 0 to exit: ");
        } else if (machine.state == SELECTING) {
            printf("Insert coins (1-100), need %d more: ", 50 - machine.balance);
        } else if (machine.state == PROCESSING) {
            printf("Processing transaction...\n");
        } else if (machine.state == DISPENSING) {
            printf("Dispensing item...\n");
        } else if (machine.state == ERROR) {
            printf("Error occurred. Resetting...\n");
        }
        
        if (machine.state == PROCESSING || machine.state == DISPENSING || machine.state == ERROR) {
            int result;
            switch (machine.state) {
                case PROCESSING:
                    result = handle_processing(&machine);
                    break;
                case DISPENSING:
                    result = handle_dispensing(&machine);
                    break;
                case ERROR:
                    result = handle_error(&machine);
                    break;
                default:
                    break;
            }
            continue;
        }
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (sscanf(buffer, "%d", &input) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        int result = -1;
        switch (machine.state) {
            case IDLE:
                result = handle_idle(&machine, input);
                if (result == 0) {
                    running = 0;
                }
                break;
            case SELECTING:
                result = handle_selecting(&machine, input);
                break;
            default:
                break;
        }
        
        if (result == -1) {
            printf("Invalid operation for current state.\n");
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}