# Double-Snake
Two snakes playing chase on an empty 2-d grid
This is an implementation of a twist on the classic snake game: the food is not static, and there are two snakes. the food for each snake is on the body of the other snake(one snake's head
and the other's tail), and both employ a graph search AI to get to their goals.

Linux installation

requires cmake 2.6.4 minimum

In project folder

    ```
    $ mkdir b
    $ cd b
    $ cmake ..
    $ make
    $ cd ../bin
    $ ./2snake
    ```

The [space] key may be used to pause and unpause the snake

The code has been commented with relevant algorithmic indications. The snakes are intialised in random columns, fixed rows and fixed lengths. I created a similar academic project
in a different language during a course on graph theory in college, with the expectation the results would be interesting to assess from a mathematical point of view. It is notable
that neither snake succeeds in getting to their objective.
