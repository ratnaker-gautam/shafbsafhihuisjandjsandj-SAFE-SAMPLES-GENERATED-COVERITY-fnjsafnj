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
    printf("Processed line %d: %s\n", line_count, line);
}

void find_errors(const char* line) {
    if (strstr(line, "ERROR") != NULL || strstr(line, "error") != NULL) {
        printf("Found error: %s\n", line);
    }
}

void analyze_length(const char* line) {
    int len = strlen(line);
    if (len > 80) {
        printf("Long line (%d chars): %.80s...\n", len, line);
    }
}

int validate_line(const char* line) {
    if (line == NULL) return 0;
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    return 1;
}

void process_log(LogData* data, LogProcessor processor) {
    if (data == NULL || processor == NULL) return;
    
    for (int i = 0; i < data->count; i++) {
        if (validate_line(data->lines[i])) {
            processor(data->lines[i]);
        }
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
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (strlen(buffer) == 0) break;
        
        if (strlen(buffer) < MAX_LINE_LENGTH) {
            strncpy(data->lines[data->count], buffer, MAX_LINE_LENGTH - 1);
            data->lines[data->count][MAX_LINE_LENGTH - 1] = '\0';
            data->count++;
        }
    }
    
    return data->count > 0;
}

int main() {
    LogData log_data;
    
    if (!read_log_file(&log_data)) {
        fprintf(stderr, "Failed to read log data\n");
        return 1;
    }
    
    printf("\n=== Line Count Analysis ===\n");
    process_log(&log_data, count_lines);
    
    printf("\n=== Error Detection ===\n");
    process_log(&log_data, find_errors);
    
    printf("\n=== Length Analysis ===\n");
    process_log(&log_data, analyze_length);
    
    printf("\nAnalysis complete. Processed %d log lines.\n", log_data.count);
    
    return 0;
}