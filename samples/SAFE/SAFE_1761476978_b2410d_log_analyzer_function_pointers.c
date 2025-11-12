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
    if (strstr(line, "ERROR") != NULL) {
        printf("Found ERROR: %s\n", line);
    }
}

void find_warnings(const char* line) {
    if (strstr(line, "WARNING") != NULL) {
        printf("Found WARNING: %s\n", line);
    }
}

void process_log(LogData* data, LogProcessor processor) {
    if (data == NULL || processor == NULL) return;
    
    for (int i = 0; i < data->count; i++) {
        processor(data->lines[i]);
    }
}

int read_log_file(LogData* data, const char* filename) {
    if (data == NULL || filename == NULL) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    data->count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (fgets(buffer, sizeof(buffer), file) != NULL && data->count < MAX_LINES) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        if (len > 0) {
            strncpy(data->lines[data->count], buffer, MAX_LINE_LENGTH - 1);
            data->lines[data->count][MAX_LINE_LENGTH - 1] = '\0';
            data->count++;
        }
    }
    
    fclose(file);
    return data->count > 0;
}

void print_menu() {
    printf("\nLog Analyzer Menu:\n");
    printf("1. Count all lines\n");
    printf("2. Find ERROR messages\n");
    printf("3. Find WARNING messages\n");
    printf("4. Exit\n");
    printf("Choose an option: ");
}

int get_menu_choice() {
    char input[100];
    if (fgets(input, sizeof(input), stdin) == NULL) return -1;
    
    char* endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') return -1;
    if (choice < 1 || choice > 4) return -1;
    
    return (int)choice;
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
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    if (!read_log_file(&log_data, filename)) {
        printf("Error reading log file: %s\n", filename);
        return 1;
    }
    
    printf("Successfully loaded %d log entries\n", log_data.count);
    
    LogProcessor processors[] = {count_lines, find_errors, find_warnings};
    const char* processor_names[] = {"Line counter", "Error finder", "Warning finder"};
    
    while (1) {
        print_menu();
        int choice = get_menu_choice();
        
        if (choice == -1) {
            printf("Invalid choice. Please try again.\n");
            continue;
        }
        
        if (choice == 4) {
            printf("Exiting...\n");
            break;
        }
        
        printf("\nRunning %s:\n", processor_names[choice - 1]);
        process_log(&log_data, processors[choice - 1]);
    }
    
    return 0;
}