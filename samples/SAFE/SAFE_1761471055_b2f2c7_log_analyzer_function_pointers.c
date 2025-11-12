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
    printf("Processed %d lines\n", line_count);
}

void find_errors(const char* line) {
    if (strstr(line, "ERROR") != NULL || strstr(line, "error") != NULL) {
        printf("Found error: %s\n", line);
    }
}

void find_warnings(const char* line) {
    if (strstr(line, "WARNING") != NULL || strstr(line, "warning") != NULL) {
        printf("Found warning: %s\n", line);
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
        return 0;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return 0;
    }
    
    log_data->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (fgets(buffer, sizeof(buffer), file) != NULL && log_data->count < MAX_LINES) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (len - 1 < sizeof(log_data->lines[0])) {
            strncpy(log_data->lines[log_data->count], buffer, sizeof(log_data->lines[0]) - 1);
            log_data->lines[log_data->count][sizeof(log_data->lines[0]) - 1] = '\0';
            log_data->count++;
        }
    }
    
    fclose(file);
    return 1;
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
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    if (!read_log_file(filename, &log_data)) {
        return 1;
    }
    
    if (log_data.count == 0) {
        printf("No log data found\n");
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