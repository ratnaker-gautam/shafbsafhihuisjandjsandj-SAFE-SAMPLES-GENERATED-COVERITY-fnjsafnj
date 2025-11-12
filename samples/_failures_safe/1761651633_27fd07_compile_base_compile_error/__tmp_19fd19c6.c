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
    char earliest[32];
    char latest[32];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    int entry_count;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    memset(analyzer->stats.earliest, 0, sizeof(analyzer->stats.earliest));
    memset(analyzer->stats.latest, 0, sizeof(analyzer->stats.latest));
}

int parse_timestamp(const char *str, char *output) {
    if (str == NULL || output == NULL) return 0;
    size_t len = strlen(str);
    if (len < 19 || len >= 32) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (i == 10 && str[i] != ' ') return 0;
        if (i == 13 && str[i] != ':') return 0;
        if (i == 16 && str[i] != ':') return 0;
        if ((i < 10 || (i > 10 && i < 13) || (i > 13 && i < 16) || i > 16) && !isdigit(str[i])) return 0;
    }
    
    strncpy(output, str, 19);
    output[19] = '\0';
    return 1;
}

int parse_log_level(const char *str, char *output, int *severity) {
    if (str == NULL || output == NULL || severity == NULL) return 0;
    
    if (strcmp(str, "ERROR") == 0) {
        strcpy(output, "ERROR");
        *severity = 3;
        return 1;
    } else if (strcmp(str, "WARNING") == 0) {
        strcpy(output, "WARNING");
        *severity = 2;
        return 1;
    } else if (strcmp(str, "INFO") == 0) {
        strcpy(output, "INFO");
        *severity = 1;
        return 1;
    }
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    
    if (!parse_timestamp(timestamp, entry->timestamp)) return 0;
    if (!parse_log_level(level, entry->level, &entry->severity)) return 0;
    
    size_t msg_len = strlen(message);
    if (msg_len >= sizeof(entry->message)) return 0;
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    analyzer->entry_count++;
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL || analyzer->entry_count == 0) return;
    
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    
    strcpy(analyzer->stats.earliest, analyzer->entries[0].timestamp);
    strcpy(analyzer->stats.latest, analyzer->entries[0].timestamp);
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        if (entry->severity == 3) analyzer->stats.error_count++;
        else if (entry->severity == 2) analyzer->stats.warning_count++;
        else if (entry->severity == 1) analyzer->stats.info_count++;
        
        if (strcmp(entry->timestamp, analyzer->stats.earliest) < 0) {
            strcpy(analyzer->stats.earliest, entry->timestamp);
        }
        if (strcmp(entry->timestamp, analyzer->stats.latest) > 0) {
            strcpy(analyzer->stats.latest, entry->timestamp);
        }
    }
}

void print_stats(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("Errors: %d\n", analyzer->stats.error_count);
    printf("Warnings: %d\n", analyzer->stats.w