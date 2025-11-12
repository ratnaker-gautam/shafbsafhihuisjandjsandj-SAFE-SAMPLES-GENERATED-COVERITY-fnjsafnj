//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

struct LogStats {
    int error_count;
    int warning_count;
    int info_count;
    char most_frequent_level[16];
    int max_severity;
};

void trim_whitespace(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
}

int parse_log_level(const char *level) {
    char lower[16];
    size_t i;
    for (i = 0; i < sizeof(lower) - 1 && level[i]; i++) {
        lower[i] = tolower((unsigned char)level[i]);
    }
    lower[i] = 0;
    
    if (strcmp(lower, "error") == 0) return 3;
    if (strcmp(lower, "warning") == 0) return 2;
    if (strcmp(lower, "info") == 0) return 1;
    return 0;
}

int read_log_entries(struct LogEntry *entries, int max_entries) {
    char buffer[MAX_LINE_LEN];
    int count = 0;
    
    printf("Enter log entries (format: timestamp level message). Empty line to finish.\n");
    
    while (count < max_entries && fgets(buffer, sizeof(buffer), stdin)) {
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        trim_whitespace(buffer);
        if (strlen(buffer) == 0) break;
        
        int parsed = sscanf(buffer, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed < 2) {
            printf("Invalid format. Skipping.\n");
            continue;
        }
        
        if (strlen(timestamp) > 0 && strlen(level) > 0) {
            strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
            entries[count].timestamp[sizeof(entries[count].timestamp) - 1] = 0;
            strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
            entries[count].level[sizeof(entries[count].level) - 1] = 0;
            if (parsed == 3) {
                strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
                entries[count].message[sizeof(entries[count].message) - 1] = 0;
            }
            entries[count].severity = parse_log_level(level);
            count++;
        }
    }
    
    return count;
}

void analyze_logs(struct LogEntry *entries, int count, struct LogStats *stats) {
    int level_counts[4] = {0};
    char *level_names[] = {"unknown", "info", "warning", "error"};
    
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->max_severity = 0;
    
    for (int i = 0; i < count; i++) {
        int severity = entries[i].severity;
        if (severity >= 1 && severity <= 3) {
            level_counts[severity]++;
        }
        
        if (severity > stats->max_severity) {
            stats->max_severity = severity;
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
    
    if (max_level > 0) {
        strncpy(stats->most_frequent_level, level_names[max_level], sizeof(stats->most_frequent_level) - 1);
        stats->most_frequent_level[sizeof(stats->most_frequent_level) - 1] = 0;
    } else {
        strncpy(stats->most_frequent_level, "unknown", sizeof(stats->most_frequent_level) - 1);
        stats->most_frequent_level[sizeof(stats->most_frequent_level) - 1] = 0;
    }
}

void display_analysis(struct LogStats *stats, int total_entries) {
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries: %d\n", total_entries);
    printf("Info entries: %d\n", stats->info_count);
    printf("Warning entries: %d\n", stats->warning_count);
    printf("Error entries: %d\n", stats->error_count);
    printf("Most frequent level: %s