//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int parse_level(const char* str) {
    if (!str) return -1;
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strncmp(str, level_names[i], strlen(level_names[i])) == 0) {
            return i;
        }
    }
    return -1;
}

int parse_log_line(const char* line, struct log_entry* entry) {
    if (!line || !entry) return 0;
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp_start = temp;
    char* level_start = strchr(temp, ' ');
    if (!level_start) return 0;
    *level_start = '\0';
    level_start++;
    
    if (!parse_timestamp(timestamp_start, entry->timestamp)) return 0;
    
    char* message_start = strchr(level_start, ' ');
    if (!message_start) return 0;
    *message_start = '\0';
    message_start++;
    
    entry->level = parse_level(level_start);
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
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
            printf("%s [%s] %s\n", entries[i].timestamp, level_names[level], entries[i].message);
        }
    }
    printf("\n");
}

int main(void) {
    char line[MAX_LINE_LEN];
    int line_num = 0;
    
    printf("Enter log entries (empty line to finish):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line_num++;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            level_counts[entries[entry_count].level]++;
            entry_count++;
        } else {
            printf("Invalid log format on line %d: %s\n", line_num, line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    print_summary();
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (level