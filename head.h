
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <vector>


int abc_set( char *s );
int abc_str_to_code( char* src, int** dst );
void abc_code_to_str( int* src, char** dst, int len );
void abc_print_code( int *a, int len );
void abc_print_char( int c );


void simply_search( int* str, int str_len, int* tem, int tem_len, std::vector<int> *res=NULL );
int* bm_tabBC( int* tem, int tem_len, int abc_len );
int* bm_tabGS_slow( int* tem, int tem_len );
int* bm_tabGS( int* tem, int tem_len );
int* bm_tabGS_union( int* tem, int tem_len );
void boyer_moore( int* str, int str_len, int* tem, int tem_len, int abc_len, bool use_bc=true, bool use_gs=true, std::vector<int> *res=NULL );


void test();
void test_abc();
void test_gen_gs_table();
void test_bm_long_string( bool use_bc, bool use_gs );
void test_bm_all( bool use_bc, bool use_gs );