//
// Created by AMD on 2020/4/7.
//

#include "Appearance.h"

//定位按钮鼠标位置
int findMouse(int x, int y)
{
    choiceButton button;
    if(x < button.width)
    {
        if(y < (450 + button.height) && y > 450) return 0;
        else return -1;
    }
    else if(x < 2*button.width)
    {
        if(y < (500 + button.height) && y > 500) return 1;
        else return -1;
    }
    else if(x < 3*button.width)
    {
        if(y < (450 + button.height) && y > 450) return 2;
        else return -1;
    }
    else if(x < 4*button.width)
    {
        if(y < (500 + button.height) && y > 500) return 3;
        else return -1;
    }
    else
        return -1;
}

//显示透明png图片,将原图中纯白色消除
void putTransPic(LPCTSTR buffer, int leftPos, int upPos, int width, int height, \
                int leftPosOri, int upPosOri, int widthOri, int heightOri, long color)
{
    IMAGE img;
    loadimage(&img, buffer);
    HDC dstDC = GetImageHDC();
    HDC srcDC = GetImageHDC(&img);
    TransparentBlt(dstDC, leftPos, upPos, width, height, srcDC, leftPosOri, upPosOri, widthOri, heightOri, color);
    //FlushBatchDraw();
}
//显示难度选项
void difChoicePic(int order[8])
{
    int num = 0;
    choiceButton button = {200,100};
    cleardevice();
    BeginBatchDraw();
    IMAGE wallpaper;
    loadimage(&wallpaper, difWallPaper);
    putimage(0, 0, &wallpaper);
    for(num; num < 4; num ++)
    {
        putTransPic(posS[order[num] + 4*order[num+4]], button.width*num, 450 + 50*(num%2),\
        button.width, button.height, 0, 0, button.width, button.height);
    }
    FlushBatchDraw();
}

//显示欢迎界面
void startGame()
{
    //initgraph(WINDOWWITH, WINDOWHEIGHT, EW_NOCLOSE);
    initgraph(WINDOWWITH, WINDOWHEIGHT);

    char *buffer, note[] = "\n\n图片加载失败\n请检查resource文件夹是否在该目录下\n\n按任意键继续\0", helloImg[30];
    buffer = _getcwd(NULL, 0);
    int length = strlen(buffer), i = 0, j = 0, flag;
    char *imgErr = (char *)malloc(sizeof(char)*(length + length /48 + 10)+ strlen(note)*sizeof(char));
    memset(imgErr, 0, sizeof(imgErr));
    for(i = 0; i < length; i ++)
    {
        flag = i % (WINDOWWITH);
        if(flag >= 70 && imgErr[j-1] == '\\') imgErr[j++] = '\n';
        imgErr[j++] = buffer[i];
    }

    imgErr[j] = '\0';


    length = strlen(imgErr) + strlen(note) + 1;
    strcat_s(imgErr, length,note);

    setbkcolor(WHITE);
    setfillcolor(WHITE);
    fillrectangle(0, 0, WINDOWWITH, WINDOWHEIGHT);
    settextcolor(BLACK);
    RECT REC = {0, 0, WINDOWWITH, WINDOWHEIGHT};
    CString sName(imgErr);
    //LPCTSTR lpszName = sName;
    settextstyle(0, 0, sName);
    drawtext(sName, &REC, DT_CENTER | DT_VCENTER);

    IMAGE img;
    for(i = 1;i <= 16;i ++)
    {
        if(i <= 6)
            sprintf_s(helloImg, ".\\resource\\hello%d.png", i);
        else
            sprintf_s(helloImg, ".\\resource\\hellop%d.png", i - 6);
        CString imgS(helloImg);
        loadimage(&img, imgS);
        putimage(0, 0, &img);
        Sleep(100);
    }

    while(true)
    {
        if (MouseHit())
        {
            if(GetMouseMsg().uMsg == WM_LBUTTONDOWN ||\
            GetMouseMsg().uMsg == WM_RBUTTONDOWN)
            {
                cleardevice();
                free(imgErr);
                free(buffer);
                break;
            }
            Sleep(1);
        }
    }
}

//设置选择难度
int chooseDifficulty()
{
    IMAGE wallpaper;
    setbkcolor(WHITE);
    loadimage(&wallpaper, difWallPaper);
    putimage(0, 0, &wallpaper);
    int order[8] = {0, 1, 2, 3, false, false, false, false}, position = -1, ifChoose = 3;
    difChoicePic(order);
    MOUSEMSG mouse;
    while(true)
    {
        //cleardevice();
        //BeginBatchDraw();
        if(MouseHit())
        {
            mouse = GetMouseMsg();
            switch(mouse.uMsg)
            {
                case WM_MOUSEMOVE:
                {
                    if(position != findMouse(mouse.x, mouse.y))
                    {
                        if (position != -1)
                            order[position + 4] = false;
                        //cleardevice();
                        position = findMouse(mouse.x, mouse.y);
                        if (position != -1)
                            order[position + 4] = true;
                        if (position != -1)
                        {
                            _beginthreadex(NULL, 0, playMusic, music + button, 0, NULL);
                            difChoicePic(order);
                        }
                        else difChoicePic(order);
                        //FlushBatchDraw();
                    }
                    break;
                }
                case WM_LBUTTONDOWN:
                {
                    if(position != -1)
                    {
                        if(ifChoose == 0 || order[position] != 0) return order[position];
                        else
                        {
                            ifChoose --;
                            int temp, random, i;
                            temp = order[position];
                            do
                            {
                                random = rand() % 4;
                            }while(random == temp);

                            for(i = 0;i < 4; i++)
                            {
                                if(order[i] == random)
                                {
                                    order[i] = temp;
                                    order[position] = random;
                                    break;
                                }
                            }
                            _beginthreadex(NULL, 0, playMusic, music + move, 0, NULL);
                            difChoicePic(order);
                        }
                    }
                }
                case WM_RBUTTONDOWN:
                    break;
            }
        }
        //FlushBatchDraw();
        Sleep(1);
    }
}

