#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
	int page_ref;
	int last_used;
} lru_node;

int const memorysize = 7;
int fifo[7];
lru_node lru[7];
int head;
int *pagereference;
int size;

// insert items into the respective lists.
int insert_item_fifo(int page) {
	fifo[head] = page;
	head = (head+1)%memorysize;
	return 0;
}

int insert_item_lru(int page, int full) {
	if(full){
		int i;
		int temp = lru[0].last_used;
		int tempindex = 0;
		for (i = 1; i < memorysize; i++) {
			if(lru[i].last_used < temp) {
				temp = lru[i].last_used;
				tempindex = i;
			}
		}
		lru[tempindex].page_ref = page;
	} else {
		lru[head].page_ref = page;
		head = (head+1)%memorysize;
	}
}

//checks if a page fault occured.
int check_fault_fifo(int page) {
	int i;
	int fault = 1;
	for (i = 0; i < memorysize; i++) {
		if (fifo[i] == page) {
			fault = 0;
		}
	}
	return fault;
}

int check_fault_lru(int page, int cur) {
	int i;
	int fault = 1;
	for (i = 0; i < memorysize; i++) {
		if (lru[i].page_ref == page) {
			fault = 0;
			lru[i].last_used = cur;
			break;
		}
	}
	return fault;
}


//runs the two algorithms.
int first_in_first_out() {
	int numfaults = 0;
	int i;
	for (i = 0; i < size; i++) {
		//printf("Current array \n------------- \n");
		//display_array_fifo();
		//printf("Attempting to access %i \n", pagereference[i]);
		if(check_fault_fifo(pagereference[i])) {
			//printf("Page Fault\n");
			insert_item_fifo(pagereference[i]);
			numfaults++;
			//printf("New array \n------------- \n");
			//display_array_fifo();
		}
	}
	return numfaults;
}

int least_recently_used() {
	int numfaults = 0;
	int i;
	int count = 0;
	head = 0;
	for (i = 0; i < size; i++) {
		//printf("Current array \n------------- \n");
		//display_array_lru();
		//printf("Attempting to access %i \n", pagereference[i]);
		if(check_fault_lru(pagereference[i], i)) {
			//printf("Page Fault\n");
			if (count > 7) {
				insert_item_lru(pagereference[i], 1);
			} else {
				insert_item_lru(pagereference[i], 0);
			}
			count++;
			numfaults++;
			//printf("New array \n------------- \n");
			//display_array_lru();
		}
	}
	return numfaults;
}

int display_array_fifo() {
	int i;
	for (i = 0; i < memorysize; i++) {
		printf("%i ", fifo[i]);
	}
	printf("\n");
}

int display_array_lru() {
	int i;
	for (i = 0; i < memorysize; i++) {
		printf("%i ", lru[i].page_ref);
	}
	printf("\n");
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	int i;
	int r;
	size = atoi(argv[1]);
	pagereference = malloc(sizeof(int)*size);
	for (i = 0; i < size; i++) {
		pagereference[i] = rand() % 10;
	}
	printf("\n");

	int numfaults = first_in_first_out();

	printf("First in first out statistics\n");
	printf("Fault count: %i\n", numfaults);
	printf("Miss rate: %f\n\n", numfaults/(size*1.0)*100);

	numfaults = least_recently_used();
	printf("Least recently used statistics\n");
	printf("Fault count: %i\n", numfaults);
	printf("Miss rate: %f\n", numfaults/(size*1.0)*100);

}
