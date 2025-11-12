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
    if (machine->selection < 0 || machine->selection >= 5) {
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
    printf("State: ");
    switch (machine->state) {
        case IDLE: printf("IDLE"); break;
        case SELECTING: printf("SELECTING"); break;
        case PROCESSING: printf("PROCESSING"); break;
        case DISPENSING: printf("DISPENSING"); break;
        case ERROR: printf("ERROR"); break;
    }
    printf(" | Balance: %d | Selection: %d\n", machine->balance, 
           machine->selection >= 0 ? machine->selection + 1 : 0);
    printf("Inventory: [");
    for (int i = 0; i < 5; i++) {
        printf("%d", machine->inventory[i]);
        if (i < 4) printf(", ");
    }
    printf("]\n");
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
    
    printf("Vending Machine Simulator\n");
    printf("Items: 1-5 (Cost: 2 coins each)\n");
    printf("Commands: 1-5 (select), 1-5 (add coins), 0 (cancel)\n");
    printf("Enter -1 to exit\n\n");
    
    while (1) {
        print_state(&machine);
        printf("Enter command: ");
        
        int input = get_valid_input();
        if (input == -1) {
            break;
        }
        
        int processed = 0;
        switch (machine.state) {
            case IDLE:
                processed = handle_idle(&machine, input);
                break;
            case SELECTING:
                processed = handle_selecting(&machine, input);
                break;
            case PROCESSING:
                if (input == 0) {
                    processed = handle_error(&machine);
                }
                break;
            case DISPENSING:
                processed = handle_dispensing(&machine);
                break;
            case ERROR:
                processed = handle_error(&machine);
                break;
        }
        
        if (machine.state == PROCESSING && !processed) {
            processed = handle_processing(&machine);
        }
        
        if (!processed && machine.state != PROCESSING) {
            printf("Invalid input for current state\n");
        }
        
        printf("\n");
    }
    
    return 0;
}