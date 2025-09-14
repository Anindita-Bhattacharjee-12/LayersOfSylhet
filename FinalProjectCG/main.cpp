#include<GL/freeglut.h>
#include<cmath>
#include<stdio.h>
#define PI 3.1416
float trainPosX=-1.2f;
float trainSpeed=0.009f;
float cloudPosX=-1.2f;
float cloudSpeed=0.003f;
float trainspeedback=trainSpeed;
float smokeY=0.0f;
float cloudspeedback=cloudSpeed;
float handAngle=0.0f;
bool animateHand=true;
bool handUp=false;
float busPos1=1.2f;
float busPos2=1.8f;
float busSpeed1=-0.01f;
float busSpeed1back=busSpeed1;
float busSpeed2=-0.008f;
float busSpeed2back=busSpeed2;
float position1=0.0f;
float speed1=0.005f;
float speed1back=speed1;
float planeY=0.4f;
float planeX=-0.9f;
float planeSpeedY=-0.01f;
float planeSpeedX=0.003f;
bool isTimerRunning=false;

int scene=0; // 0 = first, 1 = second, 2 = third

struct Bird{
    float x,y;
    float speed;
};
Bird birds[3]={
    {-1.0f,0.7f,0.003f},
    {-1.5f,0.75f,0.0025f},
    {-2.0f,0.8f,0.0035f}
};
void drawBird(float x,float y){
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
    glVertex2f(x,y);
    glVertex2f(x+0.02f,y+0.02f);
    glVertex2f(x+0.02f,y+0.02f);
    glVertex2f(x+0.04f,y);
    glEnd();
}
void drawCloud(float x,float y){
    glColor3f(1.0f,1.0f,1.0f);
    float radius=0.04f;
    float offsets[5][2]={
        {0.0f,0.0f},
        {0.04f,0.02f},
        {-0.04f,0.02f},
        {0.08f,0.0f},
        {-0.08f,0.0f}
    };
    for(int i=0;i<5;i++){
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x+offsets[i][0],y+offsets[i][1]);
        for(int angle=0;angle<=360;angle+=5){
            float rad=angle*3.14159f/180.0f;
            glVertex2f(x+offsets[i][0]+cosf(rad)*radius,y+offsets[i][1]+sinf(rad)*radius);
        }
        glEnd();
    }
}
void drawSmoke(float x,float y,float radius,float alpha){
    glColor4f(0.8f,0.8f,0.8f,alpha);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i=0;i<=100;i++){
        float angle=2.0f*3.14159f*i/100;
        glVertex2f(x+cosf(angle)*radius,y+sinf(angle)*radius);
    }
    glEnd();
}
void TeaStaller(float x,float y,float scale,float r,float g,float b){
    glColor3ub(255,255,204);
    glBegin(GL_POLYGON);
    for (int i=0;i<100;i++){
        float theta=2.0f*3.1415926f*i/100.0f;
        float dx=scale*0.05f*cosf(theta);
        float dy=scale*0.05f*sinf(theta);
        glVertex2f(x+dx,y+scale*0.1f+dy);
    }
    glEnd();

    glColor3f(r,g,b);
    glBegin(GL_QUADS);
    glVertex2f(x-scale*0.03f,y+scale*0.05f);
    glVertex2f(x+scale*0.03f,y+scale*0.05f);
    glVertex2f(x+scale*0.03f,y-scale*0.05f);
    glVertex2f(x-scale*0.03f,y-scale*0.05f);
    glEnd();

    glColor3ub(255,255,204);
    glLineWidth(5.0f);
    glBegin(GL_LINES);
    glVertex2f(x-scale*0.02f,y-scale*0.05f);
    glVertex2f(x-scale*0.02f,y-scale*0.12f);
    glVertex2f(x+scale*0.02f,y-scale*0.05f);
    glVertex2f(x+scale*0.02f,y-scale*0.12f);
    glEnd();

    glLineWidth(6.0f);
    glBegin(GL_LINES);
    glVertex2f(x-scale*0.03f,y+scale*0.04f);
    glVertex2f(x-scale*0.07f,y-scale*0.02f);
    glEnd();

    float angleRad=handAngle*3.14159f/180.0f;
    float dx=scale*0.04f*cosf(angleRad);
    float dy=scale*0.04f*sinf(angleRad);

    glBegin(GL_LINES);
    glVertex2f(x+scale*0.03f,y+scale*0.04f);
    glVertex2f(x+scale*0.03f+dx,y+scale*0.04f-dy);
    glEnd();

    glColor3ub(153,102,51);
    glBegin(GL_POLYGON);
    for (int i=0;i<100;i++){
        float theta=2.0f*3.14159f*i/100.0f;
        float cup_dx=scale*0.04f*cosf(theta);
        float cup_dy=scale*0.02f*sinf(theta);
        glVertex2f(x+scale*0.10f+cup_dx,y-scale*0.02f+cup_dy);
    }
    glEnd();


    float baseX = x + scale * 0.10f;
    float baseY = y - scale * 0.02f;
    for(int i = 0; i < 3; i++) {
        float offsetY = fmodf(smokeY + i * 0.05f, 0.15f);
        float alpha = 0.6f - 0.2f * i;
        drawSmoke(baseX, baseY + offsetY, scale * 0.02f * (1.0f - 0.3f * i), alpha);
    }

    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_LINES);
    glVertex2f(x+scale*0.10f,y+scale*0.01f);
    glVertex2f(x+scale*0.10f,y+scale*0.03f);
    glEnd();

    glLineWidth(1.0f);
}

