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
    char level_str[32];
    
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    line[strcspn(line, "\n")] = '\0';
    
    if (strlen(line) == 0) return 0;
    
    char* timestamp = strtok(line, " ");
    char* level_token = strtok(NULL, " ");
    
    if (timestamp == NULL || level_token == NULL) return 0;
    
    if (strlen(level_token) >= sizeof(level_str)) return 0;
    strcpy(level_str, level_token);
    
    int level = parse_log_level(level_str);
    if (level == -1) return 0;
    
    char* message = strtok(NULL, "");
    if (message == NULL) return 0;
    
    entry->level = level;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, message);
    
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

void print_entries_by_level(struct LogEntry* entries, int count, int target_level, int index) {
    if (index >= count) return;
    if (entries[index].level == target_level) {
        printf("%s: %s\n", level_to_string(target_level), entries[index].message);
    }
    print_entries_by_level(entries, count, target_level, index + 1);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", argv[1]);
        return 1;
    }
    
    struct LogEntry entries[MAX_LEVELS];
    int entry_count = 0;
    
    read_all_entries(file, entries, MAX_LEVELS, &entry_count);
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("Log Summary:\n");
    print_all_summaries(entries, entry_count, 0);
    
    printf("\nDetailed entries by level:\n");
    for (int level = 0; level <= 4; level++) {
        int count = count_entries_by_level(entries, entry_count, level, 0);
        if (count > 0) {
            printf("\n%s entries:\n", level_to_string(level));
            print_entries_by_level(entries, entry_count, level, 0);
        }
    }
    
    return 0;
}