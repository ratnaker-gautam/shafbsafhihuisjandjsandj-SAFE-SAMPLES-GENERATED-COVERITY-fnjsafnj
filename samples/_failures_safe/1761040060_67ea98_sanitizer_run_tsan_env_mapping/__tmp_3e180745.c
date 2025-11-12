//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
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

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};
int entry_count = 0;
LogEntry entries[MAX_ENTRIES];

int parse_timestamp(const char* str) {
    if (!str || strlen(str) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else if (!isdigit(str[i])) {
            return 0;
        }
    }
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

int add_entry(const char* timestamp, int level, const char* message) {
    if (entry_count >= MAX_ENTRIES) return 0;
    if (!parse_timestamp(timestamp)) return 0;
    if (!IS_VALID_LEVEL(level)) return 0;
    if (!message || strlen(message) >= sizeof(entries[0].message)) return 0;
    
    strncpy(entries[entry_count].timestamp, timestamp, TIMESTAMP_LEN - 1);
    entries[entry_count].timestamp[TIMESTAMP_LEN - 1] = '\0';
    entries[entry_count].level = level;
    strncpy(entries[entry_count].message, message, sizeof(entries[0].message) - 1);
    entries[entry_count].message[sizeof(entries[0].message) - 1] = '\0';
    entry_count++;
    return 1;
}

void print_stats() {
    int counts[LOG_LEVELS] = {0};
    for (int i = 0; i < entry_count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            counts[entries[i].level]++;
        }
    }
    
    printf("Log Statistics:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d entries\n", level_names[i], counts[i]);
    }
}

void filter_by_level(int level) {
    if (!IS_VALID_LEVEL(level)) return;
    
    printf("Entries with level %s:\n", level_names[level]);
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == level) {
            printf("[%s] %s: %s\n", entries[i].timestamp, level_names[level], entries[i].message);
        }
    }
}

int main() {
    char line[MAX_LINE_LEN];
    char timestamp[TIMESTAMP_LEN];
    char level_str[16];
    char message[MAX_LINE_LEN];
    int line_num = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter empty line to finish input\n");
    
    while (fgets(line, sizeof(line), stdin)) {
        line_num++;
        if (line[0] == '\n') break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) >= MAX_LINE_LEN - 1) {
            printf("Line %d too long, skipping\n", line_num);
            continue;
        }
        
        int parsed = sscanf(line, "%19s %15s %[^\n]", timestamp, level_str, message);
        if (parsed != 3) {
            printf("Invalid format on line %d, skipping\n", line_num);
            continue;
        }
        
        int level = parse_level(level_str);
        if (level == -1) {
            printf("Invalid level on line %d, skipping\n", line_num);
            continue;
        }
        
        if (!add_entry(timestamp, level, message)) {
            printf("Failed to add entry on line %d, skipping\n", line_num);
            continue;
        }
    }
    
    if (entry_count == 0) {
        printf("No valid entries processed\n");
        return 1;
    }
    
    printf("\nProcessing %d log entries...\n\n", entry_count);
    
    print_stats();
    printf("\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        filter_by_level(i);
        printf("\n");
    }
    
    return 0;
}