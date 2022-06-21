#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include<set>
#include<limits.h>
#include <array>
#include<queue>
using namespace std;

#define INF INT_MAX
#define NEG_INF -INT_MAX

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
    State(std::array<std::array<int, SIZE>, SIZE>b){
        for(int i=0;i<SIZE;i++){
            for(int j=0;j<SIZE;j++){
                this->Board[i][j] = b[i][j];
            }
        }
    }
    State(State& copy){
        for(int i=0;i<SIZE;i++){
            for(int j=0;j<SIZE;j++){
                this->Board[i][j] = copy.Board[i][j];
            }
        }
        //this->player = copy.player;
    }

    int exam_jugongge(int i, int j,std::array<std::array<int, SIZE>, SIZE> gomoku){
        if(i>0 && i<SIZE-1){
            if(j>0 && j<SIZE-1){
                if(gomoku[i-1][j-1]>0 || gomoku[i-1][j]>0 || gomoku[i-1][j+1]>0 || gomoku[i][j-1]>0 || gomoku[i][j+1]>0 || gomoku[i+1][j-1]>0 || gomoku[i+1][j]>0 || gomoku[i+1][j+1]>0)
                    return true;
            }
            else if(j==0){
                if(gomoku[i-1][j]>0 || gomoku[i-1][j+1]>0 || gomoku[i][j+1]>0 || gomoku[i+1][j]>0 || gomoku[i+1][j+1]>0) return true;
            }
            else{
                if(gomoku[i-1][j]>0 || gomoku[i-1][j-1]>0 || gomoku[i+1][j]>0 || gomoku[i][j-1]>0 || gomoku[i+1][j-1]>0) return true;
            }
        }
        else if(i == 0){
            if(j>0 && j<SIZE-1){
                if(gomoku[i][j-1]>0 || gomoku[i][j+1]>0 || gomoku[i+1][j-1]>0 || gomoku[i+1][j]>0 || gomoku[i][j+1]>0)return true;
            }
            else if(j==0){
                if(gomoku[i+1][j]>0 || gomoku[i][j+1]>0 || gomoku[i+1][j+1]>0) return true;
            }
            else{
                if(gomoku[i+1][j]>0 || gomoku[i][j-1]>0 || gomoku[i+1][j-1]>0) return true;
            }
        }
        else{
            if(j>0 && j<SIZE-1){
               if(gomoku[i-1][j-1]>0 ||gomoku[i][j-1]>0  || gomoku[i-1][j]>0 || gomoku[i-1][j+1]>0 || gomoku[i][j+1]>0) return true;
            }
            else if(j==0){
                if(gomoku[i-1][j+1]>0 || gomoku[i-1][j]>0 || gomoku[i][j+1]>0)return true;
            }
            else{
                if(gomoku[i-1][j-1]>0 || gomoku[i][j-1]>0  || gomoku[i-1][j]>0)return true;
            }
        }
        return false;
    }

    void next_move_enum(){
        //scan the whole board
        for(int i=0;i<SIZE;i++){
            for(int j=0;j<SIZE;j++){
                if(Board[i][j] == EMPTY){
                    //check Jiugongge
                    
                    if(exam_jugongge(i,j,board) == 1){
                        this->enum_move_point.insert(Point(i,j));
                    }
                }
            }
        }
        return;
    }

    void add_Point(Point point, int disc){
        Board[point.x][point.y] = disc;
    }
    void remove_Point(Point point){
        Board[point.x][point.y] = EMPTY;
    }

