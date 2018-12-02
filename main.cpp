
#include "head.h"


//#define DEBUG_OUT
#define MAX(a,b) ((a)>(b)?(a):(b))



void simply_search( int* str, int str_len, int* tem, int tem_len, std::vector<int> *res )
{
	for( int p=0; p<=str_len-tem_len; p++ )
	{
		bool match = true;
		for( int i=0; i<tem_len; i++ )
			if( str[p+i] != tem[i] )
			{
				match = false;
				break;
			}

		if( match && res )
			res->push_back( p );
	}
}


int* bm_tabBC( int* tem, int tem_len, int abc_len )
{
	int* tabBC = new int[abc_len];

	for( int i=0; i<abc_len; i++ )
		tabBC[i] = tem_len;

	for( int i=0; i<tem_len-1; i++ )
		tabBC[ tem[i] ] = (tem_len-1) - i;

	#ifdef DEBUG_OUT
	printf("===== BC =====\n");
	for( int i=0; i<abc_len; i++ )
	{
		printf("  ");
		abc_print_char( i );
	}
	printf("\n");
	for( int i=0; i<abc_len; i++ )
		printf(" %2d", tabBC[i]);
	printf("\n\n");
	#endif

	return tabBC;
}


int* bm_tabGS_slow( int* tem, int tem_len )
{
	int* tabGS = new int[tem_len+1];

	for( int i=0; i<tem_len+1; i++ )
		tabGS[i] = -5;


	for( int suff_len=0; suff_len<=tem_len; suff_len++ )
	{
		int pre_suff = -1;
		if( suff_len < tem_len )
			pre_suff = tem[ (tem_len-1) - suff_len ];

		for( int i=1; i<=tem_len; i++ )
		{
			bool match = true;
			int j;
			for( j=0; j<suff_len; j++ )
			{
				int id = (tem_len-1) - j;

				if( id - i < 0 )
					break;

				if( tem[ id ] != tem[ id - i ] )
				{
					match = false;
					break;
				}
			}

			if( match )
			{
				if( j == suff_len && i+suff_len < tem_len && pre_suff != -1 )
					if( pre_suff == tem[ (tem_len-1) - (i+suff_len) ] )
						continue;

				tabGS[ suff_len ] = i;
				break;
			}
		}
	}


	#ifdef DEBUG_OUT
	printf("===== GS =====\n");
	for( int i=0; i<tem_len; i++ )
	{
		printf("  ");
		abc_print_char( tem[i] );
	}
	printf("\n");
	for( int i=tem_len; i>=0; i-- )
		printf(" %2d", tabGS[i]);
	printf("\n\n");
	#endif

	return tabGS;
}


int* bm_tabGS( int* tem, int tem_len )
{
	int* tabGS = new int[tem_len+1];

	int shift = tem_len;
	for( int i=1; i<tem_len; i++ )
	{
		bool match = true;
		for( int j=0; j<i; j++ )
			if( tem[ i - j - 1 ] != tem[ (tem_len-1) - j ] )
			{
				match = false;
				break;
			}

		if( match )
			shift = tem_len - i;

		tabGS[i] = shift;
	}
	tabGS[tem_len] = shift;


	for( int i=0; i<tem_len; i++ )
	{
		for( int j=0; ; j++ )
		{
			bool pre = true;
			if( i - j - 1 >= 0 )
				if( tem[ i - j - 1 ] == tem[ (tem_len-1) - j ] )
					pre = false;
			if( pre )
				tabGS[j] = tem_len - i;

			if( j==i )
				break;

			if( tem[ i - j - 1 ] != tem[ (tem_len-1) - j ] )
				break;
		}
	}



	#ifdef DEBUG_OUT
	printf("===== GS =====\n");
	for( int i=0; i<tem_len; i++ )
	{
		printf("  ");
		abc_print_char( tem[i] );
	}
	printf("\n");
	for( int i=tem_len; i>=0; i-- )
		printf(" %2d", tabGS[i]);
	printf("\n\n");
	#endif

	return tabGS;
}


