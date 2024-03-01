# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>

# define CYLINDERS 200
# define REQUESTS    8

int fcfs(int* req, int head){
    int move = 0;
    int curr = head;
    for(int i = 0; i < REQUESTS; i++){
        move += abs(req[i] - curr);
        curr = req[i];
    }
    
    return move;
}

int sstf(int* req, int head){
    int move = 0;
    int visited[REQUESTS] = {0};
    
    for(int i = 0; i < REQUESTS; i++){
        int shortest = INT_MAX;
        int diff;
        int next = -1;
        
        for(int j = 0; j < REQUESTS; j++){
            if(!visited[j]){
                diff = abs(head - req[j]);
                if(diff < shortest){
                    shortest = diff;
                    next = j;
                }
            }
        }
        
        if(next != -1){
            head = req[next];
            move += shortest;
            visited[next] = 1;
        }
    }

    return move;
}

int scan(int* req, int head){
    int move = 0;
    int* sorted_req = malloc(REQUESTS * sizeof(int));
    memcpy(sorted_req, req, REQUESTS * sizeof(int));
    
    for(int i = 0; i < REQUESTS; i++){
        for(int j = 0; j < REQUESTS - i - 1; j++){
            if(sorted_req[j] > sorted_req[j + 1]){
                int tmp = sorted_req[j];
                sorted_req[j] = sorted_req[j + 1];
                sorted_req[j + 1] = tmp;
            }
        }
    }

    int pos = 0;
    while(pos < REQUESTS && sorted_req[pos] < head) pos++;
    int direction = head < CYLINDERS / 2 ? -1 : 1;
    pos = direction > 1 ? pos : pos - 1;
    int opposite = pos - direction;
    
    while(1){
        move += abs(head - sorted_req[pos]);
        head = sorted_req[pos];
        
        if(pos == 0 || pos == REQUESTS - 1) break;
        pos += direction;
    }
    
    direction *= -1;
    move += direction > 0 ? head : CYLINDERS - head - 1;
    head = direction > 0 ? 0 : CYLINDERS - 1;
    pos = opposite;
    
    while(pos >= 0 && pos < REQUESTS){
        move += abs(head - sorted_req[pos]);
        head = sorted_req[pos];
        pos += direction;
    }
    
    return move;
}

int cscan(int* req, int head){
    int move = 0;
    int* sorted_req = malloc(REQUESTS * sizeof(int));
    memcpy(sorted_req, req, REQUESTS * sizeof(int));
    
    for(int i = 0; i < REQUESTS; i++){
        for(int j = 0; j < REQUESTS - i - 1; j++){
            if(sorted_req[j] > sorted_req[j + 1]){
                int tmp = sorted_req[j];
                sorted_req[j] = sorted_req[j + 1];
                sorted_req[j + 1] = tmp;
            }
        }
    }

    int pos = 0;
    while(pos < REQUESTS && sorted_req[pos] < head) pos++;
    // int direction = head < CYLINDERS / 2 ? -1 : 1;
    int direction = 1;
    pos = direction > 1 ? pos : pos - 1;
    int opposite = pos - direction;
    
    while(1){
        move += abs(head - sorted_req[pos]);
        head = sorted_req[pos];
        
        if(pos == 0 || pos == REQUESTS - 1) break;
        pos += direction;
    }
    
    printf("%d\n", move);
    
    move += direction > 0 ? CYLINDERS - head - 1 : head;
    head = direction > CYLINDERS - 1 ? 0 : 0;
    pos = direction > CYLINDERS - 2 ? 0 : 1;
    
    printf("%d\n", move);
    
    while(pos >= 0 && pos < REQUESTS){
        move += abs(head - sorted_req[pos]);
        if(pos == opposite) break;
        head = sorted_req[pos];
        pos += direction;
    }
    printf("%d\n", move);
    
    return move;
}

int look(int* req, int head){
    int move = 0;
    int* sorted_req = malloc(REQUESTS * sizeof(int));
    memcpy(sorted_req, req, REQUESTS * sizeof(int));
    
    for(int i = 0; i < REQUESTS; i++){
        for(int j = 0; j < REQUESTS - i - 1; j++){
            if(sorted_req[j] > sorted_req[j + 1]){
                int tmp = sorted_req[j];
                sorted_req[j] = sorted_req[j + 1];
                sorted_req[j + 1] = tmp;
            }
        }
    }
    
    int pos = 0;
    while(pos < REQUESTS && sorted_req[pos] < head) pos++;
    int direction = head < CYLINDERS / 2 ? -1 : 1;
    pos = direction > 1 ? pos : pos - 1;
    int opposite = pos - direction;
    
    while(1){
        move += abs(head - sorted_req[pos]);
        head = sorted_req[pos];
        
        if(pos == 0 || pos == REQUESTS - 1) break;
        pos += direction;
    }
    
    direction *= -1;
    head = direction > 0 ? head : CYLINDERS - head - 1;
    pos = opposite;
    
    while(pos >= 0 && pos < REQUESTS){
        move += abs(head - sorted_req[pos]);
        head = sorted_req[pos];
        pos += direction;
    }
    
    return move;
}

int clook(int* req, int head){


}

int main(int argc, char** argv){
    if(argc < 2) exit(EXIT_FAILURE);
    if(atoi(argv[1]) > 4999) exit(EXIT_FAILURE);

    int head = atoi(argv[1]);
    int req[REQUESTS] = {98, 183, 37, 122, 14, 124, 65, 67};
    // for(int i = 0; i < REQUESTS; i++) req[i] = rand() % CYLINDERS;
    
    printf("FCFS  head movement: %d\n", fcfs(req, head));
    printf("SSTF  head movement: %d\n", sstf(req, head));
    printf("SCAN  head movement: %d\n", scan(req, head));
    printf("CSCAN head movement: %d\n", cscan(req, head));
    printf("LOOK  head movement: %d\n", look(req, head));
    // printf("CLOOK head movement: %d\n", clook(req, head));
    
    return 0;
}
