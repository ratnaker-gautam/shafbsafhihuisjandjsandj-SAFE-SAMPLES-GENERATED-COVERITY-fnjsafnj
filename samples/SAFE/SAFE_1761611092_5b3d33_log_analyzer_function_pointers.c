//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LINES 1000

typedef struct {
    char lines[MAX_LINES][MAX_LINE_LENGTH];
    int count;
} LogData;

typedef void (*LogProcessor)(LogData*);

void count_lines(LogData* data) {
    printf("Total lines: %d\n", data->count);
}

void find_errors(LogData* data) {
    int error_count = 0;
    for (int i = 0; i < data->count; i++) {
        if (strstr(data->lines[i], "ERROR") != NULL) {
            error_count++;
        }
    }
    printf("ERROR occurrences: %d\n", error_count);
}

void find_warnings(LogData* data) {
    int warning_count = 0;
    for (int i = 0; i < data->count; i++) {
        if (strstr(data->lines[i], "WARNING") != NULL) {
            warning_count++;
        }
    }
    printf("WARNING occurrences: %d\n", warning_count);
}

void analyze_log(LogData* data, LogProcessor processor) {
    if (data == NULL || processor == NULL) {
        return;
    }
    processor(data);
}

int read_log_file(LogData* data, const char* filename) {
    if (data == NULL || filename == NULL) {
        return 0;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }
    
    data->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (data->count < MAX_LINES && fgets(buffer, sizeof(buffer), file) != NULL) {
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

void display_menu(void) {
    printf("\nLog Analyzer Menu:\n");
    printf("1. Count total lines\n");
    printf("2. Find ERROR messages\n");
    printf("3. Find WARNING messages\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

int main(void) {
    LogData log_data;
    char filename[256];
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
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
    
    if (!read_log_file(&log_data, filename)) {
        printf("Failed to read log file: %s\n", filename);
        return 1;
    }
    
    if (log_data.count == 0) {
        printf("Log file is empty\n");
        return 1;
    }
    
    LogProcessor processors[] = {count_lines, find_errors, find_warnings};
    int choice;
    
    while (1) {
        display_menu();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        while (getchar() != '\n');
        
        if (choice == 4) {
            break;
        }
        
        if (choice >= 1 && choice <= 3) {
            analyze_log(&log_data, processors[choice - 1]);
        } else {
            printf("Invalid choice\n");
        }
    }
    
    return 0;
}