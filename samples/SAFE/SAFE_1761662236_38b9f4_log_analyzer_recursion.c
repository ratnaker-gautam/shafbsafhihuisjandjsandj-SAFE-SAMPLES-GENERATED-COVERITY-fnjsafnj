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
    struct LogEntry *next;
};

int parse_log_level(const char *line) {
    if (strstr(line, "[ERROR]")) return 3;
    if (strstr(line, "[WARN]")) return 2;
    if (strstr(line, "[INFO]")) return 1;
    if (strstr(line, "[DEBUG]")) return 0;
    return -1;
}

void print_log_entry(struct LogEntry *entry, int current_level) {
    if (entry == NULL) return;
    
    if (entry->level >= current_level) {
        printf("%s", entry->line);
    }
    
    print_log_entry(entry->next, current_level);
}

struct LogEntry* read_log_file(const char *filename, int *count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return NULL;
    }
    
    struct LogEntry *head = NULL;
    struct LogEntry *tail = NULL;
    char buffer[MAX_LINE_LENGTH];
    *count = 0;
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (*count >= MAX_LEVELS) {
            fprintf(stderr, "Error: Too many log entries\n");
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        struct LogEntry *new_entry = malloc(sizeof(struct LogEntry));
        if (new_entry == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            break;
        }
        
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
        
        (*count)++;
    }
    
    if (ferror(file)) {
        fprintf(stderr, "Error: Reading file failed\n");
    }
    
    fclose(file);
    return head;
}

void free_log_entries(struct LogEntry *head) {
    if (head == NULL) return;
    free_log_entries(head->next);
    free(head);
}

int count_entries_by_level(struct LogEntry *head, int target_level) {
    if (head == NULL) return 0;
    int count = (head->level == target_level) ? 1 : 0;
    return count + count_entries_by_level(head->next, target_level);
}

void print_summary(struct LogEntry *head) {
    printf("Log Summary:\n");
    printf("DEBUG entries: %d\n", count_entries_by_level(head, 0));
    printf("INFO entries: %d\n", count_entries_by_level(head, 1));
    printf("WARN entries: %d\n", count_entries_by_level(head, 2));
    printf("ERROR entries: %d\n", count_entries_by_level(head, 3));
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <logfile> <min_level>\n", argv[0]);
        fprintf(stderr, "Levels: 0=DEBUG, 1=INFO, 2=WARN, 3=ERROR\n");
        return 1;
    }
    
    char *endptr;
    long level = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || level < 0 || level > 3) {
        fprintf(stderr, "Error: Invalid level. Use 0-3\n");
        return 1;
    }
    
    int entry_count;
    struct LogEntry *log_entries = read_log_file(argv[1], &entry_count);
    if (log_entries == NULL) {
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No log entries found.\n");
        free_log_entries(log_entries);
        return 0;
    }
    
    printf("Filtered log entries (level >= %ld):\n", level);
    printf("-----------------------------------\n");
    print_log_entry(log_entries, (int)level);
    printf("-----------------------------------\n");
    print_summary(log_entries);
    
    free_log_entries(log_entries);
    return 0;
}