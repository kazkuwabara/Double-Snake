#include "ctrl.h"
#include "utility/utility.h"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <cstdio>

#ifdef OS_WIN
#include <Windows.h>
#endif

using std::list;
using std::string;
using namespace std;

const string Ctrl::MSG_BAD_ALLOC = "Not enough memory to run the game.";
const string Ctrl::MSG_LOSE = "body collision";
const string Ctrl::MSG_ESC = "Game ended.";
const string Ctrl::s1 = "snake 2 wins";
const string Ctrl::s2 = "snake 1 wins";

Ctrl::Ctrl() {}

Ctrl::~Ctrl() {
    if (grid) {
        delete grid;
        grid = nullptr;
    }
}

Ctrl* Ctrl::getInstance() {
    static Ctrl instance;
    return &instance;
}

void Ctrl::setFPS(const double fps_) {
    fps = fps_;
}

void Ctrl::setMoveInterval(const long ms) {
    moveInterval = ms;
}

void Ctrl::setGridRow(const SizeType n) {
    GridRowCnt = n;
}

void Ctrl::setGridCol(const SizeType n) {
    GridColCnt = n;
}

int Ctrl::run() {
    try {
        init();
        mainLoop();
        return 0;
    } catch (const std::exception &e) {
        exitGameErr(e.what());
        return -1;
    }
}

void Ctrl::sleepFPS() const {
    utility::sleep((long)((1.0 / fps) * 1000));
}

void Ctrl::exitGame(const std::string &msg) {
    mutexExit.lock();
    if (runMainThread) {
        utility::sleep(100);
        runSubThread = false;
        utility::sleep(100);
        printMsg(msg);
    }
    mutexExit.unlock();
    runMainThread = false;
}

void Ctrl::exitGameErr(const std::string &err) {
    exitGame("ERR: " + err);
}

void Ctrl::printMsg(const std::string &msg) {
    C::setCursor(0, (int)GridRowCnt);
    C::writeWithColor(msg + "\n", ConsoleColor(WHITE, BLACK, true, false));
}

void Ctrl::mainLoop() {
    while (runMainThread) {
        if (!pause) {
            grid->createFood(hebi.getHead());
            hebi1.decideNext();
            grid->removeFood();
            grid->createFood(hebi1.getTail());
            hebi.decideNext();
            grid->removeFood();
            if(hebi.isDead() or hebi1.isDead()) {
                exitGame(MSG_LOSE);
                return;
            }
            else {
                dualhebi();
            }
        }
        utility::sleep(moveInterval);
    }
}

void Ctrl::dualhebi() {
    mutexMove.lock();
    try {
        bool bo = false;
        bool bo1 = false;
        hebi.move(bo1);
        if(bo1) {
            exitGame(s1);
        }
        hebi1.move(bo);
        if(bo) {
            exitGame(s2);
        }
        mutexMove.unlock();
    } catch (const std::exception) {
        mutexMove.unlock();
        throw;
    }
}

void Ctrl::init() {
    C::clear();
    initGrid();
    inithebi();
    startSubThreads();
}

void Ctrl::initGrid() {
    if (GridRowCnt < 5 || GridColCnt < 5) {
        string msg = "Ctrl.initGrid(): Grid size at least 5*5. Current size "
            + utility::toString(GridRowCnt) + "*" + utility::toString(GridColCnt) + ".";
        throw std::range_error(msg.c_str());
    }
    grid = new Grid(GridRowCnt, GridColCnt);
    if (!grid) {
        exitGameErr(MSG_BAD_ALLOC);
    }
}

