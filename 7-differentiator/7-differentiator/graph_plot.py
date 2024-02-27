import numpy as np
import matplotlib.pyplot as plt
import sympy as sp
x = sp.symbols('x')
expr_str = "cos(x)-x^2/2*-1+1"
expr = sp.sympify(expr_str)
my_function = sp.lambdify(x, expr, 'numpy')
x_values = np.linspace(-10, 10, 100)
y_values = my_function(x_values)
plt.plot(x_values, y_values, label=expr_str)
plt.title('Bailando Taylor Diff Graph: {}'.format(expr_str))
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)
plt.savefig('Bailando Taylor Diff Graph.png')
plt.show()
