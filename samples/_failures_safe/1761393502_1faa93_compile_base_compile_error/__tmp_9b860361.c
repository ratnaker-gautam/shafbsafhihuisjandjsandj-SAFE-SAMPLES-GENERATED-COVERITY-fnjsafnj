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

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    if (!validate_timestamp(timestamp)) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
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
}

void calculate_most_frequent(struct LogStats *stats, const struct LogEntry entries[], int count) {
    if (stats == NULL || entries == NULL || count <= 0) return;
    
    int error_count = 0, warn_count = 0, info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "INFO") == 0) info_count++;
    }
    
    if (error_count >= warn_count && error_count >= info_count) {
        strcpy(stats->most_frequent_level, "ERROR");
    } else if (warn_count >= error_count && warn_count >= info_count) {
        strcpy(stats->most_frequent_level, "WARN");
    } else if (info_count >= error_count && info_count >= warn_count) {
        strcpy(stats->most_frequent_level, "INFO");
    }
}

void print_stats(const struct LogStats *stats) {
    if (stats == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("Errors: %d\n", stats->error_count);
    printf("Warnings: %d\n", stats->warning_count);
    printf("Info: %d\n", stats->info_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    printf("Maximum severity: %d\n", stats->max_severity);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    int entry_count = 0;
    char line[MAX_L