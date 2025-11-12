//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

int parse_timestamp(const char* str, char* timestamp) {
    if (!str || !timestamp) return 0;
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else if (!isdigit(str[i])) {
            return 0;
        }
    }
    strncpy(timestamp, str, TIMESTAMP_LEN - 1);
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_level(const char* str) {
    if (!str) return -1;
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int read_log_file(const char* filename, LogEntry* entries, int* count) {
    if (!filename || !entries || !count) return 0;
    
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LEN];
    *count = 0;
    
    while (fgets(line, sizeof(line), file) && *count < MAX_ENTRIES) {
        if (strlen(line) == 0) continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[10];
        
        if (parse_timestamp(line, timestamp)) {
            const char* rest = line + TIMESTAMP_LEN;
            if (sscanf(rest, " %9s", level_str) == 1) {
                int level = parse_level(level_str);
                if (level >= 0) {
                    const char* msg_start = rest;
                    while (*msg_start && *msg_start != ' ') msg_start++;
                    while (*msg_start && *msg_start == ' ') msg_start++;
                    
                    strncpy(entries[*count].timestamp, timestamp, TIMESTAMP_LEN - 1);
                    entries[*count].timestamp[TIMESTAMP_LEN - 1] = '\0';
                    entries[*count].level = level;
                    strncpy(entries[*count].message, msg_start, sizeof(entries[*count].message) - 1);
                    entries[*count].message[sizeof(entries[*count].message) - 1] = '\0';
                    
                    char* newline = strchr(entries[*count].message, '\n');
                    if (newline) *newline = '\0';
                    
                    (*count)++;
                }
            }
        }
    }
    
    fclose(file);
    return 1;
}

void analyze_logs(const LogEntry* entries, int count) {
    if (!entries || count <= 0) return;
    
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= 0 && entries[i].level < LOG_LEVELS) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", level_names[i], level_counts[i], 
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
}

void show_recent_errors(const LogEntry* entries, int count) {
    if (!entries || count <= 0) return;
    
    printf("\nRecent ERROR entries (last 5):\n");
    int shown = 0;
    for (int i = count - 1; i >= 0 && shown < 5; i--) {
        if (entries[i].level == 3) {
            printf("%s: %s\n", entries[i].timestamp, entries[i].message);
            shown++;
        }
    }
    if (shown == 0) {
        printf("No ERROR entries found.\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int count = 0;
    
    if (!read_log_file(argv[1], entries, &count)) {
        printf("Error: Could not read log file '%s'\n", argv[1]);
        return