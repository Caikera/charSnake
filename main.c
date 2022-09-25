#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

typedef unsigned int  uint_32;
typedef unsigned char uint_8;
typedef char int_8;

#define MAP_WIDTH  40
#define MAP_HEIGHT 20
#define LEN        200

HANDLE v_out_1,v_out_2;
COORD coord={0,0};


typedef struct snake_body{
    uint_8 pos_x[LEN];
    uint_8 pos_y[LEN];
    uint_8 shape[LEN];
    int    vel_x;
    int    vel_y;
    int_8 len;
} snake_body_template;

typedef struct map_def{
    uint_8 map[MAP_HEIGHT][MAP_WIDTH];
    uint_8 map_width;
    uint_8 map_height;
} map_def_template;

typedef struct fruit_def{
    uint_8 being;
    uint_8 pos_x;
    uint_8 pos_y;
} fruit_def_template;

void pos_update(snake_body_template * snake_body_0);
void map_init(map_def_template *main_map);
void mapping(snake_body_template * snake_body_0,fruit_def_template *fruit,map_def_template * main_map);
void key_turn(snake_body_template *snake_body_0);
void fruiting(snake_body_template *snake_body_0,fruit_def_template *fruit);
uint_8 eating(snake_body_template *snake, fruit_def_template *fruit);
void all_print(map_def_template *main_map);
uint_8 overing(snake_body_template *snake);
void show(map_def_template *map,snake_body_template *snake);

DWORD nLength;

void v_buf_setup(){
    v_out_1 = CreateConsoleScreenBuffer(
            GENERIC_WRITE,
            FILE_SHARE_WRITE,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL
    );
    v_out_2 = CreateConsoleScreenBuffer(
            GENERIC_WRITE,
            FILE_SHARE_WRITE,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL
    );
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = 0;
    cci.dwSize = 1;
    SetConsoleCursorInfo(v_out_1, &cci);
    SetConsoleCursorInfo(v_out_2, &cci);
}

int main() {
    unsigned int dl=0;
    printf("Enter difficult level to restart (1,2,3,4,5,10,20,25,50,100,250,500) :");
    scanf("%u",&dl);
    system("cls");
    while(1){
        snake_body_template snake_body_0={
                {12,11,10,9,
                 8,7,0,0,
                 0,0,0,0,
                 0,0,0,0},
                {6,6,6,6,
                 6,6,0,0,
                 0,0,0,0,
                 0,0,0,0},
                {'O','O','O','O',
                 'O','O','O','O',
                 'O','O','O','O',
                 'O','O','O','O',},
                1,0,1
        };
        v_buf_setup();
        map_def_template main_map;
        fruit_def_template  fruit = {0, 0, 0};
        map_init(&main_map);
        fruiting(&snake_body_0,&fruit);
        while(1){
            Sleep(500/dl);
            key_turn(&snake_body_0);
            if(!eating(&snake_body_0,&fruit))
                pos_update(&snake_body_0);
            fruiting(&snake_body_0,&fruit);
            map_init(&main_map);
            mapping(&snake_body_0,&fruit,&main_map);
            //all_print(&main_map);
            show(&main_map,&snake_body_0);
            //printf("vel_x=%d, vel_y=%d, len=%d",snake_body_0.vel_x,snake_body_0.vel_y,snake_body_0.len);
            if(overing(&snake_body_0)) break;
        }
        //SetConsoleActiveScreenBuffer(NULL);
        CloseHandle(v_out_1);
        CloseHandle(v_out_2);
        all_print(&main_map);
        printf("\n");
        printf("======GAME OVER======");
        printf("Your score is %d.\n",snake_body_0.len);
        printf("Enter difficult level to restart (1,2,3,4,5,10,20,25,50,100,250,500) :");
        scanf("%u",&dl);
    }
    return 0;
}

void pos_update(snake_body_template * snake_body_0){
    uint_8 i;
    uint_8 pos_x_buf[LEN];
    uint_8 pos_y_buf[LEN];
    for(i=0;i<=snake_body_0->len-1;i++){
        pos_x_buf[i]=snake_body_0->pos_x[i];
        pos_y_buf[i]=snake_body_0->pos_y[i];
        if(!i){
            if( snake_body_0->pos_x[i]==0 && snake_body_0->vel_x==-1 )
                snake_body_0->pos_x[i]=MAP_WIDTH-1;
            else if( snake_body_0->pos_x[i]==MAP_WIDTH-1 && snake_body_0->vel_x==1 )
                snake_body_0->pos_x[i]=0;
            else
                snake_body_0->pos_x[i]+=snake_body_0->vel_x;

            if( snake_body_0->pos_y[i]==0 && snake_body_0->vel_y==-1 )
                snake_body_0->pos_y[i]=MAP_HEIGHT-1;
            else if( snake_body_0->pos_y[i]==MAP_HEIGHT-1 && snake_body_0->vel_y==1 )
                snake_body_0->pos_y[i]=0;
            else
                snake_body_0->pos_y[i]+=snake_body_0->vel_y;
        }
        else{
            pos_x_buf[i]=snake_body_0->pos_x[i];
            pos_y_buf[i]=snake_body_0->pos_y[i];
            snake_body_0->pos_x[i]=pos_x_buf[i-1];
            snake_body_0->pos_y[i]=pos_y_buf[i-1];
        }
    }
    return;
}

