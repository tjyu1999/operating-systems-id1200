#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define CYLINDER 5000
#define REQUESTS 1000

int fcfs(int* req, int init);
int sstf(int* req, int init);
int scan(int* req, int init);
int cscan(int* req, int init);
int look(int* req, int init);
int clook(int* req, int init);

int req[REQUESTS];

int compare(const void* a, const void* b){    
    if(*((int *)a) > *((int *)b)) return 1;
    if(*((int *)a) < *((int *)b)) return -1;
    return 0;
}

int fcfs(int* req, int init){
    int res = 0;
    for(int i = 0, curr = init; i < REQUESTS; curr = req[i++]) res += abs(curr - req[i]);
    
    return res;
}

int sstf(int* req, int init){
    int res = 0;
    int adj = 0;
    
    int* sorted_req = malloc(REQUESTS * sizeof(int));
    memcpy(sorted_req, req, REQUESTS * sizeof(int));
    qsort(sorted_req, REQUESTS, sizeof(*req), compare);
    while(abs(sorted_req[adj + 1] - init) <= abs(sorted_req[adj] - init)) adj++;

    res = abs(sorted_req[adj] - init);
    int curr_val = sorted_req[adj];
    int l_pos = adj - 1;
    int r_pos = adj + 1;

    while(l_pos > 0 || r_pos < REQUESTS - 1){
        int l_diff = l_pos > 0 ? abs(sorted_req[l_pos - 1] - curr_val) : CYLINDER + 1;
        int r_diff = r_pos < (REQUESTS - 1) ? abs(sorted_req[r_pos + 1] - curr_val) : CYLINDER + 1;

        if(l_diff < r_diff){
            res += l_diff;
            l_pos--;
            curr_val = sorted_req[l_pos];
        }
        else{
            res += r_diff;
            r_pos++;
            curr_val = sorted_req[r_pos];
        }
    }

    return res;
}

int scan(int* req, int init){
    int res = 0;
    int adj = 0;
    int curr = init;

    int* sorted_req = malloc(REQUESTS * sizeof(int));
    memcpy(sorted_req, req, REQUESTS * sizeof(int));
    qsort(sorted_req, REQUESTS, sizeof(*req), compare);
    while(abs(sorted_req[adj + 1] - init) <= abs(sorted_req[adj] - init)) adj++;

    for(int i = adj; i >= 0 && adj >= 0; i--){
        res += abs(sorted_req[i] - curr);
        curr = sorted_req[i];
    }

    res += curr;
    curr = 0;
    for(int i = adj + 1; i < REQUESTS && adj < REQUESTS; i++){
        res += abs(sorted_req[i] - curr);
        curr = sorted_req[i];
    }
    
    return res;
}

int cscan(int* req, int init){
    int res = 0;
    int adj = 0;

    int* sorted_req = malloc(REQUESTS * sizeof(int));
    memcpy(sorted_req, req, REQUESTS * sizeof(int));
    qsort(sorted_req, REQUESTS, sizeof(*req), compare);
    while(abs(sorted_req[adj + 1] - init) <= abs(sorted_req[adj] - init)) adj++;

    int curr = sorted_req[adj];
    for(int i = adj, j = 0; j < REQUESTS; j++){
        int pos = i++ % REQUESTS;
        res += abs(sorted_req[pos] - curr);
        curr = sorted_req[pos];
    }

    res += (2 * CYLINDER) - sorted_req[REQUESTS - 1] + sorted_req[0];

    return res;
}

int look(int* req, int init){
    int res = 0;
    int adj = 0;
    int curr = init;

    int* sorted_req = malloc(REQUESTS * sizeof(int));
    memcpy(sorted_req, req, REQUESTS * sizeof(int));
    qsort(sorted_req, REQUESTS, sizeof(*req), compare);
    while(abs(sorted_req[adj + 1] - init) <= abs(sorted_req[adj] - init)) adj++;

    for(int i = adj; i >= 0 && adj >= 0; i--){
        res += abs(sorted_req[i] - curr);
        curr = sorted_req[i];
    }

    for(int i = adj + 1; i < REQUESTS && adj < REQUESTS; i++){
        res += abs(sorted_req[i] - curr);
        curr = sorted_req[i];
    }
    
    return res;
}

int clook(int* req, int init){
    int res = 0;
    int adj = 0;
    int curr = init;

    int* sorted_req = malloc(REQUESTS * sizeof(int));
    memcpy(sorted_req, req, REQUESTS * sizeof(int));
    qsort(sorted_req, REQUESTS, sizeof(*req), compare);
    while(abs(sorted_req[adj + 1] - init) <= abs(sorted_req[adj] - init)) adj++;

    for(int i = adj, j = 0; j < REQUESTS; j++){
        res += abs(sorted_req[i % REQUESTS] - curr);
        curr = sorted_req[i++ % REQUESTS];
    }
    
    return res;
}

int main(int argc, char** argv){
    if(argc < 2) exit(EXIT_FAILURE);
    if(atoi(argv[1]) > 4999) exit(EXIT_FAILURE);

    int init = atoi(argv[1]);
    for(int i = 0; i < REQUESTS; i++) req[i] = rand() % CYLINDER;
    
    printf("Total head movement\n");
    printf("FCFS : %d\n", fcfs(req, init));
    printf("SSTF : %d\n", sstf(req, init));
    printf("SCAN : %d\n", scan(req, init));
    printf("CSCAN: %d\n", cscan(req, init));
    printf("LOOK : %d\n", look(req, init));
    printf("CLOOK: %d\n", clook(req, init));
    
    return 0;
}
