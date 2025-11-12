//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LINES 1000

typedef struct {
    char lines[MAX_LINES][MAX_LINE_LENGTH];
    int count;
} LogData;

typedef void (*LogProcessor)(LogData*);

void count_lines(LogData* data) {
    if (data == NULL) return;
    printf("Total lines: %d\n", data->count);
}

void find_errors(LogData* data) {
    if (data == NULL) return;
    int error_count = 0;
    for (int i = 0; i < data->count; i++) {
        if (strstr(data->lines[i], "ERROR") != NULL) {
            error_count++;
        }
    }
    printf("Error lines: %d\n", error_count);
}

void find_warnings(LogData* data) {
    if (data == NULL) return;
    int warning_count = 0;
    for (int i = 0; i < data->count; i++) {
        if (strstr(data->lines[i], "WARNING") != NULL) {
            warning_count++;
        }
    }
    printf("Warning lines: %d\n", warning_count);
}

void print_all(LogData* data) {
    if (data == NULL) return;
    for (int i = 0; i < data->count; i++) {
        printf("%s\n", data->lines[i]);
    }
}

int read_log_file(const char* filename, LogData* data) {
    if (filename == NULL || data == NULL) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    data->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (fgets(buffer, sizeof(buffer), file) != NULL && data->count < MAX_LINES) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        if (len < sizeof(data->lines[0])) {
            strncpy(data->lines[data->count], buffer, sizeof(data->lines[0]) - 1);
            data->lines[data->count][sizeof(data->lines[0]) - 1] = '\0';
            data->count++;
        }
    }
    
    fclose(file);
    return 1;
}

void show_menu(void) {
    printf("\nLog Analyzer Menu:\n");
    printf("1. Count total lines\n");
    printf("2. Find ERROR lines\n");
    printf("3. Find WARNING lines\n");
    printf("4. Print all lines\n");
    printf("5. Exit\n");
    printf("Enter choice (1-5): ");
}

int main(void) {
    LogData log_data;
    char filename[256];
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    if (!read_log_file(filename, &log_data)) {
        printf("Error reading file: %s\n", filename);
        return 1;
    }
    
    if (log_data.count == 0) {
        printf("No data read from file\n");
        return 1;
    }
    
    LogProcessor processors[] = {
        count_lines,
        find_errors,
        find_warnings,
        print_all,
        NULL
    };
    
    int choice;
    char input[16];
    
    while (1) {
        show_menu();
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input\n");
            continue;
        }
        
        if (choice == 5) {
            break;
        }
        
        if (choice < 1 || choice > 4) {
            printf("Invalid choice\n");
            continue;
        }
        
        if (processors[choice - 1] != NULL) {
            processors[choice - 1](&log_data);
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}