void map_init(map_def_template *main_map){
    uint_8 i,j;
    main_map->map_height=MAP_HEIGHT;
    main_map->map_width=MAP_WIDTH;
    for(i=0;i<=MAP_HEIGHT-1;i++)
        for(j=0;j<=MAP_WIDTH-1;j++)
            main_map->map[i][j]=0;
    return;
}

void mapping(snake_body_template * snake_body_0,fruit_def_template *fruit,map_def_template * main_map){
    uint_8 i;
    for(i=0;i<=snake_body_0->len-1;i++){
        //printf("i=%d  ",i);
        //printf("y=%d ,x=%d  ",snake_body_0->pos_y[i],snake_body_0->pos_x[i]);
        main_map->map[snake_body_0->pos_y[i]][snake_body_0->pos_x[i]]=snake_body_0->shape[i];
    }
    main_map->map[fruit->pos_y][fruit->pos_x]=fruit->being;
    //printf("\n");
    return;
}

void key_turn(snake_body_template *snake_body_0){
    uint_8 key;
    key=0;
    if(_kbhit()){
        key=_getch();
        switch(key){
            case('w'): if(snake_body_0->vel_y!=1) snake_body_0->vel_y=-1; snake_body_0->vel_x=0; break;
            case('s'): if(snake_body_0->vel_y!=-1)  snake_body_0->vel_y=1;  snake_body_0->vel_x=0; break;
            case('a'): if(snake_body_0->vel_x!=1) snake_body_0->vel_x=-1; snake_body_0->vel_y=0; break;
            case('d'): if(snake_body_0->vel_x!=-1)  snake_body_0->vel_x=1;  snake_body_0->vel_y=0; break;
            case('W'): if(snake_body_0->vel_y!=1) snake_body_0->vel_y=-1; snake_body_0->vel_x=0; break;
            case('S'): if(snake_body_0->vel_y!=-1)  snake_body_0->vel_y=1;  snake_body_0->vel_x=0; break;
            case('A'): if(snake_body_0->vel_x!=1) snake_body_0->vel_x=-1; snake_body_0->vel_y=0; break;
            case('D'): if(snake_body_0->vel_x!=-1)  snake_body_0->vel_x=1;  snake_body_0->vel_y=0; break;
        }
    }
    return;
}

void fruiting(snake_body_template *snake_body_0,fruit_def_template *fruit){
    uint_8  snake_pos_line[LEN] = {0};
    uint_32 fruit_pos_line;
    uint_8  i;
    uint_8  overlap;
    if(!fruit->being) {
        for (i = 0; i <= snake_body_0->len-1; i++)
            snake_pos_line[i] = (snake_body_0->pos_y[i]) * MAP_WIDTH + snake_body_0->pos_x[i];
        do {
            overlap = 0;
            fruit_pos_line = rand() % (MAP_WIDTH * MAP_HEIGHT);
            for (i = 0; i <= snake_body_0->len-1; i++) {
                if (fruit_pos_line == snake_pos_line[i]) {
                    overlap = 1;
                    break;
                }
            }
        } while (overlap);
        fruit->being = rand()%94+33;
        fruit->pos_y = fruit_pos_line / MAP_WIDTH;
        fruit->pos_x = fruit_pos_line % MAP_WIDTH;
    }
    return;
}

uint_8 eating(snake_body_template *snake, fruit_def_template *fruit){
    uint_8 i;
    uint_8 buf_x[LEN],buf_y[LEN],buf_s[LEN];
    if( (snake->pos_x[0]==fruit->pos_x && snake->pos_y[0]+snake->vel_y==fruit->pos_y)
        ||  (snake->pos_x[0]==fruit->pos_x && snake->pos_y[0]==0 && fruit->pos_y==MAP_HEIGHT-1 && snake->vel_y==-1)
        ||  (snake->pos_x[0]==fruit->pos_x && snake->pos_y[0]==MAP_HEIGHT-1 && fruit->pos_y==0 && snake->vel_y==1)
        ||  (snake->pos_y[0]==fruit->pos_y && snake->pos_x[0]+snake->vel_x==fruit->pos_x)
        ||  (snake->pos_y[0]==fruit->pos_y && snake->pos_x[0]==0 && fruit->pos_x==MAP_WIDTH-1 && snake->vel_x==-1)
        ||  (snake->pos_y[0]==fruit->pos_y && snake->pos_x[0]==MAP_WIDTH-1 && fruit->pos_x==0 && snake->vel_x==1) )
        {
        if(snake->len<=LEN-1){
            buf_x[0]=fruit->pos_x;
            buf_y[0]=fruit->pos_y;
            buf_s[0]=fruit->being;
            for(i=0;i<=snake->len-1;i++){
                buf_x[i+1]=snake->pos_x[i];
                buf_y[i+1]=snake->pos_y[i];
                buf_s[i+1]=snake->shape[i];
                snake->pos_x[i]=buf_x[i];
                snake->pos_y[i]=buf_y[i];
                snake->shape[i]=buf_s[i];
            }
            snake->pos_x[snake->len]=buf_x[snake->len];
            snake->pos_y[snake->len]=buf_y[snake->len];
            snake->shape[snake->len]=buf_s[snake->len];
            snake->len+=1;
            fruit->being=0;
            return 1;
        }
    }
    return 0;
}
uint_8 overing(snake_body_template *snake){
    uint_8 i;
    for(i=1;i<=snake->len-1;i++)
    {
        if(snake->pos_x[0]==snake->pos_x[i] &&
           snake->pos_y[0]==snake->pos_y[i])
            return 1;
    }
    return 0;
}

