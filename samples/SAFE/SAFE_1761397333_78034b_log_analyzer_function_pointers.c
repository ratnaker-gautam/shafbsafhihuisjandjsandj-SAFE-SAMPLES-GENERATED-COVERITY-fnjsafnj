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

void show_longest_line(LogData* data) {
    int max_length = 0;
    int max_index = 0;
    
    for (int i = 0; i < data->count; i++) {
        int len = strlen(data->lines[i]);
        if (len > max_length) {
            max_length = len;
            max_index = i;
        }
    }
    
    if (data->count > 0) {
        printf("Longest line (%d chars): %s\n", max_length, data->lines[max_index]);
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
        
        if (len - 1 < sizeof(data->lines[data->count])) {
            strncpy(data->lines[data->count], buffer, sizeof(data->lines[data->count]) - 1);
            data->lines[data->count][sizeof(data->lines[data->count]) - 1] = '\0';
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

int main() {
    LogData log_data;
    char filename[256];
    
    printf("Enter log filename: ");
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
    
    if (!read_log_file(filename, &log_data)) {
        fprintf(stderr, "Error reading file: %s\n", filename);
        return 1;
    }
    
    if (log_data.count == 0) {
        printf("No log data found\n");
        return 0;
    }
    
    LogProcessor processors[] = {
        count_lines,
        find_errors,
        find_warnings,
        show_longest_line
    };
    
    const char* processor_names[] = {
        "Line Count",
        "Error Search",
        "Warning Search",
        "Longest Line"
    };
    
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    for (int i = 0; i < num_processors; i++) {
        printf("%s: ", processor_names[i]);
        process_log(&log_data, processors[i]);
    }
    
    return 0;
}