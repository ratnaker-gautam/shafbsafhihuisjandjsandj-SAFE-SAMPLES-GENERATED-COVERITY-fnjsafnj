//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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

void initialize_stats(struct LogStats *stats) {
    stats->total_entries = 0;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    strcpy(stats->most_frequent_level, "INFO");
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
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->severity = parse_severity(level);
    
    return 1;
}

void update_stats(struct LogStats *stats, const struct LogEntry *entry) {
    if (stats == NULL || entry == NULL) return;
    
    stats->total_entries++;
    
    if (entry->severity == 3) stats->error_count++;
    else if (entry->severity == 2) stats->warning_count++;
    else if (entry->severity == 1) stats->info_count++;
    
    if (entry->severity > stats->max_severity) {
        stats->max_severity = entry->severity;
    }
    
    if (stats->error_count > stats->warning_count && stats->error_count > stats->info_count) {
        strcpy(stats->most_frequent_level, "ERROR");
    } else if (stats->warning_count > stats->error_count && stats->warning_count > stats->info_count) {
        strcpy(stats->most_frequent_level, "WARNING");
    } else {
        strcpy(stats->most_frequent_level, "INFO");
    }
}

void display_entry(const struct LogEntry *entry) {
    if (entry == NULL) return;
    printf("%s [%s] %s\n", entry->timestamp, entry->level, entry->message);
}

void display_stats(const struct LogStats *stats) {
    if (stats == NULL) return;
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("ERROR count: %d\n", stats->error_count);
    printf("WARNING count: %d\n", stats->warning_count);
    printf("INFO count: %d\n", stats->info_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    printf("Maximum severity: %d\n", stats->max_severity);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    initialize_stats(&stats);
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            update_stats(&stats, &entry);
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nParsed log entries:\n");
    for (int i = 0; i < entry_count; i++) {
        display_entry(&entries[i]);
    }
    
    display_stats(&stats);
    
    return 0;
}