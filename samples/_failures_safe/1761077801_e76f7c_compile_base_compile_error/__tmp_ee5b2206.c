//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
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

#define IS_VALID_LEVEL(l) ((l) >= LOG_LEVEL_ERROR && (l) <= LOG_LEVEL_DEBUG)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

static const char *level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
static int level_counts[LOG_LEVELS] = {0};
static struct log_entry entries[MAX_ENTRIES];
static int entry_count = 0;

static int parse_timestamp(const char *str) {
    if (strlen(str) != 19) return 0;
    if (str[4] != '-' || str[7] != '-' || str[10] != ' ' || str[13] != ':' || str[16] != ':') return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7 || i == 10 || i == 13 || i == 16) continue;
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

static int parse_level(const char *str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

static int parse_log_line(const char *line, struct log_entry *entry) {
    char level_str[16];
    char timestamp[TIMESTAMP_LEN];
    
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    
    int scanned = sscanf(line, "%19s %15s %[^\n]", timestamp, level_str, entry->message);
    if (scanned != 3) return 0;
    
    if (!parse_timestamp(timestamp)) return 0;
    
    int level = parse_level(level_str);
    if (!IS_VALID_LEVEL(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    entry->level = level;
    
    return 1;
}

static void print_summary(void) {
    printf("\nLog Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", entry_count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
}

static void print_entries_by_level(int level) {
    if (!IS_VALID_LEVEL(level)) return;
    
    printf("\n%s entries:\n", level_names[level]);
    printf("------------\n");
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == level) {
            printf("%s [%s] %s\n", entries[i].timestamp, level_names[level], entries[i].message);
        }
    }
}

int main(void) {
    char line[MAX_LINE_LEN];
    int choice;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct log_entry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            level_counts[entry.level]++;
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
        printf("\nOptions:\n");
        printf("1. Show summary\n");
        printf("2. Show ERROR entries\n");
        printf("3. Show WARN entries\n");
        printf("4. Show INFO entries\n");
        printf("5. Show DEBUG entries\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                print_summary();
                break;
            case 2:
                print_entries_by_level(LOG_LEVEL_ERROR);
                break;
            case 3:
                print_entries_by_level(LOG_LEVEL_WARN);
                break;
            case 4:
                print_entries_by_level(LOG