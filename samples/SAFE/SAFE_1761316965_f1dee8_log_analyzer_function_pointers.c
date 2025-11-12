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

typedef void (*LogProcessor)(LogData*);

void count_lines(LogData* data) {
    if (data == NULL) return;
    printf("Total lines: %d\n", data->count);
}

void find_errors(LogData* data) {
    if (data == NULL) return;
    int error_count = 0;
    for (int i = 0; i < data->count; i++) {
        if (strstr(data->lines[i], "ERROR") != NULL) {
            error_count++;
        }
    }
    printf("Error lines: %d\n", error_count);
}

void find_warnings(LogData* data) {
    if (data == NULL) return;
    int warning_count = 0;
    for (int i = 0; i < data->count; i++) {
        if (strstr(data->lines[i], "WARNING") != NULL) {
            warning_count++;
        }
    }
    printf("Warning lines: %d\n", warning_count);
}

void print_all(LogData* data) {
    if (data == NULL) return;
    for (int i = 0; i < data->count; i++) {
        printf("%s", data->lines[i]);
    }
}

int read_log_file(const char* filename, LogData* data) {
    if (filename == NULL || data == NULL) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    data->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (fgets(buffer, sizeof(buffer), file) != NULL && data->count < MAX_LINES) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        strncpy(data->lines[data->count], buffer, MAX_LINE_LENGTH-1);
        data->lines[data->count][MAX_LINE_LENGTH-1] = '\0';
        data->count++;
    }
    
    fclose(file);
    return 1;
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
        printf("Log file is empty\n");
        return 0;
    }
    
    LogProcessor processors[] = {
        count_lines,
        find_errors,
        find_warnings,
        print_all
    };
    
    const char* names[] = {
        "Line Count",
        "Error Search",
        "Warning Search",
        "Print All"
    };
    
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("Log Analysis Results for: %s\n", argv[1]);
    printf("========================\n");
    
    for (int i = 0; i < num_processors; i++) {
        printf("\n%s:\n", names[i]);
        printf("--------\n");
        processors[i](&log_data);
    }
    
    return 0;
}