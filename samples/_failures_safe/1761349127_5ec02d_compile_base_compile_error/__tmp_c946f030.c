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
    machine->item_count = 5;
    machine->max_items = 10;
}

int handle_idle(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 1) {
        machine->state = SELECTING;
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

int handle_processing(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 1) {
        if (machine->item_count > 0) {
            machine->balance += 1;
            if (machine->balance >= 2) {
                machine->state = DISPENSING;
            }
            return 1;
        } else {
            machine->state = ERROR;
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

int handle_dispensing(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 1) {
        if (machine->item_count > 0) {
            machine->item_count--;
            machine->balance = 0;
            machine->selection = -1;
            machine->state = IDLE;
            return 1;
        } else {
            machine->state = ERROR;
            return 1;
        }
    }
    return 0;
}

int handle_error(struct vending_machine *machine, int input) {
    if (machine == NULL) return 0;
    if (input == 1) {
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
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
    if (machine == NULL) return;
    
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
    printf(" | Balance: %d | Items: %d", machine->balance, machine->item_count);
    if (machine->selection != -1) {
        printf(" | Selection: %d", machine->selection);
    }
    printf("\n");
}

void print_menu(void) {
    printf("\nOptions:\n");
    printf("1 - Start selection\n");
    printf("2 - Select item 1\n");
    printf("3 - Select item 2\n");
    printf("4 - Select item 3\n");
    printf("5 - Insert coin\n");
    printf("6 - Dispense item\n");
    printf("7 - Reset/cancel\n");
    printf("0 - Exit program\n");
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    int running = 1;
    int input;
    
    printf("Vending Machine Simulator\n");
    
    while (running) {
        print_state(&machine);
        print_menu();
        printf("Enter choice: ");
        
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (input == 0) {
            running = 0;
            continue;
        }
        
        int mapped_input;
        switch (input) {
            case 1: mapped_input = 1; break;
            case 2: mapped_input = 1; break;
            case 3: mapped_input = 2; break;
            case 4: mapped_input