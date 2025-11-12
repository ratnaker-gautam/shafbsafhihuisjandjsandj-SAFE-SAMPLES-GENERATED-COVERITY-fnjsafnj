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

void initialize_log_entry(struct LogEntry *entry) {
    memset(entry->timestamp, 0, sizeof(entry->timestamp));
    memset(entry->level, 0, sizeof(entry->level));
    memset(entry->message, 0, sizeof(entry->message));
    entry->severity = 0;
}

void initialize_stats(struct LogStats *stats) {
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

int is_valid_timestamp(const char *timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int read_log_entries(struct LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[32], level[16], message[256];
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed == 3 && is_valid_timestamp(timestamp)) {
            struct LogEntry *entry = &entries[count];
            strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
            strncpy(entry->level, level, sizeof(entry->level) - 1);
            strncpy(entry->message, message, sizeof(entry->message) - 1);
            entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
            entry->level[sizeof(entry->level) - 1] = '\0';
            entry->message[sizeof(entry->message) - 1] = '\0';
            entry->severity = parse_severity(level);
            count++;
        }
    }
    
    return count;
}

void analyze_logs(struct LogEntry entries[], int count, struct LogStats *stats) {
    int level_counts[4] = {0};
    char *level_names[] = {"UNKNOWN", "INFO", "WARNING", "ERROR"};
    
    for (int i = 0; i < count; i++) {
        stats->total_entries++;
        
        switch (entries[i].severity) {
            case 3: stats->error_count++; break;
            case 2: stats->warning_count++; break;
            case 1: stats->info_count++; break;
        }
        
        if (entries[i].severity >= 0 && entries[i].severity <= 3) {
            level_counts[entries[i].severity]++;
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
    
    if (max_count > 0) {
        strncpy(stats->most_frequent_level, level_names[max_index], 
                sizeof(stats->most_frequent_level) - 1);
        stats->most_frequent_level[sizeof(stats->most_frequent_level) - 1] = '\0';
    }
}

void print_stats(const struct LogStats *stats) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("INFO entries: %d\n", stats->info_count);
    printf("WARNING entries: %d\n", stats->warning_count);
    printf("ERROR entries: %d\n", stats->error_count);
    printf("Most frequent