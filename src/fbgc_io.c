#include "fbgc.h"

new_fbgc_cfunction(fbgc_print,"print")
{
	if(arg->type == TUPLE){
		for(unsigned int i = 0; i<cast_fbgc_object_as_tuple(arg)->size; i++){
			print_fbgc_object(cast_fbgc_object_as_tuple(arg)->contents[i]);
			printf(" ");
		}
	} 
	else print_fbgc_object(arg);
	printf("\n");
	return new_fbgc_object(UNKNOWN);
}

new_fbgc_cfunction(fbgc_read,"read"){

	printf("I am a reader! but your type is :%s\n",object_name_array[arg->type] );

	return arg;
}



const struct fbgc_cmodule_object fbgc_io_module = 
{
	.name = "io",
	.functions = (const struct fbgc_cfunction*[])
	{
		&fbgc_print_struct,&fbgc_read_struct
	}
};
