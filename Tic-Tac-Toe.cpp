#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

// Liệt kê các biểu tượng (Symbol) cho các ô trên bảng
enum Symbol { X, O };

// Liệt kê trạng thái của trò choi
enum GameStatus { IN_PROGRESS, DRAW, FIRST_PLAYER_WIN, SECOND_PLAYER_WIN };

// Liệt kê các loại người chơi
enum PlayerType { HUMAN, COMPUTER };


// Lớp Board đại diện cho bảng trò choi
class Board {
private:
    vector<char> square;    // Vector chứa các ô trên bảng
    int numberOfEmptySquare; // Số ô trống trên bảng

public:
    // Constructor của Board
    Board() : square(9, '_'), numberOfEmptySquare(9) {}

    // Phương thức để đánh dấu ô trên bảng với biểu tượng (X hoặc O) tương ứng
    bool markSymbol(int row, int col, Symbol symbol) {
        int index = row * 3 + col;
        if (square[index] == '_') {
            square[index] = (symbol == X) ? 'X' : 'O';
            numberOfEmptySquare--;
            return true;
        }
        return false;
    }

    //  Phương thức để kiểm tra xem có người chiến thắng hay không ?
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

    //  Phương thức để kiểm tra xem còn ô trống nào trên bảng hay không ?
    bool isEmptyCellExist() {
        return numberOfEmptySquare > 0;
    }

    //  Phương thức để in bảng ra màn hình
    void printBoard() {
        for (int i = 0; i < 9; ++i) {
            cout << square[i] << ' ';
            if ((i + 1) % 3 == 0) cout << endl;
        }
    }
};


//  Lớp Player đại diện cho một người chơi
class Player {
protected:
    int id;         //  ID của người chơi
    string name;    //  Tên của người chơi
public:
    Player(int playerId, const string& playerName) : id(playerId), name(playerName) {}

    //  Phương thức để lấy bước đi tiếp theo của người chơi
    virtual int getNextMove(Board& board, int& row, int& col) = 0;

    //  Phương thức để lấy biểu tượng của người chơi (X hoặc O)
    virtual Symbol getSymbol() const = 0;

    //  Phương thức để lấy ID của người chơi
    int getId() const { return id; }

    //  Phương thức để lấy tên của người chơi
    string getName() const { return name; }

    //  Phương thức để kiểm tra xem người chơi có phải là máy AI hay không
    virtual bool isAI() const { return false; }

    //  Hủy bỏ đa hình
    virtual ~Player() {}
};

//  Lớp HumanPlayer kế thừa từ lớp Player, đại diện cho người chơi là con người
class HumanPlayer : public Player {
private:
    Symbol symbol; //  Biểu tượng của người chơi (X hoặc O)

public:
    // Constructor của HumanPlayer
    HumanPlayer(int playerId, const string& playerName, Symbol sym) : Player(playerId, playerName), symbol(sym) {}

    //  Phương thức để người chơi con người lấy bước đi tiếp theo
    int getNextMove(Board& board, int& row, int& col) override {
        while (true) {
            //  Nhập dòng và cột từ người chơi
            cout << "Player " << getName() << ", enter row and column (0-2): ";
            cin >> row >> col;

            //  Kiểm tra tính hợp lệ của dòng và cột nhập vào
            if (row < 0 || row > 2 || col < 0 || col > 2) {
                cout << "Invalid input. Row and column must be in range [0, 2]." << endl;
                continue;
            }

            //  Đánh dấu ô trên bảng nếu ô đó chưa được đánh dấu trước đó
            if (board.markSymbol(row, col, symbol)) {
                break;
            }
            else {
                cout << "Cell (" << row << ", " << col << ") is already taken. Please choose another cell." << endl;
            }
        }
        return 0;
    }

    //  Phương thức lấy biểu tượng của người chơi
    Symbol getSymbol() const override {
        return symbol;
    }
};

