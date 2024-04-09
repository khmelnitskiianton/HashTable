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

To start program you need to use CMake and run program

1.  Clone repository
2.  Run Makefile (compile program), write command in main directory in repo
3.  Run program(before you need to create this file) 

Configure Makefile to change hash function or data

Create `./obj` & `./Perf/perf.data` file

`make all` will do everything, set `TEST`,`SIZE`,`NAME_HASH` to manipulate tests and `CFLAGS`, `PERF`, `PERF_FLAGS` to perf profiling

```bash
git clone https://github.com/khmelnitskiianton/HashTable.git
cd ./HashTable
make all    #for histograms
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
```

## Description

In this project I search diffrent hash functions for uniformity and optimize hash table.

In first part I take many hash functions and create histograms. Data base consists of words from [William Shakespeare. The Tragedy Of Hamlet, Prince Of Denmark](http://lib.ru/SHAKESPEARE/ENGL/hamlet_en.txt) (32k words). 

Hash Table bases on double linked list. Element has hash, key, value, occurance(no duplicates). 
```
int         HT_Ctor (HashTable_t* myHashTable, size_t Size, size_t (*HashFunction) (HT_Key_t));
int         HT_Dtor (HashTable_t* myHashTable);
int         HT_Add  (HashTable_t* myHashTable, HT_Key_t Key, HT_Value_t Value);
DLL_Node_t* HT_Find (HashTable_t* myHashTable, HT_Key_t Key, HT_Value_t Value);
```
In second part I optimizes speed of functions using SIMD & asm tricks.

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

1. <U> 0 Hash </U>: Size: 10, Max Collusion: 5335. 
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/d3cc18d6-5d7d-4beb-b994-02d377d89f0f" width = 100%>

2. <U> First Letter Hash </U>: Size: 128, Max Collusion: 521. 
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/4d82c0d1-cc18-4fbf-ad78-0512cb11cb51" width = 100%>

3. <U> Length Word Hash </U>: Size 30, Max Collustion: 1023. 
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/99be4a00-f5a3-441d-a1f5-9bf6a16c35ae" width = 100%>

4. <U> Sum of letters Hash </U>: Size 1500, Max Collusion: 31.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/b612478f-ee30-4f81-b9f1-ca258164eb21" width = 100%>

5. <U> (Sum of letters)/Length Hash </U>: Size 179, Max Collusion: 636.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/6e6f7c85-1395-492c-8a07-a8b259319a2a" width = 100%>

6. <U> ROR Hash </U>: Size 6007, Max Collusion: 30.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/16fe4f77-fb2a-4fe9-ab0a-5b0608c9d4b5" width = 100%>

7. <U> ROL Hash </U>: Size 6007, Max Collusion: 9.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/b96af6bd-cf03-4f2f-bf46-73da561749ec" width = 100%>

1. <U> CRC32 Hash </U>: Size 6007, Max Collusion: 5.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/bc20032b-c720-48ce-9bf8-7b4f57fc8b0d" width = 100%>

1.  <U> ElfHash </U>: Size 6007, Max Collusion: 6.
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/60031a17-3a29-4486-8277-82cf592981ed" width = 100%>

</strong></p>

### Uniformity

When testing a hash function, the uniformity of the distribution of hash values can be evaluated by the [chi-squared test](https://en.wikipedia.org/wiki/Chi-squared_test).

Theory source: [chi-squred test for hash](https://stats.stackexchange.com/questions/532395/about-the-explanation-of-testing-and-measurement-of-hash-functions-at-wikipedia)

This test is a goodness-of-fit measure: it's the actual distribution of items in buckets versus the expected (or uniform) distribution of items.

<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/0eb840bb-b00d-4ec9-9788-aa83fd5af00e" width = 100%>

After processing all function I plot histogram of chi-squared test:

<img src = "https://github.com/khmelnitskiianton/HashTable/assets/142332024/af76129a-5e16-4917-ac91-bff43618bfd2" width = 100%>

After analysing we can see if hash function's chi tends to 1 is better in uniformity, its distribution is more homogeneous. Best in uniformity functions is CRC32, ElfHash and ROL Hash.

## Second Part

I want to speed up my hash table. So I do stress tests: load big texts and finding some keys many times. 
After finding weak point I try to optimize it with help of SIMD, ASM inserts.

Results of profiling are calculated by [`Perf`](https://perf.wiki.kernel.org/index.php/Tutorial) tool and vizualized by [HotSpot](https://github.com/KDAB/hotspot).

I find in loop all words 256 times `StressTest()`.

I use [Guide Perf](https://stackoverflow.com/questions/1777556/alternatives-to-gprof/10958510#10958510) to profile my hash table. After see console version Perf I decide to work in HotSpot where information is vizualized in application with hierarchy.

**Console version:**
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/0b8bd9b2-27c3-4acc-af5a-6180e3de446e" width = 100%>

**HotSpot version:**
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/89b9cfcd-e3fe-490a-b406-aa5daf825764" width = 100%>

**Analysing Profilier**: (Size=6007, Hash: Elf Hash)

<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/ef78e3a1-ec0b-4a28-b44c-4fde2106eff2" width = 100%>

I dont optimize functions like `InsertData` and `Dtor/Ctor` because they are single and use specific functions to work with files.

That's why most weak points are `HT_Find`, `ElfHash`

### Optimization 

1. **Inline Optimization:**
   
> Percantages in Perf aren't fixed, because it depends of processors working. That's why I have deviation $\pm 2\%$

First I decide to make search functions inline because this functions are called everytime but its body small, thats why it waste time only on calling. So I put this functions in headers and use `inline __attribute__((always_inline))`

<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/b846d5e7-adb4-4eda-88a0-defe33100d54" width = 100%>

> [GCC](https://microsin.net/programming/avr/gcc-inline-functions.html) does not embed any functions when optimization is not being done, except if you specify the always_inline attribute for the function `inline __attribute__((always_inline))`. That's why if you work with -O0 GCC will not done insertion  with only `inline` (I find out it in profilier).

I count boost with summing percantages of workload:

$boost_1 = \Delta(speed) = worload_{before} - workload_{after} = (3\% + 18.75\% + 24.58\% + 8.41\%) - 50.46\% = 4,28\%$, its net gain of inline optimizations (-O0 used). 

2. **GCC Optimization:**
   
I decided to add compilier optimization with -O1 to see what boost i will have.

<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/4f14d99e-dfca-4258-b4a9-d95985138dfc" width = 100%>

$boost_2 = 9\%$, in stress test function

1. **Hash Optimization:**

First I optimized hash by rewriting [ElfHash on asm](https://github.com/khmelnitskiianton/HashTable/tree/main/HashTable/HashAsm.nasm). it gives zero effect on workload.

That's why I change Elf Hash for CRC32 Hash. 

First version is dry many cycles of table:

<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/edb14082-895a-488f-a2d8-8ec31106f469" width = 100%>

Second version add const table of polynom, speed equal to Elf Hash:

<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/7768dfae-09c1-470f-9a58-05dd5c447a02" width = 100%>

Third version has SSE intrinsic CRC32 `_mm_crc32_u8 (crc, char)`:

<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/d9b1013c-d17d-46b2-9522-e1c1d4c88156" width = 100%>

$boost_3 = 6\%$ finally result from changing Elf Hash to CRC32 and optimized it with intrinsic.

4. **STRCMP Optimization:**

...

5. **ASM Optimization:**
  
...

### Sum up Optimization

...