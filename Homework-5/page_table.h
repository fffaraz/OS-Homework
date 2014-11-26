#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <stdint.h>

// Page table entry structure
// 16           8 7               0
// |-----------|-|-----------------|
// | reserved  |V| Physical  Base  |
// |           |N|     Address     |
// |-----------|-|-----------------|
//
// * VN : Valid/Invalid
//

typedef struct{
	uint8_t base;
	uint8_t vn:1;
} __attribute__((packed)) page_table_entry;

// Page fault handler must set for handling page faults.
//
//

typedef void (*page_fault_handler)(uint8_t base);

page_fault_handler handler;

void add_page_table_entry(uint8_t index, page_table_entry* entry);
uint8_t get_bsae_addr(uint8_t index);

#endif