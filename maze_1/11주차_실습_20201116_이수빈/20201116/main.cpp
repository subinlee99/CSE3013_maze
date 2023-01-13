#include <iostream>
#include <stdio.h>
#include <time.h>
#include <vector>

using namespace::std;
void first_line(int w);
void last_line(int w, int h);
void even_frame(int w);
void odd_frame(int w);
void print_maze(int w, int h);
void print_maze_id(int w, int h);
void merge_down(int y, int w);
void merge_right(int y, int w, int h);
vector<vector<char> > maze;
vector<vector<int> > maze_id;
FILE *fp;

int id_count=1;
int main() {
    srand((unsigned int)time(NULL));
    
    int w,h;
    cin >> w >> h ;
   
    fp = fopen("maze124.maz", "w");
    if(!fp){
        cout << "file open failed\n";
        exit(1);
    }
    //make frames
    for(int i=0;i<2*h+1;i++){
        if(i%2==0)even_frame(w);
        else odd_frame(w);
    }
    //make maze
    for(int i=1;i<2*h;i+=2){
        if(i==1) first_line(w);
        else merge_right((i-1)/2, w, h);
    }
    last_line(w, h);
    //print_maze_id(w, h);
    print_maze(w, h);
    fclose(fp);
    return 0;
}

// init frame functions
void even_frame(int w){
    vector<char> tmp;
    for(int i=0;i<=2*w;i++){
        if(i%2==0) tmp.push_back('+');
        else tmp.push_back('-');
    }
    maze.push_back(tmp);
}
void odd_frame(int w){
    vector<char> tmp;
    for(int j=0;j<=2*w;j++) {
        if(j%2==0)tmp.push_back('|');
        else tmp.push_back(' ');
    }
    maze.push_back(tmp);
}
void print_maze_id(int w, int h){
    for (int i=0; i<h; i++){
        for(int j=0;j<w;j++){
            cout << maze_id[i][j];
        }
        cout << endl;
    }
}
void print_maze(int w, int h){
    for (int i=0; i<=2*h; i++){
        for(int j=0;j<=2*w;j++){
            //cout << maze[i][j];
            fprintf(fp,"%c",maze[i][j]);
        }
        fprintf(fp,"\n");
       // cout << endl;
    }
}

//make maze functions
void first_line(int w){
    //int count=1;
    vector<int> tmp;
    for(int j=2;j<2*w;j+=2){
        tmp.push_back(id_count);
        int rnd=rand()%2;
        if(rnd==0) {
            maze[1][j]=' ';
        }
        else id_count++;
        
    }
    tmp.push_back(id_count);
    maze_id.push_back(tmp);
    //for(int i=0;i<w;i++) cout << maze_id[0][i];
    //cout << endl;
    merge_down(0,w);
}

void merge_right(int y, int w, int h){
    int id; int prev_id;
    prev_id=maze_id[y][0];
    for(int j=2;j<2*w;j+=2){
        id=maze_id[y][j/2-1];
        int rnd=rand()%2;
        if(rnd==0) {
            if(maze_id[y][j/2-1] != maze_id[y-1][j/2]) //if left and above cell has same id -> do not merge
            {
                int old_id=id; int new_id=id;
                if(id < maze_id[y][j/2]) {
                    old_id=maze_id[y][j/2];
                    maze_id[y][j/2]=id;//merged ->same id as the left cell
                }
                else {
                    maze_id[y][j/2-1] = maze_id[y][j/2];
                    new_id=maze_id[y][j/2];
                }
                maze[2*y+1][j]=' ';
                
                for(int i=0;i<w;i++){
                    if(maze_id[y][i]==old_id) maze_id[y][i]=new_id;
                    if(maze_id[y-1][i]==old_id) maze_id[y-1][i]=new_id;
                }
            }
        }
        prev_id=id;
    }
    //
    
    if(y!=h-1)merge_down(y, w);
}

void merge_down(int y, int w){
    int id; int prev_num; int merge_flag=0;
    int id_down;
    vector<int> tmp;
    prev_num=maze_id[y][0];
    
    for(int i=0;i<w;i++){
        id=maze_id[y][i]; //id of currnet block
        
        if(prev_num != id && merge_flag==0) { //block id changed but no merge down -> merge last block
            maze[2*(y+1)][2*(i-1)+1]=' ';
            merge_flag=0;
            tmp.pop_back();
            tmp.push_back(prev_num);
            id_count--;
        }
        else if(prev_num != id) merge_flag=0;
        
        int rnd=rand()%2;
        if(rnd==0) { //randomly merge downward
            maze[2*(y+1)][2*(i)+1]=' ';
            merge_flag=1; //if merged -> merge_flag =1
            id_down=id;
        }
        else id_down=++id_count;
        
        tmp.push_back(id_down);
        prev_num=id;
    }
    
    if (merge_flag==0){
        maze[2*(y+1)][2*w-1]=' ';
        tmp.pop_back();
        tmp.push_back(prev_num);
        id_count--;
    }
    
    maze_id.push_back(tmp);
    tmp.clear();
    //for(int i=0;i<w;i++) cout << maze_id[y+1][i];
    //cout << endl;
}

void last_line(int w, int h){
    int id; int prev_id;
    prev_id=maze_id[h-1][0];
    for(int j=1;j<w;j++){
        id=maze_id[h-1][j];
        if(id!=prev_id){
            //int old_id=maze_id[h-1][j-1];
            for(int i=j-1;i<w;i++){
                if(maze_id[h-1][i]==prev_id) maze_id[h-1][i]=id;
            }
            maze[2*h-1][2*j]=' ';
        }
        prev_id =id;
    }
}

