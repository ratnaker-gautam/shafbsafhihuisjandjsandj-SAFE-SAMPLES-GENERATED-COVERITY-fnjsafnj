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
        printf("Found ERROR: %s", line);
    }
}

void find_warnings(const char* line) {
    if (strstr(line, "WARNING") != NULL) {
        printf("Found WARNING: %s", line);
    }
}

void count_keyword(const char* line) {
    static int keyword_count = 0;
    if (strstr(line, "user") != NULL) {
        keyword_count++;
        printf("Found 'user' keyword %d times\n", keyword_count);
    }
}

int read_log_file(const char* filename, LogData* log_data) {
    if (filename == NULL || log_data == NULL) {
        return -1;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return -1;
    }
    
    log_data->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (log_data->count >= MAX_LINES) {
            printf("Warning: Reached maximum line limit\n");
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        strncpy(log_data->lines[log_data->count], buffer, MAX_LINE_LENGTH - 1);
        log_data->lines[log_data->count][MAX_LINE_LENGTH - 1] = '\0';
        log_data->count++;
    }
    
    if (ferror(file)) {
        fclose(file);
        return -1;
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
    char filename[256];
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Error: Empty filename\n");
        return 1;
    }
    
    if (read_log_file(filename, &log_data) != 0) {
        return 1;
    }
    
    if (log_data.count == 0) {
        printf("No log data found\n");
        return 0;
    }
    
    printf("\nProcessing log with line counter:\n");
    process_log(&log_data, count_lines);
    
    printf("\nProcessing log for errors:\n");
    process_log(&log_data, find_errors);
    
    printf("\nProcessing log for warnings:\n");
    process_log(&log_data, find_warnings);
    
    printf("\nProcessing log for 'user' keyword:\n");
    process_log(&log_data, count_keyword);
    
    printf("\nLog analysis complete. Processed %d lines.\n", log_data.count);
    
    return 0;
}