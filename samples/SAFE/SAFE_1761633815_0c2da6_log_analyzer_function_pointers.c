//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LEN];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_errors(LogEntry* entry) {
    static int error_count = 0;
    if (strcmp(entry->level, "ERROR") == 0) {
        error_count++;
        printf("Error count: %d\n", error_count);
    }
}

void print_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("Warning: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_timestamps(LogEntry* entry) {
    static time_t first_time = 0;
    static time_t last_time = 0;
    
    struct tm tm = {0};
    if (strptime(entry->timestamp, "%Y-%m-%d %H:%M:%S", &tm) != NULL) {
        time_t current = mktime(&tm);
        if (current != -1) {
            if (first_time == 0) first_time = current;
            last_time = current;
            
            if (first_time != 0 && last_time != 0) {
                double duration = difftime(last_time, first_time);
                printf("Log duration: %.0f seconds\n", duration);
            }
        }
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, MAX_LINE_LEN - 1);
    entry->message[MAX_LINE_LEN - 1] = '\0';
    
    return 1;
}

void process_log_file(const char* filename, LogProcessor* processors, int num_processors) {
    if (filename == NULL || processors == NULL || num_processors <= 0) return;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return;
    }
    
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    while (fgets(line, MAX_LINE_LEN, file) != NULL && line_count < MAX_ENTRIES) {
        line_count++;
        line[MAX_LINE_LEN - 1] = '\0';
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            for (int i = 0; i < num_processors; i++) {
                if (processors[i] != NULL) {
                    processors[i](&entry);
                }
            }
        }
    }
    
    if (fclose(file) != 0) {
        printf("Error closing file\n");
    }
}

int main() {
    printf("Log Analyzer - Processing sample log data\n\n");
    
    const char* sample_logs[] = {
        "2024-01-15 10:30:00 INFO System started successfully",
        "2024-01-15 10:31:15 WARNING High memory usage detected",
        "2024-01-15 10:32:30 ERROR Database connection failed",
        "2024-01-15 10:33:45 INFO Backup completed",
        "2024-01-15 10:34:20 ERROR File not found: config.xml",
        "2024-01-15 10:35:10 WARNING CPU temperature rising"
    };
    
    FILE* temp_file = tmpfile();
    if (temp_file == NULL) {
        printf("Cannot create temporary file\n");
        return 1;
    }
    
    for (int i = 0; i < 6; i++) {
        fprintf(temp_file, "%s\n", sample_logs[i]);
    }
    
    rewind(temp_file);
    
    LogProcessor processors[] = {count_errors, print_warnings, process_timestamps};
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    process_log_file("sample.log", processors, num_processors);
    
    fclose(temp_file);
    
    printf("\nAnalysis complete.\n");
    return 0;
}