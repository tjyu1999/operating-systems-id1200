# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>

# define PAGE_TABLE_SIZE 256
# define TLB_SIZE         16
# define BUFFER_SIZE     256
# define MEM_SIZE        256

struct TLB{
    unsigned char tlb_page[TLB_SIZE];
    unsigned char tlb_frame[TLB_SIZE];
    int idx;
};

int read_from_disk(int page_num, char* memory, int* open_frame){
    FILE *back_file = fopen("BACKING_STORE.bin", "rb");
    if(back_file == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    if(fseek(back_file, page_num * MEM_SIZE, SEEK_SET) != 0){
        perror("fseek");
        exit(EXIT_FAILURE);
    }
    
    if(fread(buffer, sizeof(char), MEM_SIZE, back_file) == 0){
        perror("fread");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < MEM_SIZE; i++) *((memory + (*open_frame) * MEM_SIZE) + i) = buffer[i];
    (*open_frame)++;

    return (*open_frame) - 1;
}

int find_page(int logi_addr, char* page_table, struct TLB* tlb,  char* memory, int* open_frame, int* page_fault, int* tlb_hit_sum){
    unsigned char page_num;
    unsigned char offset;
    unsigned char mask = 0xFF;    
    int val;
    int frame = 0;
    int new_frame = 0;
    bool tlb_hit = false;

    printf("Virtual adress: %d\t", logi_addr);

    page_num = (logi_addr >> 8) & mask;
    offset = logi_addr & mask;
    
    for(int j = 0; j < TLB_SIZE; j++){
        if(tlb->tlb_page[j] == page_num){
            frame = tlb->tlb_frame[j];
            tlb_hit = true;
            (*tlb_hit_sum)++;
        }		
    }
    
    if(tlb_hit == false){
        if(page_table[page_num] == -1){
            new_frame = read_from_disk(page_num, memory, open_frame);
            page_table[page_num] = new_frame;
            (*page_fault)++;
        }
        
        frame = page_table[page_num];
        tlb->tlb_page[tlb->idx] = page_num;
        tlb->tlb_frame[tlb->idx] = page_table[page_num];
        tlb->idx = (tlb->idx + 1) % TLB_SIZE;	
    }
    
    int phys_addr = ((unsigned char)frame * MEM_SIZE) + offset;
    val = *(memory + phys_addr);
    printf("Physical address: %d\t Value: %d\t\n", phys_addr, val);

    return 0;
}

int main(int argc, char** argv){
    if(argc < 2) exit(EXIT_FAILURE);

    int logi_addr;
    int phys_addr;
    int val;
    int open_frame = 0;
    int page_fault = 0;
    int tlb_hit_sum = 0;
    int cnt = 0;
    
    unsigned char page_table[PAGE_TABLE_SIZE];
    char memory[MEM_SIZE][MEM_SIZE];
    
    FILE* addr_file = fopen(argv[1], "r");
    if(addr_file == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    memset(page_table, -1, sizeof(page_table));	

    struct TLB tlb;	
    memset(tlb.tlb_page, -1, sizeof(tlb.tlb_page));
    memset(tlb.tlb_frame, -1, sizeof(tlb.tlb_frame));
    tlb.idx = 0;
    
    while((fscanf(addr_file, "%d", &logi_addr) == 1)){
        find_page(logi_addr, page_table, &tlb, (char*)memory, &open_frame, &page_fault, &tlb_hit_sum);
        cnt++;
    }

    printf("Page Fault Rate: %.4f\n", (float)page_fault / (float)cnt);
    printf("TLB hit rate:    %.4f\n", (float)tlb_hit_sum / (float)cnt);
    fclose(addr_file);

    return 0;
}
