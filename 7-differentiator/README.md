Bailando Differentiator
====
## Description
This differentiator is the part of C language, Data Structures and Compilers course in **MIPT**. 
It can differentiate any superposition of functions.
## How it works?
### Input:
In the input this program takes formatted tree string, that contains functions, num and variables.
**Example of tree str:**

Then after the read of the tree, we are free to use our algorithm
### Algorithm:
The entire program is based on one recursive algorithm. Recursively transforming every node in the exact way we will get differentiated function tree in the end. So what are these transformations?
### Examples of transformation:
**Constant Transformation:**

**Variable Transformation:**

**Summary Transformation:**

**Multiply Transformation:**
### Optimizations:
But by doing this algorithm we will face a problem connected with unnecessary exceptions that spoil final result.
To solve these we have different optimizations that will help us to make a good-looking result.
**Constant Folding**
If we have some sub-tree that consist only from constants we will calculate this tree and turn it into one constant node:

**Multiplying on One/Summing with Zero**
If we have sub-tree with multiply on one/summing with zero nodes we can simply delete them:

## How to use it?
Firstly you need to initialize expression tree
```c
InitializeExprTree(&expr_tree, "source.txt");
```
After it you can use GetDifferentiatedTree function, and specify the variable by which you want to differentiate
```c
expr_tree = GetDifferentiatedTree(expr_tree, 'x');
```
If you want to use Optimizations (First and Second) you are free to use this function:
```c
OptimizeTree(&expr_tree);
```
## Another Features
**Python Graphs**
You can plot Tree Graph, by using function PlotTreeGraph, it will automatically generate .py file with graph and will open it.
```c
PlotTreeGraph(&expr_tree, "Graph");
```
**Teylor Series**
Also you can plot Taylor Series Graph, And Taylor Series and initial function subtract graph
```c
PlotTaylorDiffGraph(&expr_tree, "Taylor Diff Graph", 'x', 3, 0);
```
### Latex Print
This program automatically save all your differentiating actions in LaTex file with Bailando explanations, so feel free to use it!
