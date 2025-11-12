//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LINES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
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
    memcpy(entry->timestamp, timestamp_start, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *level_start = ptr;
    while (ptr < end && *ptr != ' ') ptr++;
    if (ptr >= end) return 0;
    size_t level_len = ptr - level_start;
    if (level_len >= sizeof(entry->level)) return 0;
    memcpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *msg_start = ptr;
    while (ptr < end && *ptr != '\n' && *ptr != '\r') ptr++;
    size_t msg_len = ptr - msg_start;
    if (msg_len >= sizeof(entry->message)) return 0;
    memcpy(entry->message, msg_start, msg_len);
    entry->message[msg_len] = '\0';
    
    if (strcmp(entry->level, "ERROR") == 0) entry->severity = 3;
    else if (strcmp(entry->level, "WARN") == 0) entry->severity = 2;
    else if (strcmp(entry->level, "INFO") == 0) entry->severity = 1;
    else entry->severity = 0;
    
    return 1;
}

int main(void) {
    struct LogEntry logs[MAX_LINES];
    int log_count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    printf("Enter log lines (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter empty line to finish.\n");
    
    while (log_count < MAX_LINES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';
        if (buffer[0] == '\0') break;
        
        if (parse_log_line(buffer, &logs[log_count])) {
            log_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    
    struct LogEntry *log_ptr = logs;
    struct LogEntry *log_end = logs + log_count;
    
    while (log_ptr < log_end) {
        if (log_ptr->severity == 3) error_count++;
        else if (log_ptr->severity == 2) warn_count++;
        else if (log_ptr->severity == 1) info_count++;
        log_ptr++;
    }
    
    printf("Total entries: %d\n", log_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARN entries: %d\n", warn_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent ERROR entries:\n");
    log_ptr = logs;
    int error_shown = 0;
    while (log_ptr < log_end && error_shown < 5) {
        if (log_ptr->severity == 3) {
            printf("%s: %s\n", log_ptr->timestamp, log_ptr->message);
            error_shown++;
        }
        log_ptr++;
    }
    
    if (error_shown == 0) {
        printf("No ERROR entries found.\n");
    }
    
    return 0;
}