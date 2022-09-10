/* Macros para Display, Buffer e posições */

#define DISPLAY_HEIGHT  800
#define DISPLAY_WIDTH   495

/* Macros para bola, velocidade, posições */
#define BALL_SIZE    10
#define GRID         4.0
#define SPEED        10
#define SPEED2       (SPEED * 1.05)
#define BALL_FLOOR   (DISPLAY_HEIGHT - BALL_SIZE)
#define TAM          2

/* Tamanho da matriz de Blocos*/
#define SIZE_BLOCK_COLUMNS 7
#define SIZE_BLOCK_LINES   9

/* Largura, altura e margem dos blocos */
#define SQUARE_WIDTH    70
#define SQUARE_HEIGHT   70
#define MARGIN   5

#define FONT_NUM_SIZE 50
#define FONT_BALL_SIZE 18
#define FONT_GAMEOVER_SIZE 200
#define FONT_LOGO_SIZE 100

#define PLAY_BUTTON_POS_X1 (DISPLAY_WIDTH/2-100)
#define PLAY_BUTTON_POS_Y1 (DISPLAY_HEIGHT/2)
#define PLAY_BUTTON_POS_X2 (DISPLAY_WIDTH/2+100)
#define PLAY_BUTTON_POS_Y2 (DISPLAY_HEIGHT/2+40)
#define ROUNDED_SIZE       20

#define BOTAO_ROSA         234, 35, 94
#define MAX_ALIVE 4
#define LIMIT_Y_GAME      (DISPLAY_HEIGHT/8)

#define MARGIN_TIMER 3

#define FONT_SCORE_GAMEOVER_SIZE 100

/* Structs */
typedef struct ball
{
    float x,y;
    float dx,dy;
    bool ground, moving, launched;
} 
ball_t;

typedef struct ballSight
{
    // ball_t balls;
    float x,y;
}
ballSight_t;

typedef struct text
{
    float x,y;
} 
text_t;

typedef struct square
{
    float x1,y1,x2,y2;
    int life;
    bool alive;
    text_t text;
} 
square_t;

typedef struct ballBlock
{
    float x,y;
    bool alive;
} 
ballBlock_t;

typedef struct mouse 
{
    float x,y;
    int button_state;
} 
mouse_t;

typedef struct distance
{
    float x,y;
    float angle;
} 
distance_t;

/* Collide Functions */

void blocksInit(square_t square[SIZE_BLOCK_LINES][SIZE_BLOCK_COLUMNS], ballBlock_t ballBlock[SIZE_BLOCK_LINES][SIZE_BLOCK_COLUMNS]);

bool collide(int p1_x1, int p1_y1, int p1_x2, int p1_y2, int p2_x1, int p2_y1, int p2_x2, int p2_y2);

bool collide_vertical(float square_x1, float square_y1, float square_x2, float square_y2, float ball_x, float ball_y, float dx, float dy);

bool collide_lateral(float square_x1, float square_y1, float square_x2, float square_y2, float ball_x, float ball_y, float dx, float dy);

int click_centre_button(ALLEGRO_MOUSE_STATE *mouse_state);

int click_side_button(ALLEGRO_MOUSE_STATE *mouse_state);

int scoreCompare(int lastScore);

ball_t* createBall(float x, float y);

ball_t** initBallsArray(ball_t* ball);

ball_t** addBalls(ball_t** balls, ball_t * aux, int tam);

int ballsGround(ball_t** balls, int tam);

void resetBalls(ball_t** balls, float initialX, int tam);