void Ctrl::inithebi() {
    hebi.setGrid(grid);
    hebi1.setGrid(grid);
    vector<int> v;
    do {
        cout<<v.size()<<endl;
        if(v.size()==2) {
            break;
        }
        int i = rand() % 15 + 1;
        if(find(v.begin(), v.end(), i) == v.end()) {
            v.push_back(i);
            continue;
        }
        else continue;
    } while(true);
    hebi.addBody(P(1, v[0]+4));
    hebi.addBody(P(1, v[0]+3));
    hebi.addBody(P(1, v[0]+2));
    hebi.addBody(P(1, v[0]+1));
    hebi.addBody(P(1, v[0]));
    hebi1.addBody(P(15, v[1]));
    hebi1.addBody(P(15, v[1]+1));
    hebi1.addBody(P(15, v[1]+2));
}

void Ctrl::startSubThreads() {
    runSubThread = true;
    drawThread = std::thread(&Ctrl::draw, this);
    drawThread.detach();
    keyboardThread = std::thread(&Ctrl::keyboard, this);
    keyboardThread.detach();
}

void Ctrl::draw() {
    try {
        while (runSubThread) {
            drawGridContent();
            sleepFPS();
        }
    } catch (const std::exception &e) {
        exitGameErr(e.what());
    }
}

void Ctrl::drawGridContent() const {
    C::setCursor();
    SizeType row = grid->getRowCount(), col = grid->getColCount();
    for (SizeType i = 0; i < row; ++i) {
        for (SizeType j = 0; j < col; ++j) {
            const Pt &pt = grid->getPoint(P(i, j));
            switch (pt.getType()) {
                case Pt::Type::EMPTY:
                    C::writeWithColor("  ", ConsoleColor(BLACK, BLACK));
                    break;
                case Pt::Type::WALL:
                    C::writeWithColor(CYAN, CYAN, true, true));
                    break;
                case Pt::Type::FOOD:
                    C::writeWithColor("  ", ConsoleColor(RED, RED, true, true));
                    break;
                case Pt::Type::hebi_HEAD:
                    C::writeWithColor("  ", ConsoleColor(BLUE, BLUE, true, true));
                    break;
                case Pt::Type::hebi_BODY:
                    C::writeWithColor("  ", ConsoleColor(MAGENTA, MAGENTA, true, true));
                    break;
                case Pt::Type::hebi_TAIL:
                    C::writeWithColor("  ", ConsoleColor(YELLOW, YELLOW, true, true));
                    break;
                default:
                    break;
            }
        }
        C::write("\n");
    }
}

void Ctrl::keyboard() {
    try {
        while (runSubThread) {
            if (C::kbhit()) {
                switch (C::getch()) {
                    case 'w':
                        keyboardMove(hebi, Dir::UP);
                        break;
                    case 'a':
                        keyboardMove(hebi, Dir::LEFT);
                        break;
                    case 's':
                        keyboardMove(hebi, Dir::DOWN);
                        break;
                    case 'd':
                        keyboardMove(hebi, Dir::RIGHT);
                        break;
                    case 72:
                        keyboardMove(hebi1, Dir::UP);
                        break;
                    case 75:
                        keyboardMove(hebi1, Dir::LEFT);
                        break;
                    case 80:
                        keyboardMove(hebi1, Dir::DOWN);
                        break;
                    case 77:
                        keyboardMove(hebi1, Dir::RIGHT);
                        break;
                    case ' ':
                        pause = !pause;  // Pause or resume game
                        break;
                    case 27:  // Esc
                        exitGame(MSG_ESC);
                        break;
                    default:
                        break;
                }
            }
            sleepFPS();
        }
    } catch (const std::exception &e) {
        exitGameErr(e.what());
    }
}

void Ctrl::movehebi(Hebi &s) {
    mutexMove.lock();
    try {
        s.move();
        mutexMove.unlock();
    } catch (const std::exception) {
        mutexMove.unlock();
        throw;
    }
}

void Ctrl::keyboardMove(Hebi &s, const Dir d) {
    if (pause) {
        s.setDirection(d);
        movehebi(s);
    }
    if (s.getDirection() == d) {
        movehebi(s);  // Accelerate
    } else {
        s.setDirection(d);
    }
}
