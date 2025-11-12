//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
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
            char timestamp[9];
            strncpy(timestamp, ptr, 8);
            timestamp[8] = '\0';
            printf("Found timestamp: %s\n", timestamp);
            break;
        }
        ptr++;
    }
}

int validate_line(const char* line) {
    if (line == NULL) return 0;
    size_t len = strlen(line);
    if (len == 0 || len >= MAX_LINE_LENGTH) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint(line[i]) && line[i] != '\t' && line[i] != '\n') {
            return 0;
        }
    }
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
        "2023-10-15 14:30:25 INFO System started successfully",
        "2023-10-15 14:31:10 ERROR Database connection failed",
        "2023-10-15 14:32:45 WARNING High memory usage detected",
        "2023-10-15 14:33:20 INFO User login: john_doe",
        "2023-10-15 14:34:05 ERROR File not found: config.xml",
        "2023-10-15 14:35:30 INFO Backup completed",
        "2023-10-15 14:36:15 WARNING CPU temperature high",
        "2023-10-15 14:37:40 INFO New device connected",
        "2023-10-15 14:38:25 error Network timeout",
        "2023-10-15 14:39:10 INFO System shutdown initiated"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    if (sample_count > MAX_LINES) {
        sample_count = MAX_LINES;
    }
    
    data->count = 0;
    for (int i = 0; i < sample_count; i++) {
        size_t len = strlen(sample_logs[i]);
        if (len < MAX_LINE_LENGTH) {
            strncpy(data->lines[data->count], sample_logs[i], len);
            data->lines[data->count][len] = '\0';
            data->count++;
        }
    }
    
    return data->count > 0;
}

int main() {
    LogData log_data;
    
    if (!read_log_data(&log_data)) {
        fprintf(stderr, "Failed to initialize log data\n");
        return 1;
    }
    
    printf("=== Processing log with line counter ===\n");
    process_log(&log_data, count_lines);
    
    printf("\n=== Searching for errors ===\n");
    process_log(&log_data, find_errors);
    
    printf("\n=== Extracting timestamps ===\n");
    process_log(&log_data, extract_timestamps);
    
    printf("\n=== Summary ===\n");
    printf("Total log entries processed: %d\n", log_data.count);
    
    int error_count = 0;
    for (int i = 0; i < log_data.count; i++) {
        if (strstr(log_data.lines[i], "ERROR") != NULL || 
            strstr(log_data.lines[i], "error") != NULL) {
            error_count++;
        }
    }
    printf("Total errors found: %d\n", error_count);
    
    return 0;
}