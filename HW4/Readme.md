# CS 444 Homework 4

## Resources:

 - [Best fit algorithm info](https://www.cs.rit.edu/~ark/lectures/gc/03_03_03.html)

## Design Plan:

We will implement the best fit algorithm as described in the RIT link above:

```
size(block) = n + size(header)
Scan free list for smallest block with nWords >= size(block)
If block not found
    Failure (time for garbage collection!)
Else if free block nWords >= size(block) + threshold*
    Split into a free block and an in-use block
    Free block nWords = Free block nWords - size(block)
    In-use block nWords = size(block)
    Return pointer to in-use block
Else
    Unlink block from free list
    Return pointer to block 
```

This code will be placed in the `slob_alloc` function.

## Testing our solution

1. First, copy hw4.patch to the base of your linux directory. Then run `patch -p1 -i hw4.patch` to update your slob file.