int* bm_tabGS_union( int* tem, int tem_len )
{
	int* tabGS = new int[tem_len+1];

	int shift = tem_len;
	for( int i=0; i<tem_len; i++ )
	{
		bool match = true;
		for( int j=0; ; j++ )
		{
			bool pre = true;
			if( i - j - 1 >= 0 )
				if( tem[ i - j - 1 ] == tem[ (tem_len-1) - j ] )
					pre = false;
			if( pre )
				tabGS[j] = tem_len - i;		
			
			if( j==i )
				break;
			
			if( tem[ i - j - 1 ] != tem[ (tem_len-1) - j ] )
			{
				match = false;
				break;
			}
		}

		if( match )
			shift = tem_len - i;

		tabGS[i] = shift;
	}
	tabGS[tem_len] = shift;


	#ifdef DEBUG_OUT
	printf("===== GS =====\n");
	for( int i=0; i<tem_len; i++ )
	{
		printf("  ");
		abc_print_char( tem[i] );
	}
	printf("\n");
	for( int i=tem_len; i>=0; i-- )
		printf(" %2d", tabGS[i]);
	printf("\n\n");
	#endif

	return tabGS;
}


void boyer_moore( int* str, int str_len, int* tem, int tem_len, int abc_len, bool use_bc, bool use_gs, std::vector<int> *res )
{
	int *tabBC = bm_tabBC( tem, tem_len, abc_len );
	int *tabGS = bm_tabGS( tem, tem_len );
	int *tabGS_union = bm_tabGS_union( tem, tem_len );

	for( int p=0; p<=str_len-tem_len; )
	{
		int suff_len = 0;
		for( ; suff_len<tem_len; suff_len++ )
			if( str[ p + (tem_len-1) - suff_len ] != tem[ (tem_len-1) - suff_len ] )
				break;


		int bc = 1;
		if( use_bc )
			if( suff_len < tem_len )
			{
				int bad_char = str[ p + (tem_len-1) - suff_len ];
				bc = tabBC[ bad_char ] - suff_len;
				bc = MAX( bc, 1 );
			}


		int gs = 1;
		if( use_gs )
			gs = tabGS[ suff_len ];


		if( suff_len == tem_len && res )
			res->push_back( p );


		#ifdef DEBUG_OUT
		abc_print_code( str, str_len );
		printf(" p: %2d s: %2d bc: %2d gs: %2d", p, suff_len, bc, gs);
		if( suff_len == tem_len )
			printf(" [M]");
		printf("\n");
		for( int i=0; i<p; i++ )
			printf(" ");
		abc_print_code( tem, tem_len );
		printf("\n");
		if( suff_len < tem_len )
		{
			for( int i=0; i<p+tem_len-suff_len-1; i++ )
				printf(" ");
			printf("X");
		}else{
			for( int i=0; i<p+tem_len-suff_len; i++ )
				printf(" ");
		}
		for( int i=0; i<suff_len; i++ )
			printf("=");
		printf("\n");
		printf("\n");
		#endif

		p += MAX( bc, gs );
	}

	delete tabBC;
	delete tabGS;
	delete tabGS_union;
}


int main()
{

	
// Init

	char abc[] = "ACGT";
	int abc_len = abc_set( abc );

	int *str;
	int str_len = abc_str_to_code( "GCATCGCAGAGAGTATACAGTACG", &str );
	//int str_len = abc_str_to_code( "AAAAAAAAAAAAAAAA", &str );

	int *tem;
	int tem_len = abc_str_to_code( "GCAGAGAG", &tem );
	//int tem_len = abc_str_to_code( "AAAA", &tem );


// Search

	//simply_search( str, str_len, tem, tem_len );
	std::vector<int> res;
	boyer_moore( str, str_len, tem, tem_len, abc_len, true, true, &res );

	//for( int i=0; i<res.size(); i++ )
	//	printf("%d\n", res[i]);


// Clear

	delete str;
	delete tem;
	
	#ifndef DEBUG_OUT
	test();
	#endif

	printf("done");
	getch();
	return 0;
}