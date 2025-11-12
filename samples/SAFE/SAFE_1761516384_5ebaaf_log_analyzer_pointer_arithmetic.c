//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_LINES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    char *end = ptr + strlen(line);
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *timestamp_start = ptr;
    while (ptr < end && *ptr != ' ') ptr++;
    if (ptr >= end) return 0;
    
    size_t timestamp_len = ptr - timestamp_start;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    strncpy(entry->timestamp, timestamp_start, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *level_start = ptr;
    while (ptr < end && *ptr != ' ') ptr++;
    if (ptr >= end) return 0;
    
    size_t level_len = ptr - level_start;
    if (level_len >= sizeof(entry->level)) return 0;
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *message_start = ptr;
    char *message_end = end;
    while (message_end > message_start && isspace(*(message_end - 1))) message_end--;
    
    size_t message_len = message_end - message_start;
    if (message_len >= sizeof(entry->message)) return 0;
    strncpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry logs[MAX_LINES];
    int log_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Press Ctrl+D (Linux) or Ctrl+Z (Windows) to finish.\n");
    
    while (log_count < MAX_LINES && fgets(line, sizeof(line), stdin) != NULL) {
        char *newline = strchr(line, '\n');
        if (newline != NULL) *newline = '\0';
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            logs[log_count] = entry;
            log_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < log_count; i++) {
        struct LogEntry *entry = logs + i;
        
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Total entries: %d\n", log_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent ERROR entries:\n");
    int error_displayed = 0;
    for (int i = log_count - 1; i >= 0 && error_displayed < 5; i--) {
        struct LogEntry *entry = logs + i;
        if (strcmp(entry->level, "ERROR") == 0) {
            printf("%s: %s\n", entry->timestamp, entry->message);
            error_displayed++;
        }
    }
    
    if (error_displayed == 0) {
        printf("No ERROR entries found.\n");
    }
    
    return 0;
}