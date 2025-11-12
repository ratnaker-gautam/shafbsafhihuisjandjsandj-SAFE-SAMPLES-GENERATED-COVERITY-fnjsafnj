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
    } else if (input == 0) {
        return 0;
    }
    return 1;
}

int handle_selecting(struct vending_machine *machine, int input) {
    if (input >= 1 && input <= 5) {
        machine->selection = input - 1;
        return 1;
    } else if (input == 10) {
        if (machine->stock[machine->selection] > 0) {
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
    return 1;
}

int handle_processing(struct vending_machine *machine, int input) {
    if (input == 25 || input == 50 || input == 100) {
        if (machine->balance + input <= 200) {
            machine->balance += input;
            if (machine->balance >= 100) {
                machine->state = DISPENSING;
            }
            return 1;
        }
    } else if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
        return 1;
    }
    return 1;
}

int handle_dispensing(struct vending_machine *machine) {
    machine->stock[machine->selection]--;
    int change = machine->balance - 100;
    machine->balance = 0;
    machine->selection = -1;
    machine->state = IDLE;
    printf("Item dispensed. Change: %d\n", change);
    return 1;
}

int handle_error(struct vending_machine *machine, int input) {
    if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
        return 1;
    }
    return 1;
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
            return handle_dispensing(machine);
        case ERROR:
            return handle_error(machine, input);
        default:
            return 0;
    }
}

void print_state(struct vending_machine *machine) {
    printf("\nCurrent state: ");
    switch (machine->state) {
        case IDLE:
            printf("IDLE");
            break;
        case SELECTING:
            printf("SELECTING (item %d)", machine->selection + 1);
            break;
        case PROCESSING:
            printf("PROCESSING (balance: %d)", machine->balance);
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

void print_menu(void) {
    printf("\nVending Machine\n");
    printf("1-5: Select item\n");
    printf("10: Confirm selection\n");
    printf("25/50/100: Insert coins\n");
    printf("0: Cancel/Return\n");
    printf("Items: 1($1.00) 2($1.00) 3($1.00) 4($1.00) 5($1.00)\n");
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    
    int running = 1;
    int input;
    
    while (running) {
        print_state(&machine);
        print_menu();
        
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (input < 0 || input > 100 || (input != 0 && input != 10 && input != 25 && input != 50 && input != 100 && (input < 1 || input > 5))) {
            printf("Invalid input. Please try again.\n");
            continue;
        }
        
        if (machine.state == DISPENSING) {
            process_input(&machine, 0);
        } else {
            running = process_input(&machine, input);
        }
    }
    
    return 0;
}