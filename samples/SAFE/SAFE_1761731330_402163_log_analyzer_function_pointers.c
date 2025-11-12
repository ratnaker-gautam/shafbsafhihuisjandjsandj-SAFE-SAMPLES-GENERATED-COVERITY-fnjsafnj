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

typedef void (*LogProcessor)(LogData*);

void count_lines(LogData* data) {
    printf("Total log entries: %d\n", data->count);
}

void find_errors(LogData* data) {
    int error_count = 0;
    for (int i = 0; i < data->count; i++) {
        if (strstr(data->lines[i], "ERROR") != NULL) {
            error_count++;
        }
    }
    printf("Error entries found: %d\n", error_count);
}

void find_warnings(LogData* data) {
    int warning_count = 0;
    for (int i = 0; i < data->count; i++) {
        if (strstr(data->lines[i], "WARNING") != NULL) {
            warning_count++;
        }
    }
    printf("Warning entries found: %d\n", warning_count);
}

void print_all(LogData* data) {
    for (int i = 0; i < data->count; i++) {
        printf("%s\n", data->lines[i]);
    }
}

int read_log_file(LogData* data, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return 0;
    }
    
    data->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (fgets(buffer, sizeof(buffer), file) != NULL && data->count < MAX_LINES) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (len > 0) {
            strncpy(data->lines[data->count], buffer, MAX_LINE_LENGTH - 1);
            data->lines[data->count][MAX_LINE_LENGTH - 1] = '\0';
            data->count++;
        }
    }
    
    fclose(file);
    return 1;
}

void process_log(LogData* data, LogProcessor processor) {
    if (data && processor) {
        processor(data);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    LogData log_data;
    if (!read_log_file(&log_data, argv[1])) {
        printf("Error: Could not open or read file '%s'\n", argv[1]);
        return 1;
    }
    
    if (log_data.count == 0) {
        printf("No log entries found in file '%s'\n", argv[1]);
        return 1;
    }
    
    LogProcessor processors[] = {
        count_lines,
        find_errors,
        find_warnings,
        print_all
    };
    
    const char* processor_names[] = {
        "Count lines",
        "Find errors",
        "Find warnings",
        "Print all entries"
    };
    
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("Available log analysis functions:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, processor_names[i]);
    }
    
    printf("Enter your choice (1-%d): ", num_processors);
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    char* endptr;
    long choice = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\n') {
        printf("Invalid input: please enter a number\n");
        return 1;
    }
    
    if (choice < 1 || choice > num_processors) {
        printf("Invalid choice: must be between 1 and %d\n", num_processors);
        return 1;
    }
    
    process_log(&log_data, processors[choice - 1]);
    
    return 0;
}