#include "fbgc.h"


#ifndef INTERPRETER_DEBUG
#define _info(s,...)
#define _info_green(s,...)
#define _warning(s,...)
#define _print(s,...)
#define _cprint(c,s,...)
#define _debug(s,...)
#define _print_object(s,obj)
#define _println_object(s,obj)
#define _obj2str(obj)
#define _gm2str(gm)
#define print_field_object_locals(x)
#endif


struct iter_function_ptr_struct{
	struct fbgc_object * (* function)(struct fbgc_object *, int,struct fbgc_object *);
};

uint8_t interpreter(struct fbgc_object ** field_obj){
	current_field = *field_obj;

	struct fbgc_ll_object * head = cast_fbgc_object_as_ll( cast_fbgc_object_as_field(*field_obj)->head );
	struct fbgc_object * pc = head->base.next; //program counter

	#define PROGRAM_STACK_SIZE 100
	struct fbgc_object * stack = new_fbgc_tuple_object(PROGRAM_STACK_SIZE);
	struct fbgc_object ** sp = tuple_object_content(stack);
	int sctr = 0;
	int fctr = -1;
	
	//## change this!!
	#define globals cast_fbgc_object_as_field(*field_obj)->locals

	struct fbgc_object * last_called_function = NULL;
	size_t recursion_ctr = 0;

#define STACK_GOTO(i) 	(sctr += i)
#define PUSH(x)		(*(sp+ sctr++ ) = (x))
#define POP()			(sp[--sctr]) /**< Detailed description after the member */
#define _POP()			(--sctr)
#define TOP()			(sp[sctr-1])
#define SECOND()		(sp[sctr-2])
#define THIRD()			(sp[sctr-3])
#define TOPN(i)			(sp[sctr-(i)])
#define SET_TOP(x)		(sp[sctr-1] = (x))
#define SET_SECOND(x)	(sp[sctr-2] = (x))
#define SET_THIRD(x)	(sp[sctr-3] = (x))
#define SET_TOPN(i, x)	(sp[sctr-(i)] = (x))	

//FRAME MACROS
#define GET_AT_FP(n)		(sp[fctr+(n)])
#define SET_AT_FP(n, x)	(sp[fctr+(n)] = (x))
#define FETCH_NEXT()(pc = pc->next)
#define RECURSION_LIMIT 1000


	_info("==========[INTERPRETER]==========\n");

	for(int i = 0;  (pc != head->tail); i++){

		if(i > 1000){
			printf("Too much iteration. Breaking the loop\n");
			goto INTERPRETER_ERROR_LABEL;
		}

		if(recursion_ctr>RECURSION_LIMIT){
			printf("Reached Recursion limit!\n");
			break;
		}

		if(sctr > PROGRAM_STACK_SIZE){
			printf("Stack Overflow!");
			break;
		}

		fbgc_token type = get_fbgc_object_type(pc);
		
		_print("################ [%d] = {%s} ########################\n",i,_obj2str(pc));

		switch(type){
			case NIL:
			case LOGIC:
			case INT:
			case DOUBLE:
			case COMPLEX:			
			case STRING:
			case CSTRING:
			case CFUN:
			case FUN:
			case CSTRUCT:
			case ROW:{
				
				PUSH(pc);
				break;
			}
			case IDENTIFIER:{	

				if(is_id_flag_GLOBAL(pc)){

					struct fbgc_identifier * tmp = (struct fbgc_identifier *) 
						get_address_in_fbgc_array_object(globals,cast_fbgc_object_as_id_opcode(pc)->loc);

					//Check undefined variable
					if(tmp->content == NULL){
						struct fbgc_object * name = tmp->name;
						printf("Undefined variable %s\n",&cast_fbgc_object_as_cstr(name)->content);
						//fbgc_error(_FBGC_UNDEFINED_IDENTIFIER_ERROR,-1);
						return 0;
					}
					
					PUSH(tmp->content);
				} 
				else if(is_id_flag_LOCAL(pc))  PUSH(GET_AT_FP(cast_fbgc_object_as_id_opcode(pc)->loc));
				else if(is_id_flag_MEMBER(pc)){
					//if(TOP()->type == CSTRING)
					struct fbgc_object * member = POP();
					struct fbgc_object * object =  get_set_fbgc_object_member(POP(),content_fbgc_cstr_object(member), NULL);

					if(object == NULL){
						printf("Object does not have %s member\n",content_fbgc_cstr_object(member));
						goto INTERPRETER_ERROR_LABEL;
					}

					PUSH(object);
				}
				break;
			}
			case BREAK:{

				struct fbgc_object * loop_obj =  cast_fbgc_object_as_jumper(pc)->content;
				if(loop_obj->type == FOR) {
					assert(0);
					STACK_GOTO(-2); //clean the for loop remainders
				}
				pc = cast_fbgc_object_as_jumper(loop_obj)->content;
				break;
			}
			case CONT:{

				stack->next = cast_fbgc_object_as_jumper(pc)->content;
				pc = stack;
				break;
			}				
			case RETURN:{

				//(LOCALS..., OLD_SCTR,FRAME_CTR,GLOBAL_ARRAY,RETURN_VALUE)
				//Notice that global array is just an indicator to collect properly

				struct fbgc_object * ret = POP();
				while(POP() != globals);
				//_POP();
				int old_fctr = fctr;
				/*if(TOP() == globals){
					POP();
					--old_fctr;
					//XXBUG
					//pop the method name, this is a bug we need to fix it in parser
					//In order to call methods as typical functions, method call pushes another global to stack 
				}*/
				fctr = cast_fbgc_object_as_int(TOP())->content;
				stack->next = SECOND();
				sctr = old_fctr;
				//STACK_GOTO(-2);
				
				//##Solve this pc->next problem!!!!!!!!
				pc = stack;
				_POP();
				PUSH(ret);

				//if(ret->type == NIL && pc->next->type != ASSIGN) 
				//	_POP();
				

				recursion_ctr = 0;

				break;
			}			
			case COLON:{

				struct fbgc_object * x = new_fbgc_range_object(SECOND(),TOP());
				STACK_GOTO(-1);
				SET_TOP(x);
				break;
			}
			case RSHIFT:
			case LSHIFT:
			case STARSTAR:
			case SLASHSLASH:
			case PLUS:
			case MINUS:
			case STAR:
			case SLASH:
			case CARET:
			case PERCENT:
			case LOEQ:
			case GREQ:
			case EQEQ:
			case NOTEQ:
			case LOWER:
			case GREATER:
			case PIPE:
			case AMPERSAND:
			{
				
				assert(TOP() != NULL && SECOND() != NULL);

				fbgc_token main_tok = MAX(get_fbgc_object_type(TOP()),get_fbgc_object_type(SECOND()));

				/*if(is_fbgc_binary_op_null(main_tok)){
					cprintf(111,"This type does not support operation.\n");
					return 0;
				}*/ //Why do we need to check this?

				struct fbgc_object * res =  call_fbgc_operator(main_tok,SECOND(),TOP(),type);
				STACK_GOTO(-1);
				//struct fbgc_object * res =  safe_call_fbgc_binary_op(POP(),POP(),main_tok,type);

				assert(res != NULL);
				SET_TOP(res);		

				break;	

			}
			case EXCLAMATION:
			case TILDE:		
			case UPLUS:
			case UMINUS:{
				
				assert(TOP() != NULL);
				struct fbgc_object * res =  call_fbgc_operator(get_fbgc_object_type(TOP()),TOP(),NULL,type);
				assert(res != NULL);
				SET_TOP(res);
				break;
			}
			case ASSIGN:
			case RSHIFT_ASSIGN:
			case LSHIFT_ASSIGN:
			case STARSTAR_ASSIGN:
			case SLASHSLASH_ASSIGN:
			case PLUS_ASSIGN:
			case MINUS_ASSIGN:
			case STAR_ASSIGN:
			case SLASH_ASSIGN:
			case CARET_ASSIGN:
			case PERCENT_ASSIGN:{

				_info("Assignment op [%x]ID<%d>\n",get_id_flag(pc),get_id_opcode_loc(pc));

				struct fbgc_object * rhs = POP();
				struct fbgc_object ** lhs = NULL;

				if(is_id_flag_GLOBAL(pc)){
					struct fbgc_identifier * tmp = 
					(struct fbgc_identifier *) get_address_in_fbgc_array_object(globals,cast_fbgc_object_as_id_opcode(pc)->loc);
					lhs = &tmp->content;
				}
				else if(is_id_flag_MEMBER(pc)){
					struct fbgc_object * member = POP();
					struct fbgc_object * object = get_set_fbgc_object_member(TOP(),content_fbgc_cstr_object(member) , rhs);

					if(object == NULL){
						printf("[%s] does not support [%s] member assignment\n",object_name_array[TOP()->type],content_fbgc_cstr_object(member));
						goto INTERPRETER_ERROR_LABEL;
					}

					_POP();
					break;

					//why do we have break here? this type of operation only supports assign, not the other types.
					//TODO make it general					
				}
				else if(is_id_flag_LOCAL(pc)){
					lhs = &(GET_AT_FP(cast_fbgc_object_as_id_opcode(pc)->loc));
				}


				//Call other assignment types
				if(type != ASSIGN)
				{
					fbgc_token op_type = RSHIFT + type - RSHIFT_ASSIGN;
					assert(lhs != NULL && *lhs != NULL);

					fbgc_token main_tok = MAX(get_fbgc_object_type( (*lhs) ),get_fbgc_object_type(rhs));
					rhs = call_fbgc_operator(main_tok,*lhs,rhs,op_type);
					assert(rhs != NULL);				
				}

				*lhs = rhs;

				
				// for x = y= 5
				if(is_id_flag_PUSH_ITSELF(pc)){
					PUSH(rhs);
				}	
				
				break;
			}
			case ASSIGN_SUBSCRIPT:
			{
				struct fbgc_object * rhs = POP();
				int index_no = cast_fbgc_object_as_int(POP())->content;
				struct fbgc_object ** lhs = NULL;
				
				struct fbgc_object * temp = TOPN(index_no+1);
				int index = 0;
				int stck_goto = index_no;
				while(index_no > 0){
					if(temp->type == TUPLE){
						index = cast_fbgc_object_as_int(TOPN(index_no))->content;
						if(index_no > 1){
							temp = get_object_in_fbgc_tuple_object(temp,index);
							index_no--;
						}
						else
						{
							index = cast_fbgc_object_as_int(TOPN(index_no))->content;
							_cprint(110,"Here index %d\n",index);
							lhs = get_object_address_in_fbgc_tuple_object(temp,index);
							break;
						}
					}
					else if(temp->type == STRING){
						index = cast_fbgc_object_as_int(TOPN(index_no))->content;
						lhs =  set_object_in_fbgc_str_object(temp,index,index+1,rhs);
						rhs = lhs;
						break;
					}
					else if(temp->type == MATRIX){
						assert(index_no > 1);
						index = cast_fbgc_object_as_int(TOPN(index_no))->content;
						int index_no2 = cast_fbgc_object_as_int(TOPN(--index_no))->content;
						lhs = set_object_in_fbgc_matrix_object(temp,index,index_no2,rhs);
						rhs = lhs;
						assert(lhs != NULL);
						break;
					}
					else {
						printf("Not index accessable!\n");
						goto INTERPRETER_ERROR_LABEL;
					}
				}
				*lhs=rhs;
				break;

			}

			case LEN:
			{
				SET_TOP( get_length_fbgc_object(TOP()) );
				break;
			}			
			case JUMP:
			{
				pc = cast_fbgc_object_as_jumper(pc)->content;
				break;
			}		
			case IF:
			case ELIF:
			case WHILE:
			{
				if(!convert_fbgc_object_to_logic(TOP())){
					pc = cast_fbgc_object_as_jumper(pc)->content;
				}
				_POP();
				break;
			}
			case FOR_BEGIN:
			{


				struct fbgc_object * seq_ob = TOP();

				struct fbgc_cstruct_object * so = (struct fbgc_cstruct_object *) 
				new_fbgc_cstruct_object(sizeof(struct iter_function_ptr_struct), NULL);
				struct iter_function_ptr_struct * ifps = (struct iter_function_ptr_struct *) so->cstruct;
				
				PUSH((struct fbgc_object *) so);
				
				if(seq_ob->type == RANGE){

					fbgc_token range_type = get_fbgc_range_object_iter_type(seq_ob);

					if(range_type == INT){
						ifps->function = get_int_element_in_fbgc_range_object;
						PUSH(new_fbgc_int_object(888));  	
					}
					else if(range_type == DOUBLE){
						ifps->function = get_double_element_in_fbgc_range_object;
						PUSH(new_fbgc_double_object(8.8));
					}
					//=====================================================
					//check the top is it sequential obj ?
					// this depends on seq obj type, if it's a tuple we do not need it
					//if range-int then it is int, range-double: double
					// string : string etc..
							
				}
				else if(seq_ob->type == STRING){
					ifps->function = get_char_from_fbgc_str_object;
					PUSH(new_fbgc_str_object("*"));
				}
				else if(seq_ob->type == TUPLE){
					ifps->function = __get_object_in_fbgc_tuple_object;
					//holder of tuple will never be used, we are sure of it but it is not a good idea to push null object
					PUSH(NULL);
				
				}

				//this is always the same!
				PUSH(new_fbgc_int_object(0)); //iterator starter
				break;
			}
			case FOR:
			{	
				//Assume that FOR_BEGIN checked everything so no need to check in this case
				//if we check something, it may introduce latency

				struct fbgc_object * seq_ob = TOPN(4);
				//this is out index iterator and we will increase it inplace
				int i = (cast_fbgc_object_as_int(TOP())->content)++;			
				struct fbgc_object * holder = SECOND();
				
				struct fbgc_cstruct_object * so = cast_fbgc_object_as_cstruct(TOPN(3));
				struct iter_function_ptr_struct * fs = (struct iter_function_ptr_struct *) so->cstruct; 
				
				//call the pre-determined function				
				seq_ob = fs->function(seq_ob,i,holder);


				if(seq_ob != NULL){
					PUSH(seq_ob);
				}
				else{
					//finish the for loop, everything worked normally
					//pop iterator,holder,sequence object and function struct
					STACK_GOTO(-4);
					pc = cast_fbgc_object_as_jumper(pc)->content;
				}

				break;
			}
			case FUN_CALL:
			{
				int arg_no = cast_fbgc_object_as_int(POP())->content;
				struct fbgc_object * funo = TOPN(arg_no+1);
				if(funo->type != FUN && funo->type != CFUN){
					printf("Object is not callable\n");
					goto INTERPRETER_ERROR_LABEL;
				}
				

				if(funo->type == CFUN){
					STACK_GOTO(-arg_no);
					struct fbgc_object * res = cfun_object_call(funo, sp+sctr, arg_no);
					_POP(); // pop cfun object 

					if(res == NULL){
						//const struct fbgc_cfunction * cc = cast_fbgc_object_as_cfun(funo);
						//cprintf(100,"Error in function %s\n",cc->name);
						goto INTERPRETER_ERROR_LABEL;
					} 
						
					//XXX solve this issue
					if(res != __fbgc_nil)
						PUSH(res);
					break;

					// In order to increase speed, DELETE new tuple creation it causes 2sec for 100,000 print('ffdfds') code
					
					//return 0;
				}
				
				if(cast_fbgc_object_as_fun(funo)->no_arg != arg_no ){
					printf("Argument match error! funo->arg %d, arg_no %d\n",cast_fbgc_object_as_fun(funo)->no_arg,arg_no);
					goto INTERPRETER_ERROR_LABEL;
				}

				if(last_called_function == funo) recursion_ctr++;
				else {
					last_called_function = funo;
					recursion_ctr = 0;
				}

				STACK_GOTO(cast_fbgc_object_as_fun(funo)->no_locals - arg_no);
				//save our first next operation after this function call
				//After returning the value we will take this space and run the main code
				PUSH(pc->next);
				//hold old frame pointer location
				PUSH(new_fbgc_int_object(fctr));
				//hold old position of sp with fp, assume that args already pushed into stack
				fctr = sctr-cast_fbgc_object_as_fun(funo)->no_locals-2;
				//execute function
				//--------------------------------
				//This next push added after first version of the fun call operations
				//when we have a for loop, it changes stack and return simply makes three basic pop operations
				//but for loops leave additional objects in the stack so we need to push something as an indicator that 
				//return will pop everything till see this object.
				//Nothing from the user side can push globals so we will push global key-value array
				PUSH(globals);
				//--------------------------------

				//##Solve this pc->next problem!!!!!!!!
				stack->next = cast_fbgc_object_as_fun(funo)->code;
				pc = stack;
				break;
			}
			case BUILD_TUPLE:
			{	

				int tuple_size = cast_fbgc_object_as_int(pc)->content;
				struct fbgc_object * to = new_fbgc_tuple_object(tuple_size);
				size_fbgc_tuple_object(to) = tuple_size;
				
				//XXXXXXXXXXXXXXXXXX
				//Instead of pop, give the starting address and size
				//XXXXXXXXXXXXXXXXXXX 	
				while(--tuple_size >= 0){
					set_object_in_fbgc_tuple_object(to,POP(),tuple_size);
				}

				PUSH(to);
				break;
			}
			case BUILD_MATRIX:
			{	
				
				if(TOP()->type == RANGE)
				{
					SET_TOP(new_fbgc_matrix_object_from_range(TOP()));
					break;
				}


				int ctr = cast_fbgc_object_as_int(pc)->content;
				if(ctr == 1 && TOP()->type == MATRIX) break;


				int row = 0;
				int col = 0;
				int msize = 0;

				//traverse reverse!

				//cprintf(111,"sctr %d, ctr %d\n",sctr,ctr);
				/*cast_fbgc_object_as_tuple(stack)->size = sctr;//sp - tuple_object_content(stack);
				cprintf(111,"\n==============Stack==========================\n");
				print_fbgc_object(stack);*/				
				
				fbgc_token matrix_sub_type = DOUBLE;

				for( int i = 1; i<=sctr && i <= ctr; ++i){
					//cprintf(011,"i = %d ctr %d\n",i,ctr);
					switch(TOPN(i)->type){
						case INT:
						case DOUBLE:
						{
							++msize;
							//cprintf(111,"int/db row:%d col:%d\n",row,col);
							break;
						}
						case COMPLEX:{
							++msize;
							matrix_sub_type = COMPLEX;
							break;
						}	
						case MATRIX:
						{
							int r = cast_fbgc_object_as_matrix(TOPN(i))->row;
							int c = cast_fbgc_object_as_matrix(TOPN(i))->column;
							//cprintf(011,"top is matrix %dx%d\n",r,c);
							msize +=  r*c;
							//row += r;
							break;					
						}
						default:
						{
							printf(111,"Type %s in matrix is cannot be located\n",object_name_array[TOPN(i+1)->type]);
							goto INTERPRETER_ERROR_LABEL;
						}
					}	
					
					if(i < sctr && TOPN(i+1)->type == ROW){
						++row;
						ctr += 1+cast_fbgc_object_as_int(TOPN(i+1))->content;
						//cprintf(010,"Top is row, msize: %d, ctr :%d ,row :%d\n",msize,ctr,row);
						++i;
					}
				}

				//cprintf(010,"Output of loop, row:%d , col :%d ctr:%d\n",row,old_col,ctr);
				
				//return 0;

				
				//struct fbgc_object * m = new_fbgc_matrix_object(ctr-row);
				struct fbgc_object * m = matrix_creation_from_stack(sp+sctr-ctr ,ctr, msize, row,matrix_sub_type);
				assert(m != NULL);
				//print_fbgc_matrix_object(m);



				STACK_GOTO(-ctr);
				PUSH(m);

				break;
				//return 0;
			}
			case POP_TOP:
			{
				_POP();
				break;
			}
			case LOAD_SUBSCRIPT:
			{
				int index_no = cast_fbgc_object_as_int(POP())->content;
				//take index values one by one and finally left last index 
				struct fbgc_object * temp = TOPN(index_no+1);
				int index = 0;
				for(int i = 0; i<index_no; i++){
					index = cast_fbgc_object_as_int(TOPN(index_no-i))->content;
					if(temp->type == TUPLE){
						//cprintf(111,"Current index %d\n",index);
						temp = get_object_in_fbgc_tuple_object(temp,index);
						//print_fbgc_object(dummy); cprintf(111,"<<<\n");
					}
					else if(temp->type == COMPLEX){
						temp = subscript_fbgc_complex_object(temp,index);
					}
					else if(temp->type == STRING){
						temp = get_object_in_fbgc_str_object(temp,index,index+1);
					}
					else if(temp->type == MATRIX){
						if(index_no == 2){
							int index_no2 = cast_fbgc_object_as_int(TOPN(index_no-1))->content;
							temp = get_object_in_fbgc_matrix_object(temp,index,index_no2);
							assert(temp != NULL);
							break;
						}

					}
					else {
						printf(111,"Not index accessible!\n");
						print_fbgc_object(temp); printf("\n");
						return 0;
					}
					
					assert(temp != NULL);

				}

				STACK_GOTO(-index_no-1);
				PUSH(temp);
				break;
			}
			
			default:
			{
				cprintf(101,"Undefined token in interpreter [%s]\n",object_name_array[type]);
				return 0;						
			}
		}


		cast_fbgc_object_as_tuple(stack)->size = sctr; //sp - tuple_object_content(stack);
		_println_object("~~~~~~Stack~~~~~\n",stack);
		_info("~~~~~~Field Globals~~~~~\n");
		
		print_field_object_locals(*field_obj);
		_info("\n==============================================\n\n");
		

		FETCH_NEXT();

	}

	return 1;

	INTERPRETER_ERROR_LABEL:
		cprintf(100,"Execution stopped!\n");
		return 0;

}
