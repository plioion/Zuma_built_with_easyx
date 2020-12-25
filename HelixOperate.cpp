//
// Created by desktop on 2020/3/21.
//

//螺旋线函数
//void helix(double theta, float& xPos, float& yPos)
//{
//    float rho = (3 * M_PI - theta) * (WINDOWWITH / (6 * M_PI));
//    xPos = WINDOWWITH / 2 - rho * cos(theta);
//    yPos = rho * sin(theta) / 2 + WINDOWHEIGHT / 2;
//}
//
//
////寻找下一个球心坐标
//bool nextHelixPos(float& x, float& y)
//{
//    float xNex, yNex, dis;
//    if ((pow(x - WINDOWWITH / 2, 2) + pow(y - WINDOWHEIGHT / 2, 2)) < 2500) return false;
//    double thetaSmall, thetaMid, thetaLarge;
//    thetaSmall = atan((y - WINDOWHEIGHT / 2) / (WINDOWWITH / 2 - x));
//    if (thetaSmall <= 2 * M_PI) thetaLarge = thetaSmall + M_PI;
//    else thetaLarge = 3 * M_PI;
//    do
//    {
//        thetaMid = (thetaLarge + thetaSmall) / 2;
//        helix(thetaMid, xNex, yNex);
//        dis = (x - xNex) * (x - xNex) + (y - yNex) * (y - yNex) - 400;
//        if (dis > 0 && x < xNex)
//            thetaLarge = thetaMid;
//        else thetaSmall = thetaMid;
//    } while (abs(dis) >= 1 || x >= xNex);
//    x = xNex;
//    y = yNex;
//    return true;
//}
//
////更新球链中球的位置
//int updateBallPos(Node* head)
//{
//    Node* p = head;
//    int numOfHurt = 0, index = 0;
//    float xPos = 10;
//    float yPos = WINDOWHEIGHT / 2;
//    while (p->next != NULL)
//    {
//        p = p->next;
//        p->data.x = xPos;
//        p->data.y = yPos;
//
//        if (!nextHelixPos(xPos, yPos))
//        {
//            numOfHurt++;
//            ListDelete(head, index);
//        }
//        index++;
//    }
//    return 0;
//}
