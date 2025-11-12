//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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
    int debug_count;
    char most_frequent_level[10];
    int max_severity;
    char max_severity_timestamp[20];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count;
    struct LogStats stats;
};

int parse_timestamp(const char* str, char* timestamp) {
    if (!str || !timestamp) return 0;
    if (strlen(str) < 19) return 0;
    
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
    
    strncpy(timestamp, str, 19);
    timestamp[19] = '\0';
    return 1;
}

int parse_log_level(const char* str, char* level, int* severity) {
    if (!str || !level || !severity) return 0;
    
    if (strcasecmp(str, "ERROR") == 0) {
        strcpy(level, "ERROR");
        *severity = 4;
    } else if (strcasecmp(str, "WARNING") == 0) {
        strcpy(level, "WARNING");
        *severity = 3;
    } else if (strcasecmp(str, "INFO") == 0) {
        strcpy(level, "INFO");
        *severity = 2;
    } else if (strcasecmp(str, "DEBUG") == 0) {
        strcpy(level, "DEBUG");
        *severity = 1;
    } else {
        return 0;
    }
    return 1;
}

void initialize_analyzer(struct LogAnalyzer* analyzer) {
    if (!analyzer) return;
    
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    strcpy(analyzer->stats.most_frequent_level, "NONE");
    analyzer->stats.max_severity = 0;
    strcpy(analyzer->stats.max_severity_timestamp, "NONE");
}

int add_log_entry(struct LogAnalyzer* analyzer, const char* timestamp, const char* level, const char* message) {
    if (!analyzer || !timestamp || !level || !message) return 0;
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    if (strlen(message) >= 256) return 0;
    
    struct LogEntry* entry = &analyzer->entries[analyzer->entry_count];
    
    if (!parse_timestamp(timestamp, entry->timestamp)) return 0;
    
    int severity;
    if (!parse_log_level(level, entry->level, &severity)) return 0;
    
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    entry->severity = severity;
    
    analyzer->entry_count++;
    return 1;
}

void analyze_logs(struct LogAnalyzer* analyzer) {
    if (!analyzer || analyzer->entry_count == 0) return;
    
    struct LogStats* stats = &analyzer->stats;
    stats->total_entries = analyzer->entry_count;
    
    int level_counts[4] = {0};
    stats->max_severity = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry* entry = &analyzer->entries[i];
        
        switch (entry->severity) {
            case 4: stats->error_count++; level_counts[0]++; break;
            case 3: stats->warning_count++; level_counts[1]++; break;
            case 2: stats->info_count++; level_counts[2]++; break;
            case 1: stats->debug_count++; level_counts[3]++; break;
        }
        
        if (entry->severity > stats->max_severity) {
            stats->max_severity = entry->severity;
            strcpy(stats->max_severity_timestamp, entry->timestamp);
        }
    }
    
    int max_count = 0;
    int max_index = -1;
    const char* level_names[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
    
    for (int i = 0; i < 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_index = i;
        }
    }
    
    if (max_index >=