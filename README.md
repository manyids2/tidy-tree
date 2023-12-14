# tidy-tree

Implementation of [Drawing Non-layered Tidy Trees in Linear Time, A.J. van der Ploeg
](https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=d45f66231e053590c64c9d901fb7b028dbc5c923)
Inspired by [h-m-m](https://github.com/nadrad/h-m-m).

Desired featues:

1. Static print with/without ANSI.
2. Serialization format that allows for newlines, saves state, undo.
3. Define hyperlinks.
4. Infinite canvas.
5. Extract trees out to windows.
6. Cherry - images.

Implementation Details:

1. Start with `c` implementation.
