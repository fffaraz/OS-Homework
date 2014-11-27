#include "page_table.h"
#include "logical.h"
#include "memory.h"
#include <stdio.h>
#include <stdint.h>

#define MAX 1000*1000

void pf_handler(uint8_t base);

int main(int argc, char* argv[]){
	FILE* logical_addrs = fopen("assignment5/addresses.txt", "r");
	uint32_t number;
	int index = 0;
	logical_addr addrs[MAX];
	while(fscanf(logical_addrs, "%u", &number) > 0){
		logical_addr_parser(number, &addrs[index]);
		index++;
	}
	fclose(logical_addrs);

	for(int i = 0; i < 256; i++){
		add_page_table_entry(i, i, 0x0);
	}
	set_handler(pf_handler);
	
	for(int i = 0; i < index; i++){
		int8_t mem = mem_read(get_base_addr(addrs[i].page_number), addrs[i].page_offset);
		printf("Physical Address: %u Value: %d\n", addrs[i].page_number * 256 + addrs[i].page_offset, mem);
	}
}

void pf_handler(uint8_t base){
	FILE* hard_disk = fopen("assignment5/BACKING_STORE.bin", "r");
	uint8_t frame[256];
	fseek(hard_disk, base * 256, SEEK_SET);
	fread(frame, sizeof(uint8_t), 256, hard_disk);
	for(int i = 0; i < 256; i++){
		mem_write(base, i, frame[i]);
	}
}
