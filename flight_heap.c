
#include <stdio.h>
#include <string.h>

#define MAX 100

typedef struct {
    char flightNo[10];
    int  time;          /* stored as HHMM, e.g. 14:30 -> 1430 */
} Flight;

Flight heap[MAX];
int size = 0;

/* ---------- helper functions ---------- */

void swap(Flight *a, Flight *b) {
    Flight temp = *a;
    *a = *b;
    *b = temp;
}

/* Move a newly inserted flight up until heap property is restored */
void heapifyUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap[i].time < heap[parent].time) {
            swap(&heap[i], &heap[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

/* Move the root flight down until heap property is restored */
void heapifyDown(int i) {
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < size && heap[left].time < heap[smallest].time)
            smallest = left;
        if (right < size && heap[right].time < heap[smallest].time)
            smallest = right;

        if (smallest != i) {
            swap(&heap[i], &heap[smallest]);
            i = smallest;
        } else {
            break;
        }
    }
}

/* ---------- main operations ---------- */

void insertFlight(char flightNo[], int hh, int mm) {
    if (size == MAX) {
        printf("Heap is full! Cannot add more flights.\n");
        return;
    }

    /* check for duplicate flight number */
    for (int i = 0; i < size; i++) {
        if (strcmp(heap[i].flightNo, flightNo) == 0) {
            printf("Flight %s already exists!\n", flightNo);
            return;
        }
    }

    strcpy(heap[size].flightNo, flightNo);
    heap[size].time = hh * 100 + mm;
    heapifyUp(size);
    size++;
    printf("Flight %s added (Departure %02d:%02d).\n", flightNo, hh, mm);
}

void viewFlights() {
    if (size == 0) {
        printf("No flights scheduled.\n");
        return;
    }
    printf("\nFlights in heap (array order):\n");
    printf("Index  Flight No   Departure\n");
    printf("-----------------------------\n");
    for (int i = 0; i < size; i++) {
        printf("%-6d %-11s %02d:%02d\n", i, heap[i].flightNo,
               heap[i].time / 100, heap[i].time % 100);
    }
}

void nextFlight() {
    if (size == 0) {
        printf("No flights scheduled.\n");
        return;
    }
    printf("Next flight: %s at %02d:%02d\n", heap[0].flightNo,
           heap[0].time / 100, heap[0].time % 100);
}

void dispatchFlight() {
    if (size == 0) {
        printf("No flights to dispatch.\n");
        return;
    }
    printf("Flight %s (%02d:%02d) has departed and is removed.\n",
           heap[0].flightNo, heap[0].time / 100, heap[0].time % 100);

    heap[0] = heap[size - 1];   /* move last flight to root */
    size--;
    heapifyDown(0);             /* restore heap property */
}

/* ---------- main ---------- */

int main() {
    int choice, hh, mm;
    char flightNo[10];

    while (1) {
        printf("\n===== Flight Departure Priority Manager =====\n");
        printf("1. Add a new flight\n");
        printf("2. View all flights\n");
        printf("3. Show next flight to depart\n");
        printf("4. Dispatch (remove) next flight\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');   /* clear input buffer */
            continue;
        }

        switch (choice) {
            case 1:
                printf("Enter flight number (e.g. AI101): ");
                scanf("%9s", flightNo);
                printf("Enter departure time (HH MM, 24-hour): ");
                if (scanf("%d %d", &hh, &mm) != 2 ||
                    hh < 0 || hh > 23 || mm < 0 || mm > 59) {
                    printf("Invalid time!\n");
                    while (getchar() != '\n');
                } else {
                    insertFlight(flightNo, hh, mm);
                }
                break;
            case 2:
                viewFlights();
                break;
            case 3:
                nextFlight();
                break;
            case 4:
                dispatchFlight();
                break;
            case 5:
                printf("Exiting program. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
}
