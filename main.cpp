#include <graphics.h>
#include <time.h>
#include <crtdbg.h> //检查内存泄露
#include "LinkList.h"
#include "ball.h"
#include "Appearance.h"
#include "Multi_tread.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define sign(num) (num>=0? 1: -1)
#define BallCentX WINDOWWITH / 2
#define BallCentY WINDOWHEIGHT / 2
int level = 0;

typedef struct
{
    int speed;
    int addTime;
    int hurt;
    int punish;
    int colorNum;
} difficulty;

const difficulty dif[4] = {{14, 2000, 1, 0, sizeof(ballColorTable) / sizeof(COLORREF) - 3},
                           {14, 1000, 1, 1, sizeof(ballColorTable) / sizeof(COLORREF) - 3},
                           {14, 500, 2, 3, sizeof(ballColorTable) / sizeof(COLORREF) - 2},
                           {14, 500, 3, 3, sizeof(ballColorTable) / sizeof(COLORREF) - 1}};

//初始化球链表
void initBallList(Node* head)
{
    int i;
    ball b;

    for (i = 0; i < 15; ++i) {
        b.c = rand() % dif[level].colorNum;
        ListInsert(head, 0, b);

    }
}

//绘制球表
void drawBallList(Node* head)
{
    Node* p;
    ball b;

    p = head;

    while (p->next != NULL)
    {
        p = p->next;
        b = p->data;

        setcolor(ballColorTable[b.c]);
        setfillcolor(ballColorTable[b.c]);
        solidcircle(b.x, b.y, BALLRADIUS);

    }
}

//更新球链中球的位置
int updateBallPos(Node* head)
{
    int ballNum = 0, outNum = 0;
    static double theta[WINDOWWITH / 3] = {4 * M_PI};
    double thetaSmall, thetaMid, thetaLarge;

    Node* p = head, *destroy;
    float xPos = BALLRADIUS, yPos = WINDOWHEIGHT / 2, disT;
    while (p->next != NULL)
    {
        if(theta[ballNum] == 0)
        {
            thetaLarge = theta[ballNum - 1];
            thetaSmall = 0;
            do
            {
                thetaMid = (thetaLarge + thetaSmall) / 2;
                xPos = WINDOWWITH / 2 * (1 - thetaMid / (4 * M_PI) * cos(thetaMid));
                yPos = WINDOWHEIGHT / 2 * (1 + thetaMid / (4 * M_PI) * sin(thetaMid));
                disT = pow(p->data.x - xPos, 2) + pow(p->data.y - yPos, 2) - pow(2 * BALLRADIUS, 2);
                if(disT < 0) thetaLarge = thetaMid;
                else thetaSmall = thetaMid;
            }while(abs(disT) >= 1e-2);
            theta[ballNum] = thetaMid;
        }
        p = p->next;
        xPos = WINDOWWITH / 2 * (1 - theta[ballNum] / (4 * M_PI) * cos(theta[ballNum]));
        yPos = WINDOWHEIGHT / 2 * (1 + theta[ballNum] / (4 * M_PI) * sin(theta[ballNum]));
        p->data.x = xPos;
        p->data.y = yPos;
        disT = pow(xPos - BallCentX, 2) + pow(yPos - BallCentY, 2) - pow(LifeDist, 2);
        if(disT >= 0) ballNum ++;
        else
        {
            _beginthreadex(NULL, 0, playMusic, music + hurt, 0, NULL);
            drawBallList(head);
            //Sleep(300);
            p = p->past;
            outNum = 0;

            while(p->next != NULL)
            {
                ListDelete(p, 0);
                outNum ++;
            }
            break;
        }
    }

    return outNum;
}

//每隔一段时间增加一个球
void extendBallList(Node *head)
{
    DataType data;

    data.c = rand() % dif[level].colorNum;
    ListInsert(head,0,data);
}

//球碰撞检测
bool collisionDetection(Node* head, ball b, int* id)
{
    Node* p = head->next;
    int dist = 0;
    int index = 0;

    while (p != NULL)
    {
        dist = (p->data.x - b.x) * (p->data.x - b.x) + (p->data.y - b.y) * (p->data.y - b.y);
        if (dist < pow(BALLRADIUS, 2) * 2)
        {
            *id = index;
            if(b.x >= p->data.x) *id += 1;

            return TRUE;
        }

        p = p->next;
        index++;
    }
    return FALSE;

}

//绘制碰撞球
void drawColBall(ball* b, int x, int y)
{
    b->x = x;
    b->y = y;

    setcolor(ballColorTable[b->c]);
    setfillcolor(ballColorTable[b->c]);
    solidcircle(b->x, b->y, BALLRADIUS);
}

