# HashTable

Project of searching and optimizing hash functions & hash table

## Table of Contents

- [HashTable](#hashtable)
  - [Table of Contents](#table-of-contents)
  - [Installation](#installation)
  - [Extra programs](#extra-programs)
  - [Description](#description)
  - [First Part](#first-part)
    - [Histograms](#histograms)
    - [Uniformity](#uniformity)
  - [Second Part](#second-part)
    - [Optimization](#optimization)
    - [Sum up Optimization](#sum-up-optimization)

## Installation

To start program you need to use Makefile and run program

1.  Clone repository
2.  Run Makefile (compile program), write command in main directory in repo
3.  Run program(before you need to create this file) 

Configure Makefile to change hash function or data

`make all` will do everything, set `TEST`,`SIZE`,`NAME_HASH` to manipulate tests and `CFLAGS`, `PERF`, `PERF_FLAGS` to perf profiling

```bash
git clone https://github.com/khmelnitskiianton/HashTable.git
cd ./HashTable
make init    #create folders for temporary objects
make all     #for histograms
make test    #for perf test
```

## Extra programs

GCC compilier, Makefile for collection, Python & Seaborn to draw histograms.

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

## Description

In this project I search diffrent hash functions for uniformity and optimize hash table.

In first part I take many hash functions and search them for homogenious distribution. Data base consists of words from [William Shakespeare. The Tragedy Of Hamlet, Prince Of Denmark](http://lib.ru/SHAKESPEARE/ENGL/hamlet_en.txt) (5k unique words). 

Hash Table bases on double linked list. Element has hash, key, value. 
```
int         HT_Ctor (HashTable_t* myHashTable, size_t Size, size_t (*HashFunction) (HT_Key_t));
int         HT_Dtor (HashTable_t* myHashTable);
int         HT_Add  (HashTable_t* myHashTable, HT_Key_t Key, HT_Value_t Value);
DLL_Node_t* HT_Find (HashTable_t* myHashTable, HT_Key_t Key, HT_Value_t Value);
```
In second part I optimizes speed of functions using aligning, SIMD and assembler inserts.

## First Part

In first part we searching diffrent hash functions & load factors.

*Functions*:
1. Hash returns 0.
2. Hash that returns ascii code of word first letter.
3. Hash returns length of word.
4. Hash returns sum of ascii codes of letters.
5. Hash returns (sum of ascii codes of letters)/(length of word).
6. ROL Hash. `hash[i] = ROL(hash[i-1]) xor str[i]`
7. ROR Hash. `hash[i] = ROR(hash[i-1]) xor str[i]`
8. [CRC32 Hash](https://ru.wikibooks.org/wiki/Реализации_алгоритмов/Циклический_избыточный_код).
9. [ElfHash](https://en.wikipedia.org/wiki/PJW_hash_function)
   
For searching uniformity I plot histograms `AmountOfCollusions(HashIndex)`. Diagrams show distribution of collusions in current hash function, there are spikes in some functions that affect on speed of working with hash table. 

Test size of hash table for big tests was prime number! 

Check theory in [wiki](https://en.wikipedia.org/wiki/Hash_function)

### Histograms

<p> <strong>

1. <U> 0 Hash </U>: Size: 10, Max Collusion: 4788. 
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/96c683bd-4517-4a3b-b01a-3369ceee4f81" width = 100%>

2. <U> First Letter Hash </U>: Size: 128, Max Collusion: 461. 
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/b9d41ce5-c9a2-48a0-8163-fcb986833191" width = 100%>

3. <U> Length Word Hash </U>: Size 30, Max Collustion: 929. 
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/57b06bba-ba51-4afd-92de-2ecc29120fd8" width = 100%>

4. <U> Sum of letters Hash </U>: Size 1500, Max Collusion: 31.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/50dbd0c6-556f-4271-97d6-f23fd0059b6c" width = 100%>

5. <U> (Sum of letters)/Length Hash </U>: Size 179, Max Collusion: 542.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/5b5f7788-5f04-45b0-baa5-377dca79f93d" width = 100%>

6. <U> ROR Hash </U>: Size 6007, Max Collusion: 27.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/b8276c9e-3c0d-45ed-8dd6-0804990ff27c" width = 100%>

7. <U> ROL Hash </U>: Size 6007, Max Collusion: 9.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/4462c3f8-3ebb-4fe1-91e2-b770c9913388" width = 100%>

8. <U> CRC32 Hash </U>: Size 6007, Max Collusion: 5.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/1b3654c0-ebb3-4489-b8b3-0907d9eed7ad" width = 100%>

9.  <U> ElfHash </U>: Size 6007, Max Collusion: 6.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/61f15010-a60d-4ada-bb0b-04395331352c" width = 100%>

</strong></p>

### Uniformity

When testing a hash function, the uniformity of the distribution of hash values can be evaluated by the [chi-squared test](https://en.wikipedia.org/wiki/Chi-squared_test).

Theory source: [chi-squred test for hash](https://stats.stackexchange.com/questions/532395/about-the-explanation-of-testing-and-measurement-of-hash-functions-at-wikipedia)

This test is a goodness-of-fit measure: it's the actual distribution of items in buckets versus the expected (or uniform) distribution of items.

<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/0eb840bb-b00d-4ec9-9788-aa83fd5af00e" width = 100%>

After processing all function I plot histogram of chi-squared test:

<img src = "https://github.com/khmelnitskiianton/HashTable/assets/142332024/af76129a-5e16-4917-ac91-bff43618bfd2" width = 100%>

After analysing we can see if hash function's chi tends to $\frac{m}{n} = 0.79$ is better in uniformity, its distribution is more homogeneous. Best in uniformity functions is CRC32, ElfHash and ROL Hash.

## Second Part

*System:*

- Linux Mint 21.3 Cinnamon
- 12th Gen Intel Core i5-12450H x 8
- GCC x86-64 -O3 -msse4.1 -msse4.2 -mavx2 -mavx

I want to speed up my hash table. So I do stress tests: load big texts and finding some keys many times. 
After finding weak point I try to optimize it with help of SIMD, ASM inserts.

Results of profiling are calculated by [`Perf`](https://perf.wiki.kernel.org/index.php/Tutorial) tool and vizualized by [HotSpot](https://github.com/KDAB/hotspot).

I find in loop all words 512 times in `StressTest()`.

I use [Guide Perf](https://stackoverflow.com/questions/1777556/alternatives-to-gprof/10958510#10958510) to profile my hash table. After see console version Perf I decide to work in HotSpot where information is vizualized in application with hierarchy.

**Console version:**
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/ef78e3a1-ec0b-4a28-b44c-4fde2106eff2" width = 100%>

**HotSpot version:**
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/6a564442-141e-4e42-bdbe-98ce62c61835" width = 100%>

**Analysing Profilier**: (Size=6007, Hash: Elf Hash)

I don't optimize functions like `InsertData` and `Dtor/Ctor` because they are single and use specific functions to work with files.

That's why most weak points are `HT_Find()`, `DLL_Find()`, `DLL_Compare()`, `ElfHash`, `strcmp()`, 

I check time of running stress test with `__rdtsc()`

First time of stress test - 1464801878 ticks

### Optimization 

0. **Inline Optimization:**
   
> Ticks aren't fixed because cpu frequency isn't constant. That's why I have deviation $\pm 2$ %

First I decide to make search functions inline because this functions are called everytime but its body small, thats why it waste time only on calling. So I put this functions in headers and use `inline`

> [GCC](https://microsin.net/programming/avr/gcc-inline-functions.html) does not embed any functions when optimization is not being done, except if you specify the always_inline attribute for the function `inline __attribute__((always_inline))`. That's why if you work with -O0 GCC will not done insertion  with only `inline` (I find out it in profilier).

New time of stress test - 1440749854 ticks. No boost on `-O3` because on this level of optimization it makes functions inlined itself.

1. **Hash Optimization:**

First I optimized hash by rewriting [ElfHash on asm](https://github.com/khmelnitskiianton/HashTable/tree/main/HashTable/AsmFunctions.nasm). It gives zero effect on workload.

That's why I change Elf Hash for CRC32 Hash. 

First version is dry with many cycles to process table.

Second version add const table of polynom, speed equal to Elf Hash.

Third version has SSE intrinsic CRC32 `_mm_crc32_u8 (crc, char)`, 1261751260 ticks:

```cpp
size_t crc = 0xFFFFFFFFUL;
for (size_t i = 0; i < length; i++)
    crc = _mm_crc32_u8 (crc, str[i]); 
return crc ^ 0xFFFFFFFFUL;
```    

I try to do it on assembler with `asm()`:

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
It has educational aim. Diffrence between writing with `_mm_crc32_u8()` or with `asm()` is zero.

Also I tried to write with `_mm_crc32_u64()`:

```cpp
size_t crc = 0xFFFFFFFFUL;    
crc = _mm_crc32_u64 (crc, *((uint64_t*) str));
crc = _mm_crc32_u64 (crc, *(((uint64_t*) str)+1));
return crc ^ 0xFFFFFFFFUL; 
```

First result is bad, time is more than 3 times longer. I search reason of this (because its strange, 2 repetitions than loop).Then I found out that aligning plays an important role, because SIMD instructions depend on cash, and buffer that upload to cash depends on address position([article](https://habr.com/ru/companies/intel/articles/262933/)).

> Spoiler: best speed is achieved when I choose setup `_mm_crc32_u64()` + `aligned_alloc()` then `_mm_crc32_u8()` + `calloc()`

That's why first action is align buffer that I get from file of words.
I use `aligned_alloc(ALIGNING, bytes)` + `memset()` than copy my words from text buffer to new, aligned buffer and work with it.

New time of stress test - 1055204350 ticks finally result from changing Elf Hash to CRC32 and optimized it with intrinsic.

> I choose optimal aligning of 16 bytes. If I use more it will be no boost. If I use 8 or less it will be delaying. So I confirmed results of [article](https://habr.com/ru/companies/intel/articles/262933/).

1. **STRCMP Optimization:**

After all optimizations the most workload process is `strcmp()`. I use AVX instructions.

*Intrinsics:*

In my dictionary longest word has 14 symbols, that's why I use for 16byte words `__m128`

```cpp
__m128i str1 = _mm_load_si128((const __m128i *) (val1.Key)); 
__m128i str2 = _mm_load_si128((const __m128i *) (val2.Key));
__m128i cmp  = _mm_cmpeq_epi8 (str1, str2);
int result   = _mm_movemask_epi8 (cmp);
return ((result == 0xFFFF) && (val1.Value == val2.Value));
```

> `_mm_load_si128()` work faster than another load functions, but it needs aligning 16 bytes, without it it will be `load of misaligned address` aka SegFault 

New time of stress test - 862053822 ticks

**Final report of Perf:**

<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/32edd8e2-a6fe-408c-82b8-2c2eb8477ccd" width = 100%>

### Sum up Optimization

After all optimizations I get boost 1.7x - 1.8x (depends on current speed of my cpu)! Its incrediable, I outrun GCC optimization `-O3` almost by 1.71 times! 

|Optimization            |Ticks      |Boost(compare with begining)|
|:------------------------:|-----------:|:-----:|
|Start with `-O3`        |1464801878|1x   |
|Inlining                |1440749854|1.02x|
|Vectorization Crc32     |1287858394|1.14x|
|Aligning                |993603597|1.47x|
|Vectorization `strcmp()`|858662731|1.71x|

In this project I search Hash Functions, use Profilier (Perf & HotSpot) to see weak points in my program, then I use inlining, SIMD instructions, aligning and ASM inserts to get boost in speed.

$DedInsideCoeff = \frac{boost}{amount \space asm-strings} \cdot 100 = \frac{171}{12} = 14,25$ !
