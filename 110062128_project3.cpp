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

int evaluate_score(){
        int h = 0;
        //set<int,int> vis;
        int opponent = 3 - player;
        for(int i = 0; i<SIZE; i++){
            for(int j=0; j<SIZE; j++){
                //cout<<i<<" "<<j<<'\n';
                int flag1=0;
                if(Board[i][j] == player){ 
                    //_x_xx_
                       
                    if(j+1<SIZE && Board[i][j+1] == player){  
                        flag1++;
                        //_xx_x_ && _x_xx_
                        if(Board[i][j-1] == EMPTY){
                            if(Board[i][j+2] == EMPTY){
                               if(Board[i][j+3] == player){
                                    if(Board[i][j+4] == EMPTY){
                                        h+=2500;
                                    }
                               }
                               if(Board[i][j-2] == player){
                                    if(Board[i][j-3] == EMPTY){
                                        h+=2500;
                                    }
                               } 
                            }     
                        }
                        if(j+2<SIZE && Board[i][j+2] == player){
                            flag1++;
                            if(j+3<SIZE && Board[i][j+3] == player){
                                flag1++;
                                if(j+4<SIZE && Board[i][j+4] == player){
                                    flag1++;
                                    h+=50000;
                                }
                                else{
                                    int pt=0;
                                    if(j+4<SIZE && Board[i][j+4] == 0){
                                        pt++;
                                    }
                                    if(j-1>=0 && Board[i][j-1] == 0){
                                        pt++;
                                    }
                                    if(pt==2){
                                        h+=3000;
                                    }
                                    else if(pt==1){
                                        h+=500;
                                    }
                                    else if(pt==0){
                                        flag1-=3;
                                    }
                                }
                            }
                            else{
                                int pt=0;
                                if(j+3<SIZE && Board[i][j+3] == 0){
                                    pt++;
                                    
                                }
                                if(j-1>=0 &&Board[i][j-1] == 0){
                                    pt++;
                                }
                                if(pt==1){
                                    h+=100;
                                }
                                if(pt==2){
                                    h+=1500;
                                }
                                else if(pt==0){
                                    flag1-=2;
                                }
                            }
                        }
                        else{
                           int pt=0;
                           if(j+2<SIZE && Board[i][j+2] == 0){
                            pt++;      
                           }
                           if(j-1>=0 &&Board[i][j-1] == 0){
                            pt++;
                           }
                           if(pt==1){
                            h+=20;
                           }
                           else if(pt==2){
                            h+=100;
                           }
                           else if(pt==0){
                            flag1--;
                           }
                        }
                        
                    }
                    if(i+1<SIZE && Board[i+1][j] == player){  
                        flag1++;
                        if(Board[i-1][j] == EMPTY){
                            if(Board[i+2][j] == EMPTY){
                               if(Board[i+3][j] == player){
                                    if(Board[i+4][j] == EMPTY){
                                        h+=2500;
                                    }
                               }
                               if(Board[i-2][j] == player){
                                    if(Board[i-3][j] == EMPTY){
                                        h+=2500;
                                    }
                               } 
                            }     
                        }
                        if(i+2<SIZE && Board[i+2][j] == player){
                            flag1++;
                            if(i+3<SIZE && Board[i+3][j] == player){
                                flag1++;
                                if(i+4<SIZE && Board[i+4][j] == player){
                                    h+=50000;
                                    flag1++;
                                }
                                    else{
                                    int pt=0;
                                    if(i+4<SIZE && Board[i+4][j] == 0){
                                        pt++;
                                    }
                                    if(i-1>=0 && Board[i-1][j] == 0){
                                        pt++;
                                    }
                                    if(pt==2){
                                        h+=3000;
                                    }
                                    if(pt==1){
                                        h+=500;
                                    }
                                    if(pt==0){
                                        flag1-=3;
                                    }
                                }
                            }
                            else{
                                int pt=0;
                                if(i+3<SIZE && Board[i+3][j] == 0){
                                    pt++;
                                    
                                }
                                if(i-1>=0 &&Board[i-1][j] == 0){
                                    pt++;
                                }
                                if(pt==1){
                                    h+=100;
                                }
                                if(pt==2){
                                    h+=1500;
                                }
                                else if(pt==0){
                                    flag1-=2;
                                }
                            }
                        }
                        else{
                           int pt=0;
                           if(i+2<SIZE && Board[i+2][j] == 0){
                                    pt++;
                                    
                                }
                                if(i-1>=0 &&Board[i-1][j] == 0){
                                    pt++;
                                }
                                if(pt==1){
                                    h+=20;
                                }
                                if(pt==2){
                                    h+=100;
                                }
                                else if(pt==0){
                                    flag1--;
                                } 
                        }
                        
                    }
                    if(i+1<SIZE && j+1<SIZE && Board[i+1][j+1] == player){
                        flag1++;
                        if(i+2<SIZE && j+2<SIZE && Board[i+2][j+2] == player){
                            flag1++;
                            if(i+3<SIZE && j+3<SIZE && Board[i+3][j+3] == player){
                                flag1++;
                                if(i+4<SIZE && j+4<SIZE && Board[i+4][j+4] == player){
                                    h+=50000;
                                    flag1++;
                                }
                                    else
                                    {
                                    int pt=0;
                                    if(j+4<SIZE &&i+4<SIZE&& Board[i+4][j+4] == 0){
                                        pt++;
                                    }
                                    if(j-1>=0 && i-1>=0&&Board[i-1][j-1] == 0){
                                        pt++;
                                    }
                                    if(pt==2){
                                        h+=3000;
                                    }
                                    if(pt==1){
                                        h+=500;
                                    }
                                    if(pt==0){
                                        flag1-=3;
                                    }
                                }
                            }
                            else{
                                int pt=0;
                                if(j+3<SIZE &&i+3<SIZE&& Board[i+3][j+3] == 0){
                                    pt++;
                                    
                                }
                                if(j-1>=0 &&i-1>=0&&Board[i-1][j-1] == 0){
                                    pt++;
                                }
                                if(pt==1){
                                    h+=100;
                                }
                                if(pt==2){
                                    h+=1500;
                                }
                                else if(pt==0){
                                    flag1-=2;
                                }
                            }
                        }
                        else{
                            int pt=0;
                                if(j+2<SIZE &&i+2<SIZE&& Board[i+2][j+2] == 0){
                                    pt++;
                                    
                                }
                                if(j-1>=0 &&i-1>=0&&Board[i-1][j-1] == 0){
                                    pt++;
                                }
                                if(pt==1){
                                    h+=20;
                                }
                                if(pt==2){
                                    h+=100;
                                }
                                else if(pt==0){
                                    flag1--;
                                }
                        }
                        
                    }
                    if(i+1<SIZE && j-1>=0 && Board[i+1][j-1] == player){
                        flag1++;
                        if(i+2<SIZE && j-2>=0 && Board[i+2][j-2] == player){
                            flag1++;
                            if(i+3<SIZE && j-3>=0 && Board[i+3][j-3] == player){
                                flag1++;
                                if(i+4<SIZE && j-4>=0 && Board[i+4][j-4] == player){
                                    h+=50000;
                                    flag1++;
                                }
                                else
                                {
                                    int pt=0;
                                    if(j-4>=0 && i+4<SIZE && Board[i+4][j-4] == 0){
                                        pt++;
                                    }
                                    if(j+1<SIZE && i-1>=0 && Board[i-1][j+1] == 0){
                                        pt++;
                                    }
                                    if(pt==2){
                                        h+=3000;
                                    }
                                    if(pt==1){
                                        h+=500;
                                    }
                                    if(pt==0){
                                        flag1-=3;
                                    }
                                }
                            }
                            else{
                                int pt=0;
                                if(j-3>=0 &&i+3<SIZE&& Board[i+3][j-3] == 0){
                                    pt++;
                                    
                                }
                                if(j+1<SIZE &&i-1>=0&&Board[i-1][j+1] == 0){
                                    pt++;
                                }
                                if(pt==1){
                                    h+=100;
                                }
                                if(pt==2){
                                    h+=1500;
                                }
                                else if(pt==0){
                                    flag1-=2;
                                }
                            }
                        }
                        else{
                            int pt=0;
                                if(j-2>=0 &&i+2<SIZE&& Board[i+2][j-2] == 0){
                                    pt++;
                                    
                                }
                                if(j+1<SIZE &&i-1>=0&&Board[i-1][j+1] == 0){
                                    pt++;
                                }
                                if(pt==1){
                                    h+=20;
                                }
                                if(pt==2){
                                    h+=100;
                                }
                                else if(pt==0){
                                    flag1--;
                                }
                    }
                }
                }else if(Board[i][j] == opponent){
                    if(j+1<SIZE && Board[i][j+1] == opponent){  
                        flag1--;
                        //_xx_x_
                        if(Board[i][j-1] == EMPTY){
                            if(Board[i][j+2] == EMPTY){
                               if(Board[i][j+3] == player){
                                    if(Board[i][j+4] == EMPTY){
                                        h-=2500;
                                    }
                               }
                               if(Board[i][j-2] == player){
                                    if(Board[i][j-3] == EMPTY){
                                        h-=2500;
                                    }
                               } 
                            }     
                        }
                        if(j+2<SIZE && Board[i][j+2] == opponent){
                            flag1--;
                            if(j+3<SIZE && Board[i][j+3] == opponent){
                                flag1--;
                                if(j+4<SIZE && Board[i][j+4] == opponent){
                                    flag1--;
                                    h-=40000;
                                }
                                else{
                                    int pt=0;
                                    if(j+4<SIZE && Board[i][j+4] == 0){
                                        pt++;
                                    }
                                    if(j-1>=0 && Board[i][j-1] == 0){
                                        pt++;
                                        
                                    }
                                    if(pt==2){
                                        h-=2500;
                                    }
                                    if(pt==1){
                                        h-=400;
                                    }
                                    if(pt==0){
                                        flag1+=3;
                                    }
                                }
                            }
                            else{
                                int pt=0;
                                if(j+3<SIZE && Board[i][j+3] == 0){
                                    pt++;
                                    
                                }
                                if(j-1>=0 &&Board[i][j-1] == 0){
                                    pt++;
                                    
                                }
                                if(pt==1){
                                    //cout<<"attntion";
                                    h-=80;
                                }
                                if(pt==2){
                                    h-=2000;
                                }
                                else if(pt==0){
                                    flag1+=2;
                                }
                            }
                        }
                        else{
                            int pt=0;
                                if(j+2<SIZE && Board[i][j+2] == 0){
                                    pt++;
                                    
                                }
                                if(j-1>=0 &&Board[i][j-1] == 0){
                                    pt++;
                                }
                                if(pt==1){
                                    h-=10;
                                }
                                if(pt==2){
                                    h-=60;
                                }
                                else if(pt==0){
                                    flag1++;
                                }
                        }
                    }
                    if(i+1<SIZE && Board[i+1][j] == opponent){  
                        flag1--;
                        if(Board[i-1][j] == EMPTY){
                            if(Board[i+2][j] == EMPTY){
                               if(Board[i+3][j] == player){
                                    if(Board[i+4][j] == EMPTY){
                                        h-=2500;
                                    }
                               }
                               if(Board[i-2][j] == player){
                                    if(Board[i-3][j] == EMPTY){
                                        h-=2500;
                                    }
                               } 
                            }     
                        }
                        if(i+2<SIZE && Board[i+2][j] == opponent){
                            h-=60;
                            flag1--;
                            if(i+3<SIZE && Board[i+3][j] == opponent){
                                h-=200;
                                flag1--;
                                if(i+4<SIZE && Board[i+4][j] == opponent){
                                    flag1--;
                                    h-=40000;
                                    //cout<<"NOOO";
                                }
                                else{
                                    int pt=0;
                                    if(i+4<SIZE && Board[i+4][j] == 0){
                                        pt++;
                                    }
                                    if(i-1>=0 && Board[i-1][j] == 0){
                                        pt++;
                                    }
                                    if(pt==2){
                                        h-=2500;
                                    }
                                    if(pt==1){
                                        h-=400;
                                    }
                                    if(pt==0){
                                        flag1+=3;
                                    }
                                }
                            }
                            else{
                                int pt=0;
                                if(i+3<SIZE && Board[i+3][j] == 0){
                                    pt++;   
                                }
                                if(i-1>=0 &&Board[i-1][j] == 0){
                                    pt++;
                                }
                                if(pt==1){
                                    h-=80;
                                }
                                if(pt==2){
                                    h-=2000;
                                    //cout<<"A";
                                }
                                else if(pt==0){
                                    flag1+=2;
                                }
                            }
                        }
                        else{
                        int pt=0;
                                if(i+2<SIZE && Board[i+2][j] == 0){
                                    pt++;
                                }
                                if(i-1>=0 &&Board[i-1][j] == 0){
                                    pt++;
                                }
                                if(pt==1){
                                    h-=10;
                                }
                                if(pt==2){
                                    h-=60;
                                }
                                else if(pt==0){
                                    flag1++;
                                }
                        }
                        
                    }
                    if(i+1<SIZE && j+1<SIZE && Board[i+1][j+1] == opponent){
                        flag1--; 
                        if(i+2<SIZE && j+2<SIZE && Board[i+2][j+2] == opponent){
                            flag1--;
                            if(i+3<SIZE && j+3<SIZE && Board[i+3][j+3] == opponent){
                                flag1--;
                                if(i+4<SIZE && j+4<SIZE && Board[i+4][j+4] == opponent){
                                    flag1--;
                                    h-=40000;
                                }
                                    else{
                                    int pt=0;
                                    if(j+4<SIZE &&i+4<SIZE&& Board[i+4][j+4] == 0){
                                        pt++;
                                    }
                                    if(j-1>=0 && i-1>=0&&Board[i-1][j-1] == 0){
                                        pt++;
                                    }
                                    if(pt==2){
                                        h-=2500;
                                    }
                                    if(pt==1){
                                        h-=400;
                                    }
                                    if(pt==0){
                                        flag1+=3;
                                    }
                                }
                            }
                            else{
                                int pt=0;
                                if(j+3<SIZE &&i+3<SIZE&& Board[i+3][j+3] == 0){
                                    pt++; 
                                }
                                if(j-1>=0 &&i-1>=0&&Board[i-1][j-1] == 0){
                                    pt++;
                                }
                                if(pt==1){
                                    h-=80;
                                }
                                if(pt==2){
                                    h-=2000;
                                    //cout<<"A";
                                }
                                else{
                                    flag1+=2;
                                }
                            }
                        }
                        else{
                            int pt=0;
                                if(j+2<SIZE &&i+2<SIZE&& Board[i+2][j+2] == 0){
                                    pt++;
                                }
                                if(j-1>=0 && i-1>=0&&Board[i-1][j-1] == 0){
                                    pt++;
                                }
                                if(pt==2){
                                    h-=10;
                                }
                                else if(pt==1){
                                    h-=60;
                                }
                                else if(pt==0){
                                    flag1++;
                                }
                        }
                    }
                    if(i+1<SIZE && j-1>=0 && Board[i+1][j-1] == opponent){
                        flag1--;
                        if(i+2<SIZE && j-2>=0 && Board[i+2][j-2] == opponent){
                            flag1--;
                            if(i+3<SIZE && j-3>=0 && Board[i+3][j-3] == opponent){
                                flag1--;
                                h-=200;
                                if(i+4<SIZE && j-4>=0 && Board[i+4][j-4] == opponent){
                                    flag1--;
                                    h-=40000;
                                }
                                else{
                                    int pt=0;
                                    if(j-4>=0 && i+4<SIZE && Board[i+4][j-4] == 0){
                                        pt++;
                                    }
                                    if(j+1<SIZE && i-1>=0 && Board[i-1][j+1] == 0){
                                        pt++;
                                    }
                                    if(pt==2){
                                        h-=2500;
                                    }
                                    if(pt==1){
                                        h-=400;
                                    }
                                    if(pt==0){
                                        flag1+=3;
                                    }
                                }
                            }
                            else
                            {
                                int pt=0;
                                if(j-3>=0 && i+3<SIZE&& Board[i+3][j-3] == 0){
                                    pt++;
                                    
                                }
                                if(j+1<SIZE && i-1>=0&&Board[i-1][j+1] == 0){
                                    pt++;
                                }
                                if(pt==1){
                                    h-=80;
                                }
                                if(pt==2){
                                    h-=2000;
                                    //cout<<"A";
                                }
                                else if(pt==0){
                                    flag1+=2;
                                }
                            }
                        }
                        else{
                            int pt=0;
                                if(j-2>=0 &&i+2<SIZE&& Board[i+2][j-2] == 0){
                                    pt++;
                                    
                                }
                                if(j+1<SIZE &&i-1>=0&&Board[i-1][j+1] == 0){
                                    pt++;
                                }
                                if(pt==1){
                                    h-=10;
                                }
                                if(pt==2){
                                    h-=60;
                                }
                                else if(pt==0){
                                    flag1++;
                                }
                        }
                        
                    }
                }
                if(flag1>1){
                   h+=flag1*50; 
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
        return state.evaluate_score();   
    }
    
    if(maximizingPlayer){
        state.next_move_enum();
        int maxEval = NEG_INF;
        for(auto child : state.enum_move_point){
            State next = state;
            next.add_Point(child, player);
            int eval = Minimax(next, depth - 1, Alpha, Beta, false);
            if(eval > maxEval){
                //state.best_point = child;
                Best = child;
            }
            maxEval = max(eval, maxEval);
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
            
            int eval = Minimax(next, depth - 1, Alpha, Beta, true);
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

    State init(board, player);
    
    write_valid_spot(fout, init);
    fin.close();
    fout.close();
    return 0;
}
