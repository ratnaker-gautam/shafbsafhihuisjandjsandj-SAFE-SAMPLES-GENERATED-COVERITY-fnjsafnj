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
    char message[MAX_LINE_LENGTH - 30];
};

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    char earliest[20];
    char latest[20];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count;
    struct LogStats stats;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    memset(analyzer->stats.earliest, 0, 20);
    memset(analyzer->stats.latest, 0, 20);
}

int parse_timestamp(const char *str, char *output) {
    if (strlen(str) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
    }
    strncpy(output, str, 19);
    output[19] = '\0';
    return 1;
}

int parse_log_level(const char *str, char *output) {
    if (strlen(str) > 9) return 0;
    if (strcmp(str, "ERROR") == 0 || strcmp(str, "WARNING") == 0 || strcmp(str, "INFO") == 0) {
        strncpy(output, str, 9);
        return 1;
    }
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
    
    if (sscanf(line, "%19s %9s %[^\n]", timestamp, level, message) != 3) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    
    if (!parse_timestamp(timestamp, entry->timestamp)) return 0;
    if (!parse_log_level(level, entry->level)) return 0;
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    analyzer->entry_count++;
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer) {
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    
    if (analyzer->entry_count == 0) return;
    
    strcpy(analyzer->stats.earliest, analyzer->entries[0].timestamp);
    strcpy(analyzer->stats.latest, analyzer->entries[0].timestamp);
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        if (strcmp(analyzer->entries[i].level, "ERROR") == 0) {
            analyzer->stats.error_count++;
        } else if (strcmp(analyzer->entries[i].level, "WARNING") == 0) {
            analyzer->stats.warning_count++;
        } else if (strcmp(analyzer->entries[i].level, "INFO") == 0) {
            analyzer->stats.info_count++;
        }
        
        if (strcmp(analyzer->entries[i].timestamp, analyzer->stats.earliest) < 0) {
            strcpy(analyzer->stats.earliest, analyzer->entries[i].timestamp);
        }
        if (strcmp(analyzer->entries[i].timestamp, analyzer->stats.latest) > 0) {
            strcpy(analyzer->stats.latest, analyzer->entries[i].timestamp);
        }
    }
}

void print_stats(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("Error count: %d\n", analyzer->stats.error_count);
    printf("Warning count: %d\n", analyzer->stats.warning_count);
    printf("Info count: %d\n", analyzer->stats.info_count);
    printf("Time range: %s to %s\n", analyzer->stats.earliest, analyzer->stats.latest);
}

void print_errors(const struct LogAnalyzer *analyzer) {
    printf("\nError entries:\n");
    for (int i = 0; i < analyzer->entry_count