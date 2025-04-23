# 🎴 Bastra (Pişti) Game in C

This project is a terminal-based implementation of the classic Turkish card game **Bastra** (a.k.a. Pişti), written in C.

---

## 💡 Features

- 52-card deck using structs and enums
- Random deck shuffle using `srand` + `getpid()`
- 4 players: supports **solo** or **team (2v2)** mode
- Turn-based gameplay loop
- **Jack clears board**, **Bastra gives 10 points**
- Card recommendation system
- Smart rule enforcement and board control
- Score tracking per round
- Clear CLI feedback and structure

---

## 🧠 Game Rules Handled

- Jack (value 11) collects the full board
- Matching cards from board sum with hand to take cards
- Special points:
  - Aces = 1 pt
  - Jacks = 1 pt
  - 2♣ = 2 pts
  - 10♦ = 3 pts
- Bastra (clearing a non-Jack empty board) = 10 pts
- Team or Individual score mode
- 10 bonus points to the player/team with most cards

---

## ▶️ How to Run

Make sure you have GCC installed:

```bash
gcc bastra.c -o bastra
./bastra
