//
// Created by AMD on 2020/4/7.
//

#ifndef ZUMA_V1_0_APPEARANCE_H
#define ZUMA_V1_0_APPEARANCE_H

#include <direct.h>
#include <atlstr.h>
#include <conio.h>
#include <graphics.h>
#include <windows.h>
#include "Multi_tread.h"

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "MSIMG32.LIB")

#define WINDOWWITH 800 //窗口的宽度
#define WINDOWHEIGHT 650 //窗口的高度
#define LifeDist 100 //扣除生命的半径
#define numOfMusic 7 //音乐数量
#define numOfMusOperate 3 //每个音乐操作的数量
#define MAX_COUNT 100 //排行榜总记录量
#define setPicRadius 60 //游戏主界面设置按钮长宽
#define setPicR 640 //设置条的右边界
#define newGameL 70 //新游戏左边界
#define newGameR 170 //新游戏右边界
#define ScoreL 190 //排行榜左边界
#define ScoreR 280 //排行榜右边界
#define endGameL 300 //结束游戏左边界
#define endGameR 420 //结束游戏右边界
#define setL 440 //设置左边界
#define setR 505 //设置右边界
#define nextl 540 //排行榜下一页按钮左边界
#define nextr 570 //排行榜下一页按钮右边界
#define pastl 230 //排行榜上一页按钮左边界
#define pastr 260 //排行榜上一页按钮右边界 
#define buttonWEIGHT 30 //排行榜按钮直径
#define buttonUp 590 //排行榜翻页按钮上边界
#define recl 100 //排行榜矩形左边界
#define rankr 200 //排行榜rank列右边界
#define namer 600 //排行榜name列右边界
#define recr 700 // 排行榜矩形右边界
#define recu 125 // 排行榜矩形上边界
#define recd 565 //排行榜矩形下边界
#define textHeight 40 //排行榜每行文字占用宽度

//各种音频接口

enum MusS
{
    bgm, shoot, hurt, insert, erase, button, move
};
static MusS music[numOfMusic] = {bgm, shoot, hurt, insert, erase, button, move};
const static CString musics[numOfMusic][numOfMusOperate] =
        {
                {"open .\\resource\\flower_dance.mp3 alias myMusic", "play myMusic repeat", "close myMusic"},
                {"open .\\resource\\shoot.mp3 alias shoot", "play shoot", "close shoot"},
                {"open .\\resource\\hurt.mp3 alias hurt", "play hurt", "close hurt"},
                {"open .\\resource\\insert.wav alias insert", "play insert", "close insert"},
                {"open .\\resource\\erase.mp3 alias erase", "play erase", "close erase"},
                {"open .\\resource\\button.mp3 alias butMus", "play butMus", "close butMus"},
                {"open .\\resource\\move.mp3 alias move", "play move", "close move"}
        };
//const CString load("open .\\resource\\flower_dance.mp3 alias myMusic"), play("play myMusic repeat"), close("close myMusic");
//const CString loadShoot("open .\\resource\\boom.mp3 alias shoot"), playShoot("play shoot"), closeShoot("close shoot");
//const CString loadHurt("open .\\resource\\hurt.mp3 alias hurt"), playHurt("play hurt"), closeHurt("close hurt");
//const CString loadIns("open .\\resource\\insert.mp3 alias insert"), playIns("play insert"), closeIns("close insert");
//const CString loadErase("open .\\resource\\erase.mp3 alias erase"), playErase("play erase"), closeErase("close erase");
//const CString loadBut("open .\\resource\\button.mp3 alias butMus"), playBut("play butMus"), closeBut("close butMus");
//const CString loadMov("open .\\resource\\move.mp3 alias move"), playMov("play move"), closeMov("close move");

//图片接口
const static CString gameWallImg(".\\resource\\gamewall.jpg");
const static CString difWallPaper(".\\resource\\wallpaper.jpg");
const static CString setting(".\\resource\\setting.jpg");
const static CString scorePic(".\\resource\\scores.jpg");
const static CString posS[8] = {".\\resource\\0Odiff.png",
                         ".\\resource\\1Odiff.png",
                         ".\\resource\\2Odiff.png",
                         ".\\resource\\3Odiff.png",
                         ".\\resource\\0Xdiff.png",
                         ".\\resource\\1Xdiff.png",
                         ".\\resource\\2Xdiff.png",
                         ".\\resource\\3Xdiff.png"};

//定义颜色
const static COLORREF ballColorTable[] = {
        0xffa2a2/*类粉*/, 0xfeff88/*天蓝(PS中为黄色)*/,\
        0x42ff00/*草绿*/, 0x71fff0/*金黄(PS中为蓝色)*/, \
        0xff0090/*纯紫*/, 0xde00ff/*玫红*/, 0xeadddf/*银白*/
};

//按钮大小
typedef struct
{
    int width = 200;
    int height = 100;
}choiceButton;

//定位按钮鼠标位置
int findMouse(int x, int y);

//显示透明png图片,将原图中纯白色消除
void putTransPic(LPCTSTR buffer, int leftPos, int upPos, int width, int height, \
                int leftPosOri, int upPosOri, int widthOri, int heightOri, long color = 0xffffff);

//显示难度选项
void difChoicePic(int order[8]);

//显示欢迎界面
void startGame();

//设置选择难度
int chooseDifficulty();

//设置菜单
void setMenu();

//排行榜
void scoreBoard(TCHAR newName[MAX_COUNT] = NULL, long newNum = 0);

#endif //ZUMA_V1_0_APPEARANCE_H
