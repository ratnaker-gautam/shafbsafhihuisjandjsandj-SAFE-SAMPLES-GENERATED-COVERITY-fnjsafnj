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

const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
int level_counts[LOG_LEVELS] = {0};
struct log_entry entries[MAX_ENTRIES];
int entry_count = 0;

int parse_timestamp(const char* str) {
    if (strlen(str) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
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

void analyze_logs() {
    printf("Log Analysis Results:\n");
    printf("====================\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d entries\n", level_names[i], level_counts[i]);
    }
    
    printf("\nRecent entries:\n");
    int show_count = (entry_count < 5) ? entry_count : 5;
    for (int i = 0; i < show_count; i++) {
        printf("%s [%s] %s\n", 
               entries[i].timestamp, 
               level_names[entries[i].level],
               entries[i].message);
    }
}

int main() {
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        line_count++;
        
        if (line_count > MAX_ENTRIES) {
            printf("Maximum entries reached (%d)\n", MAX_ENTRIES);
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
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
                level_counts[entry.level]++;
                entry_count++;
            }
        } else {
            printf("Invalid log format on line %d: %s\n", line_count, line);
        }
    }
    
    if (entry_count > 0) {
        analyze_logs();
    } else {
        printf("No valid log entries found.\n");
    }
    
    return 0;
}