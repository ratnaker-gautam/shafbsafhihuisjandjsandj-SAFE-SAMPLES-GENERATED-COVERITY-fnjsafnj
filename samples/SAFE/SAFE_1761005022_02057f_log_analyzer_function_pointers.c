//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
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

typedef void (*LogProcessor)(const char* line);

void count_lines(const char* line) {
    static int line_count = 0;
    line_count++;
    printf("Processed %d lines\n", line_count);
}

void find_errors(const char* line) {
    if (strstr(line, "ERROR") != NULL) {
        printf("Found ERROR: %s\n", line);
    }
}

void find_warnings(const char* line) {
    if (strstr(line, "WARNING") != NULL) {
        printf("Found WARNING: %s\n", line);
    }
}

void analyze_log(LogData* data, LogProcessor processor) {
    if (data == NULL || processor == NULL) return;
    
    for (int i = 0; i < data->count; i++) {
        processor(data->lines[i]);
    }
}

int read_log_file(LogData* data) {
    if (data == NULL) return 0;
    
    data->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    printf("Enter log lines (empty line to finish, max %d lines):\n", MAX_LINES);
    
    while (data->count < MAX_LINES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        if (buffer[0] == '\0') break;
        
        if (len >= MAX_LINE_LENGTH) {
            printf("Line too long, skipping\n");
            continue;
        }
        
        strncpy(data->lines[data->count], buffer, MAX_LINE_LENGTH - 1);
        data->lines[data->count][MAX_LINE_LENGTH - 1] = '\0';
        data->count++;
    }
    
    return data->count;
}

void print_menu() {
    printf("\nLog Analysis Options:\n");
    printf("1. Count all lines\n");
    printf("2. Find ERROR messages\n");
    printf("3. Find WARNING messages\n");
    printf("4. Exit\n");
    printf("Choose option: ");
}

int main() {
    LogData log_data;
    LogProcessor processors[] = {count_lines, find_errors, find_warnings};
    const char* processor_names[] = {"Line counter", "Error finder", "Warning finder"};
    int processor_count = sizeof(processors) / sizeof(processors[0]);
    
    if (read_log_file(&log_data) == 0) {
        printf("No log data entered.\n");
        return 1;
    }
    
    printf("\nLoaded %d log lines for analysis.\n", log_data.count);
    
    int choice;
    char input[16];
    
    while (1) {
        print_menu();
        
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (choice == 4) break;
        
        if (choice >= 1 && choice <= processor_count) {
            printf("\nRunning %s:\n", processor_names[choice - 1]);
            analyze_log(&log_data, processors[choice - 1]);
        } else {
            printf("Invalid choice. Please select 1-%d or 4 to exit.\n", processor_count);
        }
    }
    
    printf("Log analysis complete.\n");
    return 0;
}