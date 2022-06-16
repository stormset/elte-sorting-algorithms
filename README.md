# Sorting algorithms

## About
Course project for **Data Structures and Algorithms**.

Compares the swap, key-comparison counts and runtimes of different sorting algorithms.

## Help
```
	Usage: ./a.out [OPTION]... [OTHER]...
  	 [OPTION]         [OTHER]
    	-f          < filename >            Use file as input source.
    	-h                                  Do not print the result of sorting.
    	-s                                  Enable simple statistics.
    	-m                                  Use merge sort.
	    -q                                  Use quick sort.
    	-i                                  Use insertion sort.
    	-b                                  Use bubble sort.
    	-x                                  Use maximum selection sort.
```
<br>**The files:**<br>
```test/big``` (10000 int in range [-1000..1000])<br>
```test/small``` (8 int in range [-1000..1000])<br>
```test/reversed``` (10000 int in range [-1000..1000] in reversed order)<br>
```test/sorted``` (10000 int in range [-1000..1000] already sorted)<br>
can be used to compare how different algorithms behave with different kinds of input dataset.

**Usage example:**

Sort with all of the implemented algorithms (```-mqibx```), measure statistical informations (```-s```), and use file (```-f```) as an input:<br>
```bash
./a.out -mqibx -shf big
```