//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)
#define LEVEL_NAME(l) (level_names[(l)])
#define IS_PRINTABLE(c) ((c) >= 32 && (c) <= 126)

static const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

int parse_timestamp(const char *str, char *timestamp) {
    if (!str || !timestamp) return 0;
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        char c = str[i];
        if (!(isdigit(c) || c == '-' || c == ':' || c == ' ' || c == '.')) {
            return 0;
        }
    }
    
    strncpy(timestamp, str, TIMESTAMP_LEN - 1);
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_level(const char *str) {
    if (!str) return -1;
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int validate_message(const char *msg) {
    if (!msg) return 0;
    size_t len = strlen(msg);
    if (len == 0 || len >= MAX_LINE_LEN - TIMESTAMP_LEN - 10) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!IS_PRINTABLE(msg[i]) && msg[i] != '\n' && msg[i] != '\t') {
            return 0;
        }
    }
    return 1;
}

int read_log_entry(FILE *file, LogEntry *entry) {
    if (!file || !entry) return 0;
    
    char line[MAX_LINE_LEN];
    if (!fgets(line, sizeof(line), file)) return 0;
    
    line[strcspn(line, "\n")] = '\0';
    if (strlen(line) == 0) return 0;
    
    char timestamp[TIMESTAMP_LEN];
    char level_str[16];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
    
    if (sscanf(line, "%19s %15s %[^\n]", timestamp, level_str, message) != 3) {
        return 0;
    }
    
    if (!parse_timestamp(timestamp, entry->timestamp)) return 0;
    
    int level = parse_level(level_str);
    if (!IS_VALID_LEVEL(level)) return 0;
    entry->level = level;
    
    if (!validate_message(message)) return 0;
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs(LogEntry entries[], int count, int level_counts[]) {
    if (!entries || !level_counts || count <= 0) return;
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        level_counts[i] = 0;
    }
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            if (level_counts[entries[i].level] < INT_MAX) {
                level_counts[entries[i].level]++;
            }
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int level_counts[LOG_LEVELS];
    
    printf("Log Analyzer - Enter log entries (timestamp LEVEL message):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message\\n");
    printf("Levels: DEBUG, INFO, WARNING, ERROR\n");
    printf("Enter empty line to finish.\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Entry %d: ", entry_count + 1);
        
        if (!read_log_entry(stdin, &entries[entry_count])) {
            char buffer[MAX_LINE_LEN];
            if (fgets(buffer, sizeof(buffer), stdin)) {
                buffer[strcspn(buffer, "\n")] = '\0';
                if (strlen(buffer) == 0) break;
            }
            printf("Invalid format. Please use: YYYY-MM-DD HH:MM:SS LEVEL message\\n");
            continue;
        }
        
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count, level_counts);
    
    printf("\nLog Analysis Results