# palindrome

## Palindromes of `(a|b)+`

This Turing machine accepts palindromes of `a` and `b`.

According to the transition table, the machine itself should accept `(a|b)*`, but C's `scanf("%s")` will fail to read an empty string, thus the resulting executable accepts `(a|b)+` (using PCRE notation).
