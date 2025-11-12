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
        machine->selection = input - 1;
        return 1;
    }
    if (input == 9) {
        if (machine->selection >= 0 && machine->selection < 5) {
            if (machine->inventory[machine->selection] > 0) {
                machine->state = PROCESSING;
                return 1;
            }
        }
    }
    return 0;
}

int handle_processing(struct vending_machine *machine, int input) {
    if (input == 1) {
        if (machine->balance < 100) {
            machine->balance += 1;
        }
        if (machine->balance >= 2) {
            machine->state = DISPENSING;
        }
        return 1;
    }
    if (input == 2) {
        if (machine->balance < 100) {
            machine->balance += 5;
        }
        if (machine->balance >= 2) {
            machine->state = DISPENSING;
        }
        return 1;
    }
    if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
        return 1;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *machine) {
    if (machine->selection >= 0 && machine->selection < 5) {
        if (machine->inventory[machine->selection] > 0) {
            machine->inventory[machine->selection]--;
            machine->balance -= 2;
            if (machine->balance < 0) {
                machine->balance = 0;
            }
            machine->state = IDLE;
            machine->selection = -1;
            return 1;
        }
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
    printf("State: ");
    switch (machine->state) {
        case IDLE: printf("IDLE"); break;
        case SELECTING: printf("SELECTING"); break;
        case PROCESSING: printf("PROCESSING"); break;
        case DISPENSING: printf("DISPENSING"); break;
        case ERROR: printf("ERROR"); break;
    }
    printf(" | Balance: %d | Selection: ", machine->balance);
    if (machine->selection >= 0) {
        printf("%d", machine->selection + 1);
    } else {
        printf("None");
    }
    printf(" | Inventory: ");
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
    long value = strtol(buffer, NULL, 10);
    if (value < 0 || value > 9) {
        return -1;
    }
    return (int)value;
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    int running = 1;
    
    printf("Vending Machine Simulator\n");
    printf("1-5: Select item, 9: Confirm, 1/2: Insert coin, 0: Cancel\n");
    
    while (running) {
        print_state(&machine);
        printf("Enter input (0-9): ");
        
        int input = get_valid_input();
        if (input == -1) {
            printf("Invalid input. Please enter a digit 0-9.\n");
            continue;
        }
        
        int handled = 0;
        switch (machine.state