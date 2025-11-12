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
    return 1;
}

int handle_selecting(struct vending_machine *machine, int input) {
    if (input == 1) {
        if (machine->inventory[machine->selection] > 0) {
            machine->state = PROCESSING;
        } else {
            machine->state = ERROR;
        }
        return 1;
    } else if (input == 2) {
        machine->state = IDLE;
        machine->selection = -1;
        return 1;
    }
    return 1;
}

int handle_processing(struct vending_machine *machine, int input) {
    if (input >= 1 && input <= 5) {
        if (machine->balance + input <= 100) {
            machine->balance += input;
        }
    } else if (input == 9) {
        if (machine->balance >= 2) {
            machine->balance -= 2;
            machine->inventory[machine->selection]--;
            machine->state = DISPENSING;
        } else {
            machine->state = ERROR;
        }
        return 1;
    } else if (input == 0) {
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
        return 1;
    }
    return 1;
}

int handle_dispensing(struct vending_machine *machine, int input) {
    if (input == 1) {
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
    }
    return 1;
}

int handle_error(struct vending_machine *machine, int input) {
    if (input == 1) {
        machine->state = IDLE;
        machine->selection = -1;
        machine->balance = 0;
    }
    return 1;
}

void print_state(struct vending_machine *machine) {
    switch (machine->state) {
        case IDLE:
            printf("State: IDLE\n");
            printf("Options: Select item (1-5), Exit (0)\n");
            break;
        case SELECTING:
            printf("State: SELECTING Item %d\n", machine->selection + 1);
            printf("Balance: $%d\n", machine->balance);
            printf("Options: Confirm (1), Cancel (2)\n");
            break;
        case PROCESSING:
            printf("State: PROCESSING Item %d\n", machine->selection + 1);
            printf("Balance: $%d, Cost: $2\n", machine->balance);
            printf("Options: Add money (1-5), Purchase (9), Cancel (0)\n");
            break;
        case DISPENSING:
            printf("State: DISPENSING Item %d\n", machine->selection + 1);
            printf("Change: $%d\n", machine->balance);
            printf("Options: Continue (1)\n");
            break;
        case ERROR:
            printf("State: ERROR\n");
            printf("Options: Continue (1)\n");
            break;
    }
}

int main() {
    struct vending_machine machine;
    init_machine(&machine);
    int running = 1;
    int input;

    while (running) {
        print_state(&machine);
        printf("Enter choice: ");
        
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (machine.state) {
            case IDLE:
                running = handle_idle(&machine, input);
                break;
            case SELECTING:
                running = handle_selecting(&machine, input);
                break;
            case PROCESSING:
                running = handle_processing(&machine, input);
                break;
            case DISPENSING:
                running = handle_dispensing(&machine, input);
                break;
            case ERROR:
                running = handle_error(&machine, input);
                break;
        }
    }

    return 0;
}