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

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char *timestamp = strtok(temp_line, " ");
    char *level = strtok(NULL, " ");
    char *message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    trim_whitespace(entry->level);
    
    if (strcmp(entry->level, "ERROR") == 0) entry->severity = 3;
    else if (strcmp(entry->level, "WARNING") == 0) entry->severity = 2;
    else if (strcmp(entry->level, "INFO") == 0) entry->severity = 1;
    else entry->severity = 0;
    
    return 1;
}

void analyze_logs(struct LogEntry entries[], int count, struct LogStats *stats) {
    if (entries == NULL || stats == NULL || count <= 0) return;
    
    stats->total_entries = count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity == 3) stats->error_count++;
        else if (entries[i].severity == 2) stats->warning_count++;
        else if (entries[i].severity == 1) stats->info_count++;
    }
    
    if (stats->error_count >= stats->warning_count && stats->error_count >= stats->info_count) {
        strcpy(stats->most_frequent_level, "ERROR");
    } else if (stats->warning_count >= stats->error_count && stats->warning_count >= stats->info_count) {
        strcpy(stats->most_frequent_level, "WARNING");
    } else {
        strcpy(stats->most_frequent_level, "INFO");
    }
}

void display_stats(const struct LogStats *stats) {
    if (stats == NULL) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("ERROR count: %d\n", stats->error_count);
    printf("WARNING count: %d\n", stats->warning_count);
    printf("INFO count: %d\n", stats->info_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        if (strlen(line) > 0 && line[strlen(line)-1] == '\n') {
            line[strlen(line)-1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count, &stats);
    display_stats(&stats);
    
    return 0;
}