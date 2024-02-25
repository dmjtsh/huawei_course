import numpy as np
import matplotlib.pyplot as plt
import sympy as sp
x = sp.symbols('x')
expr_str = "1/x"
expr = sp.sympify(expr_str)
my_function = sp.lambdify(x, expr, 'numpy')
x_values = np.linspace(-10, 10, 100)
y_values = my_function(x_values)
plt.plot(x_values, y_values, label=expr_str)
plt.title('Bailando Graph: {}'.format(expr_str))
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)
plt.savefig('Bailando Graph.png')
plt.show()
