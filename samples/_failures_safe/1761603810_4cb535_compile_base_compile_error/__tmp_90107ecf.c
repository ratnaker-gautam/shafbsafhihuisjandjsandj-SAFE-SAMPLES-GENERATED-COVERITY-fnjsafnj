//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define IS_VALID_LEVEL(level) ((level) >= 0 && (level) < LOG_LEVELS)
#define IS_VALID_INDEX(idx) ((idx) >= 0 && (idx) < entry_count)
#define LEVEL_NAME(level) (level_names[(level)])

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

LogEntry entries[MAX_ENTRIES];
int entry_count = 0;
const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

int parse_timestamp(const char *str) {
    if (strlen(str) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) { if (str[i] != '-') return 0; }
        else if (i == 10) { if (str[i] != ' ') return 0; }
        else if (i == 13 || i == 16) { if (str[i] != ':') return 0; }
        else if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int parse_level(const char *str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) return i;
    }
    return -1;
}

int add_entry(const char *timestamp, int level, const char *message) {
    if (entry_count >= MAX_ENTRIES) return 0;
    if (!parse_timestamp(timestamp)) return 0;
    if (!IS_VALID_LEVEL(level)) return 0;
    if (strlen(message) >= sizeof(entries[0].message)) return 0;
    
    strncpy(entries[entry_count].timestamp, timestamp, TIMESTAMP_LEN-1);
    entries[entry_count].timestamp[TIMESTAMP_LEN-1] = '\0';
    entries[entry_count].level = level;
    strncpy(entries[entry_count].message, message, sizeof(entries[0].message)-1);
    entries[entry_count].message[sizeof(entries[0].message)-1] = '\0';
    entry_count++;
    return 1;
}

void print_summary(void) {
    int counts[LOG_LEVELS] = {0};
    for (int i = 0; i < entry_count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            counts[entries[i].level]++;
        }
    }
    
    printf("Log Summary:\n");
    printf("Total entries: %d\n", entry_count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", LEVEL_NAME(i), counts[i]);
    }
}

void print_entries_by_level(int level) {
    if (!IS_VALID_LEVEL(level)) return;
    
    printf("\n%s entries:\n", LEVEL_NAME(level));
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == level) {
            printf("[%s] %s\n", entries[i].timestamp, entries[i].message);
        }
    }
}

int main(void) {
    char line[MAX_LINE_LEN];
    char timestamp[TIMESTAMP_LEN];
    char level_str[16];
    char message[MAX_LINE_LEN];
    int level;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'done' to finish, 'summary' for statistics, 'level X' to filter\n");
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "done") == 0) break;
        
        if (strcmp(line, "summary") == 0) {
            print_summary();
            continue;
        }
        
        if (strncmp(line, "level ", 6) == 0) {
            if (sscanf(line + 6, "%15s", level_str) == 1) {
                level = parse_level(level_str);
                if (IS_VALID_LEVEL(level)) {
                    print_entries_by_level(level);
                } else {
                    printf("Invalid level: %s\n", level_str);
                }
            }
            continue;
        }
        
        if (sscanf(line, "%19s %15s %[^\n]", timestamp, level_str, message) == 3) {
            level = parse_level(level_str);
            if (IS_VALID_LEVEL(level)) {
                if (add_entry(timestamp, level, message)) {
                    printf("Added: %s %s %s\n", timestamp, level_str, message);
                } else {
                    printf("Failed to add entry\n");
                }
            } else {
                printf("Invalid log level: %s\n", level_str);
            }
        } else {
            printf("Invalid format. Use