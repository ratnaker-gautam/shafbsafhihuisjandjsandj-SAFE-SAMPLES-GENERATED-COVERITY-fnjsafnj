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
    return data->count > 0;
}

void print_menu() {
    printf("\nLog Analyzer Menu:\n");
    printf("1. Count all lines\n");
    printf("2. Find ERROR messages\n");
    printf("3. Find WARNING messages\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

int get_valid_choice() {
    char input[100];
    if (fgets(input, sizeof(input), stdin) == NULL) return -1;
    
    char* endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') return -1;
    if (choice < 1 || choice > 4) return -1;
    
    return (int)choice;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    LogData log_data;
    if (!read_log_file(&log_data, argv[1])) {
        printf("Error: Could not read log file '%s'\n", argv[1]);
        return 1;
    }
    
    printf("Loaded %d log entries from '%s'\n", log_data.count, argv[1]);
    
    LogProcessor processors[] = {count_lines, find_errors, find_warnings};
    const char* processor_names[] = {"Count lines", "Find ERRORs", "Find WARNINGs"};
    
    while (1) {
        print_menu();
        int choice = get_valid_choice();
        
        if (choice == -1) {
            printf("Invalid choice. Please enter a number between 1 and 4.\n");
            continue;
        }
        
        if (choice == 4) {
            printf("Exiting log analyzer.\n");
            break;
        }
        
        printf("\nRunning: %s\n", processor_names[choice - 1]);
        process_log(&log_data, processors[choice - 1]);
    }
    
    return 0;
}