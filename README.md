# Хэш Таблицы

For English version of report check: [README_ENG.md](https://github.com/khmelnitskiianton/HashTable/blob/main/README_ENG.md).

Проект исследования и оптимизаций хэш таблиц

## Оглавление

- [Хэш Таблицы](#хэш-таблицы)
  - [Оглавление](#оглавление)
  - [Установка](#установка)
  - [Зависимые объекты](#зависимые-объекты)
  - [Описание](#описание)
  - [Первая часть](#первая-часть)
    - [Гистограммы](#гистограммы)
    - [Однородность](#однородность)
  - [Вторая часть](#вторая-часть)
    - [Оптимизации](#оптимизации)
  - [Итоги и выводы](#итоги-и-выводы)

## Установка

```bash
git clone https://github.com/khmelnitskiianton/HashTable.git
cd ./HashTable
make init    #create folders for temporary objects
make all     #for histograms
make test    #for perf test
```

Настройте makefile, чтобы изменить хэш функцию или источник данных:

`make all` запустит все, измените `TEST`,`SIZE`,`NAME_HASH`, чтобы менять тесты и `CFLAGS`, `PERF`, `PERF_FLAGS` для профилирования.

## Зависимые объекты

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

В этой работе я исследовал различные хэш-функции на однородность и исследовал влияние разных оптимизаций на хэш таблицы.

В первой части я брал различные функции и исследовал их на однородное распределение. База данных состоит из слов произведения [William Shakespeare. The Tragedy Of Hamlet, Prince Of Denmark](http://lib.ru/SHAKESPEARE/ENGL/hamlet_en.txt).

Хэш таблица построена на двусвязных списках. 
```
int         HT_Ctor (HashTable_t* myHashTable, size_t Size, size_t (*HashFunction) (HT_Key_t));
int         HT_Dtor (HashTable_t* myHashTable);
int         HT_Add  (HashTable_t* myHashTable, HT_Key_t Key, HT_Value_t Value);
DLL_Node_t* HT_Find (HashTable_t* myHashTable, HT_Key_t Key, HT_Value_t Value);
```

Во второй части я работал с профилировщиком и оптимизировал скорость работы хэш таблиц с помощью встройки, выравниваний, ассемблерных вставок и SIMD инструкций.

## Первая часть

В этой части я тестирую различные хэш-функции на однородность

*Функции*:
1. Хэш возвращает 0.
2. Хэш возвращает ascii код 1 буквы слова.
3. Хэш возвращает длину слова.
4. Хэш возвращает сумму ascii кодов слова.
5. Хэщ возвращает (сумму букв)/(длину слова).
6. ROL хэш. `hash[i] = ROL(hash[i-1]) xor str[i]`
7. ROR хэш. `hash[i] = ROR(hash[i-1]) xor str[i]`
8. [CRC32 Хэш](https://ru.wikibooks.org/wiki/Реализации_алгоритмов/Циклический_избыточный_код).
9. [Elf Хэш](https://en.wikipedia.org/wiki/PJW_hash_function)
   
Для изучения распределения я строил гистограммы `КоличествоКоллизий(ЗначениеХэша)`. Диаграммы показывают распределение числа коллизий по значениям хэш-функции, в некоторых из них есть пики, которые влияют на скорость работы хэш таблицы(линейный поиску по списку).

Также размер хэш таблицы фиксирован и является простым числом.

Теория по хэш таблицам бралась из [Википедии](https://en.wikipedia.org/wiki/Hash_function)

### Гистограммы

<p> <strong>

В 1-4 представлены простые хэш-функции, которые не используются в реальных проектах. У них нет одинакового размера, они рассматриваются в учебных целях.

В 5-9 представлены сложные хэш-функции, которые применяются в разработке. Их я сравнивал отдельно с одинаковым размером таблицы

1. <U> 0 Hash </U>: Размер: 10, макс. коллизия: 5303. 
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/61333956-a35a-4f0f-af68-7bee1986091e" width = 100%>

2. <U> First Letter Hash </U>: Размер: 128, макс. коллизия: 517.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/8e5e56c6-c7dd-44e6-96ad-1ec14d090e33" width = 100%>

3. <U> Length Word Hash </U>: Размер 30, макс. коллизия: 1029. 
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/bb7af6ef-aeb6-4f67-b70e-cfd61a1a5fc9" width = 100%>

4. <U> Sum of letters Hash </U>: Размер 1500, макс. коллизия: 31.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/29571f02-eda4-4fdb-af57-7083f16663b2" width = 100%>

5. <U> (Sum of letters)/Length Hash </U>: Размер 179, макс. коллизия: 634.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/5f3ff2b0-f369-4c9d-a2a4-4801c5af6b19" width = 100%>

6. <U> ROR Hash </U>: Размер 6007, макс. коллизия: 31.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/7b634afc-b8d7-4860-8029-eea0b7594a65" width = 100%>

7. <U> ROL Hash </U>: Размер 6007, макс. коллизия: 9.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/894ab5e4-0485-4f3e-be01-c667d62cb658" width = 100%>

8. <U> CRC32 Hash </U>: Размер 6007, макс. коллизия: 6.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/20444915-5d1f-4b88-83c6-5bb6d043fdf8" width = 100%>

9.  <U> ElfHash </U>: Размер 6007, макс. коллизия: 6.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/48211671-75fe-467e-b408-7ed562fdfb6c" width = 100%>

</strong></p>

### Однородность

Для оценки однородности распределения хэш-функции можно использовать [тест хи-квадрат](https://en.wikipedia.org/wiki/Chi-squared_test).

Теория для хэш таблиц: [хи-квадрат из Википедии](https://stats.stackexchange.com/questions/532395/about-the-explanation-of-testing-and-measurement-of-hash-functions-at-wikipedia)

Этот тест является критерием соответствия: он сравнивает фактическое распределение элементов в ячейках с ожидаемым (или равномерным) распределением.


$$\chi^2 = \sum_{j} \frac{r_j^2}{e_j} = \sum_{j} \frac{(b_j-n/m)^2}{n/m} = \frac{m}{n}\sum_{j} \left(b_j - \frac{n}{m}\right)^2 = \frac{m}{n}\left[\sum_j b_j^2\right] - n.$$

$$ 
\text{When hashing is truly uniform and independent, the expectation of $\chi^2$ is $m - 1$ and its variance is $2 (m - 1)$.}
$$

$$
\text{Formula of chi-squared test:}
$$

$$
\begin{aligned}
W = \frac{1}{n/(2m)(n+2m-1)} \sum_j b_j(b_j+1)/2 &= \frac{1}{n+2m-1}\left(\frac{m}{n}\left[\sum_j b_j^2\right] + m\right) \\
  &= \frac{1}{n+2m-1}\left(\chi^2 + n + m\right).
\end{aligned}
$$

$$
\text{Where:}
$$

- $n$ is the number of keys
- $m$ is the number of buckets
- $b_j$ is the number of items in bucket $j$

$$
\text{A ratio within one confidence interval (0.95 - 1.05) is indicative} 
$$

$$
\text{that the hash function evaluated has an expected uniform distribution.}
$$

$$
\text{However, its expectation indeed is:}
$$

$$
E[W] = E\left[\frac{1}{n+2m-1}\left(\chi^2 + n + m\right)\right] = \frac{1}{n+2m-1}\left((m-1) + n + m\right) = 1
$$

$$
\text{as advertised. What is of greatest interest is its variance,}
$$

$$
\text{because that determines how wide a reasonable confidence interval should be:}
$$

$$
Var(W) = \frac{Var(\chi^2)}{(n+2m-1)^2} = \frac{m-1}{(n+2m-1)^2}
$$

$$
\text{Which means its standard deviation is:}
$$

$$
SD(W) = \sqrt{Var(W)} = \frac{\sqrt{m-1}}{n+2m-1}.
$$

$$
\text{Reasonable confidence limits for W should be a small multiple of this SD away from the expected value of 1.}
$$

После обработки результатов я построил диаграмму значений теста хи-квадрат:

<img src = "https://github.com/khmelnitskiianton/HashTable/assets/142332024/bf824d62-ab3d-4846-8975-d7e0d6741015" width = 100%>

После анализирования мы можем видеть что если значения теста хи квадрат стремится к $\frac{m}{n} = 0.79$, то распределение более однородно. Лучшие функции в однородности стали CRC32, ElfHash и ROL Hash.

## Вторая часть

*Система:*

- Linux Mint 21.3 Cinnamon
- 12th Gen Intel Core i5-12450H x 8
- CPU Temperature: 45-55 $^\circ C$, нет троттлинга
- GCC x86-64 -O3 -msse4.1 -msse4.2 -mavx2 -mavx

Я создал стресс тест: загружал большой текст и искал слова по таблице много раз.
Далле с помощью профилировщика я нашел узкие места, которые замедляли скорость работы таблицы и оптимизировал их.

В качестве профилировщика использовался [Perf](https://perf.wiki.kernel.org/index.php/Tutorial) & [Guide Perf](https://stackoverflow.com/questions/1777556/alternatives-to-gprof/10958510#10958510) и визуализировал с помощью [HotSpot](https://github.com/KDAB/hotspot).

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

Первое контрольное время работы программы: $3.61 \cdot 10^6$ тиков.

> Из-за многих факторов длительность фрагмента кода измеренная в тиках не постоянна([Статья](https://habr.com/ru/companies/intel/articles/260113/)), поэтому разброс значение составляет $\pm 5$%. Во-первых, Внеочередное исполнение (Out of Order Execution, OoO), процессор может исполнять машинные инструкции в порядке, отличном от использованного в программе, или даже параллельно (если они не зависят друг от друга), это означает, что исполнение rdtsc может быть задержано или, наоборот, выполнено раньше, чем того требует последовательный программный порядок. Во-вторых, управление энергопотреблением, Процессор довольно значительную долю времени может быть приостановлен для экономии энергии (C-состояния), исполняя инструкции, он может использовать динамическое изменение частоты для экономии энергии (P-состояния) или наоборот, для максимизации производительности (Turbo-состояния). В-третьих, многопроцессорность и многоядерность, в системе с несколькими потоками, ядрами или процессорами у каждого из логических процессоров будет свой TSC.

### Оптимизации

0. **Оптимизация встройкой:**
   
Во-первых, Я решил сделать функции поиска встроенными, на вызов функций сравнения тратиться время, несмотря на то что блок кода мал. Поэтому я поместил функции поиска по списку и сравнение в `.h` и использовал `inline`.

> [GCC](https://microsin.net/programming/avr/gcc-inline-functions.html) не встраивает функции при отключенной оптимизации, поэтому нужно использовать атрибут `inline __attribute__((always_inline))`. Поэтому при работе с `-O0` GCC не будет встраивать ничего через обычный `inline` (Это видно из профилировщика).

Новое время стресс теста - $3.00 \cdot 10^6$ тиков. Встраивание дало прирост 20 % в скорости работы программы, так как были убраны множественные вызовы функций.

1. **Оптимизация хэш-функции:**

Во-первых, я переписал ElfHash на ассемблере 

```assembly
global ElfHash
section .text
ElfHash:
        movsx   rdx, byte [rdi]
        xor     eax, eax
        test    dl, dl
        je      .L5
.L4:
        sal     rax, 4
        add     rdi, 1
        add     rdx, rax
        mov     rax, rdx
        and     eax, 4026531840
        mov     rcx, rax
        shr     rcx, 24
        xor     rcx, rdx
        test    rax, rax
        not     rax
        cmovne  rdx, rcx
        and     rax, rdx
        movsx   rdx, byte[rdi]
        test    dl, dl
        jne     .L4
        ret
.L5:
        ret
```

Это не дало прироста в скорости. Поэтому я решил изменить хэш с ElfHash на CRC32 Hash.

Первая версия CRC32 использовала полином с помощью постоянного массива. Скорость оказалась такой же как у ElfHash.

Вторая версия использовала встроенные функции SSE `_mm_crc32_u8 (crc, char)`, $2.73 \cdot 10^6$ тиков:

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

Первый результат был странным, время возросло в 2 раза. Это вызвано тем, что выравнивание играет важную роль в скорости работы, т.к. SIMD инструкции зависят от кэша и буфер который загружается в кэш зависит от адреса([Статья от Intel](https://habr.com/ru/companies/intel/articles/262933/))

> Лучшее время достигается при использовании `_mm_crc32_u64()` + `aligned_alloc()`, а не `_mm_crc32_u8()` + `calloc()`

Поэтому сначала я создал выравненный буфер и поместил туда все слова.
Я использовал `aligned_alloc(ALIGNING, bytes)` + `memset()` затем скопировал все слова.

Итоговое время стресс теста - $2.19 \cdot 10^6$ тиков (прирост 19 %)

> Я использовал выравнивание по 16 байт. Большее выравнивание не дает ускорения. Если использовать выравнивание по 8 байт и менее это приведет к задержкам, поэтому я подтвердил результаты статьи.

2. **STRCMP Оптимизация:**

После всех оптимизаций самый долгий процесс это `strcmp()`. Я использовал AVX инструкции.

В моих данных самое длинное слово размером 14 букв, поэтому я использовал 16 байтные вектора `__m128`.

```cpp
__m128i str1 = _mm_load_si128((const __m128i *) (val1.Key)); 
__m128i str2 = _mm_load_si128((const __m128i *) (val2.Key));
__m128i cmp  = _mm_cmpeq_epi8 (str1, str2);
int result   = _mm_movemask_epi8 (cmp);
return ((result == 0xFFFF) && (val1.Value == val2.Value));
```

> `_mm_load_si128()` работает быстрее других функций загрузки в вектор, но она требует выравнивания адреса по 16 байт, иначе это приведет к ошибке `load of misaligned address`-Ошибка сегментации.

Новое время - $1.89 \cdot 10^6$ тиков (прирост 13%)

**Итоговый отчет Perf:**

<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/32edd8e2-a6fe-408c-82b8-2c2eb8477ccd" width = 100%>

## Итоги и выводы

После всех оптимизаций я ускорил работу программы в  1.85x - 1.9x (погрешность)! Я добавил скорость к оптимизации компилятора GCC на `-O3` почти в 2 раза.

*Таблица результатов*:

|Оптимизация               |Тики ($10^6$)     |Ускорение(в сравнении с началом) |
|:------------------------:|-----------------:|:-------------------------------:|
|Начало с `-O3`            | 3.61             |1.00x                            |
|Встройка                  | 3.01             |1.20x                            |
|Смена ElfHash на CRC32    | 2.73             |1.32x                            |
|Выравнивание              | 2.64             |1.36x                            |
|Векторизация CRC32        | 2.19             |1.64x                            |
|Векторизация `strcmp()`   | 1.89             |1.90x                            |

В этом проекте я исследовал хэш-функции, работал с профилировщиком(Perf & HotSpot), ищя узкие места в работе программы и устранял их с помощью оптимизаций таких как: встройка, выравнивание, SIMD инструкции, ассемблерные вставки.

$DedInsideCoeff = \frac{boost}{amount \space asm \space strings} \cdot 100 = \frac{190}{12} = 15.8$ 
