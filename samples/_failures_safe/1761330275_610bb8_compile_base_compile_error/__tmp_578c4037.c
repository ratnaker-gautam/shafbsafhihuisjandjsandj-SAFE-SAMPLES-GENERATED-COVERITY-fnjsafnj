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
    char message[256];
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
    strcpy(analyzer->stats.most_frequent_level, "INFO");
    analyzer->stats.max_severity = 0;
}

int parse_timestamp(const char *str, char *timestamp) {
    if (strlen(str) < 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
    }
    strncpy(timestamp, str, 19);
    timestamp[19] = '\0';
    return 1;
}

int parse_log_level(const char *str, char *level, int *severity) {
    if (strncmp(str, "ERROR", 5) == 0) {
        strcpy(level, "ERROR");
        *severity = 3;
        return 1;
    } else if (strncmp(str, "WARN", 4) == 0) {
        strcpy(level, "WARN");
        *severity = 2;
        return 1;
    } else if (strncmp(str, "INFO", 4) == 0) {
        strcpy(level, "INFO");
        *severity = 1;
        return 1;
    }
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *line) {
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    char timestamp[20];
    char level[10];
    int severity;
    char message[256];
    
    if (!parse_timestamp(line, timestamp)) return 0;
    
    const char *level_start = line + 20;
    while (*level_start == ' ') level_start++;
    if (!parse_log_level(level_start, level, &severity)) return 0;
    
    const char *msg_start = level_start + strlen(level);
    while (*msg_start == ' ') msg_start++;
    
    size_t msg_len = strlen(msg_start);
    if (msg_len >= sizeof(message)) msg_len = sizeof(message) - 1;
    strncpy(message, msg_start, msg_len);
    message[msg_len] = '\0';
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->severity = severity;
    
    analyzer->entry_count++;
    return 1;
}

void analyze_logs(struct LogAnalyzer *analyzer) {
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.max_severity = 0;
    
    int level_counts[3] = {0};
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        if (entry->severity == 3) {
            analyzer->stats.error_count++;
            level_counts[0]++;
        } else if (entry->severity == 2) {
            analyzer->stats.warning_count++;
            level_counts[1]++;
        } else if (entry->severity == 1) {
            analyzer->stats.info_count++;
            level_counts[2]++;
        }
        
        if (entry->severity > analyzer->stats.max_severity) {
            analyzer->stats.max_severity = entry->severity;
        }
    }
    
    int max_count = level_counts[0];
    strcpy(analyzer->stats.most_frequent_level, "ERROR");
    if (level_counts[1] > max_count) {
        max_count = level_counts[1];
        strcpy(analyzer->stats.most_frequent_level, "WARN");
    }
    if (level_counts[2] > max_count) {
        strcpy(analyzer->stats.most_frequent_level, "INFO");
    }
}

void print_stats(const struct LogAnalyzer *analyzer) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("Errors: %d\n", analyzer->stats.error_count