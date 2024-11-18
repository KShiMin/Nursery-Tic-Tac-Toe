import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.naive_bayes import GaussianNB
import pickle


class TicTacToeNaiveBayes:
    def __init__(self, data_file: str, model_file: str) -> None:
        self.data_file = data_file
        self.model_file = model_file
        self.model = None
        self.X = None
        self.y = None
        self.accuracy = None

    def load_data(self) -> None:
        data = pd.read_csv(self.data_file)
        self.X = data.iloc[:, :-1]  # All columns except the last are features
        self.y = data.iloc[:, -1]   # The last column is the target
        print(f"Data loaded: {self.X.shape[0]} samples")

    def train_model(self, test_size: float = 0.2, random_state: int = 42) -> None:
        X_train, X_test, y_train, y_test = train_test_split(self.X, self.y, test_size=test_size, random_state=random_state)
        self.model = GaussianNB()
        self.model.fit(X_train, y_train)
        self.accuracy = self.model.score(X_test, y_test)
        print(f"Model trained with accuracy: {self.accuracy * 100:.2f}%")

    def save_model(self) -> None:
        if self.model is None:
            raise ValueError("No model trained. Train the model before saving.")
        with open(self.model_file, 'wb') as f:
            pickle.dump(self.model, f)
        print(f"Model saved to {self.model_file}")

    def load_model(self) -> None:
        with open(self.model_file, 'rb') as f:
            self.model = pickle.load(f)
        print(f"Model loaded from {self.model_file}")

    def evaluate_model(self, X_test: pd.DataFrame | np.ndarray, y_test: pd.Series | np.ndarray | list) -> float:   
        if self.model is None:
            raise ValueError("No model loaded. Load or train a model before evaluation.")
        accuracy = self.model.score(X_test, y_test)
        print(f"Model evaluation accuracy: {accuracy * 100:.2f}%")
        return accuracy

