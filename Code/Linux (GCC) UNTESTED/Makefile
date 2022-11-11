CC=g++
CFLAGS=-std=c++17 -Wall -Ofast
.DEFAULT_GOAL := all

ifneq (,$(findstring xterm,${TERM}))
	BLACK        := $(shell tput -Txterm setaf 0)
	RED          := $(shell tput -Txterm setaf 1)
	GREEN        := $(shell tput -Txterm setaf 2)
	YELLOW       := $(shell tput -Txterm setaf 3)
	LIGHTPURPLE  := $(shell tput -Txterm setaf 4)
	PURPLE       := $(shell tput -Txterm setaf 5)
	BLUE         := $(shell tput -Txterm setaf 6)
	WHITE        := $(shell tput -Txterm setaf 7)
	RESET        := $(shell tput -Txterm sgr0)
else
	BLACK        := ""
	RED          := ""
	GREEN        := ""
	YELLOW       := ""
	LIGHTPURPLE  := ""
	PURPLE       := ""
	BLUE         := ""
	WHITE        := ""
	RESET        := ""
endif

all:
	@clear
	@$(foreach file, $(wildcard dependencies/*.cpp), \
		echo "Currently compiling: ${PURPLE}$(notdir $(file:%.cpp=%))${RESET}\n--------------------\n\n" ; \
		$(CC) $(CFLAGS) -c $(file) -o compiled/$(notdir $(file:%.cpp=%)).o || exit; \
		clear; )

	@echo "Currently compiling: ${PURPLE}WiCE${RESET}\n--------------------\n\n"
	@$(CC) $(CFLAGS) -c -o WiCE WiCE.cpp $(wildcard compiled/*.o)
	@clear
	
	@echo "--- ${YELLOW}Begin Execution${RESET} ---\n\n"
	@./WiCE
	@echo "\n\n--- ${YELLOW}End Execution${RESET} ---\n\n"