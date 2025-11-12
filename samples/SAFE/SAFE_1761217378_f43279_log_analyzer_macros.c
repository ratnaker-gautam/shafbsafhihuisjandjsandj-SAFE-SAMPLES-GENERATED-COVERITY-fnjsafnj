//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
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
    struct tm tm = {0};
    return strptime(str, "%Y-%m-%d %H:%M:%S", &tm) != NULL;
}

int parse_level(const char* str) {
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
        return -1;
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file) && entry_count < MAX_ENTRIES) {
        char timestamp[TIMESTAMP_LEN];
        char level_str[16];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];

        if (sscanf(line, "%19s %15s %255[^\n]", timestamp, level_str, message) == 3) {
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
    return 0;
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
    if (!IS_VALID_LEVEL(level)) {
        printf("Invalid log level\n");
        return;
    }

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
    if (scanf("%255s", filename) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    if (read_log_file(filename) != 0) {
        printf("Error reading log file\n");
        return 1;
    }

    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }

    int choice;
    do {
        printf("\nLog Analyzer Menu:\n");
        printf("1. Show statistics\n");
        printf("2. Search by log level\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                print_statistics();
                break;
            case 2:
                printf("Enter log level (0=DEBUG, 1=INFO, 2=WARNING, 3=ERROR): ");
                int level;
                if (scanf("%d", &level) == 1) {
                    search_by_level(level);
                } else {
                    printf("Invalid input\n");
                    while (getchar() != '\n');
                }
                break;
            case 3:
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 3);

    return 0;
}