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
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (strlen(buffer) == 0) break;
        
        if (strlen(buffer) < MAX_LINE_LENGTH) {
            strncpy(log->lines[log->count], buffer, MAX_LINE_LENGTH - 1);
            log->lines[log->count][MAX_LINE_LENGTH - 1] = '\0';
            log->count++;
        }
    }
    
    return log->count > 0;
}

int main() {
    LogData log;
    
    if (!read_log_file(&log)) {
        printf("No valid log data entered.\n");
        return 1;
    }
    
    printf("\nProcessing log with line counter:\n");
    process_log(&log, count_lines);
    
    printf("\nSearching for errors:\n");
    process_log(&log, find_errors);
    
    printf("\nAnalyzing timestamps:\n");
    process_log(&log, analyze_timestamps);
    
    printf("\nLog analysis complete. Processed %d lines.\n", log.count);
    
    return 0;
}