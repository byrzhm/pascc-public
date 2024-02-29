# About pascal

- [wiki](https://en.wikipedia.org/wiki/Pascal_(programming_language))
- [standard](https://web.archive.org/web/20160127044422/http://pascal-central.com/docs/iso7185.pdf)
- [compiler explore](https://godbolt.org/)

## Lexical tokens

### General

$$
\begin{alignat*}{2}
&\text{letter} &&= 'a' | 'b' | \dots | 'z' | 'A' | 'B' |\dots | 'Z' \\
&\text{digit} &&= '0' | '1' | \dots | '9'
\end{alignat*}
$$

### Special symbols

![special-symbols](image/special-symbols.png)

### Identifiers

> <font color=red>不区分大小写</font>

$$
\text{identifier} = \text{letter}\ \text{(letter | digit)}^*
$$

### Numbers

$$
\begin{alignat*}{2}
&\text{signed-number} &&= \text{signed-integer | signed-real} \\
&\text{signed-real} &&= \text{sign}_{\text{opt}} \quad \text{unsigned-real} \\
&\text{signed-integer} &&= \text{sign}_{\text{opt}} \quad \text{unsigned-integer} \\

&\text{unsigned-number} &&= \text{unsigned-integer | unsigned-real} \\
&\text{sign} &&= '+' | '-' \\
&\text{unsigned-real} &&= \text{digit-sequence}\ '.'\ \text{fractional-part}\ {['e'\ \text{scale-factor}]}_\text{opt}\\
&&& |\ \text{digit-sequence}\ 'e'\ \text{scale-factor} \\
&\text{unsigned-integer} &&= \text{digit-sequence} \\
&\text{fractional-part} &&= \text{digit-sequence} \\
&\text{scale-factor} &&= \text{sign}_\text{opt}\quad\text{digit-sequence} \\
&\text{digit-sequence} &&= \text{digit}^+
\end{alignat*}
$$

Examples:

- 1e10
- 1
- +100
- -0.1
- 5e-3
- 87.35E+8

### Character-strings

$$
\begin{alignat*}{2}
&\text{character-string} &&= '''\ \text{string-element}^+\ '''\\
&\text{string-element} &&= \text{apostrophe-image}\ |\ \text{string-character}\\
&\text{apostrophe-image} &&= ''''\\
&\text{string-character} &&= \text{one-of-a-set-of-implementation-defined-characters}
\end{alignat*}
$$

Examples:

- 'A'
- ';'
- ''''
- 'Pascal'
- 'THIS IS A STRING'

### Token separators

> Comments, spaces (except in character-strings), and the separations of consecutive lines shall be
considered to be token separators.

#### Comments

``` pascal
{ commentary }
(* commentary *)
// commentary
```

## Grammar

$$
\begin{alignat*}{2}
&\text{program}\quad&&=\quad\text{program-heading}\quad\ ';'\quad \text{program-block}\quad\ '.'\\
&\text{program-heading}\quad&&=\quad\ '\text{program}'\quad \text{identifier}\quad {['('\quad\text{program-parameter-list}\quad\ ')']}_\text{opt}\\
&\text{program-parameter-list}\quad&&=\quad\text{identifier-list}\\
&\text{identifier-list}\quad&&=\quad\text{identifier}\quad{[','\quad\text{identifier}]}^*\\
&\text{program-block}\quad&&=\quad\text{block}\\
&\textcolor{red}{block}\quad&&=\quad\text{constant-definition-part}_\text{opt}\quad\text{type-definition-part}_\text{opt}\\
&&&\quad\text{variable-declaration-part}_\text{opt}\quad\text{procedure-and-function-declaration-part}_\text{opt}\\
&&&\quad\text{statement-part}\\
&\text{constant-definition-part}\quad&&=\quad\ '\text{const}'\quad\text{const-definition}\quad\ ';'\quad {[\text{const-definition}\quad\ ';']}^* \\
&\text{const-definition}\quad&&=\quad\text{identifier}\quad\ '=' \quad\text{constant}\\
&\text{constant}\quad&&=\quad\text{sign}_\text{opt}\quad(\text{unsigned-number}\quad |\quad \text{constant-identifier})\\
&&&|\quad\text{character-string}\\
&\text{constant-identifier}\quad&&=\quad\text{identifier}\\
&\text{type-definition-part}\quad&&=\quad\dots\\
&\text{variable-declaration-part}\quad&&=\quad\ '\text{var}'\quad\text{variable-declaration}\quad\ ';'\quad {[\text{variable-declaration}\quad\ ';']}^* \\
&\text{variable-declaration}\quad&&=\quad\text{identifier-list}\quad\ ':' \quad\text{type-denoter}\\
&\text{type-denoter}\quad&&=\quad\dots\\
&\text{procedure-and-function-declaration-part}\quad&&=\quad\ {[(\text{procedure-declaration}\quad|\quad\text{function-declaration})\quad\ ';']}^+\\
&\text{procedure-declaration}\quad&&=\quad\text{procedure-heading}\quad\ ';'\quad\text{procedure-block}\\
&\text{procedure-heading}\quad&&=\quad\ '\text{procedure}'\quad\text{identifier}\quad{[\text{formal-parameter-list}]}_\text{opt}\\
&\text{procedure-block}\quad&&=\quad\text{block}\\
&\text{function-declaration}\quad&&=\quad\text{function-heading}\quad\ ';'\quad\text{function-block}\\
&\text{function-heading}\quad&&=\quad\ '\text{function}'\quad\text{identifier}\quad{[\text{formal-parameter-list}]}_\text{opt}\quad\ ':'\quad \text{result-type}\\
&\text{function-block}\quad&&=\quad\text{block}\\
&\text{statement-part}\quad&&=\quad\text{compound-statement}\\
&\text{compound-statement}\quad&&=\quad\ '\text{begin}' \quad\text{statement-sequence}\quad\ '\text{end}'\\
&\text{statement-sequence}\quad&&=\quad\text{statement}\quad\ {[';'\quad\text{statement}]^*}\\
&\text{statement}\quad&&=\quad\text{simple-statement}\quad |\quad \text{structured-statement}\\
&\text{simple-statement}\quad&&=\quad\text{empty-statement}\quad |\quad \text{assignment-statement}\\
&&&|\quad \text{procedure-statement}\\
&\text{empty-statement}\quad&&=\quad\epsilon\\
&\text{assignment-statement}\quad&&=\quad(\text{variable-access}\quad|\quad\text{function-identifier})\quad\ ':='\quad\text{expression}\\
&\text{procedure-statement}\quad&&=\quad\dots\\
&\text{structured-statement}\quad&&=\quad\text{compound-statement}\quad|\quad\text{conditional-statement}\\
&&&|\quad\text{repetitive-statement}\\
&\text{conditional-statement}\quad&&=\quad\dots\\
&\text{repetitive-statement}\quad&&=\quad\dots
\end{alignat*}
$$

与ISO标准的区别:

- Record
- Label
- With-statement