void drawGlass(float centerX,float centerY,float radius){
    glColor3ub(210,176,76);
    glBegin(GL_POLYGON);
    for (int i=0;i<100;i++){
        float theta=2.0f*3.14159f*i/100.0f;
        float x=centerX+radius*cosf(theta);
        float y=centerY+radius*sinf(theta);
        glVertex2f(x,y);
    }
    glEnd();
}
void drawPerson(float x,float y,float scale,float r,float g,float b){
    glColor3ub(255,255,204);
    glBegin(GL_POLYGON);
    for (int i=0;i<100;i++){
        float theta=2.0f*3.1415926f*float(i)/100.0f;
        float dx=scale*0.05f*cosf(theta);
        float dy=scale*0.05f*sinf(theta);
        glVertex2f(x+dx,y+scale*0.1f+dy);
    }
    glEnd();
    glColor3f(r,g,b);
    glBegin(GL_QUADS);
    glVertex2f(x-scale*0.03f,y+scale*0.05f);
    glVertex2f(x+scale*0.03f,y+scale*0.05f);
    glVertex2f(x+scale*0.03f,y-scale*0.05f);
    glVertex2f(x-scale*0.03f,y-scale*0.05f);
    glEnd();

    glLineWidth(10.0f);
    glColor3ub(255,255,204);
    glBegin(GL_LINES);
    glVertex2f(x-scale*0.02f,y-scale*0.05f);
    glVertex2f(x-scale*0.02f,y-scale*0.12f);
    glVertex2f(x+scale*0.02f,y-scale*0.05f);
    glVertex2f(x+scale*0.02f,y-scale*0.12f);
    glEnd();

    float angleRad=handAngle*3.14159f/180.0f;
    float dx=scale*0.04f*cosf(angleRad);
    float dy=scale*0.04f*sinf(angleRad);

    glLineWidth(10.0f);
    glBegin(GL_LINES);
    glVertex2f(x-scale*0.03f,y+scale*0.04f);
    glVertex2f(x-scale*0.03f-dx,y+scale*0.04f-dy);
    glVertex2f(x+scale*0.03f,y+scale*0.04f);
    glVertex2f(x+scale*0.03f+dx,y+scale*0.04f-dy);
    glEnd();

    glColor3ub(139, 69, 19);
    glBegin(GL_QUADS);
    glVertex2f(x - scale * 0.05f, y + scale * 0.07f);
    glVertex2f(x + scale * 0.05f, y + scale * 0.07f);
    glVertex2f(x + scale * 0.05f, y - scale * 0.03f);
    glVertex2f(x - scale * 0.05f, y - scale * 0.03f);
    glEnd();

    glColor3ub(205,133,63);
    glBegin(GL_LINES);
    for(int i=1;i<7;++i){
    float offset=i*scale*0.02f;
    glVertex2f(x-scale*0.07f+offset,y-scale*0.03f);
    glVertex2f(x-scale*0.07f+offset,y+scale*0.08f);
    }
    glEnd();

    glBegin(GL_LINES);
    for(int j=1;j<3;++j){
    float offset=j*scale*0.04f;
    glVertex2f(x-scale*0.05f,y-scale*0.03f+offset);
    glVertex2f(x+scale*0.05f,y-scale*0.03f+offset);
}
    glEnd();

    glLineWidth(1.0f);

}
void AirportPlane(){
    // Plane front
    glBegin(GL_TRIANGLES);
    glColor3ub(0, 85, 137);
    glVertex2f(planeX + 0.38f, planeY + 0.65f);
    glVertex2f(planeX + 0.40f, planeY + 0.62f);
    glVertex2f(planeX + 0.40f, planeY + 0.68f);
    glEnd();

    // Plane body
    glBegin(GL_POLYGON);
    glColor3ub(80,102,198);
    glVertex2f(planeX + 0.40f, planeY + 0.62f);
    glVertex2f(planeX + 0.49f, planeY + 0.62f);
    glVertex2f(planeX + 0.52f, planeY + 0.71f);
    glVertex2f(planeX + 0.50f, planeY + 0.71f);
    glVertex2f(planeX + 0.49f, planeY + 0.68f);
    glVertex2f(planeX + 0.40f, planeY + 0.68f);
    glEnd();

    // Upper wing
    glBegin(GL_QUADS);
    glColor3ub(0, 153, 153);
    glVertex2f(planeX + 0.44f, planeY + 0.68f);
    glVertex2f(planeX + 0.46f, planeY + 0.68f);
    glVertex2f(planeX + 0.47f, planeY + 0.71f);
    glVertex2f(planeX + 0.45f, planeY + 0.71f);
    glEnd();

    // Lower wing
    glBegin(GL_QUADS);
    glColor3ub(0, 153, 153);
    glVertex2f(planeX + 0.44f, planeY + 0.62f);
    glVertex2f(planeX + 0.46f, planeY + 0.62f);
    glVertex2f(planeX + 0.47f, planeY + 0.58f);
    glVertex2f(planeX + 0.45f, planeY + 0.58f);
    glEnd();
}
void Plane()
{
    glBegin(GL_TRIANGLES);  // plane front
    glColor3ub(80,102,198);
    glVertex2f(position1+0.38f,0.65f);
    glVertex2f(position1+0.40f,0.62f);
    glVertex2f(position1+0.40f,0.68f);
    glEnd();

    glBegin(GL_POLYGON);  // plane body
    glColor3ub(0,85,137);
    glVertex2f(position1 + .40, .62);
    glVertex2f(position1 + .49, .62);
    glVertex2f(position1 + .52, .71);
    glVertex2f(position1 + .50, .71);
    glVertex2f(position1 + .49, .68);
    glVertex2f(position1 + .40, .68);
    glEnd();

    glBegin(GL_QUADS);  // plane wing upper
    glColor3ub(0, 153, 153);
    glVertex2f(position1 + .44, .68);
    glVertex2f(position1 + .46, .68);
    glVertex2f(position1 + .47, .71);
    glVertex2f(position1 + .45, .71);
    glEnd();

    glBegin(GL_QUADS);  // plane wing lower
    glColor3ub(0, 153, 153);
    glVertex2f(position1 + .44, .62);
    glVertex2f(position1 + .46, .62);
    glVertex2f(position1 + .47, .58);
    glVertex2f(position1 + .45, .58);
    glEnd();
}
void drawTree(float x,float y){
    glColor3ub(139,69,19);
    glBegin(GL_POLYGON);
    glVertex2f(x-0.02f,y-0.05);
    glVertex2f(x+0.02f,y-0.05);
    glVertex2f(x+0.02f,y+0.2f);
    glVertex2f(x-0.02f,y+0.2f);
    glEnd();

    glColor3ub(34,139,34);
    glBegin(GL_POLYGON);
    glVertex2f(x-0.1f,y+0.2f);
    glVertex2f(x,y+0.35f);
    glVertex2f(x+0.1f,y+0.2f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x-0.08f,y+0.27f);
    glVertex2f(x,y+0.42f);
    glVertex2f(x+0.08f,y+0.27f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x-0.06f,y+0.34f);
    glVertex2f(x,y+0.48f);
    glVertex2f(x+0.06f,y+0.34f);
    glEnd();
}
void drawSun(float x,float y){
    glColor3f(1.0f,1.0f,0.0f);
    glBegin(GL_POLYGON);
    float r=0.06f;
    for (int i=0;i<100;i++) {
        float angle=(i*2.0f*3.1416f)/100;
        float cx=x+r*cosf(angle);
        float cy=y+r*sinf(angle);
        glVertex2f(cx,cy);
    }
    glEnd();
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(x,y);
    glVertex2f(x,y-0.2f);
    glVertex2f(x,y);
    glVertex2f(x-0.1f,y-0.2f);
    glVertex2f(x,y);
    glVertex2f(x+0.1f,y-0.2f);
    glVertex2f(x,y);
    glVertex2f(x,y+0.2f);
    glVertex2f(x,y);
    glVertex2f(x+0.1f,y+0.2f);
    glVertex2f(x,y);
    glVertex2f(x-0.1f,y+0.2f);
    glVertex2f(x,y);
    glVertex2f(x+0.4f,y+0.2f);
    glVertex2f(x,y);
    glVertex2f(x+0.2f,y);
    glVertex2f(x,y);
    glVertex2f(x+0.2f,y-0.2f);
    glVertex2f(x,y);
    glVertex2f(x-0.3f,y-0.1f);
    glVertex2f(x,y);
    glVertex2f(x-0.3f,y-0.3f);
    glVertex2f(x,y);
    glVertex2f(x-0.3f,y+0.1f);
    glEnd();
}
void drawCircleForStadium(float cx, float cy, float r, int num_segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // center
    for(int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}
void drawBackground() {
    glLineWidth(7.5f);
    // drawing a road
    glBegin(GL_QUADS);
    glColor3ub(150, 150, 150);
    glVertex2f(-1.0f, -0.5f);
    glVertex2f( 1.0f, -0.5f);
    glVertex2f( 1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();

    // Stadium circle
    glLineWidth(2.5f);
    glColor3f(0.0f, 0.7f, 0.0f);
    drawCircleForStadium(0.7f, 0.8f, 0.7f, 100);
    glColor3f(0.0f, 0.0f, 1.0f);
    drawCircleForStadium(0.7f, 0.8f, 0.8f, 100);
}
void drawBus(float busPos, float scale, float busYOffset, float r, float g, float b) {
    float bodyX[] = {0.5f, 1.0f, 1.0f, 0.55f, 0.5f};
    float bodyY[] = {-0.166f, -0.166f, 0.0f, 0.0f, -0.033f};

    glBegin(GL_POLYGON);
    glColor3f(r, g, b);
    for (int i = 0; i < 5; i++) {
        glVertex2f(busPos + bodyX[i]*scale, busYOffset + bodyY[i]*scale);
    }
    glEnd();

    float frontX[] = {0.5f, 0.5166f, 0.5166f, 0.5f};
    float frontY[] = {-0.1f, -0.1f, -0.066f, -0.066f};

    glBegin(GL_POLYGON);
    glColor3f(0.0f, 1.0f, 1.0f);
    for (int i = 0; i < 4; i++) {
        glVertex2f(busPos + frontX[i]*scale, busYOffset + frontY[i]*scale);
    }
    glEnd();

    float roofX[] = {0.55f, 1.0f, 1.0f, 0.55f};
    float roofY[] = {0.0f, 0.0f, 0.116f, 0.116f};

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 0.0f);
    for (int i = 0; i < 4; i++) {
        glVertex2f(busPos + roofX[i]*scale, busYOffset + roofY[i]*scale);
    }
    glEnd();

    float winStart = 0.5833f;
    float winEnd = 0.966f;
    float winStep = 0.0667f;
    float winY1 = 0.016f, winY2 = 0.1f;
    float winWidth = 0.05f;

    for (float wx = winStart; wx < winEnd; wx += winStep) {
        glBegin(GL_POLYGON);
        glColor3f(0.26f, 0.26f, 0.26f);
        glVertex2f(busPos + wx*scale, busYOffset + winY1*scale);
        glVertex2f(busPos + (wx + winWidth)*scale, busYOffset + winY1*scale);
        glVertex2f(busPos + (wx + winWidth)*scale, busYOffset + winY2*scale);
        glVertex2f(busPos + wx*scale, busYOffset + winY2*scale);
        glEnd();
    }
    float wheelX[4] = {0.58f,0.63f,0.88f,0.93f};
    float wheelY = -0.18f;
    float radius = 0.025f * scale;

    for (int i = 0; i <4; ++i) {
        glBegin(GL_TRIANGLE_FAN);
        glColor3ub(0,0,0);
        for (int s = 0; s <= 100; ++s) {
            float theta = 2 * 3.1415926f * s / 100;
            float x = busPos + wheelX[i]*scale + radius * cosf(theta);
            float y = busYOffset + wheelY*scale + radius * sinf(theta);
            glVertex2f(x, y);
        }
        glEnd();
    }
}
void drawCircle(float ax,float ay,float r,int num_segments){
    glBegin(GL_POLYGON);
    for(int i=0;i<num_segments;i++){
        float theta=2.0f*3.1415926f*float(i)/float(num_segments);
        float bx=r*cosf(theta);
        float by=r*sinf(theta);
        glVertex2f(ax+bx,ay+by);
    }
    glEnd();
}
void drawTrain(float x){

    glColor3ub(163,146,116);
    glBegin(GL_QUADS);
    glVertex2f(x-0.1f,-0.25f);
    glVertex2f(x-0.0000001f,-0.25f);
    glVertex2f(x-0.0000001f,-0.15f);
    glVertex2f(x-0.1f,-0.15f);
    glEnd();

    glColor3ub(120, 120, 120);
    glBegin(GL_TRIANGLES);
    glVertex2f(x,-0.25f);
    glVertex2f(x+0.05f,-0.2f);
    glVertex2f(x,-0.15f);
    glEnd();

    glColor3ub(139,69,19);
    drawCircle(x-0.05f,-0.1f,0.01f,50);
    glColor3ub(160,82,45);
    glBegin(GL_QUADS);
    glVertex2f(x-0.07f,-0.15f);
    glVertex2f(x-0.025f,-0.15f);
    glVertex2f(x-0.025f,-0.05f);
    glVertex2f(x-0.07f,-0.05f);
    glEnd();

    glColor4f(0.7f,0.7f,0.7f,0.5f);
    drawCircle(x-0.05f,-0.08f+smokeY,0.02f,20);
    drawCircle(x-0.045f,-0.05f+smokeY,0.017f,20);
    drawCircle(x-0.06f,-0.02f+smokeY,0.019f,20);
    glColor3ub(101,67,33);
    glBegin(GL_QUADS);
    glVertex2f(x-0.9f,-0.25f);
    glVertex2f(x-0.6f,-0.25f);
    glVertex2f(x-0.6f,-0.05f);
    glVertex2f(x-0.9f,-0.05f);
    glEnd();
    glColor3ub(163,146,116);
    glBegin(GL_QUADS);
    glVertex2f(x-0.6f,-0.25f);
    glVertex2f(x-0.3f,-0.25f);
    glVertex2f(x-0.3f,-0.05f);
    glVertex2f(x-0.6f,-0.05f);
    glEnd();
    glColor3ub(101,67,33);
    glBegin(GL_QUADS);
    glVertex2f(x-0.3f,-0.25f);
    glVertex2f(x-0.1f,-0.25f);
    glVertex2f(x-0.1f,-0.05f);
    glVertex2f(x-0.3f,-0.05f);
    glEnd();

    glColor3f(0.8f,0.9f,1.0f);
    glBegin(GL_QUADS);

    glVertex2f(x-0.85f,-0.12f);
    glVertex2f(x-0.81f,-0.12f);
    glVertex2f(x-0.81f,-0.07f);
    glVertex2f(x-0.85f,-0.07f);

    glVertex2f(x-0.77f,-0.12f);
    glVertex2f(x-0.73f,-0.12f);
    glVertex2f(x-0.73f,-0.07f);
    glVertex2f(x-0.77f,-0.07f);

    glVertex2f(x-0.70f,-0.12f);
    glVertex2f(x-0.67f,-0.12f);
    glVertex2f(x-0.67f,-0.07f);
    glVertex2f(x-0.70f,-0.07f);

    glVertex2f(x-0.64f,-0.12f);
    glVertex2f(x-0.61f,-0.12f);
    glVertex2f(x-0.61f,-0.07f);
    glVertex2f(x-0.64f,-0.07f);

    glVertex2f(x-0.57f,-0.12f);
    glVertex2f(x-0.53f,-0.12f);
    glVertex2f(x-0.53f,-0.07f);
    glVertex2f(x-0.57f,-0.07f);

    glVertex2f(x-0.47f,-0.12f);
    glVertex2f(x-0.43f,-0.12f);
    glVertex2f(x-0.43f,-0.07f);
    glVertex2f(x-0.47f,-0.07f);

    glVertex2f(x-0.37f,-0.12f);
    glVertex2f(x-0.33f,-0.12f);
    glVertex2f(x-0.33f,-0.07f);
    glVertex2f(x - 0.37f,-0.07f);

    glVertex2f(x-0.2f,-0.12f);
    glVertex2f(x-0.27f,-0.12f);
    glVertex2f(x-0.27f,-0.07f);
    glVertex2f(x-0.2f,-0.07f);
    glEnd();

    float wheelRadius=0.04f;
    float cx[]={x-0.85f,x-0.75f,x-0.65f,x-0.55f,x-0.45f,x-0.35f,x-0.25f,x-0.15f};
    for (int i=0;i<8;i++){
        glColor3f(0.1f,0.1f,0.1f);
        glBegin(GL_POLYGON);
        for(int j=0;j<200;j++){
            float theta=2.0f*3.1416f*float(j)/float(200);
            float px=wheelRadius*cosf(theta);
            float py=wheelRadius*sinf(theta);
            glVertex2f(cx[i]+px,-0.27f+py);
        }
        glEnd();
    }
}
void drawRailwayLine(){
    glColor3ub(120,120,120);
    glLineWidth(10.0f);
    glBegin(GL_LINES);
    glVertex2f(-1.0f,-0.25f);
    glVertex2f(1.0f,-0.25f);
    glVertex2f(-1.0f,-0.35f);
    glVertex2f(1.0f,-0.35f);
    glEnd();

    glLineWidth(1.0f);
    glColor3ub(101,67,33);
    for(float x=-1.0f;x<=1.0f;x+=0.1f){
        glBegin(GL_QUADS);
        glVertex2f(x-0.02f,-0.35f);
        glVertex2f(x+0.02f,-0.35f);
        glVertex2f(x+0.02f,-0.25f);
        glVertex2f(x-0.02f,-0.25f);
        glEnd();
    }
}
void drawText(const char* text,float x,float y){
    glColor3f(0.0f,0.0f,0.0f);
    glRasterPos2f(x,y);
    for(int i=0;text[i]!='\0';i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,text[i]);
    }
}
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    // Sky
    glColor3ub(0,191,255);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f,0.0f);
    glVertex2f(1.0f,0.0f);
    glVertex2f(1.0f,1.0f);
    glVertex2f(-1.0f,1.0f);
    glEnd();

    drawSun(0.8f,0.8f);

    // Ground
    glBegin(GL_QUADS);
    glColor3ub(1,62,11);
    glVertex2f(-1.0f,0.0f);
    glVertex2f(1.0f,0.0f);
    glVertex2f(1.0f,-1.0f);
    glVertex2f(-1.0f,-1.0f);
    glEnd();

    //Hills
    glBegin(GL_TRIANGLES);
    glColor3ub(1,53,9);
    glVertex2f(-0.75f,-0.01f);
    glVertex2f(-0.5f,0.6f);
    glVertex2f(-0.3f,-0.01f);
    glEnd();

    drawTree(-0.6f,-0.1f);

    glBegin(GL_TRIANGLES);
    glColor3ub(1,27,5);
    glVertex2f(-1.2f,-0.1f);
    glVertex2f(-0.8f,0.5f);
    glVertex2f(-0.5f,-0.1f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(1,53,9);
    glVertex2f(-0.2f,-0.01f);
    glVertex2f(-0.01f,0.65f);
    glVertex2f(0.3f,-0.05f);
    glEnd();

    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.4f,-0.1f);
    glVertex2f(-0.37f,-0.1f);
    glVertex2f(-0.37f,0.6f);
    glVertex2f(-0.4f,0.6f);
    glEnd();

    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.37f,0.6f);
    glVertex2f(-0.37f,0.2f);
    glVertex2f(0.0f,0.2f);
    glVertex2f(0.0f,0.6f);
    glEnd();

    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0.0f,-0.1f);
    glVertex2f(0.03f,-0.1f);
    glVertex2f(0.03f,0.6f);
    glVertex2f(0.0f,0.6f);
    glEnd();

    drawText("Welcome To Tea-Heaven",-0.35f,0.4f);
    drawText("Sreemongal,Moulovibazar",-0.35f,0.3f);

    glBegin(GL_TRIANGLES);
    glColor3ub(2,72,12);
    glVertex2f(-0.4f,-0.1f);
    glVertex2f(-0.3f,0.3f);
    glVertex2f(-0.05f,-0.1f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(1,53,9);
    glVertex2f(-0.75f,-0.15f);
    glVertex2f(-0.5f,0.2f);
    glVertex2f(-0.25f,-0.15f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(2,71,12);
    glVertex2f(0.2f,-0.15f);
    glVertex2f(0.6f,0.6f);
    glVertex2f(0.8f,-0.15f);
    glEnd();

    drawTree(0.2f,-0.1f);

    glBegin(GL_TRIANGLES);
    glColor3ub(1,45,8);
    glVertex2f(0.1f,-0.15f);
    glVertex2f(0.4f,0.4f);
    glVertex2f(0.6f,-0.15f);
    glEnd();

    drawTree(0.7f,-0.1f);

    glBegin(GL_TRIANGLES);
    glColor3ub(2,71,12);
    glVertex2f(0.6f,-0.15f);
    glVertex2f(0.9f,0.6f);
    glVertex2f(1.2f,-0.15f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(1,45,8);
    glVertex2f(-0.2f,-0.15f);
    glVertex2f(0.1f,0.2f);
    glVertex2f(0.4f,-0.15f);
    glEnd();

    drawRailwayLine();
    drawTrain(trainPosX);

    drawTree(-0.9f,-0.6f);

    glBegin(GL_TRIANGLES);
    glColor3ub(1,45,8);
    glVertex2f(-0.75f,-0.6f);
    glVertex2f(-0.5f,-0.1f);
    glVertex2f(-0.25f,-0.6f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(2,71,12);
    glVertex2f(-1.2f,-0.8f);
    glVertex2f(-0.6f,-0.1f);
    glVertex2f(-0.4f,-0.8f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(1,45,8);
    glVertex2f(-1.2f,-1.0f);
    glVertex2f(-0.8f,-0.1f);
    glVertex2f(-0.3f,-1.0f);
    glEnd();

    drawTree(-0.6f,-0.6f);

    glBegin(GL_TRIANGLES);
    glColor3ub(1,45,8);
    glVertex2f(-0.8f,-1.0f);
    glVertex2f(-0.5f,-0.2f);
    glVertex2f(-0.3f,-1.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(1,555,8);
    glVertex2f(-0.4f,-0.8f);
    glVertex2f(-0.1f,-0.2f);
    glVertex2f(0.3f,-0.8f);
    glEnd();

    drawTree(0.0f,-0.7f);

    glBegin(GL_TRIANGLES);
    glColor3ub(1,55,8);
    glVertex2f(-0.2f,-0.9f);
    glVertex2f(0.2f,-0.2f);
    glVertex2f(0.5f,-0.9f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(1,5,8);
    glVertex2f(-0.8f,-1.0f);
    glVertex2f(-0.5f,-0.2f);
    glVertex2f(-0.3f,-1.0f);
    glEnd();

    drawTree(0.5f,-0.6f);

    glBegin(GL_TRIANGLES);
    glColor3ub(2,71,12);
    glVertex2f(0.1f,-1.0f);
    glVertex2f(0.4f,-0.4f);
    glVertex2f(0.8f,-1.0f);
    glEnd();

    drawTree(0.9f,-0.7f);

    glBegin(GL_TRIANGLES);
    glColor3ub(2,71,12);
    glVertex2f(0.5f,-1.0f);
    glVertex2f(0.9f,-0.5f);
    glVertex2f(1.2f,-1.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(1,5,8);
    glVertex2f(0.4f,-1.0f);
    glVertex2f(0.7f,-0.2f);
    glVertex2f(0.9f,-1.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(2,71,12);
    glVertex2f(-0.5f,-1.0f);
    glVertex2f(-0.2f,-0.5f);
    glVertex2f(0.2f,-1.0f);
    glEnd();

    drawCloud(cloudPosX,0.7f);
    drawCloud(cloudPosX+0.4f,0.8f);

    for(int i=0;i<3;i++){
    drawBird(birds[i].x,birds[i].y);
    birds[i].x+=birds[i].speed;
    if(birds[i].x>1.2f){
            birds[i].x=-1.2f;
    }
}
   glFlush();
}
void Seconddisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    // Sky
    glColor3ub(0,191,255);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f,0.0f);
    glVertex2f(1.0f,0.0f);
    glVertex2f(1.0f,1.0f);
    glVertex2f(-1.0f,1.0f);
    glEnd();
    drawSun(0.8f,0.8f);
    // Ground
    glBegin(GL_QUADS);
    glColor3ub(92,67,39);
    glVertex2f(-1.0f,0.0f);
    glVertex2f(1.0f,0.0f);
    glVertex2f(1.0f,-1.0f);
    glVertex2f(-1.0f,-1.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(78,53,36);
    glVertex2f(-0.85f,-0.05f);
    glVertex2f(-0.6f,0.0f);
    glVertex2f(-0.6f,0.3f);
    glVertex2f(-0.85f,0.25f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(102,51,0);
    glVertex2f(-1.0f,-0.03f);
    glVertex2f(-0.85f,-0.05f);
    glVertex2f(-0.85f,0.25f);
    glVertex2f(-1.0f,0.20f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(51,36,33);
    glVertex2f(-0.8f,-0.05f);
    glVertex2f(-0.62f,-0.01f);
    glVertex2f(-0.62f,0.28f);
    glVertex2f(-0.8f,0.23f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(150,150,150);
    glVertex2f(-1.02f,0.2f);
    glVertex2f(-0.8f,0.25f);
    glVertex2f(-0.55f, 0.3f);
    glVertex2f(-1.0f, 0.28f);
    glEnd();

    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.4f,-0.1f);
    glVertex2f(-0.37f,-0.1f);
    glVertex2f(-0.37f,0.6f);
    glVertex2f(-0.4f,0.6f);
    glEnd();
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.37f,0.6f);
    glVertex2f(-0.37f,0.3f);
    glVertex2f(0.0f,0.3f);
    glVertex2f(0.0f,0.6f);
    glEnd();
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0.0f,-0.1f);
    glVertex2f(0.03f,-0.1f);
    glVertex2f(0.03f,0.6f);
    glVertex2f(0.0f,0.6f);
    glEnd();
    drawText("Beauty Of Nature",-0.3f,0.5f);
    glBegin(GL_QUADS);
	glColor3ub(1,53,9);
    glVertex2f(-1.0f,-0.3f);
    glVertex2f(1.0f,-0.3f);
    glVertex2f(1.0f,-1.0);
    glVertex2f(-1.0f,-1.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(192,192,192);
    glVertex2f(-0.4f,-0.05f);
    glVertex2f(-0.68,-0.05f);
    glVertex2f(-0.7f,-0.09f);
    glVertex2f(-0.45f,-0.09f);
    glEnd();
    drawGlass(-0.55f,-0.06f,0.02f);
    drawGlass(-0.65f,-0.06f,0.015f);
    drawGlass(-0.48f,-0.07f,0.018f);

    glLineWidth(5.0f);
    glBegin(GL_LINES);
	glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-0.4f,-0.05f);
    glVertex2f(-0.4f,-0.15f);
    glVertex2f(-0.45f,-0.09);
    glVertex2f(-0.45f,-0.15f);
    glVertex2f(-0.7f,-0.09f);
    glVertex2f(-0.7f,-0.15f);
    glVertex2f(-0.68f,-0.09f);
    glVertex2f(-0.68f,-0.15f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(192,192,192);
    glVertex2f(-0.75f,-0.05f);
    glVertex2f(-0.8f,-0.09f);
    glVertex2f(-0.5f,0.0f);
    glVertex2f(-0.55f,0.01f);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(0.0f,0.0f,0.0f);
    glVertex2f(-0.75f,-0.08f);
    glVertex2f(-0.75f,-0.15f);
    glVertex2f(-0.5f,-0.0f);
    glVertex2f(-0.5f,-0.05f);
    glVertex2f(-0.8f,-0.09f);
    glVertex2f(-0.8f,-0.15f);
    glVertex2f(-0.55f,-0.01f);
    glVertex2f(-0.55f,-0.05f);
    glEnd();
    TeaStaller(-0.6f,-0.06f, 1.0f, 0.5f, 0.3f, 1.0f);
    glLineWidth(1.0f);

    glBegin(GL_TRIANGLES);
    glColor3ub(2,71,12);
    glVertex2f(0.2f,-0.15f);
    glVertex2f(0.6f,0.6f);
    glVertex2f(0.8f,-0.15f);
    glEnd();

    drawTree(0.2f,-0.1f);

    glBegin(GL_TRIANGLES);
    glColor3ub(1,45,8);
    glVertex2f(0.1f,-0.15f);
    glVertex2f(0.4f,0.4f);
    glVertex2f(0.6f,-0.15f);
    glEnd();

    drawTree(0.7f,-0.1f);

    glBegin(GL_TRIANGLES);
    glColor3ub(2,71,12);
    glVertex2f(0.6f,-0.15f);
    glVertex2f(0.9f,0.6f);
    glVertex2f(1.2f,-0.15f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(1,45,8);
    glVertex2f(-0.5f,0.0f);
    glVertex2f(-0.3f,0.45f);
    glVertex2f(0.0f,0.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(1,45,8);
    glVertex2f(-0.4f,-0.1f);
    glVertex2f(-0.05f,0.4f);
    glVertex2f(0.1f,-0.1f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(1,45,8);
    glVertex2f(-0.2f,-0.15f);
    glVertex2f(0.1f,0.2f);
    glVertex2f(0.4f,-0.15f);
    glEnd();

    drawTree(-0.9f,-0.6f);

    glBegin(GL_TRIANGLES);
    glColor3ub(2,71,12);
    glVertex2f(-0.3f,-0.15f);
    glVertex2f(0.0f,0.2f);
    glVertex2f(0.2f,-0.15f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(1,45,8);
    glVertex2f(-0.75f,-0.6f);
    glVertex2f(-0.5f,-0.1f);
    glVertex2f(-0.25f,-0.6f);
    glEnd();

    drawPerson(-0.3f,-0.6f,1.0f,1.0f,1.0f,1.0f);

    glBegin(GL_TRIANGLES);
    glColor3ub(2,71,12);
    glVertex2f(-1.2f,-0.8f);
    glVertex2f(-0.6f,-0.1f);
    glVertex2f(-0.4f,-0.8f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(1,45,8);
    glVertex2f(-1.2f,-1.0f);
    glVertex2f(-0.8f,-0.1f);
    glVertex2f(-0.3f,-1.0f);
    glEnd();

    drawTree(-0.6f,-0.6f);
    drawPerson(-0.8f,-0.6f,1.0f,0.5f,0.5f,0.5f);

    glBegin(GL_TRIANGLES);
    glColor3ub(1,45,8);
    glVertex2f(-0.8f,-1.0f);
    glVertex2f(-0.5f,-0.2f);
    glVertex2f(-0.3f,-1.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(1,555,8);
    glVertex2f(-0.4f,-0.8f);
    glVertex2f(-0.1f,-0.2f);
    glVertex2f(0.3f,-0.8f);
    glEnd();

    drawTree(0.0f,-0.7f);

    glBegin(GL_TRIANGLES);
    glColor3ub(1,55,8);
    glVertex2f(-0.2f,-0.9f);
    glVertex2f(0.2f,-0.2f);
    glVertex2f(0.5f,-0.9f);
    glEnd();

    drawPerson(0.1f,-0.6f,1.0f,0.0f,0.0f,0.0f);

    glBegin(GL_TRIANGLES);
    glColor3ub(1,5,8);
    glVertex2f(-0.8f,-1.0f);
    glVertex2f(-0.5f,-0.2f);
    glVertex2f(-0.3f,-1.0f);
    glEnd();

    drawTree(0.5f,-0.6f);

    glBegin(GL_TRIANGLES);
    glColor3ub(2,71,12);
    glVertex2f(0.1f,-1.0f);
    glVertex2f(0.4f,-0.4f);
    glVertex2f(0.8f,-1.0f);
    glEnd();
    drawTree(0.9f,-0.7f);
    drawPerson(0.9f,-0.5f,1.0f,1.0f,0.0f,0.0f);
    glBegin(GL_TRIANGLES);
    glColor3ub(2,71,12);
    glVertex2f(0.5f,-1.0f);
    glVertex2f(0.9f,-0.5f);
    glVertex2f(1.2f,-1.0f);
    glEnd();

    drawPerson(-0.6f,-0.8f,1.0f,1.0f,0.0f,0.0f);

    glBegin(GL_TRIANGLES);
    glColor3ub(1,5,8);
    glVertex2f(0.4f,-1.0f);
    glVertex2f(0.7f,-0.2f);
    glVertex2f(0.9f,-1.0f);
    glEnd();

    drawPerson(0.45f,-0.8f,1.0f,1.0f,1.0f,.0f);

    glBegin(GL_TRIANGLES);
    glColor3ub(2,71,12);
    glVertex2f(-0.5f,-1.0f);
    glVertex2f(-0.2f,-0.5f);
    glVertex2f(0.2f,-1.0f);
    glEnd();

    drawCloud(cloudPosX,0.7f);
    drawCloud(cloudPosX+0.4f,0.8f);

    for(int i=0;i<3;i++){
    drawBird(birds[i].x,birds[i].y);
    birds[i].x+=birds[i].speed;
    if(birds[i].x>1.2f){
            birds[i].x=-1.2f;
    }
}
   glFlush();
}
void Thirddisplay() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    glColor3ub(200,200,100);
    glVertex2f(-1.0f,1.0f);
    glVertex2f(1.0,0.0f);
    glVertex2f(0.0f,0.0f);
    glVertex2f(-1.0f,-1.2f);
    glEnd();
    glLineWidth(7.5);
    glBegin(GL_QUADS);
    glColor3ub(139,69,19);
    glVertex2f(-0.3f, 0.0f);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(-1.0f, -0.0f);
    glVertex2f(-1.0f, -0.05f);
    glVertex2f(-1.0f, -0.05f);
    glVertex2f(-0.3f, -0.05f);
    glVertex2f(-0.3f, -0.05f);
    glVertex2f(-0.3f, -0.0f);
    glVertex2f(-0.25f, 0.0f);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(-1.0f, -0.1f);
    glVertex2f(-1.0f, -0.1f);
    glVertex2f(-0.25f, -0.1f);
    glVertex2f(-0.25f, -0.1f);
    glVertex2f(-0.25f, -0.0f);
    glVertex2f(-0.25f, -0.1f);
    glVertex2f(-1.0f, -0.1f);
    glVertex2f(-1.0f, -0.1f);
    glVertex2f(-1.0f, -0.4f);
    glVertex2f(-1.0f, -0.4f);
    glVertex2f(-0.25f, -0.4f);
    glVertex2f(-0.25f, -0.4f);
    glVertex2f(-0.25f, -0.1f);
    glVertex2f(-0.25f, -0.4f);
    glVertex2f(-1.0f, -0.4f);
    glVertex2f(-1.0f, -0.4f);
    glVertex2f(-1.0f, -0.45f);
    glVertex2f(-1.0f, -0.45f);
    glVertex2f(-0.25f, -0.45f);
    glVertex2f(-0.25f, -0.45f);
    glVertex2f(-0.25f, -0.4f);
    glVertex2f(-0.5f, -0.0f);
    glVertex2f(-0.9f, -0.0f);
    glVertex2f(-0.9f, -0.0f);
    glVertex2f(-0.9f, 0.2f);
    glVertex2f(-0.9f, 0.2f);
    glVertex2f(-0.5f, 0.2f);
    glVertex2f(-0.5f, 0.2f);
    glVertex2f(-0.5f, -0.0f);
    glVertex2f(-0.5f, 0.2f);
    glVertex2f(-0.9f, 0.2f);
    glVertex2f(-0.9f, 0.2f);
    glVertex2f(-0.92f, 0.3f);
    glVertex2f(-0.92f, 0.3f);
    glVertex2f(-0.48f, 0.3f);
    glVertex2f(-0.48f, 0.3f);
    glVertex2f(-0.5f, 0.2f);
    glVertex2f(-0.5f, 0.1f);
    glVertex2f(-0.9f, 0.1f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f,1.0f,1.0f);
    glVertex2f(-0.55f, 0.22f);
    glVertex2f(-0.6f, 0.22f);
    glVertex2f(-0.6f, 0.28f);
    glVertex2f(-0.55f, 0.28f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.8f, 0.22f);
    glVertex2f(-0.85f, 0.22f);
    glVertex2f(-0.85f, 0.28f);
    glVertex2f(-0.8f, 0.28f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.6f, 0.1f);
    glVertex2f(-0.62f, 0.1f);
    glVertex2f(-0.62f, 0.2f);
    glVertex2f(-0.6f, 0.2f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.7f, 0.1f);
    glVertex2f(-0.72f, 0.1f);
    glVertex2f(-0.72f, 0.2f);
    glVertex2f(-0.7f, 0.2f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.8f, 0.1f);
    glVertex2f(-0.82f, 0.1f);
    glVertex2f(-0.82f, 0.2f);
    glVertex2f(-0.8f, 0.2f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.35f, 0.0f);
    glVertex2f(-0.37f, 0.0f);
    glVertex2f(-0.37f, -0.05f);
    glVertex2f(-0.35f, -0.05f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.4f, 0.0f);
    glVertex2f(-0.42f, 0.0f);
    glVertex2f(-0.42f, -0.05f);
    glVertex2f(-0.4f, -0.05f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, 0.0f);
    glVertex2f(-0.52f, 0.0f);
    glVertex2f(-0.52f, -0.05f);
    glVertex2f(-0.5f, -0.05f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.6f, 0.0f);
    glVertex2f(-0.62f, 0.0f);
    glVertex2f(-0.62f,- 0.05f);
    glVertex2f(-0.6f, -0.05f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.7f, 0.0f);
    glVertex2f(-0.72f, 0.0f);
    glVertex2f(-0.72f, -0.05f);
    glVertex2f(-0.7f, -0.05f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.8f, 0.0f);
    glVertex2f(-0.82f, 0.0f);
    glVertex2f(-0.82f, -0.05f);
    glVertex2f(-0.8f, -0.05f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.9f, 0.0f);
    glVertex2f(-0.92f, 0.0f);
    glVertex2f(-0.92f, -0.05f);
    glVertex2f(-0.9f, -0.05f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.95f, 0.0f);
    glVertex2f(-0.97f, 0.0f);
    glVertex2f(-0.97f, -0.05f);
    glVertex2f(-0.95f, -0.05f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.9f, -0.1f);
    glVertex2f(-0.95f, -0.1f);
    glVertex2f(-0.95f, -0.4f);
    glVertex2f(-0.9f, -0.4f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.1f);
    glVertex2f(-0.45f, -0.1f);
    glVertex2f(-0.45f, -0.4f);
    glVertex2f(-0.5f, -0.4f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.55f, -0.1f);
    glVertex2f(-0.85f, -0.1f);
    glVertex2f(-0.85f, -0.4f);
    glVertex2f(-0.55f, -0.4f);
    glEnd();
    // road
    glBegin(GL_QUADS);
    glColor3ub(50,50,50);
    glVertex2f(1.0f, -0.5f);
    glVertex2f(-1.0f, -0.5f);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glEnd();

    //Airport road
    glBegin(GL_QUADS);
    glColor3ub(150,150,150);
    glVertex2f(-1.0f,0.25f);
    glVertex2f(-1.0f,1.0f);
    glVertex2f(-0.4f,1.0f);
    glVertex2f(0.3f,1.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(30,144,255);
    glVertex2f(-0.5f,0.3f);
    glVertex2f(-0.9f,0.3f);
    glVertex2f(-0.9f,0.5f);
    glVertex2f(-0.5f,0.6f);
    glEnd();

    drawText("Airport",-0.75f,0.4f);

    glBegin(GL_LINES);
    glColor3f(1.0f,1.0f,1.0f);
    glVertex2f(-1.0f, 0.78f);
    glVertex2f(-0.85f, 0.85f);
    glVertex2f(-0.75f, 0.89f);
    glVertex2f(-0.48f, 1.0f);

    glVertex2f(-1.0f, 0.5f);
    glVertex2f(-0.85f, 0.59f);
    glVertex2f(-0.75f, 0.65f);
    glVertex2f(-0.5f, 0.79f);

    glVertex2f(-0.4f, 0.84f);
    glVertex2f(-0.1f, 1.0f);
    glEnd();
    Plane();
    // blue rectangle ground
    glBegin(GL_QUADS);
    glColor3f(0.055f,0.30f,0.45f);
    glVertex2f(0.0f, -0.2f);
    glVertex2f(0.0f, -0.5f);
    glVertex2f(1.0f, -0.2f);
    glVertex2f(1.0f, -0.5f);
    glEnd();
    // two red triangles
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); // red
    glVertex2f(0.0f, -0.2f);
    glVertex2f(0.2f, 0.1f);
    glVertex2f(0.4f,-0.2f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); // red
    glVertex2f(0.4f,-0.2f);
    glVertex2f(0.6f, 0.2f);
    glVertex2f(0.8f, -0.2f);
    glEnd();
    // STADIUM BASE
    glBegin(GL_QUADS);
    glColor3ub(60, 60, 60);
    glVertex2f(0.3f, 0.0f);
    glVertex2f(0.9f, 0.0f);
    glVertex2f(0.9f, -0.4f);
    glVertex2f(0.3f, -0.4f);
    glEnd();
    drawBackground();
    AirportPlane();

    // STADIUM ENTRANCE (GATE)
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);
    glVertex2f(0.55f, -0.4f);
    glVertex2f(0.65f, -0.4f);
    glVertex2f(0.65f, -0.25f);
    glVertex2f(0.55f, -0.25f);
    glEnd();

    // Draw green circle (stadium ground)
    glLineWidth(2.5);
    glColor3f(0.0f, 0.7f, 0.0f);  // green
    drawCircleForStadium(0.7f, 0.8f, 0.7f, 100);
     glLineWidth(2.5);
    glColor3f(0.0f, 0.0f, 1.0f);
    drawCircleForStadium(0.7f, 0.8f, 0.8f, 100);
    glLineWidth(2.5);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircleForStadium(0.7f, 0.8f, 0.8f, 100);

    glLineWidth(2.5);
    glColor3f(0.0f, 0.7f, 0.0f);  // green
    drawCircleForStadium(0.7f, 0.8f, 0.7f, 100);

    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.18f,0.1f);
    glVertex2f(-0.15f,0.1f);
    glVertex2f(-0.15f,0.6f);
    glVertex2f(-0.18f,0.6f);
    glEnd();

    glColor3ub(204,50,50);
    glBegin(GL_QUADS);
    glVertex2f(-0.15f,0.3f);
    glVertex2f(-0.15f,0.6f);
    glVertex2f(0.04f,0.7f);
    glVertex2f(0.04f,0.4f);
    glEnd();

    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0.04f,0.2f);
    glVertex2f(0.07f,0.2f);
    glVertex2f(0.07f,0.7f);
    glVertex2f(0.04f,0.7f);
    glEnd();

    drawText("Stadium",-0.10f,0.5f);
    // STADIUM UPPER STANDS
    glBegin(GL_QUADS);
    glColor3ub(100, 0, 0);
    glVertex2f(0.35f, 0.0f);
    glVertex2f(0.85f, 0.0f);
    glVertex2f(0.75f, 0.25f);
    glVertex2f(0.45f, 0.25f);
    glEnd();
    glBegin(GL_LINES);
    glColor3ub(1, 1, 0);
    glVertex2f(0.62f, 0.25f);
    glVertex2f(0.62f, 0.35f);
    glEnd();
     glBegin(GL_TRIANGLES);
    glColor3ub(255, 0, 0); // green flag
    glVertex2f(0.62f, 0.35f);
    glVertex2f(0.67f, 0.33f);
    glVertex2f(0.62f, 0.31f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(0, 0, 0);
    glVertex2f(0.3f, 0.3f);
    glVertex2f(0.3f, 0.35f);
    glVertex2f(0.4f, 0.35f);
    glVertex2f(0.4f, 0.3f);
    glEnd();
    glBegin(GL_LINES);
    glColor3ub(0, 0, 0);
    glVertex2f(0.39f, 0.1f);
    glVertex2f(0.39f, 0.3f);
    glEnd();

    drawBus(busPos1,0.5f, -0.7f, 1.0f, 0.0f, 0.0f);
    drawBus(busPos2,0.5f, -0.7f, 0.0f, 1.0f, 0.0f);
    Plane();

    glFlush();

}
void timer(int value){
    if(scene==0){
    trainPosX+=trainSpeed;
    if(trainPosX>2.0f){
        trainPosX=-1.2f;
    }
    cloudPosX+=cloudSpeed;
    if(cloudPosX>1.2f){
        cloudPosX=-1.2f;
    }
    smokeY+=0.005f;
    if(smokeY>0.4f){
        smokeY=0.0f;
    }
    }
    else if(scene==1){
    cloudPosX+=cloudSpeed;
    if(cloudPosX>1.2f){
        cloudPosX=-1.2f;
    }
     smokeY+=0.005f;
    if(smokeY>0.4f){
        smokeY=0.0f;
    }
    if(animateHand){
        if(handUp){
            handAngle += 1.0f;
            if(handAngle >= 30.0f)
                handUp = false;
        } else{
            handAngle -= 1.0f;
            if(handAngle <= -30.0f)
                handUp = true;
        }
    }
    }
    else if(scene==2){
     busPos1+=busSpeed1;
    if (busPos1<-1.6f)
        busPos1=1.2f;
        busPos2+=busSpeed2;
    if (busPos2<-1.7f)
        busPos2=1.2f;
    if (position1<-1.5f)
        position1=1.0f;
        position1-=speed1;

   float roadStartX = -1.0f;
    float roadStartY = 0.3f;
    float roadEndX = 0.3f;
    float roadEndY=1.0f;

   float dirX = roadStartX - roadEndX;
   float dirY = roadStartY - roadEndY;
   float length = sqrt(dirX * dirX + dirY * dirY);
   dirX /= length;
   dirY /= length;

   float speed=0.002f;
  if(planeY>roadStartY){
    planeY+=planeSpeedY;
    planeX+=planeSpeedX;
    if(planeY<-1.2f || planeX<-1.2f){
        planeY=1.0f;
        planeX=1.0f;
    }
} else {
    planeY = roadStartY;
    planeSpeedY = 0.0f;
    planeX += dirX * speed;
    planeY += dirY * speed;
}
    }
    glutPostRedisplay();
    glutTimerFunc(20,timer,0);
}
void startTimerOnce(){
    if (!isTimerRunning){
        glutTimerFunc(20,timer,0);
        isTimerRunning=true;
    }
}
void mouse(int button,int state,int x,int y){
    if(button==GLUT_LEFT_BUTTON &&state==GLUT_DOWN){
        trainspeedback=trainSpeed;
        cloudspeedback=cloudSpeed;
        busSpeed1back=busSpeed1;
        busSpeed2back=busSpeed2;
        speed1back=speed1;
        animateHand=false;
        trainSpeed=0.0f;
        cloudSpeed=0.0f;
        busSpeed1=0.0f;
        busSpeed2=0.0f;
        speed1=0.0f;
    }
    if(button==GLUT_RIGHT_BUTTON&&state==GLUT_DOWN){
     trainSpeed=trainspeedback;
     cloudSpeed=cloudspeedback;
     animateHand=true;
     busSpeed1=busSpeed1back;
     busSpeed2=busSpeed2back;
     speed1=speed1back;
    }
}
void handleKeypress(unsigned char key,int x,int y){
    switch(key){
    case 'A':
        trainspeedback=trainSpeed;
        cloudspeedback=cloudSpeed;
        busSpeed1back=busSpeed1;
        busSpeed2back=busSpeed2;
        speed1back=speed1;
        animateHand=false;
        trainSpeed=0.0f;
        cloudSpeed=0.0f;
        busSpeed1=0.0f;
        busSpeed2=0.0f;
        speed1=0.0f;
        break;
    case 'B':
        trainSpeed=trainspeedback;
        cloudSpeed=cloudspeedback;
        animateHand=true;
        busSpeed1=busSpeed1back;
        busSpeed2=busSpeed2back;
        speed1=speed1back;
        break;
    case 'C':
        trainSpeed-=0.002f;
        cloudSpeed-=0.001f;
        if(trainSpeed<0.0f)
            trainSpeed=0.0f;
        if(cloudSpeed<0.0f)
            cloudSpeed = 0.0f;
        break;
    case 'D':
        trainSpeed+=0.002f;
        cloudSpeed+=0.001f;
        break;
    case 'f':
        scene=0;
        glutDisplayFunc(display);
        startTimerOnce();
        break;
    case 's':
        scene=1;
        glutDisplayFunc(Seconddisplay);
        startTimerOnce();
        break;
    case 't':
        scene=2;
        glutDisplayFunc(Thirddisplay);
        startTimerOnce();
        break;
    }
    glutPostRedisplay();
}
void timer3(int value){
    scene++;
    if(scene>2)
        scene=0;

    switch(scene){
        case 0:
            glutDisplayFunc(display);
            break;
        case 1:
            glutDisplayFunc(Seconddisplay);
            break;
        case 2:
            glutDisplayFunc(Thirddisplay);
            break;
    }

    glutPostRedisplay();
    glutTimerFunc(20000,timer3,0);
}
int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800,600);
    glutCreateWindow("Scenic View");
    glClearColor(0.4f,0.8f,1.0f,1.0f);
    startTimerOnce();
    glutDisplayFunc(display);
    glutTimerFunc(0,timer,0);
    glutTimerFunc(15000,timer3,0);
    glutKeyboardFunc(handleKeypress);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
