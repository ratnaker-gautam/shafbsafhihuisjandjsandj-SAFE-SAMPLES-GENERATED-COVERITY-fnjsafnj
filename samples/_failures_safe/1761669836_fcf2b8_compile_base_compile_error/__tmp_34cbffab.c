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
    int debug_count;
    char most_frequent_level[16];
};

void trim_whitespace(char *str) {
    int i = 0, j = 0;
    while (str[i] != '\0') {
        if (!isspace((unsigned char)str[i])) {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0';
}

int parse_severity(const char *level) {
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "DEBUG") == 0) return 1;
    return 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    trim_whitespace(entry->level);
    trim_whitespace(entry->message);
    
    entry->severity = parse_severity(entry->level);
    
    return 1;
}

void analyze_logs(struct LogEntry *entries, int count, struct LogStats *stats) {
    if (entries == NULL || stats == NULL || count <= 0) return;
    
    memset(stats, 0, sizeof(struct LogStats));
    stats->total_entries = count;
    
    int level_counts[4] = {0};
    
    for (int i = 0; i < count; i++) {
        switch (entries[i].severity) {
            case 4: stats->error_count++; level_counts[0]++; break;
            case 3: stats->warning_count++; level_counts[1]++; break;
            case 2: stats->info_count++; level_counts[2]++; break;
            case 1: stats->debug_count++; level_counts[3]++; break;
        }
    }
    
    int max_count = 0;
    int max_index = -1;
    for (int i = 0; i < 4; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_index = i;
        }
    }
    
    if (max_index == 0) strcpy(stats->most_frequent_level, "ERROR");
    else if (max_index == 1) strcpy(stats->most_frequent_level, "WARNING");
    else if (max_index == 2) strcpy(stats->most_frequent_level, "INFO");
    else if (max_index == 3) strcpy(stats->most_frequent_level, "DEBUG");
    else strcpy(stats->most_frequent_level, "UNKNOWN");
}

void print_stats(const struct LogStats *stats) {
    if (stats == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("ERROR count: %d\n", stats->error_count);
    printf("WARNING count: %d\n", stats->warning_count);
    printf("INFO count: %d\n", stats->info_count);
    printf("DEBUG count: %d\n", stats->debug_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LENGTH];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        if (strlen(line) > 0 && line[strlen(line)-1] == '\n') {
            line[strlen(line)-1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {