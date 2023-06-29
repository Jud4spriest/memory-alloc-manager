#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define MEMORY_SIZE 1024

#pragma pack(push, 1)
struct mem_block {
    int is_free;   // Se o bloco esta livre 1 senao 0.
    size_t size;   // Tamanho do bloco de memoria.
    void* mem_ptr; // Ponteiro para o bloco de memoria.
    struct mem_block* next; // Ponteiro para o proximo bloco na lista.
};
#pragma pack(pop)



struct mem_block* memory_sim;

void initMemory(size_t size) {
    void* memory_alloc = malloc(size);
    memory_sim = (struct mem_block*) memory_alloc;
    memory_sim->is_free = 1;
    memory_sim->size = size;
    memory_sim->mem_ptr = memory_alloc;
    memory_sim->next = NULL;
}



void* smalloc(size_t size) {
    printf("Alocating %d bytes...\n", size);
    // printf("Entrou!!\n");
    int counter = 0;
    struct mem_block* current_block = memory_sim;
    while (1){
        // printf("current_block: %x\n", current_block);
        // printf("counter: %d\n", counter);
        if (current_block->is_free && current_block->size >= (size + sizeof(struct mem_block))){
            
            struct mem_block* new_block = (struct mem_block*) (current_block + size + sizeof(struct mem_block));
            new_block->is_free = 1;
            new_block->size = current_block->size - sizeof(struct mem_block) - size;
            new_block->mem_ptr = current_block + sizeof(struct mem_block);
            new_block->next = NULL;

            current_block->is_free = 0;
            current_block->size = size;
            current_block->next = new_block;
            return new_block->mem_ptr;
        }
        if (current_block->next == NULL) break;
        current_block = current_block->next;
        counter++;
    }
    
}



void sfree(void* p) {
    printf("Dealocating...\n");
    struct mem_block* current_block = memory_sim;
    
    while (current_block->mem_ptr != p) {
            current_block = current_block->next;
            // printf("Mem: %x\n", current_block);
    }
    // printf("Mem: %x\n", current_block);
    // printf("Mem: %d\n", current_block->size);
    current_block->is_free = 1;
    

    struct mem_block* ant_block = memory_sim;
    current_block = (struct mem_block*) memory_sim->next;
    while (1) {
        // printf("free: %d| %d\n", current_block->mem_ptr, ant_block->mem_ptr);
        // printf("free: %d| %d\n", current_block->is_free, ant_block->is_free);
        if (current_block->is_free == 1 && ant_block->is_free == 1){
            ant_block->size = ant_block->size + current_block->size + sizeof(struct mem_block);
            ant_block->next = NULL;
            break;
        }
        if (current_block->next == NULL) break;
        ant_block = current_block;
        current_block = current_block->next;  
    }
}




void print_mem_block(struct mem_block *m){
    printf("------------ MEMORY ------------\n");    
    while(m){

        printf("is_free: %d\n", m->is_free);
        printf("size: %ld\n", m->size);
        printf("mem_ptr: %p\n", m->mem_ptr);
        printf("next: %p\n", m->next);
        printf("--------------------------------\n");
        m = m->next;
    }
};

//TESTBENCGH
int main(){
    initMemory(1024);
    
    /* Exemplo de uso do smalloc(), sfree() e print_mem_block() */
    print_mem_block((struct mem_block*) memory_sim);

    char *p1 = (char*) smalloc(200);
    char *p2 = (char*) smalloc(100);
    char *p3 = (char*) smalloc(300);

    print_mem_block((struct mem_block*) memory_sim);    
    // printf("%d bytes\n",sizeof(memory_sim));

    
    sfree(p2);
    sfree(p1);
    sfree(memory_sim);

    print_mem_block((struct mem_block*) memory_sim);            

    return 0;
}