int evaluate_score(int who){
    int N5=0, open4=0, half4=0, open3=0, half3=0, open2=0, half2=0, special1=0;
    //int h = 0;
    
    int the_other = 3-who;
      for(int i=0;i<SIZE;i++){ 
        for(int j=0;j<SIZE;j++){
            if(Board[i][j] == who){

                //FIVE_IN_ROW ooooo
                if(Board[i][j+1]==who && Board[i][j+2]==who && Board[i][j+3]==who && Board[i][j+4]==who){
                    N5++;
                }
                if(Board[i+1][j]==who && Board[i+2][j]==who && Board[i+3][j]==who && Board[i+4][j]==who){
                    N5++;
                }
                if(Board[i+1][j+1]==who && Board[i+2][j+2]==who && Board[i+3][j+3]==who && Board[i+4][j+4]==who){
                    N5++;
                }
                if(Board[i+1][j-1]==who && Board[i+2][j-2]==who && Board[i+3][j-3]==who && Board[i+4][j-4]==who){
                    N5++;
                }

                //LIVE_FOUR _oooo_
                if(Board[i][j-1] == EMPTY && Board[i][j+1] == who && Board[i][j+2] == who && Board[i][j+3] == who && Board[i][j+4] == EMPTY){
                    open4++;
                }
                if(Board[i-1][j] == EMPTY && Board[i+1][j] == who && Board[i+2][j] == who && Board[i+3][j] == who && Board[i+4][j] == EMPTY){
                    open4++;
                    
                }
                if(Board[i-1][j-1] == EMPTY && Board[i+1][j+1] == who && Board[i+2][j+2] == who && Board[i+3][j+3] == who && Board[i+4][j+4] == EMPTY){
                    open4++;
                }
                if(Board[i-1][j+1] == EMPTY && Board[i+1][j-1] == who && Board[i+2][j-2] == who && Board[i+3][j-3] == who && Board[i+4][j-4] == EMPTY){
                    open4++;
                }

                //LIVE_THREE _ooo_
                if(Board[i][j-1] == EMPTY && Board[i][j+1] == who && Board[i][j+2] == who && Board[i][j+3] == EMPTY){
                    open3++;
                }
                if(Board[i-1][j] == EMPTY && Board[i+1][j] == who && Board[i+2][j] == who && Board[i+3][j] == EMPTY){
                    open3++;
                }
                if(Board[i-1][j-1] == EMPTY && Board[i+1][j+1] == who && Board[i+2][j+2] == who && Board[i+3][j+3] == EMPTY){
                    open3++;
                }
                if(Board[i-1][j+1] == EMPTY && Board[i+1][j-1] == who && Board[i+2][j-2] == who && Board[i+3][j-3] == EMPTY){
                    open3++;
                }
                //DEADFOUR _oooox || xoooo_
                if(Board[i][j-1] == EMPTY && Board[i][j+1] == who && Board[i][j+2] == who && Board[i][j+3] == who && Board[i][j+4] == the_other){
                    half4++;
                }
                if(Board[i-1][j] == EMPTY && Board[i+1][j] == who && Board[i+2][j] == who && Board[i+3][j] == who && Board[i+4][j] == the_other){
                    half4++;
                }
                if(Board[i-1][j-1] == EMPTY && Board[i+1][j+1] == who && Board[i+2][j+2] == who && Board[i+3][j+3] == who && Board[i+4][j+4] == the_other){
                    half4++;
                }
                if(Board[i-1][j+1] == EMPTY && Board[i+1][j-1] == who && Board[i+2][j-2] == who && Board[i+3][j-3] == who && Board[i+4][j-4] == the_other){
                    half4++;
                }
                
                // OR

                if(Board[i][j-1] == the_other && Board[i][j+1] == who && Board[i][j+2] == who && Board[i][j+3] == who && Board[i][j+4] == EMPTY){
                    half4++;
                }
                if(Board[i-1][j] == the_other && Board[i+1][j] == who && Board[i+2][j] == who && Board[i+3][j] == who && Board[i+4][j] == EMPTY){
                    half4++;
                }
                if(Board[i-1][j-1] == the_other && Board[i+1][j+1] == who && Board[i+2][j+2] == who && Board[i+3][j+3] == who && Board[i+4][j+4] == EMPTY){
                    half4++;
                }
                if(Board[i-1][j+1] == the_other && Board[i+1][j-1] == who && Board[i+2][j-2] == who && Board[i+3][j-3] == who && Board[i+4][j-4] == EMPTY){
                    half4++;
                }
                //DEAD_THREE _ooox || xooo_
                if(Board[i][j-1] == EMPTY && Board[i][j+1] == who && Board[i][j+2] == who && Board[i][j+3] == the_other){
                    half3++;
                }
                if(Board[i-1][j] == EMPTY && Board[i+1][j] == who && Board[i+2][j] == who && Board[i+3][j] == the_other){
                    half3++;
                   
                }
                if(Board[i-1][j-1] == EMPTY && Board[i+1][j+1] == who && Board[i+2][j+2] == who && Board[i+3][j+3] == the_other){
                    half3++;
                }
                if(Board[i-1][j+1] == EMPTY && Board[i+1][j-1] == who && Board[i+2][j-2] == who && Board[i+3][j-3] == the_other){
                    half3++;
                }

                //OR 

                if(Board[i][j-1] == the_other && Board[i][j+1] == who && Board[i][j+2] == who && Board[i][j+3] == EMPTY){
                    half3++;
                }
                if(Board[i-1][j] == the_other && Board[i+1][j] == who && Board[i+2][j] == who && Board[i+3][j] == EMPTY){
                    half3++;
                }
                if(Board[i-1][j-1] == the_other && Board[i+1][j+1] == who && Board[i+2][j+2] == who && Board[i+3][j+3] == EMPTY){
                    half3++;
                }
                if(Board[i-1][j+1] == the_other && Board[i+1][j-1] == who && Board[i+2][j-2] == who && Board[i+3][j-3] == EMPTY){
                    half3++;
                }
                //DEAD_TWO _oox || xoo_
                if(Board[i][j-1] == EMPTY && Board[i][j+1] == who && Board[i][j+2] == the_other){
                    half2++;
                }
                if(Board[i-1][j] == EMPTY && Board[i+1][j] == who && Board[i+2][j] == the_other){
                    half2++;
                }
                if(Board[i-1][j-1] == EMPTY && Board[i+1][j+1] == who && Board[i+2][j+2] == the_other){
                    half2++;
                }
                if(Board[i-1][j+1] == EMPTY && Board[i+1][j-1] == who && Board[i+2][j-2] == the_other){
                    half2++;
                }

                // OR

                if(Board[i][j-1] == the_other && Board[i][j+1] == who && Board[i][j+2] == EMPTY){
                    half2++;;
                }
                if(Board[i-1][j] == the_other && Board[i+1][j] == who && Board[i+2][j] == EMPTY){
                    half2++;
                }
                if(Board[i-1][j-1] == the_other && Board[i+1][j+1] == who && Board[i+2][j+2] == EMPTY){
                    half2++;
                }
                if(Board[i-1][j+1] == the_other && Board[i+1][j-1] == who && Board[i+2][j-2] == EMPTY){
                    half2++;
                }
                //LIVE_TWO _oo_
                if(Board[i][j-1] == the_other && Board[i][j+1] == who && Board[i][j+2] == EMPTY){
                    open2++;
                }
                if(Board[i-1][j] == EMPTY && Board[i+1][j] == who && Board[i+2][j] == EMPTY){
                    open2++;
                }
                if(Board[i-1][j-1] == EMPTY && Board[i+1][j+1] == who && Board[i+2][j+2] == EMPTY){
                    open2++;
                }
                if(Board[i-1][j+1] == EMPTY && Board[i+1][j-1] == who && Board[i+2][j-2] == EMPTY){
                    open2++;
                } 

                //_oo_o_ || _o_oo_
                if(Board[i][j-1] == EMPTY && Board[i][j+1] == who && Board[i][j+2] == EMPTY && Board[i][j+3] == who && Board[i][j+4] == EMPTY){
                    special1++;
                }
                if(Board[i-1][j] == EMPTY && Board[i+1][j] == EMPTY && Board[i+2][j] == who && Board[i+3][j] == who && Board[i+4][j] == EMPTY){
                    special1++;
                }
                if(Board[i-1][j-1] == EMPTY && Board[i+1][j+1] == EMPTY && Board[i+2][j+2] == who && Board[i+3][j+3] == who && Board[i+4][j+4] == EMPTY){
                    special1++;
                }
                if(Board[i-1][j+1] == EMPTY && Board[i+1][j-1] == EMPTY && Board[i+2][j-2] == who && Board[i+3][j-3] == who && Board[i+4][j-4] == EMPTY){
                    special1++;
                }

                //OR

                if(Board[i][j-1] == EMPTY && Board[i][j+1] == EMPTY && Board[i][j+2] == who && Board[i][j+3] == who && Board[i][j+4] == EMPTY){
                    special1++;
                }
                if(Board[i-1][j] == EMPTY && Board[i+1][j] == EMPTY && Board[i+2][j] == who && Board[i+3][j] == who && Board[i+4][j] == EMPTY){
                    special1++;
                }
                if(Board[i-1][j-1] == EMPTY && Board[i+1][j+1] == EMPTY && Board[i+2][j+2] == who && Board[i+3][j+3] == who && Board[i+4][j+4] == EMPTY){
                    special1++;
                }
                if(Board[i-1][j+1] == EMPTY && Board[i+1][j-1] == EMPTY && Board[i+2][j-2] == who && Board[i+3][j-3] == who && Board[i+4][j-4] == EMPTY){
                    special1++;
                }
                
            }
        }
      }
      int g;
      if(who == player)
        g = 50000*N5 + 4800*open4 + 2500*half4 + 800*special1 + 2000*open3 + 200*half3 + 50*open2 + 10*half2;
      else 
        g = 55000*N5 + 5500*open4 + 3000*half4 + 1000*special1 + 2000*open3 + 200*half3 + 50*open2 + 10*half2; 
      return g;
    }      
};




