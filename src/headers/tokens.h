#ifndef TOKENS_H
#define TOKENS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t fbgc_token;

typedef struct {
	fbgc_token tokencode;
	const char * name;
}token_struct;


//TOKENS_BEGIN
//Autogenerated by helper/token_creator.py
#define UNKNOWN 0 // Begin with unknown
#define NIL 1 // null or none
#define INT 2 // int class
#define DOUBLE 3 // double class
#define COMPLEX 4 // complex class
#define STRING 5 // string class
#define NAME 6 // name class, name:value pairs
#define IDENTIFIER 7 // identifiers
#define CFUN 8 // c function
#define FUN 9 // builtin fbgc function
#define REFERENCE 10 //
#define LIST 11 //
#define MONATRIX 12 //
#define MATRIX 13 //
#define NUPLE 14 //
#define MONUPLE 15 //
#define TUPLE 16 //
#define ARRAY 17 //
#define LINKED_LIST 18 //
#define LINKED_LIST_TAIL 19 //
#define STACK 20 //
#define CMODULE 21 //
#define GARBAGE 22 //
#define FIELD 23 //
#define CSTRING 24 //
#define END 25 // end
#define FUN_MAKE 26 //
#define ELIF 27 //
#define ELSE 28 //
#define WHILE 29 //
#define BREAK 30 //
#define CONT 31 //
#define LOAD 32 //
#define TRUE 33 //
#define FALSE 34 //
#define IF 35 //
#define RETURN 36 //
#define NEWLINE 37 //
#define LPARA 38 // (
#define RPARA 39 // )
#define LBRACK 40 // [
#define RBRACK 41 // ]
#define LBRACE 42 // {
#define RBRACE 43 // }
#define THREE_DOT 44 // ...
#define R_ARROW 45 // ->
#define L_ARROW 46 // <-
#define RW_ARROW 47 // ~>
#define LW_ARROW 48 // <~
#define TWO_COLON 49 // ::
#define PLUS_ASSIGN 50 // +=
#define MINUS_ASSIGN 51 // -=
#define STAR_ASSIGN 52 // *=
#define SLASH_ASSIGN 53 // /=
#define CONST_ASSIGN 54 // :=
#define PLUSPLUS 55 // ++
#define MINUSMINUS 56 // --
#define IF_THEN 57 // =>
#define STARSTAR 58 // **
#define SLASHSLASH 59 // //
#define LO_EQ 60 // <=
#define GR_EQ 61 // >=
#define EQ_EQ 62 // ==
#define NOT_EQ 63 // !=
#define R_SHIFT 64 // >>
#define L_SHIFT 65 // <<
#define CARET 66 // ^
#define PERCENT 67 // %
#define LOWER 68 // <
#define GREATER 69 // >
#define PIPE 70 // |
#define AMPERSAND 71 // &
#define SLASH 72 // /
#define STAR 73 // *
#define MINUS 74 // -
#define PLUS 75 // +
#define EXCLAMATION 76 // !
#define TILDE 77 // ~
#define SEMICOLON 78 // ;
#define COMMA 79 // :
#define DOT 80 // .
#define COLON 81 // :
#define ASSIGN 82 // =
#define UMINUS 83 // -
#define UPLUS 84 // +
#define START 85 //
#define JUMP 86 //
#define AND 87 //
#define OR 88 //
#define NOT 89 //
#define ROW 90 //
#define EXPRESSION 91 //
#define ASSIGNMENT 92 //
#define STATEMENT 93 //
#define ASSIGNMENT_EXPRESSION 94 //
#define UNARY_EXPRESSION 95 //
#define BINARY_EXPRESSION 96 //
#define BALANCED_EXPRESSION_LIST 97 //
#define UNBALANCED_EXPRESSION_LIST 98 //
#define IF_BEGIN 99 //
#define ELIF_BEGIN 100 //
#define WHILE_BEGIN 101 //
#define FOR_BEGIN 102 //
#define FUN_CALL 103 //
#define BUILD_TUPLE 104 //
#define BUILD_MATRIX 105 //
#define TOKEN_LIST_AS_STRINGS()\
"UNKNOWN",\
"NIL",\
"INT",\
"DOUBLE",\
"COMPLEX",\
"STRING",\
"NAME",\
"IDENTIFIER",\
"CFUN",\
"FUN",\
"REFERENCE",\
"LIST",\
"MONATRIX",\
"MATRIX",\
"NUPLE",\
"MONUPLE",\
"TUPLE",\
"ARRAY",\
"LINKED_LIST",\
"LINKED_LIST_TAIL",\
"STACK",\
"CMODULE",\
"GARBAGE",\
"FIELD",\
"CSTRING",\
"END",\
"FUN_MAKE",\
"ELIF",\
"ELSE",\
"WHILE",\
"BREAK",\
"CONT",\
"LOAD",\
"TRUE",\
"FALSE",\
"IF",\
"RETURN",\
"NEWLINE",\
"LPARA",\
"RPARA",\
"LBRACK",\
"RBRACK",\
"LBRACE",\
"RBRACE",\
"THREE_DOT",\
"R_ARROW",\
"L_ARROW",\
"RW_ARROW",\
"LW_ARROW",\
"TWO_COLON",\
"PLUS_ASSIGN",\
"MINUS_ASSIGN",\
"STAR_ASSIGN",\
"SLASH_ASSIGN",\
"CONST_ASSIGN",\
"PLUSPLUS",\
"MINUSMINUS",\
"IF_THEN",\
"STARSTAR",\
"SLASHSLASH",\
"LO_EQ",\
"GR_EQ",\
"EQ_EQ",\
"NOT_EQ",\
"R_SHIFT",\
"L_SHIFT",\
"CARET",\
"PERCENT",\
"LOWER",\
"GREATER",\
"PIPE",\
"AMPERSAND",\
"SLASH",\
"STAR",\
"MINUS",\
"PLUS",\
"EXCLAMATION",\
"TILDE",\
"SEMICOLON",\
"COMMA",\
"DOT",\
"COLON",\
"ASSIGN",\
"UMINUS",\
"UPLUS",\
"START",\
"JUMP",\
"AND",\
"OR",\
"NOT",\
"ROW",\
"EXPRESSION",\
"ASSIGNMENT",\
"STATEMENT",\
"ASSIGNMENT_EXPRESSION",\
"UNARY_EXPRESSION",\
"BINARY_EXPRESSION",\
"BALANCED_EXPRESSION_LIST",\
"UNBALANCED_EXPRESSION_LIST",\
"IF_BEGIN",\
"ELIF_BEGIN",\
"WHILE_BEGIN",\
"FOR_BEGIN",\
"FUN_CALL",\
"BUILD_TUPLE",\
"BUILD_MATRIX",\
//TOKENS_END

