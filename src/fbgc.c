#include "fbgc.h"

#include "../cmodules/cmodules.h"

//Forward declarations
void print_logo();


struct fbgc_object * current_field = NULL;

struct fbgc_object __nil__ = {.type = NIL};
struct fbgc_object * __fbgc_nil = &__nil__;


static void compile_file(struct fbgc_object * main_field,const char *file_name){

	clock_t begin,end;
	double parser_time,interpreter_time;


	begin = clock();
	 
   int par = parse_file(&main_field,file_name);

	
	
	end = clock();
	parser_time = (double)(end - begin) / CLOCKS_PER_SEC; 

	 
	begin = clock();
	if(par) 
		par = interpreter(&main_field); 
	end = clock();
	interpreter_time = (double)(end - begin) / CLOCKS_PER_SEC; 


	#ifdef TIME_DEBUG
	cprintf(110,"\n\n\n\n\n\n[=======================================================================]\n");   
	printf("Execution time [PARSER + LEXER] :%f\n",parser_time);    
	printf("Execution time [INTERPRETER] :%f\n",interpreter_time);  
	printf("Total exc. time %f\n",parser_time+interpreter_time);  
	#endif

	  
}




struct fbgc_object * fbgc_load_module(const char * module_name,const char * fun_name, uint8_t load_key){
		
	//load_key == 0, just return the module
	//load_key == 1, load all and return 
	//load_key == 2, load specific and return, requires fun_name

	assert(current_field != NULL);

	const struct fbgc_cmodule * cm = NULL; 
	for(uint8_t i = 0; i<sizeof(__cmodules)/sizeof(__cmodules[0]); ++i){
		if(strcmp(module_name,__cmodules[i]->initializer.name) == 0){
			cm = __cmodules[i];
		}
	}

	if(cm == NULL){
		return NULL;
	}

	if(load_key != 0){
		//call function initializer
		if(cm->initializer.function != NULL){
			struct fbgc_cfun_arg cfarg = {.arg = &current_field, .argc = 1, .kwargs_flag = 0};
			cm->initializer.function(&cfarg); //need to send the address of current field
		}

		for (const struct fbgc_cfunction * cc = cm->functions; cc->name != NULL; ++cc){
			if(load_key == 2 && my_strcmp(fun_name,cc->name)){
				//if no match, 
				continue;
			}

			add_variable_in_field_object(current_field,cc->name,new_fbgc_cfun_object(cc->function));
			if(load_key == 2) break;		
		}
	}

	return new_fbgc_cmodule_object(cm);

}

struct fbgc_object * fbgc_load_file(const char * file_name,const char * fun_name, uint8_t load_key){

	struct fbgc_object * prev_field = current_field;	

	struct fbgc_object * file_field = new_fbgc_field_object();
	if(parse_file(&file_field,file_name))
		interpreter(&file_field);
	else 
		assert(0);

	current_field = file_field;

	if(load_key != 0){
		//now merge two field objects, new field and prev field
		//prev_field_local_array_object
		struct fbgc_object * prev_ao = cast_fbgc_object_as_field(prev_field)->locals;
		//current field local array object
		struct fbgc_object * current_ao = cast_fbgc_object_as_field(current_field)->locals;

		unsigned int last_j = 0;

		char atleast_matched = 0;
		

		for(unsigned int i = 0;  i<size_fbgc_array_object(current_ao); ++i){	
			struct fbgc_identifier * ctemp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(current_ao,i);
			struct fbgc_identifier * ptemp_id;
			char found = 0;

			//cprintf(111,"ctemp_id name:[%s]fun:[%s]\n",content_fbgc_str_object(ctemp_id->name),fun_name);
			//cprintf(100,"result %d\n",my_strcmp(fun_name,content_fbgc_str_object(ctemp_id->name)));
			if(load_key == 2 && my_strcmp(fun_name,content_fbgc_str_object(ctemp_id->name)) ){
				//if no match
				continue;
			}

			//so matched with the given function name
			atleast_matched = 1;

			for(unsigned int j = last_j; j<size_fbgc_array_object(prev_ao); ++j){
				ptemp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(prev_ao,j);
				//cprintf(111,"ptemp_id name :"); print_fbgc_object(ptemp_id->name);
				//cprintf(111," | ctemp_id name :"); print_fbgc_object(ctemp_id->name);
				
				if(!my_strcmp(content_fbgc_str_object(ptemp_id->name),content_fbgc_str_object(ctemp_id->name))){
					//change the content of ptemp_id object with the second one
					found = 1;
					last_j = ++j;
					//cprintf(100," (Equal)\n");
					break;
				}
			}

			if(found){
				//cprintf(101,"Found!\n");
				ptemp_id->content = ctemp_id->content;
			}else{			
				//cprintf(101,"creating new local\n");
				prev_ao = push_back_fbgc_array_object(prev_ao,ctemp_id);	
			}
			if(load_key == 2) break;
		}
		cast_fbgc_object_as_field(prev_field)->locals = prev_ao;
		

		if(!atleast_matched){
			cprintf(100,"Error '%s' could not be found in '%s'\n",fun_name,file_name);
		}
		assert(atleast_matched);
	}

