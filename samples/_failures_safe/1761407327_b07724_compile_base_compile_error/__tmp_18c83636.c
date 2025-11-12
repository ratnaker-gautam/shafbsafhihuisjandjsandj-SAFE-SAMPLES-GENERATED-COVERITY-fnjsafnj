//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 16
#define MAX_MESSAGE_LENGTH 256

struct LogEntry {
    char timestamp[32];
    char level[MAX_LEVEL_LENGTH];
    char message[MAX_MESSAGE_LENGTH];
    int severity;
};

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    int debug_count;
    char most_frequent_level[MAX_LEVEL_LENGTH];
    int max_level_count;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    int entry_count;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    analyzer->stats.max_level_count = 0;
    memset(analyzer->stats.most_frequent_level, 0, MAX_LEVEL_LENGTH);
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32] = {0};
    char level[MAX_LEVEL_LENGTH] = {0};
    char message[MAX_MESSAGE_LENGTH] = {0};
    
    int result = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    if (strcmp(level, "ERROR") == 0) entry->severity = 4;
    else if (strcmp(level, "WARNING") == 0) entry->severity = 3;
    else if (strcmp(level, "INFO") == 0) entry->severity = 2;
    else if (strcmp(level, "DEBUG") == 0) entry->severity = 1;
    else entry->severity = 0;
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    if (analyzer == NULL || entry == NULL) return;
    
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) analyzer->stats.error_count++;
    else if (strcmp(entry->level, "WARNING") == 0) analyzer->stats.warning_count++;
    else if (strcmp(entry->level, "INFO") == 0) analyzer->stats.info_count++;
    else if (strcmp(entry->level, "DEBUG") == 0) analyzer->stats.debug_count++;
}

void calculate_frequent_level(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    int counts[4] = {0};
    const char *levels[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(analyzer->entries[i].level, levels[j]) == 0) {
                counts[j]++;
                break;
            }
        }
    }
    
    int max_index = 0;
    for (int i = 1; i < 4; i++) {
        if (counts[i] > counts[max_index]) {
            max_index = i;
        }
    }
    
    if (counts[max_index] > 0) {
        strncpy(analyzer->stats.most_frequent_level, levels[max_index], MAX_LEVEL_LENGTH - 1);
        analyzer->stats.most_frequent_level[MAX_LEVEL_LENGTH - 1] = '\0';
        analyzer->stats.max_level_count = counts[max_index];
    }
}

void display_stats(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("ERROR count: %d\n", analyzer->stats.error_count);
    printf("WARNING count: %d\n", analyzer->stats.warning_count);
    printf("INFO count: %d\n", analyzer->stats.info_count);
    printf("DEBUG count: %d\n", analyzer->stats.debug_count);
    
    if (analyzer->stats.max_level_count > 0) {
        printf("Most frequent level: %s (%d occurrences)\n", 
               analyzer->stats.most_frequent_level, analyzer->stats.max_level_count);
    }
    
    printf("\nSample entries:\n");
    int sample_count = analyzer