//  Lớp ComputerPlayer kế thừa từ lớp Player, đại diện cho người chơi là máy tính
class ComputerPlayer : public Player {
private:
    Symbol symbol; // Biểu tượng của người chơi (X hoặc O)

public:
    // Constructor của ComputerPlayer
    ComputerPlayer(int playerId, const string& playerName, Symbol sym) : Player(playerId, playerName), symbol(sym) {}

    //  Phương thức để người chơi máy tính lấy bước đi tiếp theo
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

    // Phương thức lấy biểu tượng của người chơi
    Symbol getSymbol() const override {
        return symbol;
    }

    //  Phương thức để kiểm tra xem người chơi có phải là máy AI hay không
    bool isAI() const override { return true; }
};

// Lớp Game đại diện cho trò choi
class Game {
private:
    Board board;            // Bảng trò choi
    Player* firstPlayer;    // Người chơi thứ 1
    Player* secondPlayer;   // Người chơi thứ 2
    GameStatus gameStatus;  // Trạng thái của trò chơi

public:
    // Constructor của Game
    Game(Player* p1, Player* p2) : firstPlayer(p1), secondPlayer(p2), gameStatus(IN_PROGRESS) {}

    //  Phương thức để thực hiện trò chơi
    void play() {
        int row, col;
        while (board.isEmptyCellExist() && gameStatus == IN_PROGRESS) {
            //  Lượt đi của người chơi thứ 1
            firstPlayer->getNextMove(board, row, col);
            board.markSymbol(row, col, firstPlayer->getSymbol());
            board.printBoard(); //  In bảng sau lượt đi của người chơi 1
            if (board.checkWin()) {
                gameStatus = FIRST_PLAYER_WIN;
                break;
            }
            if (!board.isEmptyCellExist()) {
                gameStatus = DRAW;
                break;
            }

            // Lượt đi của người chơi thứ 2
            secondPlayer->getNextMove(board, row, col);
            board.markSymbol(row, col, secondPlayer->getSymbol());
            board.printBoard(); // In bảng sau lượt đi của người chơi 2
            if (board.checkWin()) {
                gameStatus = SECOND_PLAYER_WIN;
                break;
            }
        }
    }

    // Phương thức để in kết quả của trò chơi ra màn hình
    void printResult() const {
        if (gameStatus == FIRST_PLAYER_WIN) {
            cout << "Congratulations! " << firstPlayer->getName() << " wins!" << endl;
        }
        else if (gameStatus == SECOND_PLAYER_WIN) {
            cout << "Congratulations! " << secondPlayer->getName() << " wins!" << endl;
        }
        else {
            cout << "It's a draw!" << endl;
        }
    }
};

//  Hàm tạo người chơi dựa trên lựa chọn của người dùng 
Player* createPlayer(int& playerId) {
    string playerName;
    cout << "Enter name for Player " << playerId << ": ";
    cin >> playerName;
    char playerTypeChoice;
    cout << "Is Player " << playerId << " a human player? (y/n): ";
    cin >> playerTypeChoice;
    Symbol playerSymbol = (playerId == 1) ? X : O; //  Người chơi đầu tiên sẽ chọn X, người chơi thứ 2 sẽ chọn O
    if (tolower(playerTypeChoice) == 'y') {
        return new HumanPlayer(playerId++, playerName, playerSymbol);
    }
    else {
        return new ComputerPlayer(playerId++, "Computer", playerSymbol);
    }
}

// Hàm main
int main() {
    int playerId = 1;           //  Khởi tạo ID của người chơi
    Player* p1 = createPlayer(playerId);   // Tạo người chơi thứ 1
    Player* p2 = createPlayer(playerId);   // Tạo người chơi thứ 2

    //  Tạo trò chơi với hai người chơi đã được tạo
    Game game(p1, p2);
    // Bắt đàu trò chơi
    game.play();
    //  In kết quả của trò chơi ra màn hình
    game.printResult();

    //  Giải phóng bộ nhớ
    delete p1;
    delete p2;

    return 0;
}