	current_field = prev_field;
	return file_field;
}


void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
}


int8_t parse_tuple_content(struct fbgc_cfun_arg * cfun_arg,const char * format, ...){

	
	//cprintf(111,"parsing tuple sz:%d | format'%s'\n",sz,format);

	//Verilecek hatalar
	//eğer hiç arg verilmemişse argdaki objelerin sadece tipleri kontrol edilecek
	//eğer arg varsa mümkün olan arg'a kadar arrayin o objesi assign edilecek
	//argüman sayısı uyuşmazsa match hatası verilip null dönülecek
	// parse(a,b,"ij");

	struct fbgc_object ** c = cfun_arg->arg;
	int sz = cfun_arg->argc;

    va_list args;
    va_start(args, format);
    
    uint8_t have_arg = 0;
    if(*format == '!'){
    	//so only verification, assuming no arg is passed
    	//cprintf(110,"Not assignment, only verification\n");
    	++format;
    }
    else have_arg = 1;


    uint8_t ok = 0;
    int8_t level = 0;

    const char * level_start = format;

    uint8_t count_match = 0;
    
	for(int i = 0; i<sz; ++i){
		struct fbgc_object * co = c[i];
		//cprintf(111,"Current[%d] type :%s | format:%c\n",i,objtp2str(co),*format);

	switch(*format){
		case 'b':{
			ok = (co->type == LOGIC);
			//cprintf(001,"b:logic:%d\n",ok);
			break;
		}
		case 'i':{
			ok = (co->type == INT);
			//cprintf(001,"i:int:%d\n",ok);
			break;
		}
		case 'd':{
			ok = (co->type == DOUBLE);
			//cprintf(001,"d:double:%d\n",ok);
			break;
		}
		case 'j':{
			ok = (co->type == COMPLEX);
			//cprintf(001,"c:complex:%d\n",ok);
			break;
		}
		case 's':{
			ok = (co->type == STRING);
			//cprintf(001,"s:string:%d\n",ok);
			break;
		}
		case 'm':{
			ok = (co->type == MATRIX);
			////cprintf(001,"m:matrix:%d\n",ok);
			break;
		}
		case 't':{
			ok = (co->type == TUPLE);
			//cprintf(001,"t:tuple:%d\n",ok);
			break;
		}
		case 'r':{
			ok = (co->type == RANGE);
			//cprintf(001,"r:range:%d\n",ok);
			break;
		}		
		case '.':{
			ok = (co->type == CSTRUCT);
			//cprintf(001,".:cstruct:%d\n",ok);
			break;	
		}
		case 'o':{
			//any type 
			ok = 1;
			//cprintf(001,"o:any:%d\n",ok);
			break;
		}
		case '|':{
			if(ok)
				goto end_of_parse_tuple_content;
			
			++format;
			//cprintf(001,"or\n");
			continue;
		}
		case ':':{
			//cprintf(001,"colon\n");
			if(ok == 1){
				//No need to print error message
			}
			else{
				const char * er_start = ++format;
				while(*er_start != '\0' && *er_start != '|')
					++er_start;
				//Now we have error message..

			}
			break;
		}
		case '*':{
			//cprintf(001,"star\n");
			break;
		}
		case '+':{
			//cprintf(001,"plus\n");
			if(count_match){
				format = level_start;
				continue;
			}
			else ok = 0;
			break;
		}
		case '\0':{
			//cprintf(010,"end of format!\n");
			if(ok){
				//printf("args is valid!\n");
			}
			else {
				FBGC_LOGE("args is not valid\n");
				count_match = 0;
			}

			goto end_of_parse_tuple_content;
			
		}
		default:{
			assert(0);
		}
	}

	if(ok == 1){
		++count_match;
		if(have_arg){
			//cprintf(111,"We have args, now assigning them\n");
			struct fbgc_object ** o = va_arg(args, struct fbgc_object **);
			*o = c[i];
		}
		ok = 0;
	}
	else{
		
		while(*format != '\0' && *format != '|')
			++format;

		if(*format == '\0'){
			count_match = 0;
			FBGC_LOGE("Arg match error!\n");
			break;
		}
		else{
			//printf("----------------\n");
			++level;
			count_match = 0;
			i = -1;
		}
	}

	++format;
	}


	end_of_parse_tuple_content:
	va_end(args);
	level = (count_match > 0) ? level : -1;
	//cprintf(111,"Returning level:%d\n",level);
	return level;
	
	return 0;
}


