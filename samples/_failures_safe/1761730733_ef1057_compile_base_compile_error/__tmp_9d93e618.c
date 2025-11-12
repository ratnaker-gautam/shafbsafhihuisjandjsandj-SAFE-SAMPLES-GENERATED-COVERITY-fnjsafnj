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
    if (input == 0) {
        machine->state = SELECTING;
        return 1;
    }
    return 0;
}

int handle_selecting(struct vending_machine *machine, int input) {
    if (input >= 1 && input <= 5) {
        machine->selection = input - 1;
        if (machine->selection >= 0 && machine->selection < 5 && machine->stock[machine->selection] > 0) {
            machine->state = PROCESSING;
            return 1;
        } else {
            machine->state = ERROR;
            return 0;
        }
    } else if (input == 9) {
        machine->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_processing(struct vending_machine *machine, int input) {
    if (input == 5) {
        if (machine->balance <= 1000 - 5) {
            machine->balance += 5;
        }
        return 1;
    } else if (input == 10) {
        if (machine->balance <= 1000 - 10) {
            machine->balance += 10;
        }
        return 1;
    } else if (input == 25) {
        if (machine->balance <= 1000 - 25) {
            machine->balance += 25;
        }
        return 1;
    } else if (input == 0) {
        if (machine->balance >= 50) {
            machine->balance -= 50;
            machine->state = DISPENSING;
            return 1;
        } else {
            machine->state = ERROR;
            return 0;
        }
    } else if (input == 9) {
        machine->state = IDLE;
        machine->balance = 0;
        return 1;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *machine, int input) {
    if (input == 1) {
        if (machine->selection >= 0 && machine->selection < 5) {
            machine->stock[machine->selection]--;
        }
        machine->state = IDLE;
        machine->balance = 0;
        machine->selection = -1;
        return 1;
    }
    return 0;
}

int handle_error(struct vending_machine *machine, int input) {
    if (input == 9) {
        machine->state = IDLE;
        machine->balance = 0;
        machine->selection = -1;
        return 1;
    }
    return 0;
}

void print_state(struct vending_machine *machine) {
    switch (machine->state) {
        case IDLE:
            printf("State: IDLE\n");
            printf("Options: 0-Start selection\n");
            break;
        case SELECTING:
            printf("State: SELECTING\n");
            printf("Balance: %d cents\n", machine->balance);
            printf("Options: 1-5 Select item, 9-Cancel\n");
            break;
        case PROCESSING:
            printf("State: PROCESSING\n");
            printf("Selected: Item %d\n", machine->selection + 1);
            printf("Balance: %d cents\n", machine->balance);
            printf("Options: 5/10/25-Insert coins, 0-Confirm, 9-Cancel\n");
            break;
        case DISPENSING:
            printf("State: DISPENSING\n");
            printf("Dispensing Item %d\n", machine->selection + 1);
            printf("Options: 1-Take item\n");
            break;
        case ERROR:
            printf("State: ERROR\n");
            printf("Options: 9-Reset\n");
            break;
    }
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
    
    if (value < 0 || value > 100) {
        return -1;
    }
    
    return (int)value;
}

int main(void) {
    struct vending_machine machine;
    init_machine(&machine);
    
    printf("Vending Machine Simulator\n");
    
    while (1) {
        printf("\n");
        print_state(&machine);
        printf("Enter choice: ");
        
        int input = get_valid_input();
        if (input == -1) {
            printf("Invalid input\n");
            continue;
        }
        
        int result = 0;
        switch (machine.state) {
            case IDLE:
                result = handle_idle(&machine, input);
                break;