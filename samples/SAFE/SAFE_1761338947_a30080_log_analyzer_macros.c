//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(l) ((l) >= LOG_LEVEL_ERROR && (l) <= LOG_LEVEL_DEBUG)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

static const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};

int parse_log_level(const char* str) {
    if (str == NULL) return -1;
    if (strncmp(str, "ERROR", 5) == 0) return LOG_LEVEL_ERROR;
    if (strncmp(str, "WARN", 4) == 0) return LOG_LEVEL_WARN;
    if (strncmp(str, "INFO", 4) == 0) return LOG_LEVEL_INFO;
    if (strncmp(str, "DEBUG", 5) == 0) return LOG_LEVEL_DEBUG;
    return -1;
}

int parse_timestamp(const char* str, char* output) {
    if (str == NULL || output == NULL) return 0;
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        char c = str[i];
        if (!(isdigit(c) || c == '-' || c == ':' || c == ' ' || c == '.')) {
            return 0;
        }
    }
    
    strncpy(output, str, TIMESTAMP_LEN - 1);
    output[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int read_log_entries(struct log_entry* entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0) continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[10];
        char message[MAX_LINE_LEN];
        
        if (sscanf(line, "%19s %9s %1023[^\n]", timestamp, level_str, message) != 3) {
            continue;
        }
        
        if (!parse_timestamp(timestamp, entries[count].timestamp)) {
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (!IS_VALID_LEVEL(level)) {
            continue;
        }
        
        entries[count].level = level;
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        count++;
    }
    
    return count;
}

void analyze_logs(struct log_entry* entries, int count) {
    int level_counts[LOG_LEVELS] = {0};
    char earliest[TIMESTAMP_LEN] = "";
    char latest[TIMESTAMP_LEN] = "";
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return;
    }
    
    strcpy(earliest, entries[0].timestamp);
    strcpy(latest, entries[0].timestamp);
    
    for (int i = 0; i < count; i++) {
        if (!IS_VALID_LEVEL(entries[i].level)) continue;
        
        level_counts[entries[i].level]++;
        
        if (strcmp(entries[i].timestamp, earliest) < 0) {
            strcpy(earliest, entries[i].timestamp);
        }
        if (strcmp(entries[i].timestamp, latest) > 0) {
            strcpy(latest, entries[i].timestamp);
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("Time range: %s to %s\n", earliest, latest);
    printf("Breakdown by level:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("  %s: %d (%.1f%%)\n", level_names[i], level_counts[i],
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    
    printf("Enter log entries (timestamp level message):\n");
    printf("Example: 2024-01-15 10:30:00 INFO System started\n");
    printf("Press Ctrl+D to finish input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input.\n");
        return EXIT_FAILURE;
    }
    
    analyze_logs(entries, count);
    
    return EXIT_SUCCESS;
}