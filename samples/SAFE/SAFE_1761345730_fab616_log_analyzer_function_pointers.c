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

void extract_timestamps(const char* line) {
    const char* ptr = line;
    while (*ptr) {
        if (isdigit(*ptr) && isdigit(*(ptr+1)) && *(ptr+2) == ':' && 
            isdigit(*(ptr+3)) && isdigit(*(ptr+4)) && *(ptr+5) == ':') {
            printf("Found timestamp: ");
            for (int i = 0; i < 8 && *(ptr+i); i++) {
                putchar(*(ptr+i));
            }
            printf(" in line: %s\n", line);
            break;
        }
        ptr++;
    }
}

int validate_line(const char* line) {
    if (line == NULL) return 0;
    size_t len = strlen(line);
    if (len == 0 || len >= MAX_LINE_LENGTH - 1) return 0;
    return 1;
}

void process_log(LogData* data, LogProcessor processor) {
    if (data == NULL || processor == NULL) return;
    
    for (int i = 0; i < data->count; i++) {
        if (validate_line(data->lines[i])) {
            processor(data->lines[i]);
        }
    }
}

int read_log_data(LogData* data) {
    if (data == NULL) return 0;
    
    const char* sample_logs[] = {
        "2023-10-01 10:30:15 INFO System started successfully",
        "2023-10-01 10:31:22 ERROR Database connection failed",
        "2023-10-01 10:32:45 WARNING High memory usage detected",
        "2023-10-01 10:33:10 INFO User login: john_doe",
        "2023-10-01 10:34:30 ERROR File not found: config.xml",
        "2023-10-01 10:35:55 INFO Backup completed",
        "2023-10-01 10:36:20 error Network timeout occurred",
        "2023-10-01 10:37:45 INFO System shutdown initiated"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    data->count = (sample_count < MAX_LINES) ? sample_count : MAX_LINES;
    
    for (int i = 0; i < data->count; i++) {
        strncpy(data->lines[i], sample_logs[i], MAX_LINE_LENGTH - 1);
        data->lines[i][MAX_LINE_LENGTH - 1] = '\0';
    }
    
    return 1;
}

void display_menu() {
    printf("\nLog Analyzer Menu:\n");
    printf("1. Count all lines\n");
    printf("2. Find error messages\n");
    printf("3. Extract timestamps\n");
    printf("4. Exit\n");
    printf("Choose an option (1-4): ");
}

int get_user_choice() {
    char input[10];
    if (fgets(input, sizeof(input), stdin) == NULL) return -1;
    
    if (strlen(input) == 0 || input[0] == '\n') return -1;
    
    char* endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') return -1;
    if (choice < 1 || choice > 4) return -1;
    
    return (int)choice;
}

int main() {
    LogData log_data;
    LogProcessor processors[] = {count_lines, find_errors, extract_timestamps};
    
    if (!read_log_data(&log_data)) {
        fprintf(stderr, "Failed to initialize log data\n");
        return 1;
    }
    
    printf("Log Analyzer initialized with %d log entries\n", log_data.count);
    
    while (1) {
        display_menu();
        int choice = get_user_choice();
        
        if (choice == -1) {
            printf("Invalid input. Please try again.\n");
            continue;
        }
        
        if (choice == 4) {
            printf("Exiting Log Analyzer. Goodbye!\n");
            break;
        }
        
        if (choice >= 1 && choice <= 3) {
            process_log(&log_data, processors[choice - 1]);
        }
    }
    
    return 0;
}