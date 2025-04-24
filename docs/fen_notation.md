# 🧩 Structure of FEN

A FEN string looks like this:

```
rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
```

It contains **6 space-separated fields**:

1. **Piece placement**
2. **Active color**
3. **Castling availability**
4. **En passant target square**
5. **Halfmove clock**
6. **Fullmove number**

---

## 📘 1. Piece placement

This part represents the **board layout**, starting from **rank 8 to rank 1** (top to bottom), with `/` separating ranks.

Pieces are represented by letters:
- Uppercase = White (`P`, `N`, `B`, `R`, `Q`, `K`)
- Lowercase = Black (`p`, `n`, `b`, `r`, `q`, `k`)
- Digits = number of empty squares

Example:
```
rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR
```
This is the **initial board setup**.

---

## ⚪ 2. Active color

Whose turn it is:
- `w` = White
- `b` = Black

---

## ♔ 3. Castling availability

- Letters indicate castling rights:
  - `K` = White kingside
  - `Q` = White queenside
  - `k` = Black kingside
  - `q` = Black queenside
- `-` if no castling is available

Example:
```
KQkq
```
Means both players can castle both ways.

---

## 🏹 4. En passant target square

If a pawn has just moved two squares forward, this shows the **square behind it** for en passant.

- Example: `e3` (white just moved pawn from `e2` to `e4`, black can capture en passant on `e3`)
- `-` means no en passant possible

---

## ⏱ 5. Halfmove clock

Number of **halfmoves since the last pawn move or capture**. Used for the 50-move draw rule.

---

## 🔢 6. Fullmove number

Starts at `1`, and increments **after Black's move**.

---

## 🔍 Full example explained

```
rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
```

- **Position**: Starting position
- **White to move**
- **Both sides can castle both directions**
- **No en passant**
- **0 halfmoves since capture/pawn move**
- **Move number 1**
