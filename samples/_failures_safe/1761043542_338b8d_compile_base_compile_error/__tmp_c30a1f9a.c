//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4
#define RESPONSE_BUF_SIZE 16

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
    if (strlen(str) != 19) return 0;
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
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    
    char timestamp[TIMESTAMP_LEN];
    char level_str[10];
    char message[MAX_LINE_LEN];
    
    if (sscanf(line, "%19s %9s %[^\n]", timestamp, level_str, message) != 3) {
        return 0;
    }
    
    if (!parse_timestamp(timestamp)) return 0;
    
    int level = parse_level(level_str);
    if (!IS_VALID_LEVEL(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    entry->level = level;
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void print_summary(void) {
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", entry_count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
    printf("\n");
}

void print_entries_by_level(int level) {
    if (!IS_VALID_LEVEL(level)) return;
    
    printf("Entries with level %s:\n", level_names[level]);
    printf("====================\n");
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == level) {
            printf("%s %s %s\n", entries[i].timestamp, level_names[level], entries[i].message);
        }
    }
    printf("\n");
}

int main(void) {
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: ERROR, WARN, INFO, DEBUG\n");
    printf("Enter empty line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            level_counts[entries[entry_count].level]++;
            entry_count++;
            printf("Added entry %d\n", entry_count);
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    print_summary();
    
    printf("View entries by level? (y/n): ");
    if (fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == 'y' || line[0] == 'Y') {
            for (int i = 0; i < LOG_LEVELS; i++) {
                if (level_counts[i] > 0) {
                    printf("Show %s entries? (y/n):