from dataclasses import dataclass

@dataclass(frozen=True)
class Global:
    BOARD_COLS: int = 3
    BOARD_ROWS: int = 3