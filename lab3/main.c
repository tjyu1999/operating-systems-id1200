# include <stdio.h>
# include <stdlib.h>

# define PAGE_TABLE_SIZE 256
# define TLB_SIZE         16
# define BUFFER_SIZE     256
# define MEM_SIZE        256

struct TLB{
    int key;
    int val;
};

FILE* back_file;
struct TLB tlb[TLB_SIZE];
unsigned char page_table[PAGE_TABLE_SIZE];
int idx;

int get_offset(int value){
    return value & 0xff;
}

int get_page(int value){
    return (value >> 8) & 0xff;
}

signed char get value(int phys_addr){
    return *(memory + phys_addr);
}

void allocate(unsigned char page){
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
    
    
}

int get_phys_addr(int logi_addr){
    int page = get_page(logi_addr);
    int offset = get_offset(logi_addr);
    
    for(int i = 0; i < TLB_SIZE; i++){
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
    if(idx < TLB_SIZE) idx++;
    
    return ((ptable[pnum] & 0xff) << 8) | offset;
}

int main(int argc, char** argv){
    if(argc < 2) exit(EXIT_FAILURE);
    
    int logi_addr;
    int phys_addr;
    int fault = 0;
    int hit = 0;
    
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
    
    while((fscanf(addr_file, "%d", &logi_addr) == 1)){
        printf("Virtual adress: %d\t", logi_addr);
        phys_addr = get_phys_addr(logi_addr);
        printf("Physical address: %d\t Value: %d\t\n", phys_addr, get_value(phys_addr));
    }
    
    printf("Page fault rate: %.4f\n", (float)(fault / cnt));
    printf("TLB hit rate:    %.4f\n", (float)(hit / cnt));
    fclose(addr_file);
    fclose(addr_file);
    
    return 0;
}
