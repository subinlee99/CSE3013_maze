#include "ofApp.h"
#include <string>
#include <vector>
#include <stack>
using namespace std;

#define SIZE 20
vector<vector<int>> maze_check;
int WIDTH, HEIGHT;

int start_flag=1;
int draw_flag=0;
int dfs_flag=0;
int bfs_flag=0;
vector<Pair> wall;
//--------------------------------------------------------------
void ofApp::setup(){
 
    ofSetBackgroundColor(255, 255, 255);

    ofSetLineWidth(5);
    Pair w1; w1.x=0; w1.y=1;
    wall.push_back(w1);
    Pair w2; w2.x=0; w2.y=-1;
    wall.push_back(w2);
    Pair w3; w3.x=1; w3.y=0;
    wall.push_back(w3);
    Pair w4; w4.x=-1; w4.y=0;
    wall.push_back(w4);
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(start_flag){
        ofSetColor(100);
        ofDrawBitmapString("Option 'l' : choose  maze file ", 100, 100);
        ofDrawBitmapString("Option 'd' : Do DFS (only after choosing the file) ", 100, 200);
        ofDrawBitmapString("Option 'b' : Do BFS (only after choosing the file) ", 100, 300);
    }
    if(draw_flag==1){
        ofSetColor(100);
        ofSetLineWidth(5);
            for(int i=0;i<maze.size();i++){

                for(int j=0;j<maze[i].size();j++){
                    if (maze[i][j] == '-'){
                        ofDrawLine(SIZE*(j-1), SIZE*i, SIZE*(j+1), SIZE*i);
                                    }
                    else if (maze[i][j] == '|'){
                        ofDrawLine(SIZE*j, SIZE*(i-1), SIZE*j, SIZE*(i+1));
                    }
                    else if (maze[i][j]== ' '){
                        maze_check[i][j]=0;
                    }
                }
        }
        
        if(dfs_flag==1){
            ofSetColor(255, 0, 0);
            for(int i=0; i<all_path.size();i++){
                allpath.addVertex(SIZE*all_path.top().x, SIZE*all_path.top().y);
                all_path.pop();
            }
            allpath.draw();
            
            ofSetColor(0, 255, 0);
            for(int i=0; i<s.size(); i++){
                path.addVertex(SIZE*s.top().x, SIZE*s.top().y);
                s.pop();
            }
            path.draw();
            ofSetColor(255,100,150);
            ofDrawRectangle(SIZE*(0.5), SIZE*(0.5), SIZE,SIZE);
            ofSetColor(100,150,255);
            ofDrawRectangle(SIZE*(2*WIDTH-1-0.5), SIZE*(2*HEIGHT-1-0.5), SIZE,SIZE);
            
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //cout << key << endl;
    if (key=='l'){
        start_flag=0;
        readFile();
        draw_flag=1;
    }
    if (key=='d'){
        if(draw_flag==1){
            dfs_flag=1;
            int iterate=s.size();
            if(DFS()) path.draw();
        }
        
    }
    if (key=='b') bfs_flag=1;
    

    if (key=='q'){
        freeMemory();
        readFile();
        
    }
}


//--------------------------------------------------------------
bool ofApp::readFile() {
    freeMemory();
    ofFileDialogResult openFileResult=ofSystemLoadDialog("Select .maz file");
    string filePath;
    if (openFileResult.bSuccess){
        int ith=0;
        string line;
        ofLogVerbose("User selected a file");
        string fileName = openFileResult.getName();

        filePath = openFileResult.getPath();
        printf("Open\n");
        
        ifstream input_file, input_file2;
        input_file.open(filePath);
        input_file2.open(filePath);
        
        if(input_file.is_open()){
            int count=0;
            while (getline(input_file, line)) {
                WIDTH = (line.length()-1)/2;
                count++;
            }
            HEIGHT = (count-1)/2;;
        
            string tmp;
            
            for(int i=0; i<count; i++){ //count = HEIGHT*2+1
                getline(input_file2, tmp);
                vector<char> cur;
                for(int j=0; j<WIDTH*2+1; j++) cur.push_back(tmp[j]);
                maze.push_back(cur);
            }
            
            for(int i=0;i<HEIGHT*2+1;i++){
                vector<int> tmp;
                for (int j=0;j<WIDTH*2+1;j++)tmp.push_back(1);
                maze_check.push_back(tmp);

            }
        }
        
        else{
            cout << "Failed to open file" << endl;
            draw_flag=0;
            return false;
        }
        
    }

}

/*week 3*/


bool ofApp::DFS(){
    
    draw_flag=1; dfs_flag=1;
    int h=HEIGHT*2+1;
    int w=WIDTH*2+1;

    Pair start; Pair u;
    
    //start point;
    start.x=1; start.y=1;
    maze_check[1][1]=1;

    int check;
    
    s.push(start);
    all_path.push(start);
    
    while(!s.empty()){
        all_path.push(s.top());
        if(s.top().x == w-2 && s.top().y == h-2) return true;
        check=0;
        for(int i=0; i<4; i++){
            u.y = s.top().y + wall[i].y;
            u.x = s.top().x + wall[i].x;
             if(!maze_check[u.y][u.x]){
                all_path.push(u);

                maze_check[ s.top().y + wall[i].y][ s.top().x + wall[i].x] = 1;
                s.push(u);
                check=1;
                break;
            }
        }
        if(!check) s.pop();
    }
}

//--------------------------------------------------------------
void ofApp::freeMemory(){
    for (int k = 0; k < maze.size(); ++k){
        maze[k].clear();
        maze_check[k].clear();
        maze[k].shrink_to_fit();
        maze_check[k].shrink_to_fit();
    }
    maze.clear();
    maze.shrink_to_fit();
    maze_check.clear();
    maze_check.shrink_to_fit();
    
  
    while(!s.empty())
        s.pop();
    while(!s_path.empty())
        s_path.pop();
    while(!all_path.empty())
        all_path.pop();
    
    draw_flag=0;
    dfs_flag=0;
    
    path.clear();
    allpath.clear();
}
