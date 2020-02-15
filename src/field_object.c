#include "fbgc.h"

struct fbgc_object * new_fbgc_field_object(void){

	struct fbgc_field_object * field = (struct fbgc_field_object *) fbgc_malloc(sizeof(struct fbgc_field_object ));
	//field->base.next = NULL;
	field->base.type = FIELD; 
	field->head = new_fbgc_ll_object();
	field->modules = new_fbgc_ll_object();
	field->locals = new_fbgc_array_object(1,sizeof(struct fbgc_identifier));
	return (struct fbgc_object *) field;
};

void load_module_in_field_object(struct fbgc_object * field_obj, const struct fbgc_cmodule * module){

	struct fbgc_cmodule_object * cm = (struct fbgc_cmodule_object *) fbgc_malloc(sizeof(struct fbgc_cmodule_object ));
	cm->module = module;
	cm->base.type = CMODULE;//CHANGE THIS
	push_front_fbgc_ll_object(cast_fbgc_object_as_field(field_obj)->modules,(struct fbgc_object *)cm);
	
	
	//const struct fbgc_cfunction * cc  = module->initializer;
	//cprintf(111,"WHOW name %s\n",cc->name);
	//	struct fbgc_object * q = cc->function((struct fbgc_object * ) cm);
	//cprintf(111,"BUM!\n");
	
}

struct fbgc_object * add_variable_in_field_object(struct fbgc_object * field_obj,const char * var_name, struct fbgc_object * rhs){
		
		//if(rhs == NULL) return NULL;

	struct fbgc_object * iter = new_fbgc_symbol_from_substr(var_name,var_name + strlen(var_name));
	//this location is from symbols, we need to find location in fields
	struct fbgc_object * cstr_obj = get_object_in_fbgc_tuple_object(fbgc_symbols,cast_fbgc_object_as_id_opcode(iter)->loc);


	struct fbgc_object * local_array = cast_fbgc_object_as_field(field_obj)->locals;
	struct fbgc_identifier * temp_id; 
	int where = -1;

	//first search this in created variables
	for(int i = 0; i<size_fbgc_array_object(local_array); i++){
		temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(local_array,i);
		if(temp_id->name == cstr_obj) {
			where = i;
			break;
		} 
	}

	if(where == -1){
		struct fbgc_identifier id;		
		id.name = cstr_obj; id.content = rhs;
		local_array = push_back_fbgc_array_object(local_array,&id);
		where = size_fbgc_array_object(local_array)-1;
		cast_fbgc_object_as_field(field_obj)->locals = local_array;
	}else{

		cast_fbgc_object_as_id_opcode(temp_id)->loc = where;
		temp_id->content = rhs;
	}
	set_id_flag_GLOBAL(iter);

	return iter;
}


void print_field_object_locals(struct fbgc_object * field_obj){
	struct fbgc_object * ao = cast_fbgc_object_as_field(field_obj)->locals;
	for(int i = 0; i<size_fbgc_array_object(ao); i++){
		struct fbgc_identifier * temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(ao,i);
		cprintf(010,"{"); print_fbgc_object(temp_id->name);
		cprintf(010,":"); print_fbgc_object(temp_id->content);
		cprintf(010,"}");
	}
}




void free_fbgc_field_object(struct fbgc_object * field_obj){
/*	free_fbgc_ll_object(cast_fbgc_object_as_field(field_obj)->head);
	free_fbgc_ll_object(cast_fbgc_object_as_field(field_obj)->modules);
	free_fbgc_symbol_table(cast_fbgc_object_as_field(field_obj)->global_table);
	free(field_obj);*/
}