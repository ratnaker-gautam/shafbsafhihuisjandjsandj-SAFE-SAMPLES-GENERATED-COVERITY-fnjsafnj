//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    int total_warnings = 0;
    char line_buffer[MAX_LINE_LEN];
    
    const char* sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 WARNING High memory usage detected",
        "2024-01-15T10:32:00 ERROR Database connection failed",
        "2024-01-15T10:33:45 INFO Backup completed",
        "2024-01-15T10:34:20 ERROR File not found: config.xml",
        "2024-01-15T10:35:10 WARNING CPU temperature above threshold",
        "2024-01-15T10:36:30 INFO User login: john_doe",
        "2024-01-15T10:37:05 ERROR Permission denied for user alice",
        "2024-01-15T10:38:15 INFO Cache cleared",
        "2024-01-15T10:39:00 WARNING Network latency increased"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && entry_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            if (strcmp(entries[entry_count].level, "ERROR") == 0) {
                total_errors++;
            } else if (strcmp(entries[entry_count].level, "WARNING") == 0) {
                total_warnings++;
            }
            entry_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Total errors: %d\n", total_errors);
    printf("Total warnings: %d\n", total_warnings);
    printf("Error rate: %.2f%%\n", entry_count > 0 ? (total_errors * 100.0) / entry_count : 0.0);
    
    printf("\nError Details:\n");
    printf("==============\n");
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].error_count > 0) {
            printf("Time: %s | Message: %s\n", entries[i].timestamp, entries[i].message);
        }
    }
    
    printf("\nWarning Details:\n");
    printf("================\n");
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "WARNING") == 0) {
            printf("Time: %s | Message: %s\n", entries[i].timestamp, entries[i].message);
        }
    }
    
    int longest_message_index = -1;
    size_t max_length = 0;
    for (int i = 0; i < entry_count; i++) {
        size_t len = strlen(entries[i].message);
        if (len > max_length) {
            max_length = len;
            longest_message_index = i;
        }
    }
    
    if (longest_message_index >= 0) {
        printf("\nLongest log message (%zu chars): %s\n", max_length, entries[longest_message_index].message);
    }
    
    char most_common_level[16] = "NONE";
    int info_count = 0, warning_count = 0, error_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;