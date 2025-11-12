//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp_start = strchr(temp_line, '[');
    char* timestamp_end = strchr(temp_line, ']');
    
    if (timestamp_start == NULL || timestamp_end == NULL || timestamp_end <= timestamp_start) {
        return 0;
    }
    
    size_t timestamp_len = timestamp_end - timestamp_start - 1;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    
    strncpy(entry->timestamp, timestamp_start + 1, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    char* level_start = timestamp_end + 1;
    while (*level_start == ' ') level_start++;
    
    char* level_end = strchr(level_start, ':');
    if (level_end == NULL) return 0;
    
    size_t level_len = level_end - level_start;
    if (level_len >= sizeof(entry->level)) return 0;
    
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    char* message_start = level_end + 1;
    while (*message_start == ' ') message_start++;
    
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int count_log_level(const struct LogEntry* entries, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void print_summary(const struct LogEntry* entries, int count) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", count_log_level(entries, count, "INFO"));
    printf("WARNING entries: %d\n", count_log_level(entries, count, "WARNING"));
    printf("ERROR entries: %d\n", count_log_level(entries, count, "ERROR"));
    printf("DEBUG entries: %d\n", count_log_level(entries, count, "DEBUG"));
}

void print_entries_by_level(const struct LogEntry* entries, int count, const char* level) {
    printf("\n%s entries:\n", level);
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    const char* sample_logs[] = {
        "[2024-01-15 10:30:25] INFO: System startup completed",
        "[2024-01-15 10:31:10] DEBUG: Memory usage: 45%",
        "[2024-01-15 10:32:45] WARNING: High CPU usage detected",
        "[2024-01-15 10:33:20] INFO: User login successful",
        "[2024-01-15 10:34:15] ERROR: Database connection failed",
        "[2024-01-15 10:35:00] INFO: Backup process started",
        "[2024-01-15 10:35:45] DEBUG: Network latency: 120ms",
        "[2024-01-15 10:36:30] WARNING: Disk space below 20%",
        "[2024-01-15 10:37:15] INFO: Cache cleared successfully",
        "[2024-01-15 10:38:00] ERROR: File not found: config.xml"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && entry_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            entry_count++;
        }
    }
    
    printf("Log Analyzer\n");
    printf("============\n\n");
    
    print_summary(entries, entry_count);
    
    printf("\nChoose an option:\n");
    printf("1. Show all INFO entries\n");
    printf("2. Show all WARNING entries\n");
    printf("3. Show all ERROR entries\n");