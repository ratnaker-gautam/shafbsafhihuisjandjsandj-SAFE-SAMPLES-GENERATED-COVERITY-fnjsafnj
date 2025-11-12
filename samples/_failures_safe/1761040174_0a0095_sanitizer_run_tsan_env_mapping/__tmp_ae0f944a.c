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
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int validate_timestamp(const char *timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int read_log_entries(struct LogEntry *entries, int max_count) {
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    while (count < max_count && fgets(line, sizeof(line), stdin) != NULL) {
        if (strlen(line) >= MAX_LINE_LENGTH - 1) continue;
        
        struct LogEntry entry;
        int parsed = sscanf(line, "%31s %15s %255[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (parsed != 3) continue;
        if (!validate_timestamp(entry.timestamp)) continue;
        
        entry.severity = parse_severity(entry.level);
        if (entry.severity == 0) continue;
        
        entries[count] = entry;
        count++;
    }
    
    return count;
}

void analyze_logs(struct LogEntry *entries, int count, struct LogStats *stats) {
    if (count <= 0) return;
    
    stats->total_entries = count;
    int level_counts[4] = {0};
    
    for (int i = 0; i < count; i++) {
        switch (entries[i].severity) {
            case 3: stats->error_count++; level_counts[3]++; break;
            case 2: stats->warning_count++; level_counts[2]++; break;
            case 1: stats->info_count++; level_counts[1]++; break;
        }
        
        if (entries[i].severity > stats->max_severity) {
            stats->max_severity = entries[i].severity;
        }
    }
    
    int max_count = level_counts[1];
    strcpy(stats->most_frequent_level, "INFO");
    
    if (level_counts[2] > max_count) {
        max_count = level_counts[2];
        strcpy(stats->most_frequent_level, "WARN");
    }
    if (level_counts[3] > max_count) {
        strcpy(stats->most_frequent_level, "ERROR");
    }
}

void print_analysis(const struct LogStats *stats) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("ERROR count: %d\n", stats->error_count);
    printf("WARN count: %d\n", stats->warning_count);
    printf("INFO count: %d\n", stats->info_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    printf("Highest severity: %d\n", stats->max_severity);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    
    initialize_stats(&stats);
    
    printf("Enter log entries (timestamp level message):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message text\n");
    printf("Press Ctrl+D to finish input.\n");
    
    int entry_count = read_log_entries(entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count, &stats);
    print_analysis(&stats);
    
    return 0;
}