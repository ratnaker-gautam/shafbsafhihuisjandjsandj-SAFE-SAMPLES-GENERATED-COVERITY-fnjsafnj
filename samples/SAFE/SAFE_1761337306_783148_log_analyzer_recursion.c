//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char line[MAX_LINE_LENGTH];
    int level;
    struct LogEntry* next;
};

int parse_log_level(const char* line) {
    if (strstr(line, "ERROR")) return 3;
    if (strstr(line, "WARN")) return 2;
    if (strstr(line, "INFO")) return 1;
    if (strstr(line, "DEBUG")) return 0;
    return -1;
}

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int* counts) {
    if (start > end) return;
    
    struct LogEntry* current = entries;
    for (int i = 0; i < start && current != NULL; i++) {
        current = current->next;
    }
    
    if (current == NULL) return;
    
    int level = current->level;
    if (level >= 0 && level <= 3) {
        counts[level]++;
    }
    
    analyze_log_recursive(entries, start + 1, end, counts);
}

struct LogEntry* read_log_file(const char* filename, int* total_lines) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) return NULL;
    
    struct LogEntry* head = NULL;
    struct LogEntry* tail = NULL;
    char buffer[MAX_LINE_LENGTH];
    *total_lines = 0;
    
    while (fgets(buffer, sizeof(buffer), file) != NULL && *total_lines < MAX_LEVELS) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        if (len >= sizeof(buffer)) {
            continue;
        }
        
        struct LogEntry* new_entry = malloc(sizeof(struct LogEntry));
        if (new_entry == NULL) break;
        
        strncpy(new_entry->line, buffer, sizeof(new_entry->line) - 1);
        new_entry->line[sizeof(new_entry->line) - 1] = '\0';
        new_entry->level = parse_log_level(buffer);
        new_entry->next = NULL;
        
        if (head == NULL) {
            head = new_entry;
            tail = new_entry;
        } else {
            tail->next = new_entry;
            tail = new_entry;
        }
        
        (*total_lines)++;
    }
    
    fclose(file);
    return head;
}

void free_log_entries(struct LogEntry* head) {
    while (head != NULL) {
        struct LogEntry* next = head->next;
        free(head);
        head = next;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    int total_lines = 0;
    struct LogEntry* entries = read_log_file(argv[1], &total_lines);
    
    if (entries == NULL) {
        fprintf(stderr, "Error: Could not read log file\n");
        return 1;
    }
    
    if (total_lines == 0) {
        printf("No log entries found\n");
        free_log_entries(entries);
        return 0;
    }
    
    int counts[4] = {0};
    analyze_log_recursive(entries, 0, total_lines - 1, counts);
    
    printf("Log Analysis Results:\n");
    printf("DEBUG entries: %d\n", counts[0]);
    printf("INFO entries: %d\n", counts[1]);
    printf("WARN entries: %d\n", counts[2]);
    printf("ERROR entries: %d\n", counts[3]);
    printf("Total entries processed: %d\n", total_lines);
    
    free_log_entries(entries);
    return 0;
}