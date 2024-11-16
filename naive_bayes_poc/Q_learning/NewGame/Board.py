import csv
import numpy as np
from typing import List, Optional, Tuple

from NewGame.Global import Global
from NewGame.Player import HumanPlayer

class Board: 
    def __init__(self, p1, p2) -> None:
        self.board: np.ndarray = np.zeros((Global.BOARD_ROWS, Global.BOARD_COLS), dtype=int)
        self.game_state: bool = False
        self.p1 = p1
        self.p2 = p2
        self.current_Player_Symbol: int = 1 # start with player 1
        self.data: List[Tuple[List[int, int]]] = [] # to store board states and moves

    def board_to_string(self) -> str: 
        return str(self.board.reshape(Global.BOARD_ROWS * Global.BOARD_COLS))

    def available_positions(self) -> List[Tuple[int, int]]:
        return [(i, j) for i in range(Global.BOARD_ROWS) for j in range(Global.BOARD_COLS) if self.board[i, j] == 0]

    def update_state(self, position: Tuple[int, int]) -> None: 
        self.board[position] = self.current_Player_Symbol
        self.current_Player_Symbol = -1 if self.current_Player_Symbol == 1 else 1

    def check_winner(self) -> Optional[int]:
        # check row and columns
        for i in range(Global.BOARD_ROWS):
            if abs(sum(self.board[i, :])) == 3:
                self.game_state = True
                return np.sign(sum(self.board[i, :]))
            if abs(sum((self.board[:, i]))) == 3:
                self.game_state = True
                return np.sign(sum(self.board[:, i]))

        diag_sum1 = sum(self.board[i, i] for i in range(Global.BOARD_COLS))
        diag_sum2 = sum (self.board[i, Global.BOARD_COLS - i - 1] for i in range(Global.BOARD_COLS))
        if abs(diag_sum1) == 3: 
            self.game_state = True
            return np.sign(diag_sum1)
        if abs(diag_sum2) == 3:
            self.game_state = True
            return np.sign(diag_sum2)

        # check tie
        if not self.available_positions():
            self.game_state = True
            return 0 

        return None # Game ongoing 
    
    def reward(self) -> None: 
        result = self.check_winner()
        if result == 1: 
            self.p1.feed_reward(1)
            self.p2.feed_reward(0)
        elif result == -1: 
            self.p1.feed_reward(0)
            self.p2.feed_reward(1)
        elif result == 0:
            self.p1.feed_reward(0.5)
            self.p2.feed_reward(0.5)

    def reset(self) -> None: 
        self.board.fill(0)
        self.game_state = False
        self.current_Player_Symbol = 1

    def train(self, rounds: int = 10000) -> None: 
        for i in range(rounds):
            # if i % 1000 == 0: 
            #     print(f"Training round: {i}")
            print(f"Training round: {i}")
            self.reset()

            while not self.game_state:
                for player in [self.p1, self.p2]:
                    positions = self.available_positions()
                    action = player.choose_action(positions, self.board, self.current_Player_Symbol)

                    # save the board state and action 
                    board_state_flat = self.board.flatten().tolist()
                    self.data.append((board_state_flat, positions.index(action)))

                    self.update_state(action)
                    player.add_state(self.board_to_string())
                    # self.show_board()

                    winner = self.check_winner()
                    if winner is not None: 
                        self.reward()
                        self.p1.reset()
                        self.p2.reset()
                        break
        
        self.save_data_to_csv("tttData.csv")

    def save_data_to_csv(self, filename: str) -> None: 
        with open(filename, mode="w", newline="") as f: 
            writer = csv.writer(f)
            # header
            writer.writerow([f"cell_{i}" for i in range(9)] + ["move"])
            # data
            for board_state, move in self.data:
                writer.writerow(board_state + [move])
        print(f"Training data saved to {filename}")
    
    def pve(self) -> None: 
        while not self.game_state:
            for player in [self.p1, self.p2]: 
                if isinstance(player, HumanPlayer):
                    print("Your Turn!")
                    self.show_board()
                positions = self.available_positions()
                action = player.choose_action(positions, self.board, self.current_Player_Symbol)
                self.update_state(action)
                self.show_board()

                winner = self.check_winner()
                if winner is not None: 
                    self.show_board()
                    if winner == 1: 
                        print(f"{self.p1.name} wins!")
                    elif winner == -1: 
                        print(f"{self.p2.name} wins!")
                    else: 
                        print("It's a tie!")
                    self.reset()
                    return 
    
    def show_board(self) -> None: 
        for i in range(Global.BOARD_ROWS):
            print("-------------")
            print("| " + " | ".join(self._symbol(self.board[i, j]) for j in range(Global.BOARD_COLS)) + " |")
        print("-------------")

    @staticmethod
    def _symbol(value: int) -> str: 
        if value == 1:
            return 'x'
        elif value == -1: 
            return 'o'
        return ' '
