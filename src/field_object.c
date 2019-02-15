#include "fbgc.h"

struct fbgc_object * new_fbgc_field_object(void){

	struct fbgc_field_object * field = (struct fbgc_field_object *) malloc(sizeof(struct fbgc_field_object ));
	field->base.next = NULL;
	field->base.type = UNKNOWN; 
	field->head = new_fbgc_ll_object(LINKED_LIST);
	field->global_table = new_fbgc_symbol_table();
	field->modules = new_fbgc_ll_object(LINKED_LIST);
	//(struct fbgc_cmodule_object *)malloc(sizeof(const struct fbgc_cmodule_object *));

	return (struct fbgc_object *) field;
};

void load_module_in_field_object(struct fbgc_object * field_obj, const struct fbgc_cmodule * module){

	struct fbgc_cmodule_object * cm = (struct fbgc_cmodule_object *)malloc(sizeof(struct fbgc_cmodule_object ));
	cm->module = module;
	cm->base.type = START;
	push_front_fbgc_ll_object(cast_fbgc_object_as_field(field_obj)->modules,(struct fbgc_object *)cm);

	//print_fbgc_cmodule(cm->module);
	
}

void free_fbgc_field_object(struct fbgc_object * field_obj){
	struct fbgc_field_object * field = (struct fbgc_field_object *) field_obj;
	free_fbgc_ll_object(field->head);
	free_fbgc_ll_object(field->modules);
	free_fbgc_symbol_table(field->global_table);
	//free(field->modules);
	free(field);
}