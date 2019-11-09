# Pi

Pi is a simple post-fix language. Meant to be fast for a machine to parse and execute. Not so great for humans.

Will be easier to use for those with backgrounds with HP48 calculators.

#$ Keywords

The current _Pi_ command list is:

```
true	# insert boolean true
false	# insert boolean false
assert	# assert that top of stack equates to true
#		# store a value to a name
@		# retrieve a value from a variable name
&		# push current context, then continue what is on top of data stack
...		# resume top of context stack
!		# replace current context with top of context stack
drop	# drop top of data stack
dup		# duplicate top of data stack
==		# apply equivalency operator to top two elements of data stack
swap	# swap top to elements of data stack
+		# add top to elements of data stack
-		# give difference of top to elements of data stack
*		# multiply top two elements of data stack
div		# divide top two elements of data stack
>		# pop top two elements of data stack and push true if second is greater than first, else false
M		# pop top two elements of data stack and push true if second is less than first, else false
rot		# rotate the data staack: A B C => B C A
over	# duplicate the second element of the data stack: A B => A B A
pickn	# duplicate the Nth element of datastack: A B C D 3 pickn => A B C D A
clear	# clear the data stack
gc		# perform garbage collection
size	# return size of container on top of stack
toarray	# make an array: 1 2 3 3 toarray => [1 2 3]
tolist	# an a list: 
todict	# make a dict
expand	# expand contents of a container, leaving the numnber of elements on TOS: [1 2 3] expand => 1 2 3 3
depth	# push current depth of data stack
&&		# perform logical and
new		# make a new object given a type 
xor		# perform exclusive or on top two elements on stack: 1 0 xor => true
or		# perform inclusive or: 0 0 or => false
if		# evaluate TOS. if true, execute 2nd element: "foo" true if => "foo"
ifelse	# evaluate TOS. if true, execute 2nd element, else 3rd: "foo" "bar" false iflse => "bar"
!=		# return true of top two elements are equivalent
freeze	# convert an object into a binary packet
thaw	# inverse of thaw
self	# push current top of context stack onto data stack
contents	# push contents of object on top of stack, including all properties, methods and dict entries
dropn	# drop N elemenets from data stack: 1 2 3 4 2 dropn => 1 2
```

For more information you can read the [Source/Core/Executor.cpp](Executor) code, and the unit tests.

## Literals

Comments start with a # and everything after that symbol on the same line is ignored. There are no multi-line 
comments as Pi is basically a sequence of tokens with little structure.

Integers are a sequence of number characters: 1, 20, 12313.

Floats are 1.0 etc. Expoentials are not supported.

Strings are surrounded by double quotes ".

Lists are surrounded by [ and ].

Continuations are surrounded by { and }.

All compounds are recursive.


