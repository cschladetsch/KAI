# Rho language Tests
Rho is much like _Python_, but with an even less verbose syntax.

The 'return value' for a Rho function is whatever is left on the stack(s).

Example:

```
fun a = b, c
	b + c

assert a(1, 2) == 3

fun d = e
	fun f = g
		return g*2
	f(e)
  
assert d(2) == 4
assert exists a
assert exists d
assert !(exists f)

fun h = i,j,k
	a(i,d(j)*k)

assert h(1,2,3) == 16
```