extern const char * object_name_array[];
extern const token_struct operator_token_array[];
extern const token_struct reserved_words_token_array[];

fbgc_token get_operator_code(const char *str);
fbgc_token get_operator_code_from_substr(const char *str1,const char *str2);
fbgc_token get_reserved_word_code(const char *str);
fbgc_token get_reserved_word_code_from_substr(const char *str1,const char *str2);


const char * get_token_as_str(fbgc_token tok);


#define is_fbgc_OPERATOR(T)(T>=THREE_DOT && T<=ASSIGN)
#define is_fbgc_IDENTIFIER(T)(T>=IDENTIFIER && T<=REFERENCE)
#define is_fbgc_ATOM(T)(T>= INT && T<=STRING)
#define is_fbgc_INT(T) (T == INT)
#define is_fbgc_DOUBLE(T) (T == DOUBLE)
#define is_fbgc_STRING(T) (T == STRING)
#define is_fbgc_PARA(T)(T>=LPARA && T<= RBRACK)
#define is_fbgc_UNARY_OPERATOR(T)( T==PLUSPLUS || T==MINUSMINUS || T == EXCLAMATION || T== TILDE)
#define is_fbgc_ASSIGNMENT_OPERATOR(T)( T == ASSIGN || (T>= PLUS_ASSIGN && T<= CONST_ASSIGN)  )
#define is_fbgc_BINARY_OPERATOR(T)( is_fbgc_OPERATOR(T) &&  is_fbgc_ASSIGNMENT_OPERATOR(T) == 0 && is_fbgc_UNARY_OPERATOR(T) == 0 )


#define is_fbgc_MATRIX(x)(x == MATRIX || x == BUILD_MATRIX)
#define is_fbgc_TUPLE(x)(x == NUPLE || x == MONUPLE ||x == BUILD_TUPLE || x == TUPLE)

#define is_fbgc_FUNCTIONABLE(x)(x == IF || x == ELIF || x == ELSE || x == WHILE || x == FUN || x == LOAD)

#define is_fbgc_LIST(x) (x == LIST)
#define is_fbgc_EXPRESSION(x)(is_fbgc_ATOM(x) || is_fbgc_IDENTIFIER(x)|| x == EXPRESSION || x == UNARY_EXPRESSION ||x==BINARY_EXPRESSION||\
is_fbgc_TUPLE(x) || is_fbgc_MATRIX(x) || is_fbgc_LIST(x) || x == CFUN || x == SEMICOLON)

#define is_fbgc_JUMP_STATEMENT(x)(x == BREAK || x == CONT)

#define is_fbgc_ASSIGNMENT(x)(x==ASSIGNMENT_EXPRESSION)
#define is_fbgc_STATEMENT(x)(x == STATEMENT || is_fbgc_EXPRESSION(x) || is_fbgc_ASSIGNMENT(x) || is_fbgc_JUMP_STATEMENT(x))

#ifdef  __cplusplus
}
#endif

#endif
