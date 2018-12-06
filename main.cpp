
#include "head.h"


#define DEBUG_OUT
#define MAX(a,b) ((a)>(b)?(a):(b))



// Эта функция выполняет самый примитивный поиск подстроки в строке.
// Этот метод описан в cаом начале мануала
// str - входная строка
// str_len - длинна входной строки
// tem - шаблон для поиска
// tem_len - длинна шаблона для поиска
// res - массив с результатами
void simply_search( int* str, int str_len, int* tem, int tem_len, std::vector<int> *res )
{
	// цикл по строке
	for( int p=0; p<=str_len-tem_len; p++ )
	{
		// сравнение строки с шаблоном
		bool match = true;
		for( int i=0; i<tem_len; i++ )
			if( str[p+i] != tem[i] )
			{
				match = false;
				break;
			}

		// если совпало, записываем в результаты
		if( match && res )
			res->push_back( p );
	}
}


// Функция генерации таблицы плохих символов
// tem - шаблон для поиска
// tem_len - длинна шаблона для поиска
// abc_len - длинна алфавита
// возврат - массив смещений для каждого символа
int* bm_tabBC( int* tem, int tem_len, int abc_len )
{
	// выделяем место для таблицы
	int* tabBC = new int[abc_len];

	// заполняем таблицу максимально возможным смещением
	for( int i=0; i<abc_len; i++ )
		tabBC[i] = tem_len;

	// просматривая символы щаблона слева направо 
	// заполняем таблицу смещениями символов от конца шаблона
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


// Функция генерации таблицы хороших суффиксов (медленная)
// tem - шаблон для поиска
// tem_len - длинна шаблона для поиска
// возврат - массив смещений для каждого символа
int* bm_tabGS_slow( int* tem, int tem_len )
{
	// выделяем место для таблицы
	int* tabGS = new int[tem_len+1];

	// перебираем все возможные размеры суффиксов
	for( int suff_len=0; suff_len<=tem_len; suff_len++ )
	{
		// если есть символ перед суффиксом запоминаем его
		int pre_suff = -1;
		if( suff_len < tem_len )
			pre_suff = tem[ (tem_len-1) - suff_len ];

		// цикл по шаблону справа налево
		for( int i=1; i<=tem_len; i++ )
		{
			// сравниваем символы потенцтального вхождения с суффиксом
			bool match = true;
			int j;
			for( j=0; j<suff_len; j++ )
			{
				int id = (tem_len-1) - j;

				// если вышли за левый край шаблона, то совпадение есть
				// это бывает, когда префикс частично или полностью совпал с суффиксом
				if( id - i < 0 )
					break;

				if( tem[ id ] != tem[ id - i ] )
				{
					match = false;
					break;
				}
			}

			// если найдено совпадение
			if( match )
			{
				// проверяем, что символ перед суффиксом отличается или не существует
				if( i+suff_len < tem_len && pre_suff != -1 )
					if( pre_suff == tem[ (tem_len-1) - (i+suff_len) ] )
						continue;

				// сохраняем результат
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


// Функция генерации таблицы хороших суффиксов
// tem - шаблон для поиска
// tem_len - длинна шаблона для поиска
// возврат - массив смещений для каждого символа
int* bm_tabGS( int* tem, int tem_len )
{
	// выделяем место для таблицы
	int* tabGS = new int[tem_len+1];

	// здесь ищем префиксы

	// задаём переменную shift, которая будет хранить последнее
	// найденное смещение для суффиксов длинной >= i и заменятся при нахождении
	// более длинных префиксов. подробнее в мануале
	int shift = tem_len;
	// цикл по длиннам префиксов и одновременно софиксов
	for( int i=1; i<tem_len; i++ )
	{
		// сравниваем префикс с суффиксом
		bool match = true;
		for( int j=0; j<i; j++ )
			if( tem[ i - j - 1 ] != tem[ (tem_len-1) - j ] )
			{
				match = false;
				break;
			}

		// если совпало запоминаем
		if( match )
			shift = tem_len - i;

		// записываем в таблицу
		tabGS[i] = shift;
	}
	// последний эллемент записываем отдельно, так как нет смысла сравнивать
	// суффикс полностью накладывающийся на префикс
	tabGS[tem_len] = shift;


	// здесь ищем полные совпадения

	// цткл по шаблону слева направо
	for( int i=0; i<tem_len; i++ )
	{
		// цикл по символам суффикса и потенциального вхождения
		for( int j=0; ; j++ )
		{
			// вот тут суффикс длинны j совпал с вхождением

			// проверка, что символы перед суффиксом и вхождением различны
			bool pre = true;
			if( i - j - 1 >= 0 )
				if( tem[ i - j - 1 ] == tem[ (tem_len-1) - j ] )
					pre = false;

			// если различны, записываем результат
			if( pre )
				tabGS[j] = tem_len - i;

			// вхождение дощдло до левого края шаблона
			if( j==i )
				break;

			// сравнение следёющей пары символов
			if( tem[ i - j - 1 ] != tem[ (tem_len-1) - j ] )
				break;

			// идея в том, что если совпал суффикс длинны j с вхождением, то
			// для суффикса длинны j+1 уже есть совпадение длинны j и осталось проверить пару символов
			// за счёт этого мы и уходим от 3 степени к квадрату
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


// Функция генерации таблицы хороших суффиксов (объеденённая)
// Всё тоже, что и в bm_tabGS, но только объеденино в один цикл
// Сложности это не меняет, но оптимизацию даёт
// Ощибок не происходит, поскольку обе "половины" на каждой итерации
// работают с подстроками на одном смещении от конца шаблона и не затираю друг друга
// tem - шаблон для поиска
// tem_len - длинна шаблона для поиска
// возврат - массив смещений для каждого символа
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


// Эта функция выполняет самый примитивный поиск подстроки в строке.
// Этот метод описан в cаом начале мануала
// str - входная строка
// str_len - длинна входной строки
// tem - шаблон для поиска
// tem_len - длинна шаблона для поиска
// abc_len - длинна алфавита
// use_bc - использовать таблицу плохих символов 
// use_gs - использовать таблицу хороших суффиксов
// res - массив с результатами
void boyer_moore( int* str, int str_len, int* tem, int tem_len, int abc_len, bool use_bc, bool use_gs, std::vector<int> *res )
{
	// генерируем таблицы
	int *tabBC = bm_tabBC( tem, tem_len, abc_len );
	int *tabGS = bm_tabGS( tem, tem_len );

	// цикл по смещениям шаблона относительно строки
	for( int p=0; p<=str_len-tem_len; )
	{
		// ищем длинну совпавшего суффикса
		int suff_len = 0;
		for( ; suff_len<tem_len; suff_len++ )
			if( str[ p + (tem_len-1) - suff_len ] != tem[ (tem_len-1) - suff_len ] )
				break;


		// если пользуемся таблицей плохих символов
		int bc = 1;
		if( use_bc )
			if( suff_len < tem_len )
			{
				// находим плохой символ
				int bad_char = str[ p + (tem_len-1) - suff_len ];
				// смотрим для него смещение смещение
				bc = tabBC[ bad_char ] - suff_len;
				bc = MAX( bc, 1 );
			}


		// если пользуемся таблицей хороших суффиксов
		int gs = 1;
		if( use_gs )
			// берём смещения для суффикса заданной длинны
			gs = tabGS[ suff_len ];


		// если нашли совпадения шиблона со строкой, то записываем результат
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