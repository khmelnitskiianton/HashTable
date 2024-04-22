# Хэш Таблицы

For English version of report check: [README_ENG.md](https://github.com/khmelnitskiianton/HashTable/blob/main/README_ENG.md).

Проект исследования и оптимизаций хэш таблиц

## Оглавление

- [Хэш Таблицы](#хэш-таблицы)
  - [Оглавление](#оглавление)
  - [Установка](#установка)
  - [Доп. программы](#доп-программы)
  - [Описание](#описание)
  - [Первая часть](#первая-часть)
    - [Гистограммы](#гистограммы)
    - [Однородность](#однородность)
  - [Вторая часть](#вторая-часть)
    - [Оптимизации](#оптимизации)
  - [Итоги и выводы](#итоги-и-выводы)

## Установка

Для старта программы используйте Makefile

1.  Клонировать репозиторий
2.  Запустить Makefile (для компиляции)
3.  Запустить программу 

Настройте makefile чтобы изменить хэш функцию или источник данных

`make all` запустит все, измените `TEST`,`SIZE`,`NAME_HASH`, чтобы менять тесты и `CFLAGS`, `PERF`, `PERF_FLAGS` для профилирования

```bash
git clone https://github.com/khmelnitskiianton/HashTable.git
cd ./HashTable
make init    #create folders for temporary objects
make all     #for histograms
make test    #for perf test
```

## Доп. программы

GCC compilier, Makefile для сборки, Python & Seaborn для построения гистограмм.

```bash
sudo apt update && sudo apt upgrade                           #update
sudo apt install build-essential                              #gcc
sudo apt install make                                         #makefile
sudo apt install python3 -y                                   #python
python -m pip install --upgrade pip                           #pip
pip install seaborn                                           #seaborn
sudo apt-get install linux-tools-common linux-tools-generic   #perf
sudo apt-get install hotspot                                  #hotspot
```

## Описание

В этой работе я исследовал различные хэш функции на однородность и исследовал влияние разных оптимизаций на хэш таблицы.

В первой части я брал различные функции и исследовал их на однородное распределение. База данных состоит из слов произведения [William Shakespeare. The Tragedy Of Hamlet, Prince Of Denmark](http://lib.ru/SHAKESPEARE/ENGL/hamlet_en.txt) (5к уникальных слов). 

Хэш таблица построена на двусвязных списках. 
```
int         HT_Ctor (HashTable_t* myHashTable, size_t Size, size_t (*HashFunction) (HT_Key_t));
int         HT_Dtor (HashTable_t* myHashTable);
int         HT_Add  (HashTable_t* myHashTable, HT_Key_t Key, HT_Value_t Value);
DLL_Node_t* HT_Find (HashTable_t* myHashTable, HT_Key_t Key, HT_Value_t Value);
```

Во второй части я работал с профилировщиком и оптимизировал скорость работы хэш таблиц с помощью встройки, выравниваний, ассемблерных вставок и SIMD инструкций.

## Первая часть

В этой части я тестирую различные хэш функции на однородность

*Функции*:
1. Хэш возвращает 0.
2. Хэш возвращает askii код 1 буквы слова.
3. Хэш возвращает длину слова.
4. Хэш возвращает сумму askii кодов слова.
5. Хэщ возвращает (сумму букв)/(длину слова).
6. ROL хэш. `hash[i] = ROL(hash[i-1]) xor str[i]`
7. ROR хэш. `hash[i] = ROR(hash[i-1]) xor str[i]`
8. [CRC32 Хэш](https://ru.wikibooks.org/wiki/Реализации_алгоритмов/Циклический_избыточный_код).
9. [Elf Хэш](https://en.wikipedia.org/wiki/PJW_hash_function)
   
Для изучения распределения я строил гистограммы `КолличествоКоллизий(ЗначениеХэша)`. Диаграммы показывают распределение числа коллизий по значениям хэш функции, в некоторых из них есть пики, которые влияют на скорость работы хэш таблицы(линейному поиску по списку).

Также размер хэш таблицы фиксирован и является простым числом.

Теория по хэш таблицам бралась из [Википедии](https://en.wikipedia.org/wiki/Hash_function)

### Гистограммы

<p> <strong>

1. <U> 0 Hash </U>: Размер: 10, макс. коллизия: 4788. 
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/96c683bd-4517-4a3b-b01a-3369ceee4f81" width = 100%>

2. <U> First Letter Hash </U>: Размер: 128, макс. коллизия: 461. 
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/b9d41ce5-c9a2-48a0-8163-fcb986833191" width = 100%>

1. <U> Length Word Hash </U>: Размер 30, макс. коллизия: 929. 
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/57b06bba-ba51-4afd-92de-2ecc29120fd8" width = 100%>

1. <U> Sum of letters Hash </U>: Размер 1500, макс. коллизия: 31.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/50dbd0c6-556f-4271-97d6-f23fd0059b6c" width = 100%>

1. <U> (Sum of letters)/Length Hash </U>: Размер 179, макс. коллизия: 542.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/5b5f7788-5f04-45b0-baa5-377dca79f93d" width = 100%>

1. <U> ROR Hash </U>: Размер 6007, макс. коллизия: 27.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/b8276c9e-3c0d-45ed-8dd6-0804990ff27c" width = 100%>

1. <U> ROL Hash </U>: Размер 6007, макс. коллизия: 9.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/4462c3f8-3ebb-4fe1-91e2-b770c9913388" width = 100%>

1. <U> CRC32 Hash </U>: Размер 6007, макс. коллизия: 5.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/1b3654c0-ebb3-4489-b8b3-0907d9eed7ad" width = 100%>

1.  <U> ElfHash </U>: Размер 6007, макс. коллизия: 6.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/61f15010-a60d-4ada-bb0b-04395331352c" width = 100%>

</strong></p>

### Однородность


Для оценки однородности распределения хэш функций можно использовать [тест хи-квадрат](https://en.wikipedia.org/wiki/Chi-squared_test).

Теория для хэш таблиц: [хи квадрат для хэша](https://stats.stackexchange.com/questions/532395/about-the-explanation-of-testing-and-measurement-of-hash-functions-at-wikipedia)

Этот тест является критерием соответствия: он сравнивает фактическое распределение элементов в ячейках с ожидаемым (или равномерным) распределением.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/0eb840bb-b00d-4ec9-9788-aa83fd5af00e" width = 100%>

После обработки результатов я построил диаграмму значений теста хи-квадрат:

<img src = "https://github.com/khmelnitskiianton/HashTable/assets/142332024/bf824d62-ab3d-4846-8975-d7e0d6741015" width = 100%>

После анализирования мы можем видеть что если значения теста хи квадрат стремится к $\frac{m}{n} = 0.79$, то распределение более однородно. Лучшие в распределении функции стали CRC32, ElfHash и ROL Hash.

## Вторая часть

*Система:*

- Linux Mint 21.3 Cinnamon
- 12th Gen Intel Core i5-12450H x 8, CPU Temperature: 50 $^\circ C$
- GCC x86-64 -O3 -msse4.1 -msse4.2 -mavx2 -mavx

Я создал стресс тест: загружал большой текст и искал слова по таблице много раз.
Далле с помощью профилировщика я нашел узкие места, которые замедляли скорость работы таблицы и оптимизировал их.

В качестве профилировщика использовался [Perf](https://perf.wiki.kernel.org/index.php/Tutorial) & [Guide Perf](https://stackoverflow.com/questions/1777556/alternatives-to-gprof/10958510#10958510) и визуалировал с помощью [HotSpot](https://github.com/KDAB/hotspot).

Я искал слова 512 раз в цикле, а также несуществующее слово в `StressTest()`.

**Консольная версия:**
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/ef78e3a1-ec0b-4a28-b44c-4fde2106eff2" width = 100%>

**HotSpot версия:**
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/6a564442-141e-4e42-bdbe-98ce62c61835" width = 100%>

**Анализирование профилировщика**: (Размер=6007, Хэш: Elf Hash)

Я не оптимизировал функции инициализации `InsertData` и `Dtor/Ctor`, потому что они специфичны и работают с файлами.

Поэтому узкие места это `HT_Find()`, `DLL_Find()`, `DLL_Compare()`, `ElfHash`, `strcmp()`.

Время я измерял через функцию `__rdtsc()`.

Я измерял время всей программы, которая включает создание хэш таблицы, загрузка слов и стресс тест. Поэтому итоговые данные показывают изменение скорости работы программы в целом.

> Отдельно были тесты для только стресс теста, и ускорение не зависит от измерения времени всей программы или только функций поиска.

Первое контрольное время работы программы: 3056114162 тиков

> Частота процессора не постоянна, поэтому разброс значение составляет $\pm 5$ %

### Оптимизации

0. **Оптимизация встройкой:**
   
Во-первых, Я решил сделать функции поиска встроенными, на вызов функций сравнения тратиться время, несмотря на то что блок кода мал. Поэтому я поместил функции поиска по списку и сравнение в `.h` и использовал `inline`.

> [GCC](https://microsin.net/programming/avr/gcc-inline-functions.html) не встраивает функции при отключенной оптимизации, поэтому нужно использовать атрибут `inline __attribute__((always_inline))`. Поэтому при работе с `-O0` GCC не будет встраивать ничего через обычный `inline` (Это видно из профилировщика).

Новое время стресс теста - 3006040316 тиков. Встраивание дало небольшой прирост в скорости работы программы, так как были убраны множественные вызовы функций.

1. **Оптимизация хэша:**

Во-первых, я переписал хэш на ассемблере [ElfHash on asm](https://github.com/khmelnitskiianton/HashTable/tree/main/HashTable/AsmFunctions.nasm). Это не дало прироста в скорости.

Поэтому я решил изменит хэш с Elf Hash на CRC32 Hash.

Первая версия использовала полином с помощью постоянного массива. Скорость оказалась такой же как у Elf Hash.

Вторая версия использовала встроенные функции SSE `_mm_crc32_u8 (crc, char)`, 2727707675 тиков:

```cpp
size_t crc = 0xFFFFFFFFUL;
for (size_t i = 0; i < length; i++)
    crc = _mm_crc32_u8 (crc, str[i]); 
return crc ^ 0xFFFFFFFFUL;
```    

Я попробовал переписать его на ассемблере `asm()`:

```cpp
asm(
    ".intel_syntax noprefix                     \n"
    "   add     %[len], %[str]                  \n"
    "   mov     edx, 4294967295                 \n"   
    ".for_loop:                                 \n"  
    "   mov     %[crc], rdx                     \n"  
    "   add     %[str], 1                       \n" 
    "   crc32   %[crc], byte ptr [%[str]-1]     \n" 
    "   mov     rdx, %[crc]                     \n"
    "   cmp     %[len], %[str]                  \n"
    "   jne     .for_loop                       \n"        
    "   not     %[crc]                          \n" 
    ".att_syntax noprefix                       \n"
    : [crc] "=r" (crc)
    : [str] "r"  (str), [len] "r"  (length)
);
```
Это было сделано в учебных целях. Нет разницы между написанием через `_mm_crc32_u8()` и `asm()`.

Также я попробовал использовать `_mm_crc32_u64()`:

```cpp
size_t crc = 0xFFFFFFFFUL;    
crc = _mm_crc32_u64 (crc, *((uint64_t*) str));
crc = _mm_crc32_u64 (crc, *(((uint64_t*) str)+1));
return crc ^ 0xFFFFFFFFUL; 
```

Первый результат был странным, время возросло в 2 раза. Это вызвано тем, что выравнивание играет важную роль в скорости работы, т.к. SIMD инструкции зависят от кэша и буффер который загружается в кэш зависит от адресса([Статья от Intel](https://habr.com/ru/companies/intel/articles/262933/))

> Лучшее время достигается при использовании `_mm_crc32_u64()` + `aligned_alloc()`, а не `_mm_crc32_u8()` + `calloc()`

Поэтому сначала я создал выравненный буффер и поместил туда все слова.
Я использовал `aligned_alloc(ALIGNING, bytes)` + `memset()` затем скопировал все слова.

Итоговое время стресс теста - 2196783765 тиков

> Я использовал выравнивание по 16 байт. Более большиое выравнивание не дает ускорения. Если использовать выравнивание по 8 байт и менее это приведет к задержкам, поэтому я подтвердил рузельтаты статьи.

1. **STRCMP Опимизация:**

После всех оптимизаций самый долгий процесс это `strcmp()`. Я использовал AVX инструкции.

В мих данных самое длинное слово размером 14 букв, поэтому я использовал 16 байтные вектора `__m128`.

```cpp
__m128i str1 = _mm_load_si128((const __m128i *) (val1.Key)); 
__m128i str2 = _mm_load_si128((const __m128i *) (val2.Key));
__m128i cmp  = _mm_cmpeq_epi8 (str1, str2);
int result   = _mm_movemask_epi8 (cmp);
return ((result == 0xFFFF) && (val1.Value == val2.Value));
```

> `_mm_load_si128()` работает быстрее других функций загрузки в вектор, но она требует выравнивания адресса по 16 байт, иначе это приведет к ошибке `load of misaligned address`-Ошибка сегментации.

Новое время - 1894493994 тиков

**Итоговый отчет Perf:**

<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/32edd8e2-a6fe-408c-82b8-2c2eb8477ccd" width = 100%>

## Итоги и выводы

После всех оптимизаций я ускорил работу программы в  1.85x - 1.9x (погрешность)! Я добавил скорость к оптимизации компилятора GCC на `-O3` почти в 2 раза.

*Таблица результатов*:

|Оптимизация               |Тики        |Ускорение(в сравнении с началом)|
|:------------------------:|-----------:|:-----:|
|Начало с `-O3`            |3606891139  |1.00x|
|Встройка                  |3006040316  |1.20x|
|Смена хэша на Crc32       |2727707675  |1.32x|
|Выравнивание              |2636887006  |1.36x|
|Векторизация Crc32        |2196783765  |1.64x|
|Векторизация `strcmp()`   |1894493994  |1.90x|

В этом проекте я исследовал хэш функции, работал с профилировщиком(Perf & HotSpot), ищя узкие места в работе программы и устранял их с помощью оптимизаций таких как: встройка, выравнивание, SIMD инструкции, ассемблерные вставки.

$DedInsideCoeff = \frac{boost}{amount \space asm \space strings} \cdot 100 = \frac{190}{12} = 15.8$!
