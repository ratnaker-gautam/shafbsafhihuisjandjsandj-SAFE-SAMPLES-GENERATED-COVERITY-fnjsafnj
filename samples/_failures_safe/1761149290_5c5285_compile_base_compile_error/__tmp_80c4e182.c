//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

LogEntry entries[MAX_ENTRIES];
int entry_count = 0;
const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

int parse_timestamp(const char* str) {
    if (!str || strlen(str) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) { if (str[i] != '-') return 0; }
        else if (i == 10) { if (str[i] != ' ') return 0; }
        else if (i == 13 || i == 16) { if (str[i] != ':') return 0; }
        else if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int parse_level(const char* str) {
    if (!str) return -1;
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) return i;
    }
    return -1;
}

int read_log_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file) && entry_count < MAX_ENTRIES) {
        if (strlen(line) >= MAX_LINE_LEN - 1) continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[16];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
        
        if (sscanf(line, "%19s %15s %[^\n]", timestamp, level_str, message) == 3) {
            if (parse_timestamp(timestamp)) {
                int level = parse_level(level_str);
                if (IS_VALID_LEVEL(level)) {
                    strncpy(entries[entry_count].timestamp, timestamp, TIMESTAMP_LEN - 1);
                    entries[entry_count].timestamp[TIMESTAMP_LEN - 1] = '\0';
                    entries[entry_count].level = level;
                    strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
                    entries[entry_count].message[sizeof(entries[entry_count].message) - 1] = '\0';
                    entry_count++;
                }
            }
        }
    }
    fclose(file);
    return 1;
}

void print_statistics() {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < entry_count; i++) {
        if (IS_VALID_INDEX(i) && IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Statistics:\n");
    printf("Total entries: %d\n", entry_count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
}

void search_by_level(int level) {
    if (!IS_VALID_LEVEL(level)) return;
    
    printf("Entries with level %s:\n", level_names[level]);
    for (int i = 0; i < entry_count; i++) {
        if (IS_VALID_INDEX(i) && entries[i].level == level) {
            printf("%s %s %s\n", entries[i].timestamp, level_names[entries[i].level], entries[i].message);
        }
    }
}

int main() {
    char filename[256];
    printf("Enter log file name: ");
    if (!fgets(filename, sizeof(filename), stdin)) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    if (!read_log_file(filename)) {
        printf("Failed to read log file\n");
        return 1;
    }
    
    printf("Log file loaded with %d entries\n\n", entry_count);
    
    int choice;
    do {
        printf("1. Show statistics\n");
        printf("2. Search by level\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                print_statistics();
                break;
            case 2