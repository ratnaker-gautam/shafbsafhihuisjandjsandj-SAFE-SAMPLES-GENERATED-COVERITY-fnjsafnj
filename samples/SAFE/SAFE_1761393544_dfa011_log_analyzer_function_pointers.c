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
    printf("Total lines: %d\n", data->count);
}

void find_errors(LogData* data) {
    int error_count = 0;
    for (int i = 0; i < data->count; i++) {
        if (strstr(data->lines[i], "ERROR") != NULL) {
            error_count++;
        }
    }
    printf("Lines containing 'ERROR': %d\n", error_count);
}

void find_warnings(LogData* data) {
    int warning_count = 0;
    for (int i = 0; i < data->count; i++) {
        if (strstr(data->lines[i], "WARNING") != NULL) {
            warning_count++;
        }
    }
    printf("Lines containing 'WARNING': %d\n", warning_count);
}

void print_all(LogData* data) {
    for (int i = 0; i < data->count; i++) {
        printf("%s", data->lines[i]);
    }
}

int read_log_file(const char* filename, LogData* data) {
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
        if (len < sizeof(data->lines[0])) {
            strcpy(data->lines[data->count], buffer);
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
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    LogData log_data;
    if (!read_log_file(argv[1], &log_data)) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
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
        "Line count",
        "Error search",
        "Warning search",
        "Print all"
    };
    
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("Log Analysis Menu:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, names[i]);
    }
    printf("Enter choice (1-%d): ", num_processors);
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char* endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n' || choice < 1 || choice > num_processors) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }
    
    process_log(&log_data, processors[choice - 1]);
    
    return 0;
}