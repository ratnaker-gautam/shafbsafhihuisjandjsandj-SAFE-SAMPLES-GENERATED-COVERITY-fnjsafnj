//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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
    int severity;
};

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    char most_frequent_level[16];
    int max_severity;
};

void initialize_log_entry(struct LogEntry *entry) {
    memset(entry->timestamp, 0, sizeof(entry->timestamp));
    memset(entry->level, 0, sizeof(entry->level));
    memset(entry->message, 0, sizeof(entry->message));
    entry->severity = 0;
}

void initialize_log_stats(struct LogStats *stats) {
    stats->total_entries = 0;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    memset(stats->most_frequent_level, 0, sizeof(stats->most_frequent_level));
    stats->max_severity = 0;
}

int parse_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    
    entry->severity = parse_severity(entry->level);
    return 1;
}

void analyze_logs(struct LogEntry entries[], int count, struct LogStats *stats) {
    if (entries == NULL || stats == NULL || count <= 0) return;
    
    initialize_log_stats(stats);
    stats->total_entries = count;
    
    int level_counts[4] = {0};
    char *level_names[] = {"UNKNOWN", "INFO", "WARNING", "ERROR"};
    
    for (int i = 0; i < count; i++) {
        switch (entries[i].severity) {
            case 3:
                stats->error_count++;
                level_counts[3]++;
                break;
            case 2:
                stats->warning_count++;
                level_counts[2]++;
                break;
            case 1:
                stats->info_count++;
                level_counts[1]++;
                break;
            default:
                level_counts[0]++;
                break;
        }
        
        if (entries[i].severity > stats->max_severity) {
            stats->max_severity = entries[i].severity;
        }
    }
    
    int max_count = 0;
    int max_index = 0;
    for (int i = 0; i < 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_index = i;
        }
    }
    
    strncpy(stats->most_frequent_level, level_names[max_index], 
            sizeof(stats->most_frequent_level) - 1);
}

void print_log_stats(const struct LogStats *stats) {
    if (stats == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("INFO entries: %d\n", stats->info_count);
    printf("WARNING entries: %d\n", stats->warning_count);
    printf("ERROR entries: %d\n", stats->error_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    printf("Maximum severity: %d\n", stats->max_severity);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    int entry_count = 0;
    
    for (int i = 0; i < MAX_ENTRIES; i++) {
        initialize_log_entry(&entries[i]);
    }
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    char line[MAX_LINE_LENGTH];
    while (entry_count < MAX_ENTRIES