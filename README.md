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

## Installation

To start program you need to use CMake and run program

1.  Clone repository
2.  Run Makefile (compile program), write command in main directory in repo
3.  Run program(before you need to create this file) 

Configure Makefile to change hash function or data

```c
git clone https://github.com/khmelnitskiianton/HashTable.git
cd ./HashTable
make all
```

## Extra programs

GCC compilier, Makefile for collection, Python & Seaborn to draw histograms.

```c
sudo apt update && sudo apt upgrade     #update
sudo apt install build-essential        #gcc
sudo apt install make                   #makefile
sudo apt install python3 -y             #python
python -m pip install --upgrade pip     #pip
pip install seaborn                     #seaborn
```

## Description

In this project I search diffrent hash functions for uniformity and optimize hash table.

In first part I take many hash functions and create histograms. Data base consists of words from [William Shakespeare. The Tragedy Of Hamlet, Prince Of Denmark](http://lib.ru/SHAKESPEARE/ENGL/hamlet_en.txt) (32k words). 

Hash Table bases on double linked list. Element has hash, key, value, occurance(no duplicates). 
```
int HT_Ctor (HashTable_t* myHashTable, size_t Size, size_t (*HashFunction) (HT_Key_t));
int HT_Dtor (HashTable_t* myHashTable);
int HT_Add  (HashTable_t* myHashTable, HT_Key_t Key, HT_Value_t Value);
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
6. ROL Hash.
7. CRC32 Hash.
8. My own hash.
   
For searching uniformity I plot histograms `AmountOfCollusions(HashIndex)`. Diagrams show distribution of collusions in current hash function, there are spikes in some functions that affect on speed of working with hash table.

Check theory in [hash in wiki](https://en.wikipedia.org/wiki/Hash_function)

### Histograms

<p> <strong>

1. <U> 0 Hash </U>: Size: 10, Max Collusion: 5335. 
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/972369d6-730c-488a-8849-8b1e3abff397" width = 80%>

2. <U> First Letter Hash </U>: Size: 128, Max Collusion: 521. 
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/4d82c0d1-cc18-4fbf-ad78-0512cb11cb51" width = 80%>

3. <U> Length Word Hash </U>: Size 25, Max Collustion: 1023. 
<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/99be4a00-f5a3-441d-a1f5-9bf6a16c35ae" width = 80%>

4. 
</strong></p>

### Uniformity

When testing a hash function, the uniformity of the distribution of hash values can be evaluated by the [chi-squared test](https://en.wikipedia.org/wiki/Chi-squared_test).

Theory source: [chi-squred test for hash](https://stats.stackexchange.com/questions/532395/about-the-explanation-of-testing-and-measurement-of-hash-functions-at-wikipedia)

This test is a goodness-of-fit measure: it's the actual distribution of items in buckets versus the expected (or uniform) distribution of items.

<img src="https://github.com/khmelnitskiianton/HashTable/assets/142332024/0eb840bb-b00d-4ec9-9788-aa83fd5af00e" width = 80%>

After processing all function I plot histogram of chi-squared test:

<img src = "https://github.com/khmelnitskiianton/HashTable/assets/142332024/5894eb58-b932-4e4f-ae73-3c43bee64bef" width = 80%>

After analysing we can see if hash function's chi tends to 1 is better in uniformity, its distribution is more homogeneous.