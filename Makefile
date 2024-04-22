#===============================#
#			MAKEFILE			# 
#		FOR TEST HASH TABLES	#
#===============================#
# make all - plot histograms    #
# make test - test ht with perf #
#===============================#
# 			CHANGE				#
#	TEST - NUM OF HASH FUNC		#
#   SIZE - SIZE OF HASH TABLE   #
# NAME_HASH - NAME OF HASH TABLE#
# CFLAGS - for all				# 
# PERF - for test				#
#===============================#

TEST = 1 #number of test
SIZE = 10 #size of ht
NAME_HASH = "First Letter Hash" #name of ht

#PERF_FLAGS = -fno-omit-frame-pointer -g -O3 -msse4.1 -msse4.2 -mavx2 -mavx
#PERF = sudo perf record --call-graph dwarf -o ./Perf/perf.data --

CFLAGS = -g -march=native -msse4.1 -msse4.2 -mavx2 -mavx -fno-omit-frame-pointer -D _DEBUG -ggdb3 -std=c++17 -O3 					 \
		-fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer 		 \
		-Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual 		 \
		-Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral  \
		-Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual \
		-Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel 		 \
		-Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override 			 \
		-Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros 			 \
		-Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector 		 \
		-Wlarger-than=8192 -Wstack-usage=8192 -Werror=vla 																			 \
		-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,$\
		nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,$\
		unreachable,vla-bound,vptr

CDEFINE = #-DTESTING #-DASM_HASH 

COMPILIER = g++
NAME = hash_table

SRC_ASM_EXT = .nasm
SRC_C_EXT   = .cpp

SRC_HT  = ./HashTable
SRC_DLL = ./DLinkedList
SRC_PYTHON = ./Processing
SRC_ASM = ./HashTable

OBJ     = ./obj
BIN     = ./
OUT     = $(BIN)
SRC_SRC = ./src
#===================================================================================

SOURCES_ASM 	= $(wildcard $(SRC_ASM)/*$(SRC_ASM_EXT))
OBJFILES_ASM    = $(patsubst $(SRC_ASM)/%,$(OBJ)/%,$(SOURCES_ASM:$(SRC_ASM_EXT)=.o))

SOURCES_HT   = $(wildcard $(SRC_HT)/*$(SRC_C_EXT))
OBJFILES_HT  = $(patsubst $(SRC_HT)/%,$(OBJ)/%,$(SOURCES_HT:$(SRC_C_EXT)=.o))

SOURCES_SRC  = $(wildcard $(SRC_SRC)/*$(SRC_C_EXT))
OBJFILES_SRC = $(patsubst $(SRC_SRC)/%,$(OBJ)/%,$(SOURCES_SRC:$(SRC_C_EXT)=.o))

#===================================================================================
#Draw

$(OUT)$(NAME): $(OBJFILES_HT) $(OBJFILES_SRC) $(OBJFILES_ASM) #Linking with GCC use -no-pie
	$(COMPILIER) $(PERF_FLAGS) $(CFLAGS) $^ -o $@ -no-pie

$(OBJ)/%.o: $(SRC_HT)/%$(SRC_C_EXT) #Object with GCC 
	$(COMPILIER) $(PERF_FLAGS) $(CFLAGS) -c -o $@ $<

$(OBJ)/%.o: $(SRC_SRC)/%$(SRC_C_EXT) #Object with GCC
	$(COMPILIER) $(PERF_FLAGS) $(CFLAGS) $(CDEFINE) -c -o $@ $<

$(OBJ)/%.o : $(SRC_ASM)/%$(SRC_ASM_EXT)			#Assemble with NASM -f elf64
	nasm -f elf64 -l $@.lst $< -o $@

.PHONY: init
init:
	mkdir obj
	mkdir Perf

.PHONY: all
all: $(OUT)$(NAME) run draw chi clean

.PHONY: run
run:
	$(PERF) $(OUT)$(NAME) $(SRC_PYTHON)/dictionary.txt $(SRC_PYTHON)/data.txt $(SIZE) $(TEST)

.PHONY: convert
convert:
	python3 $(SRC_PYTHON)/converter.py 

.PHONY: draw
draw:
	python3 $(SRC_PYTHON)/histogram.py $(TEST) $(NAME_HASH)

.PHONY: chi
chi:
	python3 $(SRC_PYTHON)/chi.py

.PHONY: clean
clean:
	rm -f $(OBJ)/*.o $(OBJ)/*.lst

#========================================================================================
#TEST
.PHONY: test
test: $(OUT)$(NAME) run perf_report clean

.PHONY: perf_report
perf_report: 
	perf report -g graph -i ./Perf/perf.data

.PHONY: perf_hotspot
perf_hotspot: 
	sudo hotspot ./Perf/perf.data