//寻找附近颜色相同的球
int checkNeighbor(Node *head, int index, int time = 1)
{
    int count = 1, i = 0, score = 0;
    Node *node;
    node = head;
    for(i; i <= index; i ++)
        node = node->next;
    while(node->next != NULL && node->data.c == node->next->data.c)
    {
        node = node->next;
        index ++;
    }

    while(node->past != NULL && node->past->data.c == node->data.c)
    {
        count ++;
        node = node->past;
        index --;
        //printf("%d ",index);
    }
    if(count >= 3)
    {
        i = count;
        score += count * time;
        while((--i) >= 0)
        {
            node->data.c = (sizeof(ballColorTable) / sizeof(COLORREF) - 1);
            node = node->next;
        }
        drawBallList(head);
        FlushBatchDraw();
        Sleep(100);
        _beginthreadex(NULL, 0, playMusic, music + erase, 0, NULL);
        while((--count) >= 0)
            ListDelete(head,index);
        if(node != NULL && (node->data.c == node->past->data.c))
        {
            cleardevice();
            IMAGE gameWall;
            loadimage(&gameWall, gameWallImg);
            putimage(0, 0, &gameWall);
            updateBallPos(head);
            drawBallList(head);
            //Sleep(500);
            score += checkNeighbor(head, index, 2 * time);
        }

    }
    return score;
}

