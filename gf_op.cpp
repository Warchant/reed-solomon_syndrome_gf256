#include <iostream>
#define GF 256
#define m 8 // степень RS-полинома (согласно Стандарта ECMA-130 - восемь)
#define n 255 // n = 2 * m - 1 (длина кодового слова)
#define t 1 // количество ошибок, которые мы хотим скорректировать
#define k 253 // k = n - 2 * t (длина информационного слова)
using namespace std;
// несократимый порождающий полином
// согласно Стандарту ECMA-130: P(x) = x8 + x4 + x3 + x2 + 1
int p[m + 1] = {1, 0, 1, 1, 1, 0, 0, 0, 1};

int alpha_of[n + 1]; // таблица степеней примитивного члена
int index_of[n + 1]; // индексная таблица для быстрого умножения

//----------------------------------------------------------------------------
// генерируем look-up таблицу для быстрого умножения для GF(2 ^ m) на основе
// несократимого порождающего полинома Pc от p[0] до p[m].
//
// look-up таблица:
// index->polynomial из alpha_of[] содержит j = alpha ^ i,
// где alpha есть примитивный член, обычно равный 2
// а ^ - операция возведения в степень (не XOR!);
//
// polynomial form -> index из index_of[j = alpha ^ i] = i;
//
// c Simon Rockliff
//----------------------------------------------------------------------------

void generate_gf()
{
       int i, mask;

       mask = 1; alpha_of[m] = 0;

       for (i = 0; i < m; i++)
       {
              alpha_of[i] = mask;
              index_of[alpha_of[i]] = i;

              if (p[i] != 0) alpha_of[m] ^= mask;
              mask <<= 1;
       } index_of[alpha_of[m]] = m; mask >>= 1;

       for (i = m + 1; i < n; i++)
       {
              if (alpha_of[i - 1] >= mask)
                     alpha_of[i] = alpha_of[m] ^ ((alpha_of[i - 1] ^ mask) << 1);
              else
                     alpha_of[i] = alpha_of[i - 1] << 1;

              index_of[alpha_of[i]] = i;
       } index_of[0] = -1;
}

// функция возвращает результат умножения
// двух полиномов a на b в полях Галуа
int gf_mul(int a, int b)
{
       int sum;
       if (a == 0 || b == 0) return 0;  // немного оптимизации не повредит
       sum = alpha_of[a] + alpha_of[b]; // вычисляем сумму индексов полиномов
       if (sum >= GF - 1) sum -= GF-1;  // приводим сумму к модулю GF
       return index_of[sum];            // переводим результат в полиномиальную...
                                        // ...форму и возвращаем результат
} 

// функция возвращает результат деления
// двух полиномов a на b в полях Галуа
// при попытке деления на ноль функция
// возвращает -1
int gf_div(int a, int b)
{
       int diff;
       if (a == 0) return 0;                // немного оптимизации не повредит
       if (b == 0) return -1;               // на ноль делить нельзя!
       diff = alpha_of[a] - alpha_of[b];    // вычисляем разность индексов
       if (diff < 0) diff += GF - 1;        // приводим разность к модулю GF
       return index_of[diff];               // переводим результат в полиномиальную...
                                            // ...форму и возвращаем результат
} 

void main()
{
	generate_gf();
	
	cout << gf_mul(1,2);
	system("pause");
}
