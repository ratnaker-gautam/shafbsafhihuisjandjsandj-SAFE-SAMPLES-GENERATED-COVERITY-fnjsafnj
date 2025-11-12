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
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    char level_str[32];
    if (sscanf(line, "%31s", level_str) != 1) return 0;
    
    int level = parse_log_level(level_str);
    if (level == -1) return 0;
    
    entry->level = level;
    
    const char* message_start = strchr(line, ':');
    if (message_start == NULL) return 0;
    message_start++;
    
    while (*message_start && isspace(*message_start)) message_start++;
    
    size_t msg_len = strlen(message_start);
    if (msg_len > 0 && message_start[msg_len - 1] == '\n') {
        msg_len--;
    }
    
    if (msg_len >= sizeof(entry->message)) {
        msg_len = sizeof(entry->message) - 1;
    }
    
    strncpy(entry->message, message_start, msg_len);
    entry->message[msg_len] = '\0';
    
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
    
    printf("Enter log entries (format: LEVEL: message). Enter empty line to finish:\n");
    
    while (entry_count < 1000) {
        struct LogEntry entry;
        if (!read_log_entry(stdin, &entry)) break;
        
        if (strlen(entry.message) == 0) break;
        
        entries[entry_count] = entry;
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nLog Summary:\n");
    print_all_summaries(entries, entry_count, 0);
    
    printf("\nAll entries:\n");
    for (int i = 0; i < entry_count; i++) {
        printf("%s: %s\n", level_to_string(entries[i].level), entries[i].message);
    }
    
    return 0;
}