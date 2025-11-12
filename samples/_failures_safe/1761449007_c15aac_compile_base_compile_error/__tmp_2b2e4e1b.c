//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_FILENAME 256

struct LogEntry {
    time_t timestamp;
    char level[16];
    char message[256];
    uint32_t source_ip;
};

struct LogStats {
    uint32_t info_count;
    uint32_t warning_count;
    uint32_t error_count;
    uint32_t total_entries;
    time_t earliest_time;
    time_t latest_time;
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    size_t entry_count;
};

void init_log_analyzer(struct LogAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    analyzer->stats.earliest_time = 0;
    analyzer->stats.latest_time = 0;
}

int parse_timestamp(const char *str, time_t *timestamp) {
    struct tm tm = {0};
    if (strptime(str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    *timestamp = mktime(&tm);
    return 1;
}

int parse_ip(const char *str, uint32_t *ip) {
    unsigned int a, b, c, d;
    if (sscanf(str, "%u.%u.%u.%u", &a, &b, &c, &d) != 4) {
        return 0;
    }
    if (a > 255 || b > 255 || c > 255 || d > 255) {
        return 0;
    }
    *ip = (a << 24) | (b << 16) | (c << 8) | d;
    return 1;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    char timestamp_str[32];
    char level_str[16];
    char ip_str[16];
    char message[256];
    
    if (sscanf(line, "%31s %15s %15s %255[^\n]", 
               timestamp_str, level_str, ip_str, message) != 4) {
        return 0;
    }
    
    if (!parse_timestamp(timestamp_str, &entry->timestamp)) {
        return 0;
    }
    
    if (!parse_ip(ip_str, &entry->source_ip)) {
        return 0;
    }
    
    if (strlen(level_str) >= sizeof(entry->level)) {
        return 0;
    }
    strncpy(entry->level, level_str, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    if (strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int load_log_file(struct LogAnalyzer *analyzer, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    size_t count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < MAX_ENTRIES) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            analyzer->entries[count] = entry;
            count++;
        }
    }
    
    fclose(file);
    analyzer->entry_count = count;
    return 1;
}

void analyze_logs(struct LogAnalyzer *analyzer) {
    struct LogStats *stats = &analyzer->stats;
    stats->total_entries = analyzer->entry_count;
    
    if (analyzer->entry_count == 0) {
        return;
    }
    
    stats->earliest_time = analyzer->entries[0].timestamp;
    stats->latest_time = analyzer->entries[0].timestamp;
    
    for (size_t i = 0; i < analyzer->entry_count; i++) {
        struct LogEntry *entry = &analyzer->entries[i];
        
        if (strcmp(entry->level, "INFO") == 0) {
            stats->info_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            stats->warning_count++;
        } else if (strcmp(entry->level, "ERROR") == 0) {
            stats->error_count++;
        }
        
        if (entry->timestamp < stats->earliest_time) {
            stats->earliest_time = entry->timestamp;
        }
        if (entry->timestamp > stats->latest_time) {
            stats->latest_time = entry->timestamp;
        }
    }
}

void print_stats(const struct LogAnalyzer *analyzer) {
    const struct LogStats *stats = &analyzer->stats;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %u\n", stats->total_entries);
    printf("INFO entries: %u\n", stats->info_count