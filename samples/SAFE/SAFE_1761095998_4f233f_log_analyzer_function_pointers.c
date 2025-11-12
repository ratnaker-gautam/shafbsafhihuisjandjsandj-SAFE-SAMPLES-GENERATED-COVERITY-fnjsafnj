//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
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
        printf("%s\n", data->lines[i]);
    }
}

int read_log_file(const char* filename, LogData* data) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }
    
    data->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (fgets(buffer, sizeof(buffer), file) != NULL && data->count < MAX_LINES) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        if (len < sizeof(data->lines[data->count])) {
            strncpy(data->lines[data->count], buffer, sizeof(data->lines[data->count]) - 1);
            data->lines[data->count][sizeof(data->lines[data->count]) - 1] = '\0';
            data->count++;
        }
    }
    
    fclose(file);
    return 1;
}

void process_log(LogData* data, LogProcessor processor) {
    if (data != NULL && processor != NULL) {
        processor(data);
    }
}

int main() {
    LogData log_data;
    char filename[100];
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
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
        printf("Could not open file: %s\n", filename);
        return 1;
    }
    
    if (log_data.count == 0) {
        printf("No log data found\n");
        return 1;
    }
    
    LogProcessor processors[] = {
        count_lines,
        find_errors,
        find_warnings,
        print_all
    };
    
    const char* options[] = {
        "Count total lines",
        "Find ERROR lines",
        "Find WARNING lines",
        "Print all lines"
    };
    
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("\nLog Analysis Options:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, options[i]);
    }
    
    printf("Enter choice (1-%d): ", num_processors);
    
    char input[10];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    int choice = atoi(input);
    if (choice < 1 || choice > num_processors) {
        printf("Invalid choice\n");
        return 1;
    }
    
    process_log(&log_data, processors[choice - 1]);
    
    return 0;
}