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
    int entry_count;
    struct LogStats stats;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    memset(analyzer->stats.earliest, 0, 32);
    memset(analyzer->stats.latest, 0, 32);
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
    
    strncpy(output, str, 31);
    output[31] = '\0';
    return 1;
}

int validate_level(const char *level) {
    if (level == NULL) return 0;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int add_log_entry(struct LogAnalyzer *analyzer, const char *timestamp, const char *level, const char *message) {
    if (analyzer == NULL || timestamp == NULL || level == NULL || message == NULL) return 0;
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry *entry = &analyzer->entries[analyzer->entry_count];
    
    if (!parse_timestamp(timestamp, entry->timestamp)) return 0;
    
    int severity = validate_level(level);
    if (severity == 0) return 0;
    
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    
    size_t msg_len = strlen(message);
    if (msg_len >= 256) return 0;
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->severity = severity;
    analyzer->entry_count++;
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL || analyzer->entry_count == 0) return;
    
    analyzer->stats.total_entries = analyzer->entry_count;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        switch (entry->severity) {
            case 3: analyzer->stats.error_count++; break;
            case 2: analyzer->stats.warning_count++; break;
            case 1: analyzer->stats.info_count++; break;
        }
    }
    
    if (analyzer->entry_count > 0) {
        strncpy(analyzer->stats.earliest, analyzer->entries[0].timestamp, 31);
        strncpy(analyzer->stats.latest, analyzer->entries[analyzer->entry_count-1].timestamp, 31);
        analyzer->stats.earliest[31] = '\0';
        analyzer->stats.latest[31] = '\0';
    }
}

void print_stats(const struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", analyzer->stats.total_entries);
    printf("Errors: %d\n", analyzer->stats.error_count);
    printf("Warnings: %d\n", analyzer->stats.warning_count);
    printf("Info: %d\n", analyzer->stats.info_count);
    printf("Time range: %s to %s\n", analyzer->stats.earliest, analyzer->stats.latest);
}

void print_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    if (analyzer == NULL || level == NULL) return;
    
    int severity = validate_level(level);
    if (severity == 0) return;
    
    printf("\nEntries with level %s