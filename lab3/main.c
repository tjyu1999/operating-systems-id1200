# include <stdio.h>
# include <stdlib.h>

# define TLB_SIZE         16
# define PAGE_TABLE_SIZE 256
# define FRAME_SIZE      256
# define MEM_SIZE        256

struct TLB{
    int key;
    int val;
};

FILE* back_file;
struct TLB tlb[TLB_SIZE];
int idx = 0;
int page_table[PAGE_TABLE_SIZE];
signed char* memory;
int free_frame = 0;
int fault = 0;
int hit = 0;

int get_offset(int value){
    return value & 0xff;
}

int get_page(int value){
    return (value >> 8) & 0xff;
}

signed char get_value(int phys_addr){
    return *(memory + phys_addr);
}

void allocate(unsigned char page){
    if(fseek(back_file, page << 8, SEEK_SET) != 0){
        perror("fseek");
        exit(EXIT_FAILURE);
    }
    
    if(fread(memory + free_frame, sizeof(char), FRAME_SIZE, back_file) == 0){
        perror("fread");
        exit(EXIT_FAILURE);
    }
    
    page_table[page] = (free_frame >> 8) | 0x100;
    free_frame += FRAME_SIZE;
}

int get_phys_addr(int logi_addr){
    int page = get_page(logi_addr);
    int offset = get_offset(logi_addr);
    
    for(int i = 0; i < idx; i++){
        if(tlb[i].key == page){
            hit++;
            return (tlb[i].val << 8) | offset;
        }
    }
    
    if(!(page_table[page] & 0x100)){
        allocate(page);
        fault++;
    }
    
    tlb[idx].key = page;
    tlb[idx].val = page_table[page] & 0xff;
    idx = (idx + 1) % TLB_SIZE;
    
    return ((page_table[page] & 0xff) << 8) | offset;
}

int main(int argc, char** argv){
    if(argc < 2) exit(EXIT_FAILURE);
    
    memory = malloc(MEM_SIZE * FRAME_SIZE);
    
    unsigned short logi_addr;
    unsigned short phys_addr;
    int cnt = 0;
    
    FILE* addr_file = fopen(argv[1], "r");
    if(addr_file == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    back_file = fopen("BACKING_STORE.bin", "rb");
    if(back_file == NULL){
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    while((fscanf(addr_file, "%hd", &logi_addr) == 1)){
        phys_addr = get_phys_addr(logi_addr);
        printf("Virtual adress: %d\t Physical address: %d\t Value: %d\t\n", logi_addr, phys_addr, get_value(phys_addr));
        cnt++;
    }
    
    printf("Page fault rate: %.4f\n", (float)fault / (float)cnt);
    printf("TLB hit rate:    %.4f\n", (float)hit / (float)cnt);
    fclose(addr_file);
    fclose(back_file);
    
    return 0;
}
