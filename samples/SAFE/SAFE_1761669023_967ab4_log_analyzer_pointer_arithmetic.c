//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
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

int parse_severity(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int read_log_file(struct LogEntry* logs, size_t max_logs) {
    char line[MAX_LINE_LENGTH];
    size_t count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_logs) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char* ptr = line;
        char* end = line + strlen(line);
        
        struct LogEntry* entry = logs + count;
        
        char* timestamp_start = ptr;
        while (ptr < end && *ptr != ' ') ptr++;
        if (ptr >= end) continue;
        size_t timestamp_len = ptr - timestamp_start;
        if (timestamp_len >= sizeof(entry->timestamp)) timestamp_len = sizeof(entry->timestamp) - 1;
        memcpy(entry->timestamp, timestamp_start, timestamp_len);
        entry->timestamp[timestamp_len] = '\0';
        
        ptr++;
        char* level_start = ptr;
        while (ptr < end && *ptr != ' ') ptr++;
        if (ptr >= end) continue;
        size_t level_len = ptr - level_start;
        if (level_len >= sizeof(entry->level)) level_len = sizeof(entry->level) - 1;
        memcpy(entry->level, level_start, level_len);
        entry->level[level_len] = '\0';
        
        ptr++;
        char* message_start = ptr;
        while (ptr < end && *ptr != '\n' && *ptr != '\r') ptr++;
        size_t message_len = ptr - message_start;
        if (message_len >= sizeof(entry->message)) message_len = sizeof(entry->message) - 1;
        memcpy(entry->message, message_start, message_len);
        entry->message[message_len] = '\0';
        
        entry->severity = parse_severity(entry->level);
        if (entry->severity >= 0) {
            count++;
        }
    }
    
    return count;
}

void analyze_logs(struct LogEntry* logs, size_t count) {
    int severity_count[5] = {0};
    int total_severity = 0;
    
    for (size_t i = 0; i < count; i++) {
        struct LogEntry* entry = logs + i;
        if (entry->severity >= 0 && entry->severity < 5) {
            severity_count[entry->severity]++;
            total_severity += entry->severity;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", count);
    printf("DEBUG: %d\n", severity_count[0]);
    printf("INFO: %d\n", severity_count[1]);
    printf("WARNING: %d\n", severity_count[2]);
    printf("ERROR: %d\n", severity_count[3]);
    printf("CRITICAL: %d\n", severity_count[4]);
    
    if (count > 0) {
        double avg_severity = (double)total_severity / count;
        printf("Average severity: %.2f\n", avg_severity);
    }
}

void find_most_severe(struct LogEntry* logs, size_t count) {
    if (count == 0) {
        printf("No logs to analyze.\n");
        return;
    }
    
    struct LogEntry* most_severe = logs;
    for (struct LogEntry* ptr = logs + 1; ptr < logs + count; ptr++) {
        if (ptr->severity > most_severe->severity) {
            most_severe = ptr;
        }
    }
    
    printf("Most severe entry:\n");
    printf("Timestamp: %s\n", most_severe->timestamp);
    printf("Level: %s\n", most_severe->level);
    printf("Message: %s\n", most_severe->message);
}

int main(void) {
    struct LogEntry logs[MAX_LINES];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input.\n");
    
    int count = read_log_file(logs, MAX_LINES);
    
    if (count <= 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(logs, count);
    find_most_severe(logs, count);
    
    return 0;
}