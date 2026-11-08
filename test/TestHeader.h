#pragma once

#include "DumbInclude.h"


// @reflect
struct SaveState {
    u32 playTime = 0;
    double d_something = 0.00;
    float f_thing = 0f;
    i8 invert = -1;
};

// @reflect
struct GameArgs {
    char savepath[255];
    char notpath[100] = "ar;15}";
    char i = 'j';
};

#declare MAX_SIZE 10

// @reflect
struct GameState {
    bool editing;
    World* world;
    V2<i32> globalOffset;
    V2<i32> mousePos;
    V2<V2<i64>> whoa;
    LevelMap levels[MAX_SIZE];
    //Editor* editor;
    Player* player;
};

class Game : public Curio {

public:
    Game(CurioContext& facade, PlatformAPI* platform, u32 argc, char** argv);

private:
    // ________________________________________________________________________
    // ...:'| Instance Methods |':.............................................
    // ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
    void LoadAssets();
    void ProcessCommandLineArgs(int argc, char** argv, GameArgs* gameArgs);

    // ________________________________________________________________________
    // ...:'| Private Data |':.................................................
    // ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
    MemoryArenaHandle _gameMemoryHandle;

};
