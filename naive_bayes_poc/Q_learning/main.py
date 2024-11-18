from NewGame.Player import Player, HumanPlayer
from NewGame.Board import Board

def training():
    p1 = Player("AI_Player")
    p2 = Player("AI_Opponent")
    board = Board(p1, p2)
    print("Training..")

    board.train(rounds=30000)
    p1.save("ai_player.pkl")


def play():
    p1 = Player("AI_Player", exp_rate=0)
    p1.load("ai_player.pkl")

    p2 = HumanPlayer("Human_Player")
    board = Board(p1, p2)
    board.pve()

if __name__ == "__main__":
    training()
    # play()