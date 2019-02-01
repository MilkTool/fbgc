#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H



struct fbgc_symbol_table{
	struct fbgc_object base;
	struct fbgc_object ** symbols;
	uint8_t size;  	
	//capacity is always size + 2
};


struct fbgc_object * new_fbgc_symbol_table(void);
struct fbgc_object * new_fbgc_symbol(struct fbgc_object * ,const char * ,const char * );
struct fbgc_object * get_fbgc_symbol(struct fbgc_object * );

uint8_t is_variable_in_sym_table_exist(struct fbgc_object * table,struct fbgc_object * );
struct fbgc_object * load_module_in_symbol_table(struct fbgc_object *,struct fbgc_object * );

void print_fbgc_symbol_table(struct fbgc_object * );
void free_fbgc_symbol_table(struct fbgc_object * );

#endif