
#include "head.h"


#define ERROR( s ) { printf("\n<ERROR line:%d> %s\n", __LINE__, s ); getch(); exit(0); }



void test()
{
	//test_abc();
	test_gen_gs_table();
	/*test_bm_long_string(false, false);
	test_bm_long_string(true, false);
	test_bm_long_string(false, true);
	test_bm_long_string(true, true);*/
	/*test_bm_all(false, false);
	test_bm_all(true, false);
	test_bm_all(false, true);
	test_bm_all(true, true);*/
}


void test_abc()
{
	printf("%-30s", __FUNCTION__);

	abc_set("GYMZR5");
	char str_src[] = "M5ZZRYYGY555M";

	int *code;
	int code_len = abc_str_to_code( str_src, &code );

	char *str_dst;
	abc_code_to_str( code, &str_dst, code_len );

	if( strcmp( str_src, str_dst ) != 0 )
		ERROR("Strings missmatch");

	delete code;
	delete str_dst;

	printf(" PASS\n");
}


void test_gen_gs_table()
{
	printf("%-30s", __FUNCTION__);

	abc_set("ABCD");


	int* tabGS_slow;
	int* tabGS;
	int* tabGS_union;

	const int tem_len_min = 1;
	const int tem_len_max = 8;
	int tem[tem_len_max];

	for( int tem_len=tem_len_min; tem_len<=tem_len_max; tem_len++ )
	{
		for( int i=0; i<(1<<(tem_len*2)); i++ )
		{
			int tmp = i;
			for( int j=0; j<tem_len; j++ )
			{
				tem[tem_len-1-j] = tmp%4;
				tmp /= 4;
			}

			tabGS_slow = bm_tabGS_slow( tem, tem_len );
			tabGS = bm_tabGS( tem, tem_len );
			tabGS_union = bm_tabGS_union( tem, tem_len );

			for( int k=0; k<=tem_len; k++ )
			{
				if( tabGS_slow[k] != tabGS[k] )
					ERROR("Tables tabGS_slow and tabGS missmatch");

				if( tabGS_slow[k] != tabGS_union[k] )
					ERROR("Tables tabGS_slow and tabGS_union missmatch");
			}

			delete tabGS_slow;
			delete tabGS;
			delete tabGS_union;

			//abc_print_code(tem,tem_len);
			//printf("\n");
		}
	}

	printf(" PASS\n");
}


void test_bm_long_string( bool use_bc, bool use_gs )
{
	printf("%-30s", __FUNCTION__);

	srand( 12345 );

	int abc_len = abc_set("ABCD");


	const int str_len = 1 << 12;
	int *str = new int[str_len];

	for( int i=0; i<str_len; i++ )
		str[i] = rand() % 4;

	//abc_print_code(str,str_len);
	//printf("\n");

	const int tem_len_max = 8;
	int tem_len;
	int tem[tem_len_max];

	const int iterations = 4*1024;


	for( int i=0; i<iterations; i++ )
	{
		tem_len = ( rand() % tem_len_max ) + 1;
		
		for( int j=0; j<tem_len; j++ )
			tem[j] = rand() % 4;

		//abc_print_code(tem,tem_len);
		//printf("\n");

		std::vector<int> res_slow;
		simply_search( str, str_len, tem, tem_len, &res_slow );

		std::vector<int> res;
		boyer_moore( str, str_len, tem, tem_len, abc_len, use_bc, use_gs, &res );

		if( res_slow.size() != res.size() )
			ERROR("Res size missmatch");

		int sz = res.size();
		for( int j=0; j<sz; j++ )
			if( res_slow[j] != res[j] )
				ERROR("Pos missmatch");
		
	}


	delete str;

	printf(" PASS\n");
}


void test_bm_all( bool use_bc, bool use_gs )
{
	printf("%-30s", __FUNCTION__);

	int abc_len = abc_set("ABCD");


	const int tem_len_min = 1;
	const int tem_len_max = 4;
	int tem[tem_len_max];

	const int str_len = 10;
	int str[str_len];


	for( int tem_len=tem_len_min; tem_len<=tem_len_max; tem_len++ )
	{
		for( int is=0; is<(1<<(str_len*2)); is++ )
		{
		
			int tmp = is;
			for( int j=0; j<str_len; j++ )
			{
				str[str_len-1-j] = tmp%4;
				tmp /= 4;
			}

			//abc_print_code(str,str_len);
			//printf("\n");


			for( int i=0; i<(1<<(tem_len*2)); i++ )
			{
				int tmp = i;
				for( int j=0; j<tem_len; j++ )
				{
					tem[tem_len-1-j] = tmp%4;
					tmp /= 4;
				}


				std::vector<int> res_slow;
				simply_search( str, str_len, tem, tem_len, &res_slow );

				std::vector<int> res;
				boyer_moore( str, str_len, tem, tem_len, abc_len, use_bc, use_gs, &res );

				if( res_slow.size() != res.size() )
					ERROR("Res size missmatch");

				int sz = res.size();
				for( int j=0; j<sz; j++ )
					if( res_slow[j] != res[j] )
						ERROR("Pos missmatch");


				/*abc_print_code(str,str_len);
				printf(" ");
				abc_print_code(tem,tem_len);
				printf("\n");*/
			}
		}
	}

	printf(" PASS\n");
}