import os

from scripts.NB import TicTacToeNaiveBayes

if __name__ == "__main__":
    absPath = os.path.dirname(os.path.abspath(__file__))

    data_file = os.path.join(absPath, "tttData.csv")
    model_output = os.path.join(absPath, "tttModel.pkl")

    tttNB = TicTacToeNaiveBayes(data_file=data_file, model_file=model_output)

    # train and save naive bayes model 
    tttNB.load_data()
    tttNB.train_model()
    tttNB.save_model()