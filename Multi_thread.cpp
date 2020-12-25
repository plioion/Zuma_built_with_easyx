//
// Created by 29416 on 2020/4/24.
//
#include "Multi_tread.h"

unsigned int _stdcall playMusic(LPVOID pM)
{
    mciSendString(musics[*(int *)pM][0], NULL, 0, NULL);
    mciSendString(musics[*(int *)pM][1], NULL, 0, NULL);
    Sleep(1000);
    mciSendString(musics[*(int *)pM][2], NULL, 0, NULL);
    return 0;
}