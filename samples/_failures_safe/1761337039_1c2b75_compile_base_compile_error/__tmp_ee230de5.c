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

int is_valid_timestamp(const char *timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    if (!is_valid_timestamp(timestamp)) return 0;
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    
    strncpy(entry->message, message, MAX_LINE_LENGTH - 30);
    entry->message[MAX_LINE_LENGTH - 30 - 1] = '\0';
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        analyzer->stats.info_count++;
    }
    
    if (analyzer->stats.total_entries == 1) {
        strcpy(analyzer->stats.earliest, entry->timestamp);
        strcpy(analyzer->stats.latest, entry->timestamp);
    } else {
        if (strcmp(entry->timestamp, analyzer->stats.earliest) < 0) {
            strcpy(analyzer->stats.earliest, entry->timestamp);
        }
        if (strcmp(entry->timestamp, analyzer->stats.latest) > 0) {
            strcpy(analyzer->stats.latest, entry->timestamp);
        }
    }
}

int load_log_file(struct LogAnalyzer *analyzer, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) && analyzer->entry_count < MAX_ENTRIES) {
        line[strcspn(line, "\n")] = '\0';
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            analyzer->entries[analyzer->entry_count] = entry;
            update_stats(analyzer, &entry);
            analyzer->entry_count++;
        }
    }
    
    fclose(file);
    return 1;
}

void print_stats(const struct LogStats *stats) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("Errors: %d\n", stats->error_count);
    printf("Warnings: %d\n", stats->warning_count);
    printf("Info messages: %d\n", stats->info_count);
    printf("Time range: %s to %s\n", stats->earliest, stats->latest);
}

void print_entries_by_level(const struct LogAnalyzer *analyzer, const char *level) {
    printf("\nEntries with level '%s':\n", level);
    for (int i = 0; i < analyzer->entry_count; i++) {
        if (strcmp(analyzer->entries[i].level, level) == 0) {
            printf("%s %s %s\n",