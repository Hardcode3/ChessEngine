# UCI Protocol Documentation

- 🔗 [Wikipedia - Universal Chess Interface](https://en.wikipedia.org/wiki/Universal_Chess_Interface)

## Overview

The Universal Chess Interface (UCI) is a protocol that enables communication between a chess engine and a graphical user interface (GUI). This document describes the main UCI commands and their usage.

## Basic Communication

- The engine receives commands through standard input (stdin)
- The engine sends responses through standard output (stdout)
- All commands and responses are text-based
- Each command is terminated by a newline character

## Main UCI Commands

### Initialization Commands

#### `uci`

- **Purpose**: Initial handshake between GUI and engine
- **Engine Response**:

```
id name <engine_name>
id author <author_name>
option name <option_name> type <option_type> ...
uciok
```

#### `isready`

- **Purpose**: Check if engine is ready to receive commands
- **Engine Response**: `readyok`

#### `ucinewgame`

- **Purpose**: Signal the start of a new game
- **Engine Response**: None required

### Position and Move Commands

#### `position [fen <fenstring> | startpos] moves <move1> ... <movei>`

- **Purpose**: Set up a position on the internal board
- **Parameters**:
  - `fen <fenstring>`: Set up position from FEN string
  - `startpos`: Set up initial position
  - `moves <move1> ... <movei>`: List of moves to play from the position
- **Example**: `position startpos moves e2e4 e7e5`

#### `go`

- **Purpose**: Start calculating on the current position
- **Parameters** (all optional):
  - `searchmoves <move1> ... <movei>`: Restrict search to these moves
  - `ponder`: Start searching in pondering mode
  - `wtime <x>`: White has x msec left
  - `btime <x>`: Black has x msec left
  - `winc <x>`: White increment per move in msec
  - `binc <x>`: Black increment per move in msec
  - `movestogo <x>`: There are x moves to the next time control
  - `depth <x>`: Search x plies only
  - `nodes <x>`: Search x nodes only
  - `mate <x>`: Search for mate in x moves
  - `movetime <x>`: Search exactly x msec
  - `infinite`: Search until the "stop" command
- **Engine Response**: `bestmove <move> [ponder <move>]`

#### `stop`

- **Purpose**: Stop calculating as soon as possible
- **Engine Response**: `bestmove <move> [ponder <move>]`

### Debug and Analysis Commands

#### `setoption name <id> [value <x>]`

- **Purpose**: Set the value of an option
- **Example**: `setoption name Hash value 128`

#### `debug [on | off]`

- **Purpose**: Switch debug mode on or off
- **Engine Response**: None required

### Quit Command

#### `quit`

- **Purpose**: Quit the program as soon as possible
- **Engine Response**: None required

## Engine Output During Search

During a search, the engine can send the following information:

```
info depth <x> seldepth <y> time <t> nodes <n> score cp <x> pv <move1> ... <movei>
```

Where:

- `depth`: Current search depth
- `seldepth`: Selective search depth
- `time`: Time spent searching in ms
- `nodes`: Number of nodes searched
- `score`: Evaluation score
  - `cp <x>`: Centipawns (positive for white advantage)
  - `mate <y>`: Mate in y moves
- `pv`: Principal variation (best line found)

## Example Communication

```
GUI: uci
Engine: id name MyChessEngine
Engine: id author John Doe
Engine: option name Hash type spin default 128 min 1 max 1024
Engine: uciok

GUI: isready
Engine: readyok

GUI: position startpos moves e2e4 e7e5
GUI: go depth 10
Engine: info depth 1 score cp 20 nodes 20 time 1 pv e2e4
Engine: info depth 2 score cp 15 nodes 100 time 2 pv e2e4 e7e5
...
Engine: bestmove e2e4 ponder e7e5

GUI: quit
```

## Implementation Notes

1. The engine must be able to handle all commands at any time
2. Commands should be processed in the order they are received
3. The engine should respond to commands as quickly as possible
4. During a search, the engine should regularly send info strings
5. The engine must be able to stop searching immediately when receiving the "stop" command
