#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include<set>
#include<queue>
#include<limits.h>
using namespace std;

#define INF INT_MAX
#define NEG_INF -999999999
#define DEBUG 0;
enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;

struct Point {
    int x, y, score;
	Point() : Point(0, 0) {}
	Point(float x, float y) : x(x), y(y) {}
	bool operator==(const Point& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Point& rhs) const {
		return !operator==(rhs);
	}
	Point operator+(const Point& rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}
	Point operator-(const Point& rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}
    bool operator<(const Point &r)const{
        if(x!=r.x) return x < r.x;
        if(y!=r.y) return y < r.y;
        return 0;
    } 
};

Point Best;

class State{
public:
    std::array<std::array<int, SIZE>, SIZE> Board; 
    std::set<Point> enum_move_point;
    int player,score;
    Point best_point;
    State(std::array<std::array<int, SIZE>, SIZE>b, int ply){
        for(int i=0;i<SIZE;i++){
            for(int j=0;j<SIZE;j++){
                this->Board[i][j] = b[i][j];
            }
        }
        this->player = ply;
        this->score = NEG_INF;
    }
    State(State& copy){
        for(int i=0;i<SIZE;i++){
            for(int j=0;j<SIZE;j++){
                this->Board[i][j] = copy.Board[i][j];
            }
        }
        this->player = copy.player;
    }
    void next_move_enum(){
        //scan the whole board
        for(int i=0;i<SIZE;i++){
            for(int j=0;j<SIZE;j++){
                if(Board[i][j] == EMPTY){
                    //check Jiugongge
                    this->enum_move_point.insert(Point(i,j));
                }
            }
        }
        return;
    }

    void add_Point(Point point, int disc){
        Board[point.x][point.y] = disc;
    }

    int evaluate_score(){
        int h = 0;
        //set<int,int> vis;
        int opponent = 3 - player;
        for(int i = 0; i<SIZE; i++){
            for(int j=0; j<SIZE; j++){
                if(Board[i][j] == player){
                    if(j+2<SIZE && Board[i][j+1] == player && Board[i][j+2] == player){
                          
                        h++;
                        if(j+3<SIZE && Board[i][j+3] == player){
                            h++;
                            if(Board[i][j+4]<SIZE && Board[i][j+4] == player){
                                h++;
                            }
                        }
                    }
                    if(i+2<SIZE && Board[i+1][j] == player && Board[i+2][j] == player){
                        h++;
                        if(i+3<SIZE && Board[i+3][j] == player){
                            h++;
                            if(i+4<SIZE && Board[i+4][j] == player){
                                h++;
                            }
                        }
                    }
                    if(i+2<SIZE && j+2<SIZE && Board[i+1][j+1] == player && Board[i+2][j+2] == player){
                        h++;
                        if(i+3<SIZE && j+3<SIZE && Board[i+3][j+3] == player){
                            h++;
                            if(i+4<SIZE && j+4<SIZE && Board[i+4][j+4] == player){
                                h++;
                            }
                        }
                    }
                    if(i+2<SIZE && j-2>=0 && Board[i+1][j-1] == player && Board[i+2][j-2] == player){
                        h++;
                        if(i+3<SIZE && j-3>=0 && Board[i+3][j-3] == player){
                            h++;
                            if(i+4<SIZE && j-4>=0 && Board[i+4][j-4] == player){
                                h++;
                            }
                        }
                    }
                }else if(Board[i][j] == opponent){
                    if(j+3<SIZE && Board[i][j+1] == opponent && Board[i][j+2] == opponent && Board[i][j+3] == opponent){
                        h--;
                        if(j+4<SIZE && Board[i][j+4] == opponent){
                            h--;
                        } 
                    }
                    if(i+3<SIZE && Board[i+1][j] == opponent && Board[i+2][j] == opponent && Board[i+3][j] == opponent){
                        h--;
                        if(i+4<SIZE && Board[i+4][j] == opponent){
                            h--;
                        } 
                    }
                    if(i+3<SIZE && j+3<SIZE && Board[i+1][j+1] == opponent && Board[i+2][j+2] == opponent && Board[i+3][j+3] == opponent){
                        h--;                 
                        if(i+4<SIZE && j+4<SIZE && Board[i+4][j+4] == opponent){
                            h--;
                        }
                        
                    }
                    if(i+3<SIZE && j-3>=0 && Board[i+1][j-1] == opponent && Board[i+2][j-2] == opponent && Board[i+3][j-3] == opponent){
                        h--;
                        if(i+4<SIZE && j-4>=0 && Board[i+4][j-4] == opponent){
                            h--;
                        }   
                    }
                }
            }
        }
        return h;
    }
};

int Minimax(State state, int depth, int Alpha, int Beta, bool maximizingPlayer);

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

Point Next_Point(State &state){
    
    int best_score = Minimax(state, 2, NEG_INF, INF, false);
    //cout<<"MPPPPPPPPPPPPPPPPPPPPPPPPPPPPP\n";
    cout<<"best point ("<<state.best_point.x<<","<<state.best_point.y<<")\n";
    cout<<"Best ("<<Best.x<<","<<Best.y<<")\n";
    //cout<<state.best_point.x<<" "<<state.best_point.y<<"\n";
    //cout<<state.best_point.x<<" "<<state.best_point.y<<"\n";
    //cout<<state.best_point.x<<" "<<state.best_point.y<<"\n";
    //cout<<state.best_point.x<<" "<<state.best_point.y<<"\n";
    return Best;   
}


void write_valid_spot(std::ofstream& fout, State &state) {
    srand(time(NULL));
    int x, y;
    bool flag = false;
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            if(state.Board[i][j] == player){
                flag = true;
                break;
            }
        }
    }
    if(flag == true){ 
        Point next = Next_Point(state);
        fout<<next.x<<" "<<next.y<<"\n";
    }else{
        if(board[SIZE/2][SIZE/2] == 0) fout<<SIZE/2<<" "<<SIZE/2;
        else fout<<(SIZE/2)-1<<" "<<SIZE/2;
    }
    fout.flush();
}

int Minimax(State state, int depth, int Alpha, int Beta, bool maximizingPlayer){
    if(depth == 0){
        //return the score base on the board (no recursion) 
        return state.evaluate_score();   
    }
    state.next_move_enum();
    if(maximizingPlayer){
        int maxEval = NEG_INF;
        for(auto child : state.enum_move_point){
            State next = state;
            next.add_Point(child, 1);
            int eval = Minimax(next, depth - 1, Alpha, Beta, !maximizingPlayer);
            maxEval = max(eval, maxEval);
            if(eval == maxEval){
                state.best_point = child;
                Best = child;
            }
            //child.score = maxEval;
            Alpha = max(Alpha, eval);
            if(Beta <= Alpha) break; 
        }
        return maxEval;

    }else{
        int minEval = INF;
        for(Point child : state.enum_move_point){
            State next = state;
            next.add_Point(child, 2);
            int eval = Minimax(next, depth - 1, Alpha, Beta, maximizingPlayer);
            minEval = min(minEval, eval);
            if(eval == minEval){
                state.best_point = child;
                Best = child;
            }
            //child.score = minEval;
            Beta = min(Beta, eval);
            if(Beta <= Alpha) break;
        }
        return minEval;
    }
    return 0;
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);

    State init(board, player);

    write_valid_spot(fout, init);
    fin.close();
    fout.close();
    return 0;
}
