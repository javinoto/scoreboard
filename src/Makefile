CC=gcc
CFLAGS=-Wall -ansi -pedantic-errors -g
CLIBS=-lm

TEST = 7-5/xxxxxx71xxx xxxxxxxxxxxx xx8-6-7/

all: task1_test task2_test
# libraries

linked_list.o: linked_list.c linked_list.h
	$(CC) -c $< $(CFLAGS)

tpb.o: tpb.c tpb.h
	$(CC) -c $< $(CFLAGS)

queue.o: queue.c queue.h
	$(CC) -c $< $(CFLAGS)


# task 1

task1_test: score_calculator.c linked_list.o queue.o tpb.o
	$(CC) -o task1 $^ $(CFLAGS) $(CLIBS)

task1_test_run: task1_test
	./task1 $(TEST)

task1_test_memcheck: task1_test
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./task1 $(TEST)

# task 2

tpb2.o: tpb.c tpb.h
	$(CC) -o $@ -c $< $(CFLAGS) -DSCOREBOARD

task2_test: score_calculator.c linked_list.o queue.o tpb2.o
	$(CC) -o task2 $^ $(CFLAGS) $(CLIBS) 

task2_test_run: task2_test
	./task2 $(TEST)

task2_test_memcheck: task2_test
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./task2 $(TEST)



clean:
	rm -f task* *.o
