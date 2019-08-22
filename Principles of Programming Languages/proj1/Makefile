#/*
# *********************************************
# *  314 Principles of Programming Languages  *
# *  Spring 2017                              *
# *  Authors: Ulrich Kremer                   *
# *  Student Version                          *
# *  Anirudh Tunoori                          *
# *  netid: at813                             *
# *********************************************
# */

CCFLAGS = -ggdb -Wall

all: compile optimize 

compile: Compiler.c InstrUtils.c InstrUtils.h Utils.c Utils.h
	gcc $(CCFLAGS) Compiler.c InstrUtils.c Utils.c -o compile

optimize: Optimizer.c InstrUtils.c InstrUtils.h Utils.c Utils.h
	gcc $(CCFLAGS) Optimizer.c InstrUtils.c Utils.c -o optimize

clean:
	rm -rf compile optimize tinyL.out