//显示设置菜单
void setMenu()
{

}

//显示排行榜
void scoreBoard(TCHAR newName[MAX_COUNT], long newNum)
{
    FILE *file;
    int count = 0, numOfScores = 0;
    struct
    {
        long num;
        char name[MAX_COUNT] = "\0";
    } scores[MAX_COUNT], temp;

    fopen_s(&file, ".\\GameRanking.txt", "r");
    if(file)
    {
        fscanf_s(file, "%d", &numOfScores);
        fgetc(file);
        while(count < numOfScores)
        {
            if(!fgets(scores[count].name, MAX_COUNT - 1, file))
                break;
            fscanf_s(file, "%ld", &(scores[count++].num));
            fgetc(file);
        }
        fclose(file);
    }
    if(newName)
    {
        sprintf_s(scores[count].name, MAX_COUNT, "%ls\n", newName);
        //scores[count].name[strlen(scores[count].name)] = '\n';
        scores[count ++].num = newNum;
    }


    for(int i = count;i > 0; i--)
        for(int j = 0; j < i; j++)
        {
            if(scores[j].num < scores[j + 1].num)
            {
                temp = scores[j];
                scores[j] = scores[j +1];
                scores[j + 1] = temp;
            }
        }

    fopen_s(&file, ".\\GameRanking.txt", "w");
    fprintf(file, "%d\n", count);
    for(int i = 0;i < count;i++)
    {
        fprintf(file, "%s", scores[i].name);
        fprintf(file, "%ld\n", scores[i].num);
    }
    fclose(file);

    RECT rec = { recl, recu, rankr,  recu + textHeight};
    setbkmode(TRANSPARENT);
    settextcolor(DARKGRAY);
    LOGFONT font;
    gettextstyle(&font);
    font.lfHeight = 20;
    font.lfWeight = 20;
    font.lfWeight = FW_SEMIBOLD;
    font.lfOutPrecision = OUT_TT_PRECIS;
    font.lfQuality = ANTIALIASED_QUALITY | PROOF_QUALITY;
    settextstyle(&font);

    for (int i = 0; i < count; i++)
    {
        BeginBatchDraw();
        if (i % 10 ==0)
        {
            cleardevice();
            loadimage(0, scorePic);
            rec.bottom = recu, rec.top = recu + textHeight;
            rec.left = recl, rec.right = rankr;
            drawtext(CString("rank"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
            rec.left = rankr, rec.right = namer;
            drawtext(CString("name"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
            rec.left = namer, rec.right = recr;
            drawtext(CString("score"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
            rec.bottom += textHeight, rec.top += textHeight;
        }
        {
            char tmp[MAX_COUNT];
            rec.left = recl, rec.right = rankr;
            sprintf_s(tmp, MAX_COUNT, "%ld", i + 1);
            drawtext(CString(tmp), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
            rec.left = rankr, rec.right = namer;
            drawtext(CString(scores[i].name), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
            rec.left = namer, rec.right = recr;
            sprintf_s(tmp, MAX_COUNT, "%ld", scores[i].num);
            drawtext(CString(tmp), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
        }
        FlushBatchDraw();
        if ((i + 1) % 10 == 0 || i + 1 == count)
        {
            int whichButton = 0; bool flag = false;
            MOUSEMSG mouse = GetMouseMsg();
            while (!flag)
            {
                mouse = GetMouseMsg();
                if (mouse.x <= buttonWEIGHT && mouse.y <= buttonWEIGHT)
                {
                    if (whichButton != 1) _beginthreadex(NULL, 0, playMusic, music + button, 0, NULL);
                    whichButton = 1;
                }
                else if (mouse.y - buttonUp >= 0 && mouse.y - buttonUp <= buttonWEIGHT)
                {
                    if (mouse.x >= pastl && mouse.x <= pastr)
                    {
                        if (whichButton != 2) _beginthreadex(NULL, 0, playMusic, music + button, 0, NULL);
                        whichButton = 2;
                    }
                    else if (mouse.x >= nextl && mouse.x <= nextr)
                    {
                        if (whichButton != 3) _beginthreadex(NULL, 0, playMusic, music + button, 0, NULL);
                        whichButton = 3;
                    }
                    else whichButton = 0;
                }
                else whichButton = 0;
                if (mouse.uMsg == WM_LBUTTONDOWN)
                    switch (whichButton)
                    {
                        case 1:
                            return;
                        case 2:
                            if (i / 10)
                            {
                                i -= i % 10 + 11;
                                flag = true;
                            }
                            break;
                        case 3:
                            if (i < count - 1)
                                flag = true;
                            break;
                    }
            }
        }
        rec.bottom += textHeight;
        rec.top += textHeight;
    }
    cleardevice();
}