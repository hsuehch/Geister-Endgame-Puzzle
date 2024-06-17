﻿# Random placement generation of Geister endgame puzzles

For more details, please refer to our [paper](https://www.sciencedirect.com/science/article/pii/S1875952124001046).

## Environment (64-bit systems required)

PS. The help messages have English and Japanese versions, while the former is the default.
If your PC supports Japanese characters and you want to see Japanese help messages, please change the `0` on line 27 of `IncludeLib.h` to `1`. i.e., ``#define OUTPUT_JP_MESSAGE 1``

### Windows

* Use Visual Studio 2019 (or above).
* Build the project to generate the executable file.
    * You can find the generated executable file (`GeisterEndgamePuzzle.exe`) at `x64/Release`.
    * You can then execute it using PowerShell or cmd, which you can call out by
        * typing `powershell` or`cmd` in the address bar and hitting enter, or
        * pressing the Shift key, right-clicking on the folder, and selecting `Open PowerShell here`.


### Ubuntu

* Use g++ 7.5.0 (or above).
* Use the makefile to generate the executable file (`GeisterEndgamePuzzle`).


## Usage

There are two modes, RandomGen and GetSol, as explained below.

### Mode 1/2: RandomGen

Run random placement generation with a given setting.
Usage:
`[executable file path] RandomGen [mode] [#B] [#R] [#b] [#r] [min #actions] [max #depth] [#trails] [(random seed)]`
* mode: normal or partial
    * normal: not-revealed
    * partial: partly-revealed
* #B: integer, the number of the player's blue pieces
* #R: integer, the number of the player's red pieces
* #b: integer, the number of the opponent's blue pieces
* #r: integer, the number of the opponent's red pieces
* min #actions: integer, the minimum move number required
* max #depth: integer up to 19, the maximum move number to search
* #trails: integer, the number of trials (including failed ones)
* random seed: integer, random seed (optional)
    * If not specified, `time(NULL)` is used, which will be recorded.

Example:
`GeisterEndGamePuzzle.exe RandomGen normal 1 2 2 1 5 9 500 0`

Try to make 5-9 moves not-revealed puzzles with 1 own blue piece, 2 own red pieces, 2 opponent blue pieces, and 1 opponent red piece from 500 trials (random seed of 0).
The puzzles will be saved in the `normal` folder, and the statistics will be stored in csv files.


### Mode 2/2: GetSol

Get an example solution to a given puzzle.
Usage:
`[executable file path] GetSol [board string] [#b] [(max #action)] [(RedWall)]`
* board string: 1 $\times$ 36 string showing the board
* #b: integer, the number of the opponent's blue pieces
* max #action: the depth limit of search (optional)
* RedWall: whether to check red-wall (optional)

Example:
`GeisterEndGamePuzzle.exe GetSol ..........u.B.................Ru.... 1 7 RedWall`

One example solution (while there may be many) will be output on the console if solvable.

Example:
```
not-revealed
79 nodes
78 nodes of #winning move
5 moves
<Red-Wall>
Example Solution
6 ......
5 B...u.
4 ......
3 ......
2 ......
1 Ru....
  abcdef

6 ....u.
5 B.....
4 ......
3 ......
2 ......
1 Ru....
  abcdef

6 B...u.
5 ......
4 ......
3 ......
2 ......
1 Ru....
  abcdef

6 B....u
5 ......
4 ......
3 ......
2 ......
1 Ru....
  abcdef

1. B a4 up
2. u e5 up
3. B a5 up
4. u e6 right
5. B a6 left

Total time = 0.007 s
```

## A simple viewer and pre-generated puzzles

We also prepared a simple viewer for you to see Geister endgame puzzles on GUI.
You can find the GUI in the `viewer_and_generated_puzzles` folder.
* `GeisterViewEN.exe` is the English version of the viewer.
* `GeisterViewJP.exe` is the Japanese version of the viewer.
* `allprob.csv` contains 44,826 puzzles generated in the experiments of our paper.


## Citation

```
@article{Hsueh2024GeisterEndgamePuzzle,
  title = {Proposal and generation of endgame puzzles for an imperfect information game Geister},
  author = {Chu-Hsuan Hsueh and Takefumi Ishii and Tsuyoshi Hashimoto and Kokolo Ikeda},
  journal = {Entertainment Computing},
  year = {2024},
  vol = {Pre-press}
  doi = {https://doi.org/10.1016/j.entcom.2024.100736},
  url = {https://www.sciencedirect.com/science/article/pii/S1875952124001046},
}
```
