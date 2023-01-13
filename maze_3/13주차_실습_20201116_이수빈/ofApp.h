#pragma once

#include "ofMain.h"

struct Pair {
    int x;
    int y;
};


class ofApp : public ofBaseApp{

    public:
    
        ofPolyline path;
        ofPolyline allpath;
        void setup();
        void update();
        void draw();

        void keyPressed(int key);
   
        void processOpenFileSelection(ofFileDialogResult openFileResult);
        bool readFile();

        bool DFS();
    
        void freeMemory();
        
        stack<Pair> s;
        stack<Pair> s_path;
        stack<Pair> all_path;

        vector<vector<char>> maze;
    
};
