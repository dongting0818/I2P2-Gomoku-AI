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

// //=====================================================
// //SCORE BOARD PLY
// #define FIVEINROW_PLY 1000000  //  ooooo
// #define LIVEFOUR_PLY  30000   //  _oooo_
// #define LIVETHREE_PLY 15000   //  _ooo_
// #define DEADFOUR_PLY  1000    // _oooox
// #define DEADTHREE_PLY 800     // _ooox
// #define DEADTWO_PLY   200     // _oox   
// #define LIVETWO_PLY   800     // _oo_
// #define LIVEONE_PLY   50      // _o_
// //=====================================================
// //SCORE BOARD OPPO
// #define FIVEINROW_OPPO  1000000  //  xxxxx
// #define LIVEFOUR_OPPO   550000   //  _xxxx_
// #define LIVETHREE_OPPO  25000   //  _xxx_
// #define DEADFOUR_OPPO   55000    // _xxxxo
// #define DEADTHREE_OPPO  800     // _xxxo
// #define DEADTWO_OPPO    200     // _xxo  
// #define LIVETWO_OPPO    900     // _xx_
// #define LIVEONE_OPPO    50      // _x_
// //======================================================

//=====================================================
//SCORE BOARD PLY
#define FIVEINROW_PLY 50000000  //  ooooo
#define LIVEFOUR_PLY  1000000   //  _oooo_
#define LIVETHREE_PLY 50000   //  _ooo_
#define DEADFOUR_PLY  100    // _oooox
#define DEADTHREE_PLY 100     // _ooox
#define DEADTWO_PLY   10     // _oox   
#define LIVETWO_PLY   100     // _oo_
#define LIVEONE_PLY   5      // _o_
//=====================================================
//SCORE BOARD OPPO
#define FIVEINROW_OPPO  5000000  //  xxxxx
#define LIVEFOUR_OPPO   6000000   //  _xxxx_
#define LIVETHREE_OPPO  1000000   //  _xxx_
#define DEADFOUR_OPPO   5000000    // _xxxxo
#define DEADTHREE_OPPO  100     // _xxxo
#define DEADTWO_OPPO    200     // _xxo  
#define LIVETWO_OPPO    120     // _xx_
#define LIVEONE_OPPO    5     // _x_
//======================================================

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
        //  int num=SIZE/2;
        //  int num1=(r.x-num)*(r.x-num)+(r.y-num)*(r.y-num);
        //  int num2=(x-num)*(x-num)+(y-num)*(y-num);
        //  if(num1!=num2){
        //      return num1<num2;
        // }
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
    void remove_Point(Point point){
        Board[point.x][point.y] = EMPTY;
    }
int check(int i , int j){
    if(Board[i-1][j-1] != player && Board[i-1][j]!= player && Board[i-1][j+1]!= player && Board[i][j-1]!= player && Board[i][j]!= player
        &&Board[i][j+1] != player && Board[i+1][j-1]!= player && Board[i+1][j] != player && Board[i+1][j+1]!= player
        &&Board[i-1][j-1] != 3-player && Board[i-1][j]!= 3-player && Board[i-1][j+1]!= 3-player && Board[i][j-1]!= 3-player && Board[i][j]!= 3-player
        &&Board[i][j+1] != 3-player && Board[i+1][j-1]!= 3-player && Board[i+1][j] != 3-player && Board[i+1][j+1]!=3-player){
            cout<<"in";
            return 0;
        }
    return 1;
}
int evaluate_score(int who){
    int N5=0, open4=0, half4=0, open3=0, half3=0, open2=0, half2=0;
    int h = 0;
    
    int the_other = 3-who;
      for(int i=0;i<SIZE;i++){
        
        for(int j=0;j<SIZE;j++){
            if(check(i,j) == 0){
                continue;
            }
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
                //LIVE_ONE _o_
                if(Board[i][j-1] == EMPTY && Board[i][j+1] == EMPTY){
                    //if(who == player) h+=LIVEONE_PLY;
                    //else h+=LIVEONE_OPPO;
                }
                if(Board[i-1][j] == EMPTY && Board[i+1][j] == EMPTY){
                    //if(who == player) h+=LIVEONE_PLY;
                    //else h+=LIVEONE_OPPO;
                }
                if(Board[i-1][j-1] == EMPTY && Board[i+1][j+1] == EMPTY){
                    //if(who == player) h+=LIVEONE_PLY;
                    //else h+=LIVEONE_OPPO;
                }
                if(Board[i-1][j+1] == EMPTY && Board[i+1][j-1] == EMPTY){
                    //if(who == player) h+=LIVEONE_PLY;
                    //else h+=LIVEONE_OPPO;
                }
            }
        }
      }
      //cout<<"KOKO";
      int g = 5000*N5 + 4800*open4 + 2500*half4 + 2000*open3 + 200*half3 + 50*open2 + 10*half2; 
      return g;
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
    
    int best_score = Minimax(state, 2, NEG_INF, INF, true);
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
        return state.evaluate_score(player) - state.evaluate_score(3-player);   
    }
    
    if(maximizingPlayer){
        state.next_move_enum();
        int maxEval = NEG_INF;
        for(auto child : state.enum_move_point){
            State next = state;
            next.add_Point(child, player);
            int eval = Minimax(next, depth - 1, Alpha, Beta, false);
            //cout<<"point: ("<<child.x<<","<<child.y<<") "<<"eval: "<<eval<<"\n";
            next.remove_Point(child);
            if(eval > maxEval){
                //state.best_point = child;
                Best = child;
            }
            maxEval = max(maxEval, eval);
            Alpha = max(Alpha, eval);
            if(Beta <= Alpha) break; 
        }
        return maxEval;

    }else{
        state.next_move_enum();
        int minEval = INF;
        for(Point child : state.enum_move_point){
            State next = state;
            next.add_Point(child, 3-player);
            int eval = Minimax(next, depth - 1, Alpha, Beta, true);
            next.remove_Point(child);
            if(eval < minEval){
            }
            minEval = min(minEval, eval);
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
