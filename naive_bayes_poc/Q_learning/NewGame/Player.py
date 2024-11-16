import pickle
from typing import List, Tuple
import numpy as np

from NewGame.Global import Global

class Player:
    def __init__(self, name: str, exp_rate: float = 0.3) -> None: 
        self.name: str = name
        self.states: List[str] = []
        self.state_value: dict = {}
        self.lr: float = 0.2 
        self.exp_rate: float = exp_rate
        self.decay: float = 0.9

    def choose_action(self, positions: List[Tuple[int, int]], board: np.ndarray, symbol: int) -> Tuple[int, int]:
        if np.random.uniform(0, 1) <= self.exp_rate: # explore
            return positions[np.random.choice(len(positions))]
        
        # exploit 
        value_max = -999
        action = positions[0]
        for pos in positions: 
            next_board = board.copy()
            next_board[pos] = symbol
            board_string = str(next_board.reshape(Global.BOARD_ROWS * Global.BOARD_COLS))
            value = self.state_value.get(board_string, 0)
            if value > value_max:
                value_max = value
                action = pos 
        return action

    def add_state(self, state: str) -> None: 
        self.states.append(state)
    
    def feed_reward(self, reward: float) -> None: 
        for state in reversed(self.states):
            if state not in self.state_value:
                self.state_value[state] = 0
            self.state_value[state] += self.lr * (self.decay * reward - self.state_value[state])
            reward = self.state_value[state]
    
    def reset(self) -> None: 
        self.state = []

    def save(self, filename: str) -> None: 
        with open(filename, 'wb') as f: 
            pickle.dump(self.state_value, f)
        
    def load(self, filename: str) -> None: 
        with open(filename, 'rb') as f: 
            self.state_value = pickle.load(f)
    

class HumanPlayer:
    def __init__(self, name: str) -> None: 
        self.name: str = name
    
    def choose_action(self, positions: List[Tuple[int, int]], *_) -> Tuple[int, int]:
        while True: 
            try:
                row = int(input("Enter row (0-2): "))
                col = int(input("Enter col (0-2): "))
                if (row, col) in positions:
                    return row, col
                print("Invalid position. Try again.")
            except ValueError:
                print("Invalid Input. Please enter integers only.")
    
    def add_state(self, state: str) -> None: 
        pass
    
    def feed_reward(self, reward: float) -> None: 
        pass
    
    def reset(self) -> None: 
        pass