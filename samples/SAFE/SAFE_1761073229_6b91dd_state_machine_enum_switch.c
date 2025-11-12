//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum vending_state {
    IDLE,
    SELECTING,
    PAYING,
    DISPENSING,
    ERROR
};

struct vending_machine {
    enum vending_state state;
    int selection;
    int payment;
    int balance;
    int stock[3];
};

void init_machine(struct vending_machine *vm) {
    vm->state = IDLE;
    vm->selection = -1;
    vm->payment = 0;
    vm->balance = 0;
    vm->stock[0] = 5;
    vm->stock[1] = 5;
    vm->stock[2] = 5;
}

int handle_idle(struct vending_machine *vm, int input) {
    if (input >= 0 && input <= 2) {
        if (vm->stock[input] > 0) {
            vm->selection = input;
            vm->state = SELECTING;
            return 1;
        }
    }
    return 0;
}

int handle_selecting(struct vending_machine *vm, int input) {
    if (input == 1) {
        vm->state = PAYING;
        return 1;
    } else if (input == 0) {
        vm->selection = -1;
        vm->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_paying(struct vending_machine *vm, int input) {
    if (input >= 1 && input <= 100) {
        vm->payment += input;
        if (vm->payment >= 50) {
            vm->balance = vm->payment - 50;
            vm->state = DISPENSING;
            return 1;
        }
        return 1;
    } else if (input == 0) {
        vm->balance = vm->payment;
        vm->payment = 0;
        vm->selection = -1;
        vm->state = IDLE;
        return 1;
    }
    return 0;
}

int handle_dispensing(struct vending_machine *vm) {
    if (vm->selection >= 0 && vm->selection <= 2) {
        vm->stock[vm->selection]--;
        vm->payment = 0;
        vm->selection = -1;
        vm->state = IDLE;
        return 1;
    }
    vm->state = ERROR;
    return 0;
}

void print_state(struct vending_machine *vm) {
    const char *states[] = {"IDLE", "SELECTING", "PAYING", "DISPENSING", "ERROR"};
    printf("State: %s\n", states[vm->state]);
    if (vm->state == SELECTING || vm->state == PAYING) {
        printf("Selected: %d\n", vm->selection);
    }
    if (vm->state == PAYING) {
        printf("Payment: %d, Needed: %d\n", vm->payment, 50 - vm->payment);
    }
    if (vm->state == DISPENSING) {
        printf("Dispensing item %d, Change: %d\n", vm->selection, vm->balance);
    }
    printf("Stock: A:%d B:%d C:%d\n", vm->stock[0], vm->stock[1], vm->stock[2]);
}

int main(void) {
    struct vending_machine vm;
    init_machine(&vm);
    int input;
    
    printf("Vending Machine Simulator\n");
    printf("Items: 0=A($50), 1=B($50), 2=C($50)\n");
    printf("Commands: Select(0-2), Confirm(1), Cancel(0), Pay(1-100)\n");
    
    while (1) {
        print_state(&vm);
        printf("Enter command: ");
        
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        int result = 0;
        switch (vm.state) {
            case IDLE:
                result = handle_idle(&vm, input);
                break;
            case SELECTING:
                result = handle_selecting(&vm, input);
                break;
            case PAYING:
                result = handle_paying(&vm, input);
                break;
            case DISPENSING:
                result = handle_dispensing(&vm);
                break;
            case ERROR:
                vm.state = IDLE;
                result = 1;
                break;
        }
        
        if (!result) {
            printf("Invalid command for current state\n");
        }
        
        if (vm.state == DISPENSING) {
            handle_dispensing(&vm);
        }
    }
    
    return 0;
}