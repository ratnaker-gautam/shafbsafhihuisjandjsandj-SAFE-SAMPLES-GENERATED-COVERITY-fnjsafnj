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

void analyze_length(const char* line) {
    int len = strlen(line);
    if (len > 80) {
        printf("Long line (%d chars): %.80s...\n", len, line);
    }
}

int read_log_file(const char* filename, LogData* log_data) {
    if (filename == NULL || log_data == NULL) {
        return 0;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    
    log_data->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (log_data->count < MAX_LINES && fgets(buffer, sizeof(buffer), file) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        if (len < sizeof(log_data->lines[0])) {
            strncpy(log_data->lines[log_data->count], buffer, sizeof(log_data->lines[0]) - 1);
            log_data->lines[log_data->count][sizeof(log_data->lines[0]) - 1] = '\0';
            log_data->count++;
        }
    }
    
    fclose(file);
    return 1;
}

void process_log_data(LogData* log_data, LogProcessor processor) {
    if (log_data == NULL || processor == NULL) {
        return;
    }
    
    for (int i = 0; i < log_data->count; i++) {
        processor(log_data->lines[i]);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    LogData log_data;
    
    if (!read_log_file(argv[1], &log_data)) {
        return 1;
    }
    
    if (log_data.count == 0) {
        printf("No log data found in file %s\n", argv[1]);
        return 1;
    }
    
    printf("=== Line Count Analysis ===\n");
    process_log_data(&log_data, count_lines);
    
    printf("\n=== Error Detection ===\n");
    process_log_data(&log_data, find_errors);
    
    printf("\n=== Length Analysis ===\n");
    process_log_data(&log_data, analyze_length);
    
    printf("\nTotal lines processed: %d\n", log_data.count);
    
    return 0;
}