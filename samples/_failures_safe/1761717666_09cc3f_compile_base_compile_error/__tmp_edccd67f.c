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

int parse_timestamp(const char* str, char* timestamp) {
    if (!str || !timestamp) return 0;
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        char c = str[i];
        if (i == 4 || i == 7) {
            if (c != '-') return 0;
        } else if (i == 10) {
            if (c != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (c != ':') return 0;
        } else {
            if (!isdigit(c)) return 0;
        }
        timestamp[i] = c;
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_log_level(const char* str) {
    if (!str) return -1;
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strncmp(str, level_names[i], strlen(level_names[i])) == 0) {
            return i;
        }
    }
    return -1;
}

int read_log_entries(struct log_entry* entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char* pos = line;
        char timestamp[TIMESTAMP_LEN];
        
        if (!parse_timestamp(pos, timestamp)) {
            continue;
        }
        
        pos += TIMESTAMP_LEN;
        while (*pos == ' ' && *pos != '\0') pos++;
        
        int level_end = 0;
        while (pos[level_end] != ' ' && pos[level_end] != '\0' && pos[level_end] != '\n') {
            level_end++;
        }
        
        if (level_end == 0) continue;
        
        char level_str[10];
        if (level_end >= (int)sizeof(level_str)) continue;
        strncpy(level_str, pos, level_end);
        level_str[level_end] = '\0';
        
        int level = parse_log_level(level_str);
        if (!IS_VALID_LEVEL(level)) continue;
        
        pos += level_end;
        while (*pos == ' ' && *pos != '\0') pos++;
        
        if (*pos == '\0' || *pos == '\n') continue;
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level;
        strncpy(entries[count].message, pos, MAX_LINE_LEN - TIMESTAMP_LEN - 10 - 1);
        entries[count].message[MAX_LINE_LEN - TIMESTAMP_LEN - 10 - 1] = '\0';
        
        char* newline = strchr(entries[count].message, '\n');
        if (newline) *newline = '\0';
        
        count++;
    }
    
    return count;
}

void print_summary(const struct log_entry* entries, int count) {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
    printf("\n");
}

void print_entries_by_level(const struct log_entry* entries, int count, int target_level) {
    if (!IS_VALID_LEVEL(target_level)) return;
    
    printf("%s entries:\n", level_names[target_level]);
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            printf("%s %s %s\n", entries[i].timestamp, level_names[entries[i].level], entries[i].message);
            found++;
        }
    }
    
    if (!found) {
        printf("No %s entries found.\n", level_names[target_level]);