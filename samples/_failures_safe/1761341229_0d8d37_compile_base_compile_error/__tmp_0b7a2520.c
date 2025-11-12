//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_FILENAME 256

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
    int debug_count;
    char most_frequent_level[16];
    int max_severity;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count;
    struct LogStats stats;
    char filename[MAX_FILENAME];
};

void initialize_analyzer(struct LogAnalyzer *analyzer, const char *filename) {
    if (analyzer == NULL || filename == NULL) return;
    
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    analyzer->stats.debug_count = 0;
    analyzer->stats.max_severity = 0;
    strncpy(analyzer->stats.most_frequent_level, "UNKNOWN", 15);
    analyzer->stats.most_frequent_level[15] = '\0';
    
    strncpy(analyzer->filename, filename, MAX_FILENAME - 1);
    analyzer->filename[MAX_FILENAME - 1] = '\0';
}

int parse_severity(const char *level) {
    if (level == NULL) return 0;
    
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[32] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) != 3) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->severity = parse_severity(level);
    
    return 1;
}

int load_log_file(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return 0;
    
    FILE *file = fopen(analyzer->filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    analyzer->entry_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && analyzer->entry_count < MAX_ENTRIES) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) > 0) {
            struct LogEntry entry;
            if (parse_log_line(line, &entry)) {
                analyzer->entries[analyzer->entry_count] = entry;
                analyzer->entry_count++;
            }
        }
    }
    
    fclose(file);
    return analyzer->entry_count > 0;
}

void calculate_statistics(struct LogAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    struct LogStats *stats = &analyzer->stats;
    stats->total_entries = analyzer->entry_count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->debug_count = 0;
    stats->max_severity = 0;
    
    int level_counts[5] = {0};
    const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "UNKNOWN"};
    
    for (int i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        if (entry->severity > stats->max_severity) {
            stats->max_severity = entry->severity;
        }
        
        switch (entry->severity) {
            case 1: stats->debug_count++; level_counts[0]++; break;
            case 2: stats->info_count++; level_counts[1]++; break;
            case 3: stats->warning_count++; level_counts[2]++; break;
            case 4: stats->error_count++; level_counts[3]++; break;
            default: level_counts[4]++; break;
        }
    }
    
    int max_count = 0;
    int max_index = 4;
    for (int i = 0; i < 5; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_index = i;
        }