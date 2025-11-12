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
    printf("Processed line %d: %s\n", line_count, line);
}

void find_errors(const char* line) {
    if (strstr(line, "ERROR") != NULL || strstr(line, "error") != NULL) {
        printf("Found error: %s\n", line);
    }
}

void analyze_timestamps(const char* line) {
    const char* timestamp = strstr(line, "202");
    if (timestamp != NULL) {
        printf("Timestamp found: %s\n", line);
    }
}

int read_log_file(const char* filename, LogData* log_data) {
    if (filename == NULL || log_data == NULL) {
        return -1;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    
    log_data->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (fgets(buffer, sizeof(buffer), file) != NULL && log_data->count < MAX_LINES) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (len > 0) {
            strncpy(log_data->lines[log_data->count], buffer, MAX_LINE_LENGTH - 1);
            log_data->lines[log_data->count][MAX_LINE_LENGTH - 1] = '\0';
            log_data->count++;
        }
    }
    
    fclose(file);
    return 0;
}

void process_log(const LogData* log_data, LogProcessor processor) {
    if (log_data == NULL || processor == NULL) {
        return;
    }
    
    for (int i = 0; i < log_data->count; i++) {
        processor(log_data->lines[i]);
    }
}

int main() {
    LogData log_data;
    const char* filename = "sample.log";
    
    if (read_log_file(filename, &log_data) != 0) {
        printf("Failed to read log file: %s\n", filename);
        return 1;
    }
    
    if (log_data.count == 0) {
        printf("No log entries found in %s\n", filename);
        return 1;
    }
    
    printf("=== Line Counter Analysis ===\n");
    process_log(&log_data, count_lines);
    
    printf("\n=== Error Detection ===\n");
    process_log(&log_data, find_errors);
    
    printf("\n=== Timestamp Analysis ===\n");
    process_log(&log_data, analyze_timestamps);
    
    printf("\nTotal log entries processed: %d\n", log_data.count);
    
    return 0;
}