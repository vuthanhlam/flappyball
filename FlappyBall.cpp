#include <iostream>
#include <SDL.h>
#include <ctime>
#include <cmath>
#include <stdio.h>

using namespace std;

struct fillRect
{
    int x,r1,r2;
    int widthRect;
};

int result = 0,highScore = 0;
bool check = 1;
string s[15];

fillRect b[5];
SDL_Renderer* renderer;
SDL_Window* window;
SDL_Event control;
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);

const int r = 20;
const int WIDTH = 800;
const int HEIGHT = 600;

int kc_( int x, int y,int X, int Y);
void drawRect(int x,int h,int heighRect,int widthRect);
void drawCircle(int x, int y);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();
void initRect();
void play();
void move_rect(fillRect &a);
void change();
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
void drawScore();
void paint(int x,int k);

bool checkTouch(int x,int y,int r);
bool playAgain();

int  main(int argc, char* argv[])
{
    freopen("picture.txt","r",stdin);
    int i = 0;
    while (cin >> s[i]) i ++;
    srand(time(NULL));
    initSDL(window, renderer);
    //drawScore();
    //SDL_RenderPresent(renderer);
    while (check){
        result = 0;
        play();
    }
    SDL_Delay(500);
    quitSDL(window, renderer);
    return 0;
}

void play()
{
    initRect();
    SDL_Texture *background = loadTexture("background.bmp", renderer);
    SDL_PollEvent( &control);
    int x = WIDTH/2-100;
    int y = HEIGHT/2;
    int k = 3;
    while (true){
        SDL_RenderClear(renderer);
        renderTexture(background, renderer, 0, 0, WIDTH, HEIGHT);
        for(int i=0;i < 5; i++) move_rect(b[i]);
        // kiểm tra để tăng điểmrenderTexture(background, renderer, 0, 0, WIDTH, HEIGHT);
        if (b[1].x + r == x)
            if (k>0) k--;
            else result ++;
        // kiểm tra chu kì của các ống khói
        if (b[0].widthRect <= 0) change();
        // kiểm tra tác động của chuột
        if( SDL_PollEvent( &control) && control.type==SDL_MOUSEBUTTONDOWN &&
        control.button.button==SDL_BUTTON_LEFT) y-=50;
        else y += 3;
        drawCircle(x,y);
        // vẽ ảnh thể hiện điểm
        highScore = max(highScore,result);
        drawScore();
        SDL_RenderPresent(renderer);
        if (y<=r || y+r >= HEIGHT) break;
        if (checkTouch(x,y,r)) break;
    }
    SDL_Delay(1500);
    check = playAgain();
}

void drawScore()
{
    paint(1,result);
    paint(401,highScore);
}

void paint(int x,int k)
{
    SDL_Texture *picture;
    if (x==1) picture = loadTexture("score1.bmp", renderer);
    else picture = loadTexture("highscore.bmp", renderer);
    renderTexture(picture, renderer, x, 0, 310, 62);
    for (int i=2; i>=0; i--)
    {
        int tmp = k % 10;
        k/=10;
        picture = loadTexture(s[tmp], renderer);
        int l = x + 310 + i*30;
        renderTexture(picture, renderer, l, 0, 30, 62);
    }
}

bool playAgain()
{

    SDL_Texture *picture = loadTexture("playagain.bmp",renderer);
    renderTexture(picture,renderer,200,200,400,200);
    SDL_RenderPresent(renderer);
    while (true)
    {
        if( SDL_PollEvent( &control) && control.type==SDL_MOUSEBUTTONDOWN &&
        control.button.button==SDL_BUTTON_LEFT)
            if (control.motion.x >= 200 && control.motion.x <= 400
                && control.motion.y >= 300 && control.motion.y <= 400) return 1;
            else
                if (control.motion.x > 400 && control.motion.x <= 600
                && control.motion.y >= 300 && control.motion.y <= 400) return 0;
    }
}

void change()
{
    for(int i=0;i<4;i++) b[i] = b[i+1];
    b[4].x = 950;
    b[4].r1 = rand() % 200 + 100;
    b[4].r2 = b[4].r1 + 150;
    b[4].widthRect = 0;
}

void move_rect(fillRect& a)
{
    if (a.x>800){
        a.x-=2;
        return;
    };
    if (a.x==0) a.widthRect -= 2;
    else a.x-= 2;
    if (a.x>750) a.widthRect+=2;
    drawRect(a.x,0,a.r1,a.widthRect);
    drawRect(a.x,a.r2,HEIGHT-a.r2,a.widthRect);
}

void drawRect(int x,int h,int heighRect,int widthRect)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = h;
    rect.w = widthRect;
    rect.h = heighRect;
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 0);
    SDL_RenderFillRect(renderer, &rect);
}

int kc_( int x, int y,int X, int Y)
{
    return ( X-x)*( X-x)+ ( Y-y)*( Y-y);
}

bool checkTouch(int x,int y,int r)
{
    if (b[1].x > x+r) return 0;
    if (b[1].x + 49 < x-r) return 0;
    if( y <= b[1].r1 && x+r >= b[1].x) return 1;
    if( y >= b[1].r2 && x+r >= b[1].x) return 1;
    if( ( b[1].x <= x && x<=b[1].x+49) && (y+r>=b[1].r2 || y-r<=b[1].r1) )return 1;
    if( kc_( x, y, b[1].x, b[1].r1)<=r*r) return 1;
    if( kc_( x, y, b[1].x+49, b[1].r1)<=r*r) return 1;
    if( kc_( x, y, b[1].x, b[1].r2)<=r*r) return 1;
    if( kc_( x, y, b[1].x+49, b[1].r2)<=r*r) return 1;
    return 0;
}

void initRect()
{
    for (int i=0;i<5;i++)
    {
        b[i].x = 200*i;
        b[i].r1 = 0;
        b[i].r2 = 600;
        b[i].widthRect = 50;
    }
    b[4].widthRect = 0;
}

void drawCircle(int x,int y)
{
    int j;
    for(int i= x-r;i<=x+r;i++)    {
        j = sqrt(r*r - (x-i)*(x-i));
        SDL_SetRenderDrawColor(renderer, 153, 0, 0, 0);
        SDL_RenderDrawLine(renderer,i,j+y,i,-j+y);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    window = SDL_CreateWindow("Flappy Ball", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
              SDL_RENDERER_PRESENTVSYNC);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
	SDL_Texture *texture;
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
    SDL_FreeSurface(loadedImage);
    return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
    dst.w = w;
    dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}
