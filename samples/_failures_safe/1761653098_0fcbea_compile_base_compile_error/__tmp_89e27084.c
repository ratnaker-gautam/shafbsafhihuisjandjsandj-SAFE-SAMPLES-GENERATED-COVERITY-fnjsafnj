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
    strncpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';
    
    return 1;
}

int count_log_level(const struct LogEntry* entries, int count, const char* level) {
    if (entries == NULL || level == NULL) return 0;
    
    int level_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            level_count++;
        }
    }
    return level_count;
}

void print_summary(const struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", count_log_level(entries, count, "INFO"));
    printf("WARNING entries: %d\n", count_log_level(entries, count, "WARNING"));
    printf("ERROR entries: %d\n", count_log_level(entries, count, "ERROR"));
    printf("DEBUG entries: %d\n", count_log_level(entries, count, "DEBUG"));
    
    printf("\nRecent entries:\n");
    int display_count = count < 5 ? count : 5;
    for (int i = 0; i < display_count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

void search_by_level(const struct LogEntry* entries, int count, const char* level) {
    if (entries == NULL || level == NULL || count <= 0) return;
    
    printf("\nEntries with level '%s':\n", level);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found++;
        }
    }
    if (found == 0) {
        printf("No entries found with level '%s'\n", level);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    const char* sample_logs[] = {
        "[2024-01-15 10:30:15] INFO: Application started successfully",
        "[2024-01-15 10:31:22] DEBUG: Loading configuration file",
        "[2024-01-15 10:32:45] WARNING: High memory usage detected",
        "[2024-01-15 10:33:10] INFO: User login successful",
        "[2024-01-15 10:34:55] ERROR: Database connection failed",
        "[2024-01-15 10:35:20] INFO: Retrying connection",
        "[2024-01-15 10:36:30] DEBUG: Connection pool initialized",
        "[2024-01-15 10:37:45] WARNING: Slow response time",
        "[2024-01-15 10:38:10] INFO: Request processed",
        "[2024-01-15 10:39:25] ERROR: File not found"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs