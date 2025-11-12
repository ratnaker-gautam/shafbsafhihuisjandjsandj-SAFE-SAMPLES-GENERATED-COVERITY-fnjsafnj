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

#define IS_VALID_LEVEL(c) ((c) == 'E' || (c) == 'W' || (c) == 'I' || (c) == 'D')

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
int level_counts[LOG_LEVELS] = {0};

int parse_timestamp(const char* str, char* timestamp) {
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
    }
    strncpy(timestamp, str, TIMESTAMP_LEN - 1);
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_log_level(char level_char) {
    switch (level_char) {
        case 'E': return LOG_LEVEL_ERROR;
        case 'W': return LOG_LEVEL_WARN;
        case 'I': return LOG_LEVEL_INFO;
        case 'D': return LOG_LEVEL_DEBUG;
        default: return -1;
    }
}

int read_log_entries(LogEntry entries[]) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter empty line to finish.\n");
    
    while (count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n' || line[0] == '\0') break;
        
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) < TIMESTAMP_LEN + 3) {
            printf("Invalid log entry format.\n");
            continue;
        }
        
        LogEntry entry;
        if (!parse_timestamp(line, entry.timestamp)) {
            printf("Invalid timestamp format.\n");
            continue;
        }
        
        char level_char = line[TIMESTAMP_LEN + 1];
        if (!IS_VALID_LEVEL(level_char)) {
            printf("Invalid log level.\n");
            continue;
        }
        
        entry.level = parse_log_level(level_char);
        if (entry.level == -1) {
            printf("Invalid log level.\n");
            continue;
        }
        
        const char* message_start = line + TIMESTAMP_LEN + 3;
        if (strlen(message_start) >= sizeof(entry.message)) {
            printf("Message too long.\n");
            continue;
        }
        strncpy(entry.message, message_start, sizeof(entry.message) - 1);
        entry.message[sizeof(entry.message) - 1] = '\0';
        
        entries[count] = entry;
        level_counts[entry.level]++;
        count++;
    }
    
    return count;
}

void print_summary(int total_entries) {
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", total_entries);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", level_names[i], level_counts[i], 
               total_entries > 0 ? (level_counts[i] * 100.0 / total_entries) : 0.0);
    }
}

void print_entries_by_level(LogEntry entries[], int count, int target_level) {
    printf("\n%s entries:\n", level_names[target_level]);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            printf("%s %s %s\n", entries[i].timestamp, 
                   level_names[entries[i].level], entries[i].message);
            found++;
        }
    }
    if (!found) {
        printf("No %s entries found.\n", level_names[target_level]);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = read_log_entries(entries);
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    print_summary(entry_count);
    
    printf("\nView entries by level (E=Error, W=Warning, I=Info, D=Debug): ");
    char choice;
    if (scanf(" %c", &choice) ==