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
        machine->balance += input;
        if (machine->balance >= 2) {
            machine->state = PROCESSING;
            return 1;
        }
    }
    return 0;
}

int handle_processing(struct vending_machine *machine) {
    if (machine->selection < 0 || machine->selection > 4) {
        machine->state = ERROR;
        return 0;
    }
    if (machine->inventory[machine->selection] <= 0) {
        machine->state = ERROR;
        return 0;
    }
    if (machine->balance < 2) {
        machine->state = ERROR;
        return 0;
    }
    machine->state = DISPENSING;
    return 1;
}

int handle_dispensing(struct vending_machine *machine) {
    machine->inventory[machine->selection]--;
    machine->balance -= 2;
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
        case IDLE: printf("IDLE"); break;
        case SELECTING: printf("SELECTING"); break;
        case PROCESSING: printf("PROCESSING"); break;
        case DISPENSING: printf("DISPENSING"); break;
        case ERROR: printf("ERROR"); break;
    }
    printf(" | Balance: %d | Selection: %d\n", machine->balance, 
           machine->selection >= 0 ? machine->selection + 1 : 0);
    printf("Inventory: [1:%d] [2:%d] [3:%d] [4:%d] [5:%d]\n",
           machine->inventory[0], machine->inventory[1], machine->inventory[2],
           machine->inventory[3], machine->inventory[4]);
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    
    printf("Vending Machine Simulator\n");
    printf("Items: 1-5 (Cost: 2 coins each)\n");
    printf("Commands: 1-5 (select item), 1-5 (insert coins), 0 (cancel)\n");
    printf("Enter -1 to exit\n\n");
    
    int input;
    while (1) {
        print_state(&machine);
        printf("Enter command: ");
        
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (input == -1) {
            break;
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
                if (input == 0) {
                    machine.state = IDLE;
                    machine.selection = -1;
                    valid = 1;
                } else {
                    printf("Processing... please wait\n");
                }
                break;
            case DISPENSING:
                printf("Dispensing item...\n");
                handle_dispensing(&machine);
                valid = 1;
                break;
            case ERROR:
                printf("Error occurred. Resetting...\n");
                handle_error(&machine);
                valid = 1;
                break;
        }
        
        if (machine.state == PROCESSING && valid) {
            handle_processing(&machine);
        }
        
        if (!valid && machine.state != PROCESSING && machine.state != DISPENSING) {
            printf("Invalid command for current state.\n");
        }
        
        printf("\n");
    }
    
    return 0;
}