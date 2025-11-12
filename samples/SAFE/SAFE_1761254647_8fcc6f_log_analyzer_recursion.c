//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
    int indent_level;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    int indent = 0;
    while (temp_line[indent] == ' ' || temp_line[indent] == '\t') {
        indent++;
    }
    entry->indent_level = indent / 2;
    
    char* timestamp_end = strchr(temp_line + indent, ' ');
    if (timestamp_end == NULL) return 0;
    
    size_t timestamp_len = timestamp_end - (temp_line + indent);
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    strncpy(entry->timestamp, temp_line + indent, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    char* level_start = timestamp_end + 1;
    char* level_end = strchr(level_start, ' ');
    if (level_end == NULL) return 0;
    
    size_t level_len = level_end - level_start;
    if (level_len >= sizeof(entry->level)) return 0;
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    char* message_start = level_end + 1;
    if (strlen(message_start) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, message_start);
    
    return 1;
}

void print_log_hierarchy(struct LogEntry* entries, int count, int current, int parent_level) {
    if (current >= count) return;
    
    struct LogEntry* entry = &entries[current];
    
    if (entry->indent_level > parent_level) {
        print_log_hierarchy(entries, count, current + 1, parent_level);
        return;
    }
    
    if (entry->indent_level == parent_level) {
        for (int i = 0; i < parent_level * 2; i++) {
            printf(" ");
        }
        printf("%s [%s] %s\n", entry->timestamp, entry->level, entry->message);
        
        print_log_hierarchy(entries, count, current + 1, parent_level + 1);
    }
    
    if (entry->indent_level < parent_level) {
        return;
    }
}

void analyze_log_levels(struct LogEntry* entries, int count, int current, int* level_counts) {
    if (current >= count) return;
    
    struct LogEntry* entry = &entries[current];
    
    if (strcmp(entry->level, "ERROR") == 0) {
        level_counts[0]++;
    } else if (strcmp(entry->level, "WARN") == 0) {
        level_counts[1]++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        level_counts[2]++;
    } else if (strcmp(entry->level, "DEBUG") == 0) {
        level_counts[3]++;
    }
    
    analyze_log_levels(entries, count, current + 1, level_counts);
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (parse_log_line(line, &entries[count])) {
            count++;
        }
    }
    
    fclose(file);
    return count;
}

int main(void) {
    struct LogEntry entries[1000];
    const char* filename = "sample.log";
    
    int entry_count = read_log_file(filename, entries, 1000);
    if (entry_count == 0) {
        printf("No log entries found or file cannot be read.\n");
        return 1;
    }
    
    printf("Log Hierarchy Analysis:\n");
    printf("=======================\n");
    print_log_hierarchy(entries, entry_count, 0, 0);
    
    printf("\nLog Level Statistics:\n");
    printf("====================\n");
    int level_counts[4] = {0};
    analyze_log_levels(entries, entry_count, 0, level_counts);
    
    printf("ERROR: %d\n", level_counts[0]);
    printf("WARN:  %d\n", level_counts[1]);
    printf("INFO:  %d\n", level_counts[2]);
    printf("DEBUG: %d\n", level_counts[3]);
    
    return 0;
}