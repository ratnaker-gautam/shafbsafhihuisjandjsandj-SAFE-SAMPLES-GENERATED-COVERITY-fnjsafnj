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

void print_line(const char* line) {
    printf("%s\n", line);
}

int validate_line(const char* line) {
    if (line == NULL) return 0;
    size_t len = strlen(line);
    if (len == 0 || len >= MAX_LINE_LENGTH) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)line[i]) && line[i] != '\n' && line[i] != '\t') {
            return 0;
        }
    }
    return 1;
}

int read_log_file(LogData* log_data) {
    char buffer[MAX_LINE_LENGTH];
    
    printf("Enter log lines (empty line to finish):\n");
    
    while (log_data->count < MAX_LINES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (strlen(buffer) == 0) {
            break;
        }
        
        if (!validate_line(buffer)) {
            printf("Invalid line detected, skipping\n");
            continue;
        }
        
        strncpy(log_data->lines[log_data->count], buffer, MAX_LINE_LENGTH - 1);
        log_data->lines[log_data->count][MAX_LINE_LENGTH - 1] = '\0';
        log_data->count++;
    }
    
    return log_data->count > 0;
}

void process_log(const LogData* log_data, LogProcessor processor) {
    if (log_data == NULL || processor == NULL) return;
    
    for (int i = 0; i < log_data->count; i++) {
        processor(log_data->lines[i]);
    }
}

int main() {
    LogData log_data = {0};
    int choice;
    
    if (!read_log_file(&log_data)) {
        printf("No valid log data entered\n");
        return 1;
    }
    
    printf("\nLog Analysis Menu:\n");
    printf("1. Count lines\n");
    printf("2. Find errors\n");
    printf("3. Find warnings\n");
    printf("4. Print all lines\n");
    printf("Enter choice (1-4): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    LogProcessor processors[] = {count_lines, find_errors, find_warnings, print_line};
    const char* names[] = {"Count lines", "Find errors", "Find warnings", "Print all lines"};
    
    if (choice >= 1 && choice <= 4) {
        printf("\n%s:\n", names[choice - 1]);
        process_log(&log_data, processors[choice - 1]);
    } else {
        printf("Invalid choice\n");
        return 1;
    }
    
    return 0;
}