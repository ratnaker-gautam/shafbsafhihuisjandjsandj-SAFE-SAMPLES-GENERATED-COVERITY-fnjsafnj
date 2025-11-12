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

int validate_line(const char* line) {
    if (line == NULL) return 0;
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    return 1;
}

void process_log(LogData* log, LogProcessor processor) {
    if (log == NULL || processor == NULL) return;
    
    for (int i = 0; i < log->count; i++) {
        if (validate_line(log->lines[i])) {
            processor(log->lines[i]);
        }
    }
}

int read_log_file(LogData* log) {
    if (log == NULL) return 0;
    
    printf("Enter log lines (empty line to finish, max %d lines):\n", MAX_LINES);
    
    char buffer[MAX_LINE_LENGTH];
    log->count = 0;
    
    while (log->count < MAX_LINES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (buffer[0] == '\0') {
            break;
        }
        
        if (len >= MAX_LINE_LENGTH - 1) {
            printf("Line too long, skipping...\n");
            continue;
        }
        
        strncpy(log->lines[log->count], buffer, MAX_LINE_LENGTH - 1);
        log->lines[log->count][MAX_LINE_LENGTH - 1] = '\0';
        log->count++;
    }
    
    return log->count > 0;
}

int main() {
    LogData log;
    LogProcessor processors[] = {count_lines, find_errors, analyze_length};
    const char* processor_names[] = {"Line Counter", "Error Finder", "Length Analyzer"};
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    if (!read_log_file(&log)) {
        printf("No log data to process.\n");
        return 1;
    }
    
    printf("\nProcessing log with %d lines:\n", log.count);
    
    for (int i = 0; i < num_processors; i++) {
        printf("\n--- %s ---\n", processor_names[i]);
        process_log(&log, processors[i]);
    }
    
    printf("\nLog analysis complete.\n");
    return 0;
}