int main()
{
    srand((unsigned)time(NULL));
    mciSendString(musics[bgm][0], NULL, 0, NULL);
    mciSendString(musics[bgm][1], NULL, 0, NULL);

    startGame();
    level = chooseDifficulty();

    int time = 0, index = 0, ballNum = 100, seton = 0, life = 100, score = 0;
    char cLife[20];

    cleardevice();
    IMAGE gameWall;
    loadimage(&gameWall, gameWallImg);
    putimage(0, 0, &gameWall);
    
    //初始化球链
    Node* head = NULL;
    head = CreateEmptyList();
    head->data.c = -1;
    initBallList(head);
    updateBallPos(head);
    drawBallList(head);

    //画碰撞球
    ball cBall;
    cBall.c = rand() % dif[level].colorNum;
    float mouseDirCos = -1, mouseDirSin = -1, mouseDirTan = 0, ballMovCos = -1, ballMovSin = -1;
    float cBallX = BallCentX, cBallY = BallCentY;


    mciSendString(_T("setaudio myMusic volume to 500 "), NULL, 0, NULL);
    //事件循环，处理鼠标、时间事件
    MOUSEMSG mouse = GetMouseMsg();
    bool ballMoving = FALSE;
    while (head->next)
    {
        BeginBatchDraw();

        // 获取一条鼠标消息
        if (MouseHit())
        {
            mouse = GetMouseMsg();
            //是否点击设置按钮
            if(mouse.x <= setPicRadius && mouse.y >= WINDOWHEIGHT - setPicRadius)
            {
                if(seton == 0)
                {
                    _beginthreadex(NULL, 0, playMusic, music + button, 0, NULL);
                    seton = 1;
                }

                if(mouse.uMsg == WM_LBUTTONDOWN)
                {
                    IMAGE setImg;
                    loadimage(&setImg, setting);
                    _beginthreadex(NULL, 0, playMusic, music + move, 0, NULL);
                    while(mouse.x <= setPicR && mouse.y >= WINDOWHEIGHT - setPicRadius)
                    {
                        mouse = GetMouseMsg();
                        cleardevice();
                        putimage(0, 0, &setImg);
                        drawBallList(head);
                        setlinecolor(BLUE);
                        if(mouse.x <= newGameR && mouse.x >= newGameL)
                        {
                            line(newGameL, WINDOWHEIGHT - setPicRadius + 25, newGameL, WINDOWHEIGHT - 15);
                            line(newGameR, WINDOWHEIGHT - setPicRadius + 25, newGameR, WINDOWHEIGHT - 15);
                            if(mouse.uMsg == WM_LBUTTONDOWN)
                            {
                                time = 0, index = 0, ballNum = 100, seton = 0, life = 100, score = 0;
                                level = chooseDifficulty();
                                DestroyList(head);
                                head = NULL;
                                head = CreateEmptyList();
                                head->data.c = -1;
                                initBallList(head);
                            }

                        }
                        if(mouse.x <= ScoreR && mouse.x >= ScoreL)
                        {
                            line(ScoreL, WINDOWHEIGHT - setPicRadius + 25, ScoreL, WINDOWHEIGHT - 15);
                            line(ScoreR, WINDOWHEIGHT - setPicRadius + 25, ScoreR, WINDOWHEIGHT - 15);
                            if(mouse.uMsg == WM_LBUTTONDOWN)
                            {
                                scoreBoard();
                            }
                        }
                        if(mouse.x <= endGameR && mouse.x >= endGameL)
                        {
                            line(endGameL, WINDOWHEIGHT - setPicRadius + 25, endGameL, WINDOWHEIGHT - 15);
                            line(endGameR, WINDOWHEIGHT - setPicRadius + 25, endGameR, WINDOWHEIGHT - 15);
                            if(mouse.uMsg == WM_LBUTTONDOWN)
                            {
                                TCHAR newName[MAX_COUNT];int max = MAX_COUNT;
                                if(InputBox(newName, max, _T("Please input your name"), _T("Save your score"),\
                                _T("English and nums only"), 0, 0, false))
                                {
                                    scoreBoard(newName, score);
                                    mciSendString(musics[bgm][2], NULL, 0, NULL);
                                    DestroyList(head);
                                    closegraph();
                                    if (!_CrtDumpMemoryLeaks())
                                        return 0;
                                }

                            }
                        }
                        if(mouse.x <= setR && mouse.x >= setL)
                        {
                            line(setL, WINDOWHEIGHT - setPicRadius + 25, setL, WINDOWHEIGHT - 15);
                            line(setR, WINDOWHEIGHT - setPicRadius + 25, setR, WINDOWHEIGHT - 15);
                            if(mouse.uMsg == WM_LBUTTONDOWN)
                            {
                                setMenu();
                            }
                        }
                        FlushBatchDraw();
                    }
                    continue;
                }
            }
            else seton = 0;
            switch (mouse.uMsg)
            {
                case WM_MOUSEMOVE:
                    //更新方向
                    mouseDirTan = float(mouse.y - BallCentY) / float(mouse.x - BallCentX);
                    mouseDirCos = sign(mouse.x - BallCentX) / sqrt(1 + pow(mouseDirTan, 2));
                    mouseDirSin = sign(mouse.y - BallCentY) * sqrt(1 - pow(mouseDirCos, 2));
                    if(!ballMoving)
                    {
                        ballMovCos = mouseDirCos;
                        ballMovSin = mouseDirSin;
                    }

                    break;

                case WM_LBUTTONDOWN:
                    //球开始运动
                    if (!ballMoving)
                    {
                        ballMoving = TRUE;
                        _beginthreadex(NULL, 0, playMusic, music + shoot, 0, NULL);
                    }
                    break;

                case WM_RBUTTONUP:
                    DestroyList(head);
                    closegraph();
                    if (!_CrtDumpMemoryLeaks())
                        return 0;	// 按鼠标右键退出程序
            }
        }

        if(life <= 0)
        {
            DestroyList(head);
            mciSendString(musics[bgm][2], NULL, 0, NULL);
            closegraph();
            return 0;
        }

        //定时更新球链、碰撞球
        cleardevice();
        putimage(0, 0, &gameWall);

        bool collision = collisionDetection(head, cBall, &index);
        if (collision)
        {
            ListInsert(head, index, cBall);
            //Sleep(500);
            life -= dif[level].hurt * updateBallPos(head);
            drawBallList(head);
            FlushBatchDraw();
            _beginthreadex(NULL, 0, playMusic, music + insert, 0, NULL);
            score += checkNeighbor(head, index, level + 1);
            updateBallPos(head);


            cBall.c = rand() % dif[level].colorNum;
            cBallX = BallCentX;
            cBallY = BallCentY;
            drawColBall(&cBall, cBallX, cBallY);
            ballMoving = FALSE;

        }
        time += 1;
        if(ballNum && time >= (dif[level].addTime) / log(ballNum + 1))
        {
            //printf("%d\n",ballNum);
            extendBallList(head);
            life -= dif[level].hurt * updateBallPos(head);
            ballNum --;
            time = 0;
        }
        drawBallList(head);

        //检测碰撞球是否超出范围
        if (cBall.x > WINDOWWITH || cBall.x <0 || cBall.y > WINDOWHEIGHT || cBall.y < 0)
        {
            life -= dif[level].punish;
            _beginthreadex(NULL, 0, playMusic, music + hurt, 0, NULL);
            cBall.c = rand() % dif[level].colorNum;
            cBallX = BallCentX;
            cBallY = BallCentY;
            drawColBall(&cBall, cBallX, cBallY);
            ballMoving = FALSE;
        }

        //移动、绘制碰撞球
        if(ballMoving == TRUE)
        {
            drawColBall(&cBall, cBallX += dif[level].speed * ballMovCos , cBallY += dif[level].speed * ballMovSin);
        }
        else
        {
            cBallX = BallCentX + LifeDist * mouseDirCos / 2;
            cBallY = BallCentY + LifeDist * mouseDirSin / 2;
            line(BallCentX, BallCentY, cBallX, cBallY);
            drawColBall(&cBall, cBallX, cBallY);
        }

        FlushBatchDraw();
        Sleep(1);

    }

    TCHAR newName[MAX_COUNT];int max = MAX_COUNT;
    InputBox(newName, max, _T("Please input your name"), _T("Save your score"), _T("English and nums only"));
    scoreBoard(newName, score);
    DestroyList(head);
    mciSendString(musics[bgm][2], NULL, 0, NULL);
    closegraph();
    if (!_CrtDumpMemoryLeaks())
        return 0;
    exit(1);
}
	