int Minimax(State state, int depth, int Alpha, int Beta, bool maximizingPlayer, bool flag);

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

Point Next_Point(State &state){
    
    int tmp_score = Minimax(state, 3, NEG_INF, INF, true, true);
    
    state.next_move_enum();
    
    for(Point child : state.enum_move_point){
        State next = state;
        next.add_Point(child, player);
        int score = next.evaluate_score(player);
        int opp_score = next.evaluate_score(3-player);
        if(score>550000 || opp_score>550000){
            Best = child;
        }
        state.remove_Point(child);
    }
    
    return Best ;   
}


void write_valid_spot(std::ofstream& fout, State &state) {
    srand(time(NULL));
    //int x, y;
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
        else fout<<(SIZE/2)+1<<" "<<SIZE/2+1;
    }
    fout.flush();
}

int Minimax(State state, int depth, int Alpha, int Beta, bool maximizingPlayer, bool flag){
    if(depth == 0){
        //return the score base on the board (no recursion) 
        return state.evaluate_score(player) - state.evaluate_score(3-player);   
    }
    
    if(maximizingPlayer){
        state.next_move_enum();
        int maxEval = NEG_INF;
        for(auto child : state.enum_move_point){
            State next = state;
            next.add_Point(child, player);
            int eval = Minimax(next, depth - 1, Alpha, Beta, false, false);
            next.remove_Point(child);
            if(eval > maxEval && flag == true){
                Best = child;
                Best.score = eval;
            }
            maxEval = max(maxEval, eval);
            Alpha = max(Alpha, maxEval);
            if(Beta <= Alpha) break; 
        }
        return maxEval;
    }else{
        state.next_move_enum();
        int minEval = INF;
        for(Point child : state.enum_move_point){
            State next = state;
            next.add_Point(child, 3-player);
            int eval = Minimax(next, depth - 1, Alpha, Beta, true, false);
            next.remove_Point(child);
            if(eval < minEval){
            }
            minEval = min(minEval, eval);
            Beta = min(Beta, minEval);
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

    State init(board);
    
    write_valid_spot(fout, init);
    fin.close();
    fout.close();
    return 0;
}