void all_print(map_def_template *map){
    uint_8 i,j;
    uint_8 map_show[MAP_HEIGHT + 2][2 * MAP_WIDTH + 2];

    map_show[0][0]='#';
    map_show[0][2*MAP_WIDTH+1]='#';
    map_show[MAP_HEIGHT+1][0]='#';
    map_show[MAP_HEIGHT+1][2*MAP_WIDTH+1]='#';
    for(j=1;j<=2*MAP_WIDTH;j++){
        map_show[0][j]='-';
        map_show[MAP_HEIGHT+1][j]='-';
    }

    for(i=1;i<=MAP_HEIGHT;i++){
        map_show[i][0]='|';
        for(j=1;j<=2*MAP_WIDTH-1;j+=2){
            if(!map->map[i-1][(j-1)/2]){
                map_show[i][j]=' ';
                map_show[i][j+1]=' ';
            }
            else {
                map_show[i][j]=map->map[i-1][(j-1)/2];
                map_show[i][j+1]=map->map[i-1][(j-1)/2];
            }
        }
        map_show[i][2*MAP_WIDTH+1]='|';
    }

    system("cls");
    for(i=0;i<=MAP_HEIGHT+1;i++){
        for(j=0;j<=2*MAP_WIDTH+1;j++){
            printf("%c",map_show[i][j]);
        }
        printf("\n");
    }
}

void show(map_def_template *map,snake_body_template *snake){
    static uint_8 buf_sel=0;
    uint_8 i,j;
    uint_8 map_show[MAP_HEIGHT + 2][2 * MAP_WIDTH + 2];
    LPDWORD bytes=0;
    uint_8 info[26]="vel_x=";
    uint_8 num[5];

    map_show[0][0]='#';
    map_show[0][2*MAP_WIDTH+1]='#';
    map_show[MAP_HEIGHT+1][0]='#';
    map_show[MAP_HEIGHT+1][2*MAP_WIDTH+1]='#';
    for(j=1;j<=2*MAP_WIDTH;j++){
        map_show[0][j]='-';
        map_show[MAP_HEIGHT+1][j]='-';
    }

    for(i=1;i<=MAP_HEIGHT;i++){
        map_show[i][0]='|';
        for(j=1;j<=2*MAP_WIDTH-1;j+=2){
            if(!map->map[i-1][(j-1)/2]){
                map_show[i][j]=' ';
                map_show[i][j+1]=' ';
            }
            else {
                map_show[i][j]=map->map[i-1][(j-1)/2];
                map_show[i][j+1]=map->map[i-1][(j-1)/2];
            }
        }
        map_show[i][2*MAP_WIDTH+1]='|';
    }

    itoa(snake->vel_x,num,10);
    strcat(info,num);
    strcat(info," vel_y=");
    itoa(snake->vel_y,num,10);
    strcat(info,num);
    strcat(info," len=");
    itoa(snake->len,num,10);
    strcat(info,num);
    strcat(info,"  ");

    if(!buf_sel) {
        coord.Y = 0;
        for (i = 0; i <= MAP_HEIGHT+1; i++)
        {
            coord.Y = i;
            WriteConsoleOutputCharacterA( v_out_1, map_show[i], 2*MAP_WIDTH+2, coord, &bytes);
        }
        coord.Y=MAP_HEIGHT+2;
        WriteConsoleOutputCharacterA( v_out_1, info, strlen(info), coord, &bytes);
        SetConsoleActiveScreenBuffer(v_out_1);
        buf_sel=1;
    }
    else{
        coord.Y = 0;
        for (i = 0; i <= MAP_HEIGHT+1; i++)
        {
            coord.Y = i;
            WriteConsoleOutputCharacterA( v_out_2, map_show[i], 2*MAP_WIDTH+2, coord, &bytes);
        }
        coord.Y=MAP_HEIGHT+2;
        WriteConsoleOutputCharacterA( v_out_2, info, strlen(info), coord, &bytes);
        SetConsoleActiveScreenBuffer(v_out_2);
        buf_sel=0;
    }
    return;


}
