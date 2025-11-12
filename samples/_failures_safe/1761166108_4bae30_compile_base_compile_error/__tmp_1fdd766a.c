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

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

static const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
static int level_counts[LOG_LEVELS] = {0};
static struct log_entry entries[MAX_ENTRIES];
static int entry_count = 0;

int parse_timestamp(const char* str) {
    if (strlen(str) != TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
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
    return 1;
}

int parse_level(const char* str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int parse_log_line(const char* line, struct log_entry* entry) {
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    parts[0] = strtok(temp, " ");
    parts[1] = strtok(NULL, " ");
    parts[2] = strtok(NULL, "");
    
    if (!parts[0] || !parts[1] || !parts[2]) return 0;
    
    if (!parse_timestamp(parts[0])) return 0;
    
    int level = parse_level(parts[1]);
    if (level == -1) return 0;
    
    strncpy(entry->timestamp, parts[0], TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    entry->level = level;
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void generate_sample_logs(void) {
    const char* sample_logs[] = {
        "2024-01-15 10:30:25 INFO System started successfully",
        "2024-01-15 10:31:10 WARN High memory usage detected",
        "2024-01-15 10:32:45 ERROR Database connection failed",
        "2024-01-15 10:33:20 DEBUG Processing user request",
        "2024-01-15 10:34:05 INFO User login successful",
        "2024-01-15 10:35:30 ERROR File not found: config.xml",
        "2024-01-15 10:36:15 WARN Slow response time detected",
        "2024-01-15 10:37:40 INFO Backup completed",
        "2024-01-15 10:38:25 DEBUG Cache hit ratio: 85%",
        "2024-01-15 10:39:10 INFO Session timeout"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && entry_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            level_counts[entries[entry_count].level]++;
            entry_count++;
        }
    }
}

void print_statistics(void) {
    printf("Log Analysis Results:\n");
    printf("=====================\n");
    printf("Total entries: %d\n", entry_count);
    printf("\nBreakdown by level:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%-6s: %d entries\n", level_names[i], level_counts[i]);
    }
    
    printf("\nRecent entries:\n");
    int show_count = (entry_count > 5) ? 5 : entry_count;
    for (int i = 0; i < show_count; i++) {
        int idx = entry_count - show_count + i;
        printf("%s %-5s %s\n", entries[idx].timestamp, 
               level_names[entries[idx].level], entries[idx].message);
    }
}

int main(void) {
    printf("