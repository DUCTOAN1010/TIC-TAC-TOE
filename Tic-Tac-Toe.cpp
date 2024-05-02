#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

// Li?t k� c�c bi?u tu?ng (Symbol) cho c�c � tr�n b?ng
enum Symbol { X, O };

// Li?t k� tr?ng th�i c?a tr� choi
enum GameStatus { IN_PROGRESS, DRAW, FIRST_PLAYER_WIN, SECOND_PLAYER_WIN };

// Li?t k� c�c lo?i ngu?i choi
enum PlayerType { HUMAN, COMPUTER };


// L?p Board d?i di?n cho b?ng tr� choi
class Board {
private:
    vector<char> square;    // Vector ch?a c�c � tr�n b?ng
    int numberOfEmptySquare; // S? � tr?ng tr�n b?ng

public:
    // Constructor c?a Board
    Board() : square(9, '_'), numberOfEmptySquare(9) {}

    // Phuong th?c d? d�nh d?u � tr�n b?ng v?i bi?u tu?ng (X ho?c O) tuong ?ng
    bool markSymbol(int row, int col, Symbol symbol) {
        int index = row * 3 + col;
        if (square[index] == '_') {
            square[index] = (symbol == X) ? 'X' : 'O';
            numberOfEmptySquare--;
            return true;
        }
        return false;
    }

    // Phuong th?c d? ki?m tra xem c� ngu?i chi?n th?ng hay kh�ng
    bool checkWin() {
        for (int i = 0; i < 3; ++i) {
            if (square[i] != '_' && square[i] == square[i + 3] && square[i] == square[i + 6])
                return true;
            if (square[3 * i] != '_' && square[3 * i] == square[3 * i + 1] && square[3 * i] == square[3 * i + 2])
                return true;
        }
        if (square[0] != '_' && square[0] == square[4] && square[0] == square[8])
            return true;
        if (square[2] != '_' && square[2] == square[4] && square[2] == square[6])
            return true;
        return false;
    }

    // Phuong th?c d? ki?m tra xem c�n � tr?ng n�o tr�n b?ng hay kh�ng
    bool isEmptyCellExist() {
        return numberOfEmptySquare > 0;
    }

    // Phuong th?c d? in b?ng ra m�n h�nh
    void printBoard() {
        for (int i = 0; i < 9; ++i) {
            cout << square[i] << ' ';
            if ((i + 1) % 3 == 0) cout << endl;
        }
    }
};


// L?p Player d?i di?n cho m?t ngu?i choi
class Player {
protected:
    int id;         // ID c?a ngu?i choi
    string name;    // T�n c?a ngu?i choi
public:
    Player(int playerId, const string& playerName) : id(playerId), name(playerName) {}

    // Phuong th?c ?o d? l?y bu?c di ti?p theo c?a ngu?i choi
    virtual int getNextMove(Board& board, int& row, int& col) = 0;

    // Phuong th?c ?o d? l?y bi?u tu?ng c?a ngu?i choi (X ho?c O)
    virtual Symbol getSymbol() const = 0;

    // Phuong th?c d? l?y ID c?a ngu?i choi
    int getId() const { return id; }

    // Phuong th?c d? l?y t�n c?a ngu?i choi
    string getName() const { return name; }

    // Phuong th?c ?o d? ki?m tra xem ngu?i choi c� ph?i l� m�y AI hay kh�ng
    virtual bool isAI() const { return false; }

    // H?y b? da h�nh
    virtual ~Player() {}
};

// L?p HumanPlayer k? th?a t? l?p Player, d?i di?n cho ngu?i choi l� con ngu?i
class HumanPlayer : public Player {
private:
    Symbol symbol; // Bi?u tu?ng c?a ngu?i choi (X ho?c O)

public:
    // Constructor c?a HumanPlayer
    HumanPlayer(int playerId, const string& playerName, Symbol sym) : Player(playerId, playerName), symbol(sym) {}

    // Phuong th?c d? ngu?i choi con ngu?i l?y bu?c di ti?p theo
    int getNextMove(Board& board, int& row, int& col) override {
        while (true) {
            // Nh?p d�ng v� c?t t? ngu?i choi
            cout << "Player " << getName() << ", enter row and column (0-2): ";
            cin >> row >> col;

            // Ki?m tra t�nh h?p l? c?a d�ng v� c?t nh?p v�o
            if (row < 0 || row > 2 || col < 0 || col > 2) {
                cout << "Invalid input. Row and column must be in range [0, 2]." << endl;
                continue;
            }

            // ��nh d?u � tr�n b?ng n?u � d� chua du?c d�nh d?u tru?c d�
            if (board.markSymbol(row, col, symbol)) {
                break;
            } else {
                cout << "Cell (" << row << ", " << col << ") is already taken. Please choose another cell." << endl;
            }
        }
        return 0;
    }

