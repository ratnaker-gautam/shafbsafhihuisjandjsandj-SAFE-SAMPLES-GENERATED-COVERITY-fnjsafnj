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
    for (int i = 0; line[i] != '\0'; i++) {
        if (!isprint(line[i]) && line[i] != '\t' && line[i] != '\n') {
            return 0;
        }
    }
    return 1;
}

int read_log_file(const char* filename, LogData* log_data) {
    if (filename == NULL || log_data == NULL) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    log_data->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (fgets(buffer, sizeof(buffer), file) != NULL && log_data->count < MAX_LINES) {
        if (validate_line(buffer)) {
            strncpy(log_data->lines[log_data->count], buffer, MAX_LINE_LENGTH - 1);
            log_data->lines[log_data->count][MAX_LINE_LENGTH - 1] = '\0';
            log_data->count++;
        }
    }
    
    fclose(file);
    return 1;
}

void process_log(LogData* log_data, LogProcessor processor) {
    if (log_data == NULL || processor == NULL) return;
    
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
        printf("Error: Could not read log file '%s'\n", argv[1]);
        return 1;
    }
    
    if (log_data.count == 0) {
        printf("No valid log lines found in '%s'\n", argv[1]);
        return 1;
    }
    
    printf("Processing %d log lines...\n\n", log_data.count);
    
    LogProcessor processors[] = {count_lines, find_errors, analyze_length};
    const char* processor_names[] = {"Line Counter", "Error Finder", "Length Analyzer"};
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    for (int i = 0; i < num_processors; i++) {
        printf("=== %s ===\n", processor_names[i]);
        process_log(&log_data, processors[i]);
        printf("\n");
    }
    
    return 0;
}