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
    
    while (fgets(buffer, sizeof(buffer), file) != NULL && log_data->count < MAX_LINES) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > 0) {
            strncpy(log_data->lines[log_data->count], buffer, MAX_LINE_LENGTH - 1);
            log_data->lines[log_data->count][MAX_LINE_LENGTH - 1] = '\0';
            log_data->count++;
        }
    }
    
    fclose(file);
    return 0;
}

void process_log(LogData* log_data, LogProcessor processor) {
    if (log_data == NULL || processor == NULL) {
        return;
    }
    
    for (int i = 0; i < log_data->count; i++) {
        processor(log_data->lines[i]);
    }
}

int main() {
    LogData log_data;
    const char* filename = "sample.log";
    
    printf("Log Analyzer\n");
    printf("============\n");
    
    if (read_log_file(filename, &log_data) != 0) {
        printf("Could not read log file: %s\n", filename);
        printf("Creating sample log data...\n");
        
        log_data.count = 5;
        strcpy(log_data.lines[0], "2023-10-01 10:30:15 INFO: System started");
        strcpy(log_data.lines[1], "2023-10-01 10:31:22 ERROR: Database connection failed");
        strcpy(log_data.lines[2], "2023-10-01 10:32:45 WARNING: High memory usage");
        strcpy(log_data.lines[3], "2023-10-01 10:33:10 INFO: User login successful");
        strcpy(log_data.lines[4], "2023-10-01 10:34:05 ERROR: File not found");
    }
    
    printf("\n1. Counting all lines:\n");
    process_log(&log_data, count_lines);
    
    printf("\n2. Finding errors:\n");
    process_log(&log_data, find_errors);
    
    printf("\n3. Extracting timestamps:\n");
    process_log(&log_data, extract_timestamps);
    
    printf("\nAnalysis complete. Processed %d log entries.\n", log_data.count);
    
    return 0;
}