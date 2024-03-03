💥Bailando Differentiator💥
====
## Description
This differentiator is the part of System Programing Course in **MIPT**. 
It can differentiate any expressions.
## How it works?
### Input:
This program takes formatted tree string, that contains functions, numbers and variables. \
**Example of tree string:**

<image src="tree_str.png" width=400px>

Then after the read of the tree, differentiator executes an algorithm.
### Algorithm:
The entire program is based on one recursive algorithm. Recursively transforming every node in the exact way we will get differentiated function tree in the end. So what are these transformations?
### Examples of transformation:
**Constant Transformation:**

<image src="tree_transf2.png" width=400px>

**Variable Transformation:**

<image src="tree_transf1.png" width=400px>

**Summary Transformation:**

<image src="tree_transf4.png" width=400px>

**Multiply Transformation:**

<image src="tree_transf3.png" width=400px>

### Optimizations: 
But by simbply implementing this algorithm there will be a problem with overcomplicated expressions that need to be simplified.
To solve these issues there are different optimizations that help to make a good-looking result. \
**Constant Folding** \
If there is a sub-tree that consists only from constants optimization function calculates this tree and turns it into one constant node:

<image src="tree_optimization1.png" width=400px>

**Multiplying on One/Summing with Zero** \
If there is a sub-tree with multiply on one/summing with zero nodes optimization function simply deletes them:

<image src="tree_optimization2.png" width=400px>

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

<image src="graph.png" width=400px>

**Taylor Series**
Also you can plot Taylor Series Graph, And Taylor Series and initial function subtract graph:
```c
PlotTaylorTreeGraph(&expr_tree, "Taylor Graph", 'x', 3, 0);
PlotTaylorDiffGraph(&expr_tree, "Taylor Diff Graph", 'x', 3, 0);
```

**cos(x) output:**

<image src="TaylorGraph.png" width=400px>

<image src="TaylorDiffGraph.png" width=400px>

### Latex Print
This program automatically save all your differentiating actions in LaTex file with Bailando explanations, so feel free to use it!

**Latex File Example:**

<image src="tree_latex.png" width=400px>

**💘 Dmit DREC MIPT 2023**