    // Phuong th?c d? l?y bi?u tu?ng c?a ngu?i choi
    Symbol getSymbol() const override {
        return symbol;
    }
};

// L?p ComputerPlayer k? th?a t? l?p Player, d?i di?n cho ngu?i choi l� m�y t�nh
class ComputerPlayer : public Player {
private:
    Symbol symbol; // Bi?u tu?ng c?a ngu?i choi (X ho?c O)

public:
    // Constructor c?a ComputerPlayer
    ComputerPlayer(int playerId, const string& playerName, Symbol sym) : Player(playerId, playerName), symbol(sym) {}

    // Phuong th?c d? ngu?i choi m�y t�nh l?y bu?c di ti?p theo
    int getNextMove(Board& board, int& row, int& col) override {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 2);
        do {
            row = distrib(gen);
            col = distrib(gen);
        } while (!board.markSymbol(row, col, symbol));
        return 0;
    }

    // Phuong th?c d? l?y bi?u tu?ng c?a ngu?i choi
    Symbol getSymbol() const override {
        return symbol;
    }

    // Phuong th?c d? ki?m tra xem ngu?i choi c� ph?i l� m�y AI hay kh�ng
    bool isAI() const override { return true; }
};

// L?p Game d?i di?n cho tr� choi
class Game {
private:
    Board board;            // B?ng tr� choi
    Player* firstPlayer;    // Ngu?i choi th? nh?t
    Player* secondPlayer;   // Ngu?i choi th? hai
    GameStatus gameStatus;  // Tr?ng th�i c?a tr� choi

public:
    // Constructor c?a Game
    Game(Player* p1, Player* p2) : firstPlayer(p1), secondPlayer(p2), gameStatus(IN_PROGRESS) {}

    // Phuong th?c d? th?c hi?n tr� choi
    void play() {
        int row, col;
        while (board.isEmptyCellExist() && gameStatus == IN_PROGRESS) {
            // Lu?t di c?a ngu?i choi th? nh?t
            firstPlayer->getNextMove(board, row, col);
            board.markSymbol(row, col, firstPlayer->getSymbol());
            board.printBoard(); // In b?ng sau lu?t di c?a ngu?i choi 1
            if (board.checkWin()) {
                gameStatus = FIRST_PLAYER_WIN;
                break;
            }
            if (!board.isEmptyCellExist()) {
                gameStatus = DRAW;
                break;
            }

            // Lu?t di c?a ngu?i choi th? hai
            secondPlayer->getNextMove(board, row, col);
            board.markSymbol(row, col, secondPlayer->getSymbol());
            board.printBoard(); // In b?ng sau lu?t di c?a ngu?i choi 2
            if (board.checkWin()) {
                gameStatus = SECOND_PLAYER_WIN;
                break;
            }
        }
    }

    // Phuong th?c d? in k?t qu? c?a tr� choi ra m�n h�nh
    void printResult() const {
        if (gameStatus == FIRST_PLAYER_WIN) {
            cout << "Congratulations! " << firstPlayer->getName() << " wins!" << endl;
        } else if (gameStatus == SECOND_PLAYER_WIN) {
            cout << "Congratulations! " << secondPlayer->getName() << " wins!" << endl;
        } else {
            cout << "It's a draw!" << endl;
        }
    }
};

// H�m t?o ngu?i choi d?a tr�n l?a ch?n c?a ngu?i d�ng
Player* createPlayer(int& playerId) {
    string playerName;
    cout << "Enter name for Player " << playerId << ": ";
    cin >> playerName;
    char playerTypeChoice;
    cout << "Is Player " << playerId << " a human player? (y/n): ";
    cin >> playerTypeChoice;
    Symbol playerSymbol = (playerId == 1) ? X : O; // Ngu?i choi d?u ti�n s? ch?n X, ngu?i choi th? hai s? ch?n O
    if (tolower(playerTypeChoice) == 'y') {
        return new HumanPlayer(playerId++, playerName, playerSymbol);
    } else {
        return new ComputerPlayer(playerId++, "Computer", playerSymbol);
    }
}

// H�m main
int main() {
    int playerId = 1;           // Kh?i t?o ID c?a ngu?i choi
    Player* p1 = createPlayer(playerId);   // T?o ngu?i choi th? nh?t
    Player* p2 = createPlayer(playerId);   // T?o ngu?i choi th? hai
    
    // T?o tr� choi v?i hai ngu?i choi d� du?c t?o
    Game game(p1, p2);
    // B?t d?u choi tr� choi
    game.play();
    // In k?t qu? c?a tr� choi ra m�n h�nh
    game.printResult();

    // Gi?i ph�ng b? nh?
    delete p1;
    delete p2;

    return 0;
}

