//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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
    if (timestamp_start == NULL) return 0;
    
    char* timestamp_end = strchr(timestamp_start, ']');
    if (timestamp_end == NULL) return 0;
    
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
    
    char* newline = strchr(entry->message, '\n');
    if (newline != NULL) *newline = '\0';
    
    return 1;
}

int count_log_level(const struct LogEntry* entries, int count, const char* level) {
    if (entries == NULL || level == NULL || count <= 0) return 0;
    
    int level_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            level_count++;
        }
    }
    return level_count;
}

void print_statistics(const struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    printf("Log Analysis Statistics:\n");
    printf("Total entries: %d\n", count);
    
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    int level_counts[4] = {0};
    
    for (int i = 0; i < 4; i++) {
        level_counts[i] = count_log_level(entries, count, levels[i]);
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
    
    int other_count = count;
    for (int i = 0; i < 4; i++) {
        other_count -= level_counts[i];
    }
    if (other_count > 0) {
        printf("OTHER: %d\n", other_count);
    }
}

void print_recent_entries(const struct LogEntry* entries, int count, int num_recent) {
    if (entries == NULL || count <= 0 || num_recent <= 0) return;
    
    if (num_recent > count) num_recent = count;
    
    printf("\nMost recent %d entries:\n", num_recent);
    int start_index = count - num_recent;
    if (start_index < 0) start_index = 0;
    
    for (int i = start_index; i < count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int read_sample_logs(struct LogEntry* entries) {
    if (entries == NULL) return 0;
    
    const char* sample_logs[] = {
        "[2024-01-15 10:30:15] INFO: Application started successfully",
        "[2024-01-15 10:30:20] DEBUG: Initializing modules",
        "[2024-01-15 10:30:25] INFO: Database connection established",
        "[2024-01-15 10:31:10] WARNING: High memory usage detected",
        "[2024-01-15 10:31:45] ERROR: Failed to load configuration file",
        "[2024-01-15 10:32:00] INFO: User login successful",
        "[2024-01-15 10:32:30] DEBUG: Processing request ID 12345",
        "[2024-01-15 10:33:15] WARNING: Slow response time detected",
        "[2024-01-15 10:33:45] INFO: Cache cleared successfully",
        "[2024-