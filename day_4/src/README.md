# Problem:
- We got strings of Cards (pile of Cards) with winnning numbers (left of |) and owning number (right of |)
- On a ScratchCard, which of the winning numbers appear in the owning numbers.
    - the first match results in 1 point
    - for every further match the points are doubled
-  What is the accumulated number of points of the pile of cards?

- Example data: 
    Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
    Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19

    - results in 10 points (Card 1: 8 points, Card 2: 2 points)

# Solution:

- command:
    1. cd AdventOfCode2023/day_4/diagrams
    2. drawio -f pdf -x mockup.drawio

![Mockup](../diagrams/mockup.png)

# Uneccesary complicated
- let ScratchCardParser not handle stream, just give string


# Other Notes
## Ideas to extend the project:
- different ways to input data into ScratchCard Parser
    - file
    - cli
        - opportunity to write a complete RAII wrapper
- different ways to output data
- different ways to solve the parsing
- different ways to solve the algorithm
    - different containers used --> measure speed

## How to:
- apply design patterns
    - DI (dependency injection)

## Benefit
- apply class inheritance by using certain patterns
