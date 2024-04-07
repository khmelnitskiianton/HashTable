COMPILIER = g++
CFLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
NAME	= hash_table

SRC_ASM_EXT = .nasm
SRC_C_EXT   = .cpp

SRC_HT  = ./HashTable
SRC_DLL = ./DLinkedList
SOURCES_HT   = $(wildcard $(SRC_HT)/*$(SRC_C_EXT))
SOURCES_DLL  = $(wildcard $(SRC_DLL)/*$(SRC_C_EXT))
OBJFILES_HT  = $(patsubst $(SRC_HT)/%,$(OBJ)/%,$(SOURCES_HT:$(SRC_C_EXT)=.o))
OBJFILES_DLL = $(patsubst $(SRC_DLL)/%,$(OBJ)/%,$(SOURCES_DLL:$(SRC_C_EXT)=.o))

SRC_ROOT = ./
SOURCES_ROOT  = $(wildcard $(SRC_ROOT)/*$(SRC_C_EXT))
OBJFILES_ROOT = $(patsubst $(SRC_ROOT)/%,$(OBJ)/%,$(SOURCES_ROOT:$(SRC_C_EXT)=.o))

OBJ     = ./obj
BIN     = ./
OUT     = $(BIN)

$(OUT)$(NAME): $(OBJFILES_HT) $(OBJFILES_DLL) $(OBJFILES_ROOT)	#Linking with GCC use -no-pie
	$(COMPILIER) $(CFLAGS) $^ -o $@

$(OBJ)/%.o: $(SRC_DLL)/%$(SRC_C_EXT) #Object with GCC use -O3
	$(COMPILIER) $(CFLAGS) -c -o $@ $<

$(OBJ)/%.o: $(SRC_HT)/%$(SRC_C_EXT) #Object with GCC use -O3
	$(COMPILIER) $(CFLAGS) -c -o $@ $<

$(OBJ)/%.o: $(SRC_ROOT)/%$(SRC_C_EXT) #Object with GCC use -O3
	$(COMPILIER) $(CFLAGS) -c -o $@ $<
#===================================================================================
#===================================================================================

.PHONY: convert
convert:
	python TextFile/converter.py 

.PHONY: clean
clean:
	rm -f $(OBJ)/*.o $(OBJ)/*.lst

.PHONY: run
run:
	$(OUT)$(NAME)	
