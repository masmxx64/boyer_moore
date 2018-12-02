
#include "head.h"



int size = 0;
int char_to_code[256];
char code_to_char[256];



int abc_set( char *s )
{
	int len = strlen( s );

	for( int i=0; i<256; i++ )
		char_to_code[i] = -1;

	size = 0;
	for( int i=0; i<len; i++ )
		if( char_to_code[ s[i] ] == -1 )
		{
			char_to_code[ s[i] ] = size;
			code_to_char[ size ] = s[i];
			size++;
		}

	return size;
}


int abc_str_to_code( char* src, int** dst )
{
	int len = strlen( src );
	*dst = new int[len];
	for( int i=0; i<len; i++ )
		(*dst)[i] = char_to_code[ src[i] ];
	return len;
}


void abc_code_to_str( int* src, char** dst, int len )
{
	*dst = new char[len+1];
	for( int i=0; i<len; i++ )
		(*dst)[i] = code_to_char[ src[i] ];
	(*dst)[len] = '\0';
}


void abc_print_code( int *a, int len )
{
	for( int i=0; i<len; i++ )
		printf("%c", code_to_char[ a[i] ]);
}


void abc_print_char( int c )
{
	printf("%c", code_to_char[ c ]);
}