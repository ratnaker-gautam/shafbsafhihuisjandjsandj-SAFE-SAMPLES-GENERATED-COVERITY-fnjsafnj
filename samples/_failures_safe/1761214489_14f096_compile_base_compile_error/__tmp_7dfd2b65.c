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
    int item_count;
    int max_items;
};

void init_machine(struct vending_machine *machine) {
    if (machine == NULL) return;
    machine->state = IDLE;
    machine->selection = -1;
    machine->balance = 0;
    machine->item_count = 10;
    machine->max_items = 20;
}

int handle_idle(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    
    if (input == 1) {
        machine->state = SELECTING;
        return 1;
    } else if (input == 2) {
        if (machine->balance <= 1000 - 5) {
            machine->balance += 5;
        }
        printf("Balance: %d\n", machine->balance);
        return 1;
    }
    return 0;
}

int handle_selecting(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    
    if (input >= 1 && input <= 3) {
        machine->selection = input;
        machine->state = PROCESSING;
        return 1;
    } else if (input == 0) {
        machine->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_processing(struct vending_machine *machine) {
    if (machine == NULL) return 0;
    
    int price = machine->selection * 10;
    
    if (machine->balance < price) {
        printf("Insufficient funds. Need %d, have %d\n", price, machine->balance);
        machine->state = ERROR;
        return 1;
    }
    
    if (machine->item_count <= 0) {
        printf("Out of stock\n");
        machine->state = ERROR;
        return 1;
    }
    
    machine->balance -= price;
    machine->item_count--;
    machine->state = DISPENSING;
    return 1;
}

int handle_dispensing(struct vending_machine *machine) {
    if (machine == NULL) return 0;
    
    printf("Dispensing item %d\n", machine->selection);
    printf("Remaining balance: %d\n", machine->balance);
    machine->selection = -1;
    machine->state = IDLE;
    return 1;
}

int handle_error(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    
    if (input == 0) {
        machine->state = IDLE;
        return 1;
    }
    return 0;
}

int process_input(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    
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
            return handle_error(machine, input);
        default:
            machine->state = ERROR;
            return 0;
    }
}

void print_state(struct vending_machine *machine) {
    if (machine == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "SELECTING", "PROCESSING", "DISPENSING", "ERROR"
    };
    
    int state_index = machine->state;
    if (state_index < 0 || state_index >= 5) {
        state_index = 4;
    }
    
    printf("State: %s | Balance: %d | Items: %d\n", 
           state_names[state_index], machine->balance, machine->item_count);
}

void print_menu(void) {
    printf("\n=== Vending Machine ===\n");
    printf("1. Start selection\n");
    printf("2. Insert 5 coins\n");
    printf("3. Select item 1 (10 coins)\n");
    printf("4. Select item 2 (20 coins)\n");
    printf("5. Select item 3 (30 coins)\n");
    printf("0. Cancel/Return\n");
    printf("Enter choice: ");
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
    
    if (value < 0 || value > 5) {
        return -1;
    }
    
    return (int)value;
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    
    printf("Vending Machine State Machine Demo\n");
    
    while (1) {
        print_state(&machine);
        print_menu();
        
        int input = get_valid_input();
        if (input == -1) {
            printf("Invalid input. Please enter 0-5.\n");
            continue;
        }
        
        if (!process_input(&machine, input))