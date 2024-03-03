💥Bailando Differentiator💥
====
## Description
This differentiator is the part of System Programing Course in **MIPT**. 
It can differentiate any expressions.
## How it works?
### Input:
This program takes formatted tree string, that contains functions, numbers and variables. \
**Example of tree string(sin(x^2) function):**

<image src="assets/tree_str.png" width=400px>

Then after the read of the tree, differentiator executes an algorithm.
### Algorithm:
The entire program is based on one recursive algorithm. Recursively transforming every node in the exact way we will get differentiated function tree in the end. So what are these transformations?
### Examples of transformation:
**Constant Transformation:**

<image src="assets/tree_transf2.png" width=600px>

**Variable Transformation:**

<image src="assets/tree_transf1.png" width=600px>

**Summary Transformation:**

<image src="assets/tree_transf4.png" width=600px>

**Multiply Transformation:**

<image src="assets/tree_transf3.png" width=600px>

### Optimizations: 
But by simbply implementing this algorithm there will be a problem with overcomplicated expressions that need to be simplified.
To solve these issues there are different optimizations that help to make a good-looking result. \
**Constant Folding** \
If there is a sub-tree that consists only from constants optimization function calculates this tree and turns it into one constant node:

<image src="assets/tree_optimization1.png" width=600px>

**Multiplying on One/Summing with Zero** \
If there is a sub-tree with multiply on one/summing with zero nodes optimization function simply deletes them:

<image src="assets/tree_optimization2.png" width=600px>

## How to use it?
Firstly you need to initialize expression tree:
```c
InitializeExprTree(&expr_tree, "source.txt");
```
After it you can use GetDifferentiatedTree function, and specify the variable by which you want to differentiate:
```c
expr_tree = GetDifferentiatedTree(expr_tree, 'x');
```
If you want to use Optimizations (First and Second) you are free to use this function:
```c
OptimizeTree(&expr_tree);
```

## Another Features
**Python Graphs**
You can plot Tree Graph, by using function PlotTreeGraph, it will automatically generate .py file with graph and will open it:
```c
PlotTreeGraph(&expr_tree, "Graph");
```
**cos(x) output:**

<image src="assets/graph.png" width=500px>

**Taylor Series**
Also you can plot Taylor Series Graph, And Taylor Series and initial function subtract graph:
```c
PlotTaylorTreeGraph(&expr_tree, "Taylor Graph", 'x', 3, 0);
PlotTaylorDiffGraph(&expr_tree, "Taylor Diff Graph", 'x', 3, 0);
```

**cos(x) output:**

<image src="assets/TaylorGraph.png" width=500px>

<image src="assets/TaylorDiffGraph.png" width=500px>

### Latex Print
This program automatically save all your differentiating actions in LaTex file with Bailando explanations, so feel free to use it!

**Latex File Example:**

<image src="assets/tree_latex.png" width=800px>

**💘 Dmit DREC MIPT 2023**
