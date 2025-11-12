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
        if (machine->selection >= 0 && machine->selection < 5 && machine->stock[machine->selection] > 0) {
            machine->balance = 50;
            machine->state = PROCESSING;
            return 1;
        } else {
            machine->state = ERROR;
            return 1;
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
        if (machine->balance <= 75) {
            machine->balance += 25;
        }
        if (machine->balance >= 100) {
            machine->state = DISPENSING;
        }
        return 1;
    } else if (input == 2) {
        if (machine->balance >= 100) {
            machine->state = DISPENSING;
        }
        return 1;
    } else if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        return 1;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *machine) {
    if (machine->selection >= 0 && machine->selection < 5 && machine->stock[machine->selection] > 0) {
        machine->stock[machine->selection]--;
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
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
    printf("Current state: ");
    switch (machine->state) {
        case IDLE: printf("IDLE\n"); break;
        case SELECTING: printf("SELECTING (item %d)\n", machine->selection); break;
        case PROCESSING: printf("PROCESSING (balance: %d)\n", machine->balance); break;
        case DISPENSING: printf("DISPENSING\n"); break;
        case ERROR: printf("ERROR\n"); break;
    }
}

void print_menu(struct vending_machine *machine) {
    printf("\nAvailable items:\n");
    for (int i = 0; i < 5; i++) {
        printf("%d: Item %d (stock: %d)\n", i, i, machine->stock[i]);
    }
    
    printf("\nCurrent options:\n");
    switch (machine->state) {
        case IDLE:
            printf("Enter item number (0-4) to select\n");
            break;
        case SELECTING:
            printf("1 - Insert coins (50 cents)\n");
            printf("0 - Cancel selection\n");
            break;
        case PROCESSING:
            printf("1 - Add 25 cents\n");
            printf("2 - Complete transaction\n");
            printf("0 - Cancel transaction\n");
            break;
        case DISPENSING:
            printf("Dispensing item...\n");
            break;
        case ERROR:
            printf("0 - Return to main menu\n");
            break;
    }
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    int input;
    int result;
    
    printf("Vending Machine Simulator\n");
    
    while (1) {
        print_state(&machine);
        print_menu(&machine);
        
        if (machine.state == DISPENSING) {
            handle_dispensing(&machine);
            continue;
        }
        
        printf("Enter choice: ");
        result = scanf("%d", &input);
        if (result != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        switch (machine.state) {
            case IDLE:
                if (!handle_idle(&machine, input)) {
                    printf("Invalid selection\n");
                }
                break;
            case SELECTING:
                if (!handle_selecting(&machine, input)) {
                    printf("Invalid option\n");
                }
                break;
            case PROCESSING:
                if (!handle_processing(&machine, input)) {
                    printf("Invalid option