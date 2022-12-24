#ifndef hebi_Ctrl_H
#define hebi_Ctrl_H

#include "base/hebi.h"
#include "utility/c.h"
#include <thread>
#include <mutex>

class Ctrl {
public:
    typedef Grid::SizeType SizeType;

public:
    ~Ctrl();

    static Ctrl* getInstance();

    void setFPS(const double fps_);
    void setEnableAI(const bool enableAI_);
    void setMoveInterval(const long ms);
    void setRecordMovements(const bool b);
    void setGridRow(const SizeType n);
    void setGridCol(const SizeType n);

    int run();

private:
    Ctrl();

    void sleepFPS() const;

    void init();
    void initGrid();
    void inithebi();
    void initFiles();

    void mainLoop();
    void dualhebi();
    void movehebi(Hebi &s);

    void printMsg(const std::string &msg);

    void exitGame(const std::string &msg);
    void exitGameErr(const std::string &err);

    void startSubThreads();

    void draw();
    void drawGridContent() const;

    void keyboard();
    void keyboardMove(Hebi &s, const Dir d);

private:
    static const std::string MSG_LOSE;
    static const std::string s1;
    static const std::string s2;
    static const std::string MSG_BAD_ALLOC;
    static const std::string MSG_WIN;
    static const std::string MSG_ESC;

    double fps = 60.0;
    bool enableAI = true;
    long moveInterval = 30;
    SizeType GridRowCnt = 10;
    SizeType GridColCnt = 10;

    Grid *grid = nullptr;
    Hebi hebi;
    Hebi hebi1;

    volatile bool pause = false;  // Control pause/resume game

    volatile bool runMainThread = true;  // Switch of the main thread
    volatile bool runSubThread = true;   // Switch of sub-threads

    std::thread drawThread;      // Thread to draw the Grid
    std::thread keyboardThread;  // Thread to execute keyboard instructions

    std::mutex mutexMove;  // Mutex of movehebi()
    std::mutex mutexExit;  // Mutex of exitGame()
};

#endif
