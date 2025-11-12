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

void analyze_timestamps(const char* line) {
    const char* timestamp_start = strchr(line, '[');
    if (timestamp_start != NULL) {
        const char* timestamp_end = strchr(timestamp_start, ']');
        if (timestamp_end != NULL && timestamp_end > timestamp_start) {
            printf("Timestamp found: ");
            for (const char* p = timestamp_start; p <= timestamp_end && *p != '\0'; p++) {
                putchar(*p);
            }
            putchar('\n');
        }
    }
}

int validate_line(const char* line) {
    if (line == NULL) return 0;
    size_t len = strlen(line);
    if (len == 0 || len >= MAX_LINE_LENGTH - 1) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)line[i]) && line[i] != '\n' && line[i] != '\t') {
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
        "[2024-01-15 10:30:45] INFO: System started",
        "[2024-01-15 10:31:22] ERROR: Database connection failed",
        "[2024-01-15 10:32:10] WARNING: High memory usage",
        "[2024-01-15 10:33:05] INFO: User login successful",
        "[2024-01-15 10:34:18] error: File not found",
        "[2024-01-15 10:35:42] INFO: Backup completed"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    data->count = (sample_count < MAX_LINES) ? sample_count : MAX_LINES;
    
    for (int i = 0; i < data->count; i++) {
        size_t len = strlen(sample_logs[i]);
        if (len < MAX_LINE_LENGTH) {
            strncpy(data->lines[i], sample_logs[i], len);
            data->lines[i][len] = '\0';
        } else {
            return 0;
        }
    }
    
    return 1;
}

int main() {
    LogData log_data;
    
    if (!read_log_data(&log_data)) {
        fprintf(stderr, "Failed to initialize log data\n");
        return 1;
    }
    
    printf("=== Line Counter Analysis ===\n");
    process_log(&log_data, count_lines);
    
    printf("\n=== Error Detection ===\n");
    process_log(&log_data, find_errors);
    
    printf("\n=== Timestamp Analysis ===\n");
    process_log(&log_data, analyze_timestamps);
    
    return 0;
}