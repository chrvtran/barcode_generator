AI Usage Appendix

Students shall (per GAIA)
1. Give credit to AI tools whenever used, even if only to generate ideas rather than usable text or
illustrations.
2. When using AI tools on assignments, add an appendix showing (a) the entire exchange, highlighting
the most relevant sections; (b) a description of precisely which AI tools were used (e.g. ChatGPT
private subscription version or Gemini free version), (c) an explanation of how the AI tools were used
(e.g. to generate ideas, turns of phrase, elements of text, long stretches of text, lines of argument, pieces
of evidence, maps of conceptual territory, illustrations of key concepts, etc.); (d) an account of why AI
tools were used (e.g. to save time, to surmount writer’s block, to stimulate thinking, to handle mounting
stress, to clarify prose, to translate text, to experiment for fun, etc.).
3. Employ AI detection tools and originality checks prior to submission, ensuring that their submitted
work is not mistakenly flagged.
4. Use AI tools wisely and intelligently, aiming to deepen understanding of subject matter and to support
learning.

----

Claude Sonnet 5 Medium (free default version)

/barcode.c LINES 8-32

The following highlights how I used the encoding table from the provided README.md to get a usable key table in C programming language. I used AI because I knew that this was a key element, and wanted it to be done right, with proper C syntax, and be done quickly. I also wasn't very confident in my understanding of the explanation from bits, to bytes, to digits, to encoding, as there were many elements to put together. Once I had this, I was able to more confidently proceed and then edit, understand, and iterate the results.

Me: (provided README.md and my code files for easy context)
Me: "use the following codes to create a table mask For left-hand digits:

0 = 1110010
1 = 1100110
2 = 1101100
3 = 1010000 
4 = 1011100
5 = 1001110
6 = 1000010
7 = 1000100
8 = 1001000
9 = 1110100
Notice, if the digit appears in the right side of the barcode, the above encoding is inverted.
Now notice that the start and middle patterns can be represented similarily:

middle = 10101
start = 010
end = 010"
Claude: "Here's the mask table for barcode.c. Each pattern is stored as an integer with the leftmost bar in the most significant bit, so bits read left to right when you walk from the highest bit down."
Me: "what is nbit"
Claude: "nbits is the number of bits in mask that fill_digit should use, which is also the number of columns it paints. Each bit becomes one 1-pixel-wide column.

It's needed because a mask like 0x2 doesn't say how many leading zeros it has. The start pattern 010 is the number 0x2, but so is 10, 0010, and 0000010. nbits says how many bits to read, counting from the left: 

One correction to the comment in my sketch above: with 1 = light, 010 is dark, light, dark... wait, no. Reading 0, 1, 0 in order gives dark, light, dark. That matches the start pattern in the README.

Inside fill_digit, it goes into the GET_BIT macro as n:

For i = 0 (the first column), it shifts right by nbits - 1 so the leftmost bit ends up in the lowest position, then & 1 isolates it. For mask = 0x72, nbits = 7:

i = 0: 0x72 >> 6 is 1, so light.
i = 1: 0x72 >> 5 is 3, and 3 & 1 is 1, so light.
i = 3: 0x72 >> 3 is 14, and 14 & 1 is 0, so dark.

Without nbits, the function wouldn't know which bit is the leftmost one."
Me: "why do we do data[i[ -  '0' when we call L_CODE or R_CODE"
Claude: "data[i] is a char, and a character isn't the same as the number it looks like. In C, '7' is the character code for the digit seven (55 in ASCII), not the value 7. Subtracting '0' (48 in ASCII) converts it to the actual number:

'0' - '0' = 48 - 48 = 0
'7' - '0' = 55 - 48 = 7
'9' - '0' = 57 - 48 = 9

This works because C guarantees the characters '0' through '9' are consecutive, so the distance from '0' is exactly the digit's value.

You need it because L_CODE and R_CODE are indexed by digit value, 0 through 9:

c
L_CODE[data[i]]         // WRONG: for '7' this reads L_CODE[55]
L_CODE[data[i] - '0']   // RIGHT: for '7' this reads L_CODE[7]

L_CODE has only 10 entries, so L_CODE[55] reads past the end of the array. That's undefined behavior: it returns garbage or crashes, and the compiler won't warn you.

The subtraction is safe here because the loop at the top of barcode() already rejected any character outside '0' to '9'. Every data[i] - '0' is therefore between 0 and 9, a valid index."
Me: "can we just update L_CODE in the const then"
Claude: "Yes. You can index the table by the character itself, using C's designated initializers so each entry sits at its character's position. I'll make that change and check it."