#ifndef MODULE_TEST
int main(int argc, char **argv){

	#ifdef FBGC_LOGO
		print_logo();
	#endif   

//******************************************************************
	initialize_fbgc_memory_block();

	/*int * lol = (int * ) fbgc_malloc(sizeof(int)*10);
	int val[10] = {10,20,30,40,50,60,70,80,90,100};
	memcpy(lol,val,sizeof(int)*10);


	int val2[10] = {1,2,3,4,5,6,7,8,9,10};
	int * lol2 = (int * ) fbgc_malloc(sizeof(int)*20);
	memcpy(lol2,val2,sizeof(int)*10);

	print_fbgc_memory_block();
	

	cprintf(100,"capacity lol :%ld\n",capacity_fbgc_raw_memory(lol,sizeof(int)));
	cprintf(100,"capacity lol2 :%ld\n",capacity_fbgc_raw_memory(lol2,sizeof(int)));*/
	initialize_fbgc_symbol_table();
	struct fbgc_object * main_field = new_fbgc_field_object();
	current_field = main_field;
	//load_module_in_field_object(main_field,&fbgc_math_module);
	//load_module_in_field_object(main_field,&fbgc_file_module);
		
	//parse_string(&main_field,"x = 88\n");
	//cast_fbgc_object_as_field(main_field)->code;

	/*cprintf(100,"sizeof :%lu\n",sizeof(struct fbgc_map_object));

	struct fbgc_object * map = new_fbgc_map_object(2,50);
	fbgc_map_object_insert(map,new_fbgc_str_object("end"),new_fbgc_int_object(333));
	fbgc_map_object_insert(map,new_fbgc_str_object("sep"),new_fbgc_int_object(553));
	
	cprintf(110,"CHECK sep: %d\n",fbgc_map_object_does_key_exist_str(map,"sep"));

	print_detailed_fbgc_map_object(map);*/
	//fbgc_map_object_remove_str(map,"fbgencer");
	//cprintf(100,"fbgencer found at index :%ld\n",fbgc_map_object_get_key_index_str(map,"samsung"));

	//fbgc_map_object_set_exact_size(map);
	
	//fbgc_map_object_insert(map,new_fbgc_str_object("floler4"),new_fbgc_int_object(553));

	//print_detailed_fbgc_map_object(map);

	if(argc > 1)
	{   
		if(!strcmp(argv[1],"-s")){
			//compile_one_line(main_field,argv[2]);
		}
		else{
		   compile_file(main_field, argv[1]); 
		}
		//print_fbgc_memory_block();
		
	}
	else{
		compile_file(main_field, "ex.fbgc");
	   //realtime_fbgc(main_field);
	}

	free_fbgc_memory_block();
//******************************************************************

	return 0;
}


void print_logo(){
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;

	pFile = fopen ( "./src/_fbgc_.txt" , "r" );
	if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);

	// allocate memory to contain the whole file:
	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

	printf("%s\n",buffer);

	// terminate
	fclose (pFile);
	free (buffer);

}

#endif

