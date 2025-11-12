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

void count_keyword(const char* line) {
    static int keyword_count = 0;
    if (strstr(line, "user") != NULL) {
        keyword_count++;
        printf("Found 'user' keyword %d times\n", keyword_count);
    }
}

int validate_line(const char* line) {
    if (line == NULL) return 0;
    size_t len = strlen(line);
    if (len == 0 || len >= MAX_LINE_LENGTH - 1) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)line[i]) && line[i] != '\n' && line[i] != '\t') {
            return 0;
        }
    }
    return 1;
}

int read_log_file(LogData* log_data) {
    if (log_data == NULL) return 0;
    
    printf("Enter log lines (empty line to finish, max %d lines):\n", MAX_LINES);
    
    char buffer[MAX_LINE_LENGTH];
    log_data->count = 0;
    
    while (log_data->count < MAX_LINES) {
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
        
        if (!validate_line(buffer)) {
            printf("Invalid line skipped\n");
            continue;
        }
        
        if (strlen(buffer) >= MAX_LINE_LENGTH - 1) {
            printf("Line too long, skipped\n");
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
    LogData log_data;
    
    if (!read_log_file(&log_data)) {
        printf("No valid log data to process\n");
        return 1;
    }
    
    printf("\nProcessing log with %d lines:\n", log_data.count);
    
    LogProcessor processors[] = {
        count_lines,
        find_errors,
        find_warnings,
        count_keyword
    };
    
    const char* processor_names[] = {
        "Line Counter",
        "Error Finder",
        "Warning Finder",
        "Keyword Counter"
    };
    
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    for (int i = 0; i < num_processors; i++) {
        printf("\n--- %s ---\n", processor_names[i]);
        process_log(&log_data, processors[i]);
    }
    
    printf("\nLog analysis complete\n");
    return 0;
}