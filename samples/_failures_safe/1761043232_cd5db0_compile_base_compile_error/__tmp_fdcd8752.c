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

void trim_whitespace(char *str) {
    int i = 0, j = 0;
    while (str[i] != '\0') {
        if (!isspace((unsigned char)str[i])) {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0';
}

int parse_log_level(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int read_log_entries(struct LogEntry entries[], int max_entries) {
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        trim_whitespace(line);
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        int parsed = sscanf(line, "%31s %15s %255[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (parsed == 3) {
            entry.severity = parse_log_level(entry.level);
            if (count < max_entries) {
                entries[count] = entry;
                count++;
            }
        } else {
            printf("Invalid format. Skipping line: %s\n", line);
        }
    }
    
    return count;
}

void analyze_logs(const struct LogEntry entries[], int count, struct LogStats *stats) {
    if (count <= 0 || stats == NULL) {
        return;
    }
    
    stats->total_entries = count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->max_severity = 0;
    
    int level_counts[4] = {0};
    const char *level_names[] = {"UNKNOWN", "INFO", "WARN", "ERROR"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 1 && entries[i].severity <= 3) {
            level_counts[entries[i].severity]++;
        }
        
        if (entries[i].severity > stats->max_severity) {
            stats->max_severity = entries[i].severity;
        }
    }
    
    stats->error_count = level_counts[3];
    stats->warning_count = level_counts[2];
    stats->info_count = level_counts[1];
    
    int max_count = 0;
    int max_level = 0;
    for (int i = 1; i <= 3; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_level = i;
        }
    }
    
    if (max_level >= 1 && max_level <= 3) {
        strncpy(stats->most_frequent_level, level_names[max_level], 
                sizeof(stats->most_frequent_level) - 1);
        stats->most_frequent_level[sizeof(stats->most_frequent_level) - 1] = '\0';
    } else {
        strncpy(stats->most_frequent_level, "UNKNOWN", 
                sizeof(stats->most_frequent_level) - 1);
        stats->most_frequent_level[sizeof(stats->most_frequent_level) - 1] = '\0';
    }
}

void display_stats(const struct LogStats *stats) {
    if (stats == NULL) {
        return;
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("INFO entries: %d\n", stats->info_count);
    printf("WARN entries: %d\n", stats->warning_count);
    printf("ERROR entries: %d\n", stats->error_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    printf("Highest severity: %d\n", stats->max_severity);
    
    if (stats->total_entries > 0) {
        printf("Error percentage: %.1f%%\n", 
               (stats->error_count * 100.0) / stats->total_entries);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    
    int entry_count = read_log_entries(entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n