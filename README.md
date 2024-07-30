# QueensGambit_SOC
Work done under SOC project 2024. Under this project we have to finally make a chess engine and battle against each other in a battle for the ultimate crown (It's not that dramatic; that was just my alter ego's mind running off course and wild)
<br>
Week1: Nim, Chomp, Pawnscape and GoN solve the questions given  [Update:pawnscape done! $\alpha-\beta$ pruning ftw]

Week2: Readings on game theory, algorithms/concepts involved(?, sequential and simulataneous move games), noraml form game representation. Tic-Tac-Toe assignment [->Readings done, assignment started]

Week3: Had to make an engine that was able to solve mate in 2 or mate in 3 puzzles using alpha beta pruning.

Week4 to Week7: Built chess engine present in submittingIt folder. Key Aspects explained below.
- UCI compatible
- Evaluation function takes into account possible checkmates then checks with capture, and then checks. Rest of the moves are ordered based on the material advantage they result in and the captures they make, etc.
- Utility of each square for different pieces also incorporated (thanks to Kunal sir for the values)
- Transposition matrix implemented.
