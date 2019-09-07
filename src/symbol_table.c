#include "fbgc.h"



struct fbgc_object * initialize_fbgc_symbol_table(){
    //struct fbgc_tuple_object * table = (struct fbgc_tuple_object*) new_fbgc_tuple_object(INITIAL_SYMBOL_TABLE_SIZE);
   	fbgc_symbols = new_fbgc_tuple_object(INITIAL_SYMBOL_TABLE_SIZE);
    return NULL;
}
struct fbgc_object * new_fbgc_symbol_from_substr(const char * str1,const char * str2){
	
	#ifdef SYMBOL_TABLE_DEBUG
	cprintf(100,"Symbol table new symbol from substr..\n");
	#endif
	//############
	//This part is for the recognization of the c module functions
	//struct fbgc_object * o = new_cfun_object_from_substr(field_obj,str1,str2);
	//if(o) return o;
	
	//first compare incoming str with the old identifiers
	
	//:>//struct fbgc_object * table = (struct fbgc_object *) cast_fbgc_object_as_field(field_obj)->symbols;


	struct fbgc_object * name_obj = new_fbgc_int_object(-1);

	struct fbgc_object ** symbols = tuple_object_content(fbgc_symbols);


	//############################
	//Oh. this part just fucks the time for the variable searching. Clearly, without hash table adding new variable causes lots of times!
	//############################
    //clock_t begin,end;
   // double search_time;
	//begin = clock();
	
	for(size_t i = 0; i<size_fbgc_tuple_object(fbgc_symbols); i++){
		if(!my_strncmp(str1,content_fbgc_cstr_object(symbols[i]),str2-str1))
			return new_fbgc_id_opcode(i); 
		
	}
	//end = clock();
	//search_time = (double)(end - begin) / CLOCKS_PER_SEC; 
	//printf("Time :%f\n",search_time );

	struct fbgc_object * temp_obj =  new_fbgc_cstr_object_from_substr(str1,str2);

	#ifdef SYMBOL_TABLE_DEBUG
	cprintf(100,"OLD the symbol table : ");
	//:>print_fbgc_object(cast_fbgc_object_as_field(field_obj)->symbols);
	print_fbgc_object(fbgc_symbols);
	cprintf(111,"\n");
	cprintf(110,"New symbol from s1:%s to s2:%s\n",str1,str2);
	cprintf(110,"Symbol has been created as cstring : ");
	print_fbgc_cstr_object(temp_obj);
	cprintf(111,"\n");
	#endif

	
	fbgc_symbols = push_back_fbgc_tuple_object(fbgc_symbols,temp_obj);

	//:>cast_fbgc_object_as_field(field_obj)->symbols = table;
	//fbgc_symbols = table;

	//table pointer may not be the same anymore
	symbols = tuple_object_content(fbgc_symbols);

	#ifdef SYMBOL_TABLE_DEBUG
	cprintf(100,"Succesfully added in the symbol table : ");
	//:>print_fbgc_object(cast_fbgc_object_as_field(field_obj)->symbols);
	print_fbgc_object(fbgc_symbols);
	cprintf(111,"\n");
	#endif	

	return new_fbgc_id_opcode(size_fbgc_tuple_object(fbgc_symbols)-1);
}

struct fbgc_object * new_cfun_object_from_substr(struct fbgc_object * field_obj,const char * str1,const char * str2){

	/*struct fbgc_ll_object * ll = cast_fbgc_object_as_ll( cast_fbgc_object_as_field(field_obj)->modules );
	struct fbgc_cmodule_object * cm = (struct fbgc_cmodule_object *)ll->base.next;
	while(cm!= NULL && (struct fbgc_object * )cm != ll->tail){
		const struct fbgc_cfunction * cc = cm->module->functions[0];
		cprintf(111,"Functions:\n");
		for (int i = 1; cc!=NULL; ++i){
			if(!memcmp(str1,cc->name,str2-str1)){
				cprintf(010,"\n**Function [%s] is founded in module [%s]**\n",cc->name,cm->module->name);
				return new_fbgc_cfun_object(cc->function);
			} 
			cprintf(101,"{%s}\n",cc->name);
			cc = cm->module->functions[i];
		}
		cm = (struct fbgc_cmodule_object * )cm->base.next;
	}*/

	return NULL;
}


void print_fbgc_symbol_table(struct fbgc_object * ll){


	cprintf(010,"[~~~~~~~~~~~~[SYMBOL TABLE]~~~~~~~~~~~~~]\n");
//	print_fbgc_ll_object(ll,"SYMBOLS:");
	cprintf(110,"\n");
	cprintf(010,"[~~~~~~~~~~~~~~~~~~~~~~~~~]\n");

}

void free_fbgc_symbol_table(struct fbgc_object * table_obj){
	;
}


/*
struct fbgc_object * new_fbgc_identifier(struct fbgc_object* name_cstring){ 
	struct fbgc_identifier * id =  (struct fbgc_identifier * ) fbgc_malloc( sizeof(fbgc_identifier) );
    sym->name = name_cstring;
    sym->content = NULL;
    return (struct fbgc_object*) sym;
}
*/