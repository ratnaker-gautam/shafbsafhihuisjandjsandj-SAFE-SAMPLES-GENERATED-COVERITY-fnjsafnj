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

#define IS_VALID_LEVEL(level) ((level) >= 0 && (level) < LOG_LEVELS)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

static const char *level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
static int level_counts[LOG_LEVELS] = {0};
static struct log_entry entries[MAX_ENTRIES];
static int entry_count = 0;

int parse_timestamp(const char *str, char *timestamp) {
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
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
        timestamp[i] = str[i];
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_log_level(const char *str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int parse_log_line(const char *line, struct log_entry *entry) {
    char level_str[16];
    char message[MAX_LINE_LEN];
    
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    
    int parsed = sscanf(line, "%19s %15s %[^\n]", 
                       entry->timestamp, level_str, message);
    if (parsed != 3) return 0;
    
    if (!parse_timestamp(entry->timestamp, entry->timestamp)) return 0;
    
    entry->level = parse_log_level(level_str);
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
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
            printf("%s [%s] %s\n", entries[i].timestamp, 
                   level_names[entries[i].level], entries[i].message);
        }
    }
    printf("\n");
}

int main(void) {
    char line[MAX_LINE_LEN];
    int choice;
    
    printf("Enter log entries (empty line to finish):\n");
    
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
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    do {
        printf("1. Show summary\n");
        printf("2. Show ERROR entries\n");
        printf("3. Show WARN entries\n");
        printf("4. Show INFO entries\n");
        printf("5. Show DEBUG entries\n");
        printf("6. Exit\n");
        printf("Choose option: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        
        while (getchar() != '\n');
        
        switch (choice) {
            case 1:
                print_summary();
                break;
            case 2:
                print_entries_by_level(LOG_