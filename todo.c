// todo.c
// Simple priority heap stored in array. Save/load as priority|description

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 1000
#define DESC_LEN 200

typedef struct Task {
    int priority;
    char desc[DESC_LEN];
} Task;

static Task heap[MAX_TASKS];
static int heap_size = 0;
static int use_max_heap = 1; // 1 => max-heap, 0 => min-heap

const char *get_current_user(); // from auth.c

static int cmp(int a, int b) {
    if (use_max_heap) {
        return a > b;
    }
    return a < b;
}

static void swap_tasks(int i, int j) {
    Task t = heap[i];
    heap[i] = heap[j];
    heap[j] = t;
}

static void heapify_up(int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (cmp(heap[idx].priority, heap[parent].priority)) {
            swap_tasks(idx, parent);
            idx = parent;
        } else {
            break;
        }
    }
}

static void heapify_down(int idx) {
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int best = idx;

        if (left < heap_size && cmp(heap[left].priority, heap[best].priority)) {
            best = left;
        }
        if (right < heap_size && cmp(heap[right].priority, heap[best].priority)) {
            best = right;
        }

        if (best != idx) {
            swap_tasks(idx, best);
            idx = best;
        } else {
            break;
        }
    }
}

void add_task_to_heap(int priority, const char *desc) {
    if (heap_size >= MAX_TASKS) {
        printf("Task list full.\n");
        return;
    }
    heap[heap_size].priority = priority;
    strncpy(heap[heap_size].desc, desc, DESC_LEN - 1);
    heap[heap_size].desc[DESC_LEN - 1] = '\0';
    heapify_up(heap_size);
    heap_size++;
}

void view_tasks_heap() {
    if (heap_size == 0) {
        printf("No tasks.\n");
        return;
    }
    printf("\nTasks (top-first):\n");
    for (int i = 0; i < heap_size; i++) {
        printf("%d. [P:%d] %s\n", i + 1, heap[i].priority, heap[i].desc);
    }
}

void pop_top_task() {
    if (heap_size == 0) {
        printf("No tasks to complete.\n");
        return;
    }
    printf("Completing task: [P:%d] %s\n", heap[0].priority, heap[0].desc);
    heap[0] = heap[heap_size - 1];
    heap_size--;
    heapify_down(0);
}

void save_todo_for_user(const char *username) {
    char fname[200];
    snprintf(fname, sizeof(fname), "%s_todo.txt", username);
    FILE *fp = fopen(fname, "w");
    if (!fp) {
        return;
    }

    for (int i = 0; i < heap_size; i++) {
        fprintf(fp, "%d|%s\n", heap[i].priority, heap[i].desc);
    }

    if (fclose(fp) != 0) {
        // ignore
    }
}

void load_todo_for_user(const char *username) {
    heap_size = 0;
    char fname[200];
    snprintf(fname, sizeof(fname), "%s_todo.txt", username);
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        char *tok = strtok(line, "|");
        if (!tok) {
            continue;
        }
        int p = atoi(tok);
        tok = strtok(NULL, "|");
        if (!tok) {
            continue;
        }
        add_task_to_heap(p, tok);
    }

    if (fclose(fp) != 0) {
        // ignore
    }
}

void todo_menu() {
    const char *user = get_current_user();
    if (!user || user[0] == '\0') {
        printf("Please login first.\n");
        return;
    }

    load_todo_for_user(user);

    int ch = 0;
    char desc[DESC_LEN];
    int pr;
    do {
        printf("\n=== TO-DO LIST MENU ===\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Complete Top Task\n");
        printf("4. Toggle Min/Max (current: %s)\n", use_max_heap ? "Max-Heap" : "Min-Heap");
        printf("5. Save & Go Back\n");
        printf("Enter choice: ");
        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n') {
                // consume
            }
            ch = 0;
        }
        getchar();

        switch (ch) {
            case 1: {
                printf("Enter priority (int): ");
                if (scanf("%d", &pr) != 1) {
                    pr = 0;
                }
                getchar();
                printf("Enter description: ");
                if (fgets(desc, sizeof(desc), stdin) != NULL) {
                    desc[strcspn(desc, "\n")] = '\0';
                } else {
                    desc[0] = '\0';
                }
                add_task_to_heap(pr, desc);
            } break;

            case 2: {
                view_tasks_heap();
            } break;

            case 3: {
                pop_top_task();
            } break;

            case 4: {
                use_max_heap = !use_max_heap;
                printf("Toggled heap mode. New inserts will use the selected mode.\n");
            } break;

            case 5: {
                save_todo_for_user(user);
                return;
            } break;

            default: {
                printf("Invalid choice.\n");
            } break;
        }
    } while (1);
}
