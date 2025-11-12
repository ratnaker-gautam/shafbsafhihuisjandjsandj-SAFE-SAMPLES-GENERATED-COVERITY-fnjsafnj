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
        return -1;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    
    log_data->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (log_data->count < MAX_LINES && fgets(buffer, sizeof(buffer), file) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (len < sizeof(log_data->lines[0])) {
            strncpy(log_data->lines[log_data->count], buffer, sizeof(log_data->lines[0]) - 1);
            log_data->lines[log_data->count][sizeof(log_data->lines[0]) - 1] = '\0';
            log_data->count++;
        }
    }
    
    fclose(file);
    return 0;
}

void process_log_data(LogData* log_data, LogProcessor processor) {
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
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Invalid filename\n");
        return 1;
    }
    
    if (read_log_file(filename, &log_data) != 0) {
        fprintf(stderr, "Error reading log file\n");
        return 1;
    }
    
    if (log_data.count == 0) {
        printf("No log entries found\n");
        return 0;
    }
    
    LogProcessor processors[] = {count_lines, find_errors, analyze_length};
    const char* processor_names[] = {"Line Counter", "Error Finder", "Length Analyzer"};
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("\nProcessing log with %d entries:\n", log_data.count);
    
    for (int i = 0; i < num_processors; i++) {
        printf("\n--- %s ---\n", processor_names[i]);
        process_log_data(&log_data, processors[i]);
    }
    
    printf("\nAnalysis complete. Processed %d log entries.\n", log_data.count);
    return 0;
}