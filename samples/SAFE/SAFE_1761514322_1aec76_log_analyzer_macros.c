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
#define LOG_LEVEL_WARN  1
#define LOG_LEVEL_INFO  2
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

int parse_log_level(const char* str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int read_log_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file) && entry_count < MAX_ENTRIES) {
        if (strlen(line) == 0) continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[10];
        char message[MAX_LINE_LEN];
        
        if (sscanf(line, "%19s %9s %[^\n]", timestamp, level_str, message) != 3) {
            continue;
        }
        
        if (!parse_timestamp(timestamp)) {
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (!IS_VALID_LEVEL(level)) {
            continue;
        }
        
        struct log_entry* entry = &entries[entry_count];
        strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN - 1);
        entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
        entry->level = level;
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        
        level_counts[level]++;
        entry_count++;
    }
    
    fclose(file);
    return 1;
}

void print_summary(void) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
    printf("\n");
}

void print_entries_by_level(int level) {
    if (!IS_VALID_LEVEL(level)) {
        printf("Invalid log level\n");
        return;
    }
    
    printf("Entries with level %s:\n", level_names[level]);
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == level) {
            printf("%s %s %s\n", entries[i].timestamp, level_names[entries[i].level], entries[i].message);
        }
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    if (!read_log_file(argv[1])) {
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 1;
    }
    
    print_summary();
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (level_counts[i] > 0) {
            print_entries_by_level(i);
        }
    }
    
    return 0;
}