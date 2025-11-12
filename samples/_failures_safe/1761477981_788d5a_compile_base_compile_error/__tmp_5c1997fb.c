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
    strcpy(stats->most_frequent_level, "UNKNOWN");
    stats->max_severity = 0;
}

int parse_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
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
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;
        
        char timestamp[32], level[16], message[256];
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed != 3) {
            printf("Invalid format. Skipping.\n");
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp format. Skipping.\n");
            continue;
        }
        
        strcpy(entries[count].timestamp, timestamp);
        strcpy(entries[count].level, level);
        strcpy(entries[count].message, message);
        entries[count].severity = parse_severity(level);
        
        count++;
    }
    
    return count;
}

void analyze_logs(struct LogEntry entries[], int count, struct LogStats *stats) {
    if (count <= 0) return;
    
    int level_counts[4] = {0};
    char *level_names[] = {"UNKNOWN", "INFO", "WARNING", "ERROR"};
    
    for (int i = 0; i < count; i++) {
        stats->total_entries++;
        
        if (entries[i].severity == 3) {
            stats->error_count++;
            level_counts[3]++;
        } else if (entries[i].severity == 2) {
            stats->warning_count++;
            level_counts[2]++;
        } else if (entries[i].severity == 1) {
            stats->info_count++;
            level_counts[1]++;
        } else {
            level_counts[0]++;
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
    
    strcpy(stats->most_frequent_level, level_names[max_index]);
}

void display_stats(const struct LogStats *stats) {
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("INFO entries: %d\n", stats->info_count);
    printf("WARNING entries: %d\n", stats->warning_count);
    printf("ERROR entries: %d\n", stats->error_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    printf("Highest severity: %d\n", stats->max_severity);
}

void display_high_severity_entries(struct LogEntry entries[], int count, int min_severity) {
    printf("\nEntries with severity >= %d:\n", min_severity);
    int found = 0;
    
    for (int i = 0; i < count; i