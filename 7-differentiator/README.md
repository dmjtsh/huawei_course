💥Bailando Differentiator💥
====
## Description

This differentiator is the part of System Programming Course in **MIPT**.
It can differentiate any expressions.

## How it works?
### Input

This program takes an expression as a string. This string contains a tree definition in the following form: \

<image src="assets/tree_str.png" width=400px>

**This string corresponds to sin(x²) expression**

Then after we read the tree, the program executes a symbolic differentiating algorithm.

### The Algorithm

This algorithm uses recursion to transform every node in a special way. In the end we will get differentiated function tree.
So what are these transformations?

### Examples of transformations

| **Constant Transformation:**                      | **Variable Transformation:**                      |
|:-------------------------------------------------:|:-------------------------------------------------:|
| <image src="assets/tree_transf2.png" width=290px> | <image src="assets/tree_transf1.png" width=290px> |


**Summary Transformation:**

<image src="assets/tree_transf4.png" width=600px>

**Multiply Transformation:**

<image src="assets/tree_transf3.png" width=600px>

### Optimizations
After running this algorithm there will be a problem with overcomplicated expressions that are need to be simplified.

To solve these issues there are different optimizations that help to make a good-looking result.

**Constant Folding** \
If we have a subtree which contains from constants only, an optimization pass fully calculates this subtree and turns it into one constant node:

<image src="assets/tree_optimization1.png" width=600px>

**Multiplying on One/Summing with Zero** \
If we have a subtree with multiply operation on one (or summing with zero) this optimization simply deletes the operation and the constant (1 or 0):

<image src="assets/tree_optimization2.png" width=600px>

## How to use it as Power User?

Simple non-math users obviously cannot use this program, so CLI interface was designed for Power Users :---)

````
main.exe <expression file name>
````
After that, you will receive the decent LaTex report for decent people.

## How to use it as Developer?
### Initialization

First, you need to initialize expression tree:

```c
InitializeExprTree(&expr_tree, "source.txt");
```

After that, you can use **GetDifferentiatedTree** function, and specify the variable want to differentiate by:

```c
expr_tree = GetDifferentiatedTree(expr_tree, 'x');
```

If you want to use Optimizations (the first and the second one) you should use simply

```c
OptimizeTree(&expr_tree);
```

### Python Graphs

Using **PlotTreeGraph** function you can plot a Tree Graph. It will
automatically generate .py file with Python code plotting a graph of an expression as a math function ```y = expression (x)```:

```c
PlotTreeGraph(&expr_tree, "Graph");
```

**cos(x) output:**

<image src="assets/Graph.png" width=500px>

**Taylor Series**

Also you can plot Taylor Series Graph, and a difference between initial function and Taylor approximation:

```c
PlotTaylorTreeGraph(&expr_tree, "Taylor Graph", 'x', 3, 0);
PlotTaylorDiffGraph(&expr_tree, "Taylor Diff Graph", 'x', 3, 0);
```

**cos(x) output:**

<image src="assets/TaylorGraph.png" width=500px>

<image src="assets/TaylorDiffGraph.png" width=500px>

### Latex Print

This program automatically save a history of all your actions in LaTex file with Bailando explanations™©®, so feel free to use it!

**Latex File Example:**

<image src="assets/tree_latex.png" width=800px>


**💘 Dmit DREC MIPT 2023**
