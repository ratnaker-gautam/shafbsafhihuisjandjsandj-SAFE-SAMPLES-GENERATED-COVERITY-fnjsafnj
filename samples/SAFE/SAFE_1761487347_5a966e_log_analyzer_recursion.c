//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    int level;
    char message[MAX_LINE_LENGTH];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return 0;
    if (strcmp(level_str, "INFO") == 0) return 1;
    if (strcmp(level_str, "WARNING") == 0) return 2;
    if (strcmp(level_str, "ERROR") == 0) return 3;
    if (strcmp(level_str, "CRITICAL") == 0) return 4;
    return -1;
}

const char* level_to_string(int level) {
    switch (level) {
        case 0: return "DEBUG";
        case 1: return "INFO";
        case 2: return "WARNING";
        case 3: return "ERROR";
        case 4: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

int count_entries_by_level(struct LogEntry* entries, int count, int target_level, int index) {
    if (index >= count) return 0;
    int current = (entries[index].level == target_level) ? 1 : 0;
    return current + count_entries_by_level(entries, count, target_level, index + 1);
}

void print_level_summary(struct LogEntry* entries, int count, int level) {
    int total = count_entries_by_level(entries, count, level, 0);
    if (total > 0) {
        printf("%s: %d entries\n", level_to_string(level), total);
    }
}

void print_all_summaries(struct LogEntry* entries, int count, int current_level) {
    if (current_level > 4) return;
    print_level_summary(entries, count, current_level);
    print_all_summaries(entries, count, current_level + 1);
}

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[MAX_LINE_LENGTH];
    char level_str[16];
    
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    line[strcspn(line, "\n")] = '\0';
    
    if (strlen(line) == 0) return 0;
    
    int scanned = sscanf(line, "%15s", level_str);
    if (scanned != 1) return 0;
    
    int level = parse_log_level(level_str);
    if (level == -1) return 0;
    
    const char* message_start = line + strlen(level_str);
    while (*message_start == ' ' || *message_start == '\t') message_start++;
    
    if (strlen(message_start) >= MAX_LINE_LENGTH) return 0;
    
    entry->level = level;
    strncpy(entry->message, message_start, MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
    
    return 1;
}

int read_all_entries(FILE* file, struct LogEntry* entries, int max_entries, int* count) {
    if (*count >= max_entries) return 0;
    if (read_log_entry(file, &entries[*count])) {
        (*count)++;
        return read_all_entries(file, entries, max_entries, count);
    }
    return 0;
}

int main(void) {
    struct LogEntry entries[1000];
    int entry_count = 0;
    
    printf("Enter log entries (format: LEVEL message). Enter empty line to finish.\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    
    int result = read_all_entries(stdin, entries, 1000, &entry_count);
    if (result == 0 && entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    print_all_summaries(entries, entry_count, 0);
    
    return 0;
}