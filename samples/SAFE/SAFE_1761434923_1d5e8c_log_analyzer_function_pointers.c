//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
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

typedef void (*LogProcessor)(const char* line);

void count_lines(const char* line) {
    static int line_count = 0;
    line_count++;
    printf("Processed line %d: %s\n", line_count, line);
}

void find_errors(const char* line) {
    if (strstr(line, "ERROR") != NULL || strstr(line, "error") != NULL) {
        printf("Found error: %s\n", line);
    }
}

void extract_timestamps(const char* line) {
    const char* timestamp_start = strstr(line, "[");
    if (timestamp_start != NULL) {
        const char* timestamp_end = strstr(timestamp_start, "]");
        if (timestamp_end != NULL && timestamp_end > timestamp_start) {
            size_t length = timestamp_end - timestamp_start + 1;
            if (length < MAX_LINE_LENGTH) {
                char timestamp[MAX_LINE_LENGTH];
                strncpy(timestamp, timestamp_start, length);
                timestamp[length] = '\0';
                printf("Timestamp: %s\n", timestamp);
            }
        }
    }
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

void process_log_lines(LogData* data, LogProcessor processor) {
    if (data == NULL || processor == NULL) return;
    
    for (int i = 0; i < data->count; i++) {
        if (validate_line(data->lines[i])) {
            processor(data->lines[i]);
        }
    }
}

int read_log_data(LogData* data) {
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
        
        if (strlen(buffer) == 0) break;
        
        if (!validate_line(buffer)) {
            printf("Invalid line format, skipping...\n");
            continue;
        }
        
        strncpy(data->lines[data->count], buffer, MAX_LINE_LENGTH - 1);
        data->lines[data->count][MAX_LINE_LENGTH - 1] = '\0';
        data->count++;
    }
    
    return data->count > 0;
}

void display_menu() {
    printf("\nLog Analysis Options:\n");
    printf("1. Count and display all lines\n");
    printf("2. Find error messages\n");
    printf("3. Extract timestamps\n");
    printf("4. Exit\n");
    printf("Choose option (1-4): ");
}

int main() {
    LogData log_data;
    
    if (!read_log_data(&log_data)) {
        printf("No valid log data entered.\n");
        return 1;
    }
    
    printf("\nSuccessfully loaded %d log lines.\n", log_data.count);
    
    int choice;
    LogProcessor processors[] = {count_lines, find_errors, extract_timestamps};
    
    while (1) {
        display_menu();
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (choice == 4) break;
        
        if (choice >= 1 && choice <= 3) {
            printf("\n");
            process_log_lines(&log_data, processors[choice - 1]);
        } else {
            printf("Invalid option. Please choose 1-4.\n");
        }
    }
    
    printf("Log analysis complete.\n");
    return 0;
}