//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
    int severity;
};

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    char most_frequent_level[10];
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
        
        char timestamp[20] = {0};
        char level[10] = {0};
        char message[MAX_LINE_LENGTH] = {0};
        
        int scanned = sscanf(line, "%19s %9s %1023[^\n]", timestamp, level, message);
        
        if (scanned == 3 && validate_timestamp(timestamp)) {
            strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
            entries[count].timestamp[sizeof(entries[count].timestamp) - 1] = '\0';
            strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
            entries[count].level[sizeof(entries[count].level) - 1] = '\0';
            strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
            entries[count].message[sizeof(entries[count].message) - 1] = '\0';
            entries[count].severity = parse_severity(level);
            count++;
        }
    }
    
    return count;
}

void analyze_logs(struct LogEntry entries[], int count, struct LogStats *stats) {
    if (count <= 0) return;
    
    stats->total_entries = count;
    int level_counts[4] = {0};
    
    for (int i = 0; i < count; i++) {
        switch (entries[i].severity) {
            case 3: stats->error_count++; level_counts[3]++; break;
            case 2: stats->warning_count++; level_counts[2]++; break;
            case 1: stats->info_count++; level_counts[1]++; break;
            default: level_counts[0]++; break;
        }
        
        if (entries[i].severity > stats->max_severity) {
            stats->max_severity = entries[i].severity;
        }
    }
    
    int max_count = 0;
    int max_index = 1;
    for (int i = 1; i <= 3; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_index = i;
        }
    }
    
    switch (max_index) {
        case 3: strcpy(stats->most_frequent_level, "ERROR"); break;
        case 2: strcpy(stats->most_frequent_level, "WARN"); break;
        case 1: strcpy(stats->most_frequent_level, "INFO"); break;
        default: strcpy(stats->most_frequent_level, "UNKNOWN"); break;
    }
}

void print_analysis(const struct LogStats *stats) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("ERROR entries: %d\n", stats->error_count);
    printf("WARN entries: %d\n", stats->warning_count);
    printf("INFO entries: %d\n", stats->info_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    printf("Maximum severity: %d\n", stats->max_severity);
    
    if (stats