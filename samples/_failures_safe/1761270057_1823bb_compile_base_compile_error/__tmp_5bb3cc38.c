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

int parse_level(const char *str, char *level, int *severity) {
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

int parse_log_line(const char *line, struct LogEntry *entry) {
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp, line);
    
    char *timestamp_part = strtok(temp, " ");
    if (!timestamp_part) return 0;
    if (!parse_timestamp(timestamp_part, entry->timestamp)) return 0;
    
    char *level_part = strtok(NULL, " ");
    if (!level_part) return 0;
    if (!parse_level(level_part, entry->level, &entry->severity)) return 0;
    
    char *message_part = strtok(NULL, "\n");
    if (!message_part) return 0;
    if (strlen(message_part) >= 255) return 0;
    strcpy(entry->message, message_part);
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(entry->level, "WARN") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        analyzer->stats.info_count++;
    }
    
    if (entry->severity > analyzer->stats.max_severity) {
        analyzer->stats.max_severity = entry->severity;
    }
    
    if (analyzer->stats.error_count > analyzer->stats.warning_count && 
        analyzer->stats.error_count > analyzer->stats.info_count) {
        strcpy(analyzer->stats.most_frequent_level, "ERROR");
    } else if (analyzer->stats.warning_count > analyzer->stats.error_count && 
               analyzer->stats.warning_count > analyzer->stats.info_count) {
        strcpy(analyzer->stats.most_frequent_level, "WARN");
    } else {
        strcpy(analyzer->stats.most_frequent_level, "INFO");
    }
}

int load_log_file(struct LogAnalyzer *analyzer) {
    char filename[256];
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) return 0;
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
    
    if (strlen(filename) == 0) return 0;
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (fgets(line