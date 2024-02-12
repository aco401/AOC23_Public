# Advent Of Code Day 19 - Sorting MachineParts though Workflows
- Exercise found in Exercise.md
- Idea of solution found in Solution.md
	- a little class diagram found in /diagrams/model_class.drawio

# How to build
1. ``cd [clone_path]/build/``
2. ``cmake ..``
3. ``make -j 24``

# How to run
``cd [clone_path]/build/``

## Application
``./EXEC_DAY19 -t 3 -f ../src/data_input.txt``

- ``-f`` filepath to input data
- ``-t`` amount of threads to run sorting with

## Tests
- Unit Tests: 
``./all_test_unit``

- "Integration test" - meaning tests with sleeps and threads used:
``./all_test_integration``

# Checks


## Valgrind
command: `valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./EXEC_DAY19 -f ../src/data_input.txt -t 1`

Output: 

```
==8201==
==8201== HEAP SUMMARY:
==8201==     in use at exit: 0 bytes in 0 blocks
==8201==   total heap usage: 10,842,011 allocs, 10,842,011 frees, 48,172,588 bytes allocated
==8201==
==8201== All heap blocks were freed -- no leaks are possible
==8201==
==8201== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## Code Coverage

1. `cd [clone_path]/build/`
2. `cmake -DENABLE_COVERAGE=true .. && make -j 24`
3. `./EXEC_DAY19 && make coverage`
4. Open html file: `[clone_path]/day_19/build/coverage/index.html`