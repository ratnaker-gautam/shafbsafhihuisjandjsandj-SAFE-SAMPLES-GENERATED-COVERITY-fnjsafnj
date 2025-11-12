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

int parse_log_line(const char* line, struct log_entry* entry) {
    if (!line || !entry) return 0;
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp_part = strtok(temp, " ");
    if (!timestamp_part) return 0;
    
    if (!parse_timestamp(timestamp_part, entry->timestamp)) return 0;
    
    char* level_part = strtok(NULL, " ");
    if (!level_part) return 0;
    
    int level = parse_log_level(level_part);
    if (!IS_VALID_LEVEL(level)) return 0;
    entry->level = level;
    
    char* message_part = strtok(NULL, "");
    if (!message_part) return 0;
    
    strncpy(entry->message, message_part, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void print_summary(struct log_entry* entries, int count) {
    if (!entries || count <= 0) return;
    
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Summary:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
    printf("Total entries: %d\n", count);
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: ERROR, WARN, INFO, DEBUG\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct log_entry entry;
        if (parse_log_line(line, &entry)) {
            if (entry_count < MAX_ENTRIES) {
                entries[entry_count] = entry;
                entry_count++;
                printf("Added: %s %s %s\n", entry.timestamp, level_names[entry.level], entry.message);
            }
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count > 0) {
        print_summary(entries, entry_count);
        
        printf("\nRecent entries:\n");
        int start = (entry_count > 5) ? entry_count - 5 : 0;
        for (int i = start; i < entry_count; i++) {