#include <stdio.h>
#include <stdbool.h>

#define ROWS    8 
#define COLS    8

#define RIGHT_WALL   1
#define BOTTOM_WALL  2
#define LEFT_WALL    4
#define TOP_WALL     8

unsigned char steny[ROWS][COLS] = {
    /*
    ____________________________
    |_ _ _  |_ _  | |
    |_   _|  _  |_  |
    | | |_  |_|  _ _|
    | |_   _ _ _|   |
    |  _  |    _ _| |
    | |_    |_| |  _|
    |  _| |_ _  |   |
    |_ _ _ _ _|_|_ _|
    */
    {0b1110, 0b1010, 0b1010, 0b1001, 0b1110, 0b1010, 0b1001, 0b1101}, 
    {0b1110, 0b1000, 0b1011, 0b0100, 0b1010, 0b1001, 0b0110, 0b0001}, 
    {0b1101,0b0101,0b1110,0b0001,0b1111,0b0100,0b1010,0b0011},
    {0b0101,0b0110,0b1000,0b0010,0b1010,0b0011,0b1100,0b1001},
    {0b0100,0b1010,0b0001,0b1100,0b1000,0b1010,0b0011,0b0101},
    {0b0101,0b1110,0b0000,0b0001,0b0111,0b1101,0b1100,0b0011},
    {0b0100,0b1011,0b0101,0b0110,0b1010,0b0001,0b0100,0b1001},
    {0b0110,0b1010,0b0010,0b1010,0b1011,0b0111,0b0110,0b0011},
};
char dist[ROWS][COLS];
int mposR = 0;
int mposC = 0;
int vpravo = 100;
int dole = 100;
int vlavo = 100;
int hore = 100;


//  6 & 9  =>  0110 & 1001
//   0110
// & 1001
// ----------
//   0000
//
//  6 && 9  => 1
//

/*
void nastavenie(int riadky, int stlpce)
{
    unsigned char steny[riadky][stlpce] = { {13=0b1101, 12=0b1101, 11}, {5, 4, 9}, {6, 3, 7} };
    unsigned char dlzka[riadky][stlpce];
    for (int i=0;i < riadky; i++)
    {
        for (int j=0; j < stlpce; j++)
        {
                dlzka [i][j]=100;
        }
    }
    dlzka[riadky-1][stlpce-1]=0;
}*/

bool hasWallRight(unsigned int w) {
    return (w & RIGHT_WALL) != 0;
}

bool hasWallLeft(unsigned int w) {
    return (w & LEFT_WALL) != 0;
}

bool hasWallTop(unsigned int w) {
    return (w & TOP_WALL) != 0;
}

bool hasWallBottom(unsigned int w) {
    return (w & BOTTOM_WALL) != 0;
}


void print_map(unsigned char m[ROWS][COLS])
{
    for (int j = 0; j < COLS; j++)
        printf("__");
    printf("_\n");
    
    for (int i = 0; i < ROWS; i++)
    {
      for (int j = 0; j < COLS; j++)
      {
        if (hasWallLeft(m[i][j])) printf("|");
        else printf(" ");
        char z1 = '_';
        char z2 = ' ';
        if ((i == mposR) && (j == mposC))
        {
            z1 = '2';
            z2 = '7';
        }
        if (hasWallBottom(m[i][j])) printf("%c", z1);
        else printf("%c", z2);
      }
      printf("|\n");
    }
}


void compute(unsigned char msteny[ROWS][COLS])
{
    //init dist
    for (int i=0;i < ROWS; i++)
    {
        for (int j=0; j < COLS; j++)
        {
                dist[i][j]=100;
        }
    }
    dist[ROWS-1][COLS-1]=0;
    
    for (int d = 0; d < ROWS*COLS; d++)
    {
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (dist[i][j] == d)
                {
                    unsigned int ww = msteny[i][j];//walls in this square [i][j]
                    if (j<COLS-1 && !hasWallRight(ww))
                    {
                        unsigned int n = dist[i][j+1];//walls in right neighbor square [i][j+1]
                        if (n>d+1) {
                            dist[i][j+1] = d+1;
                        }
                    }
                    
                    if (j>0 && !hasWallLeft(ww))
                    {
                        unsigned int n = dist[i][j-1];//walls in left neighbor square [i][j-1]
                        if (n>d+1) {
                            dist[i][j-1] = d+1;
                        }
                    }
                    
                    if (i<ROWS-1 && !hasWallBottom(ww))
                    {
                        unsigned int n = dist[i+1][j];//walls in bottom neighbor square [i+1][j]
                        if (n>d+1) {
                            dist[i+1][j] = d+1;
                        }
                    }
                    
                    if (i>0 && !hasWallTop(ww))
                    {
                        unsigned int n = dist[i-1][j];//walls in top neighbor square [i-1][j]
                        if (n>d+1) {
                            dist[i-1][j] = d+1;
                        }
                    }
                }
            }
        }
    }
}

int main()
{
    //print_map(steny);

    
    //init msteny
    unsigned char msteny[ROWS][COLS];
    for (int i = 0;i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
                msteny[i][j] = 0;
        }
    }
    for (int i = 0;i < ROWS; i++)
    {
        msteny[i][COLS - 1] += RIGHT_WALL;
    }
    for (int i = 0;i < COLS; i++)
    {
        msteny[ROWS - 1][i] += BOTTOM_WALL;
    }
    for (int i = 0;i < ROWS; i++)
    {
        msteny[i][0] += LEFT_WALL;
    }
    for (int i = 0;i < COLS; i++)
    {
        msteny[0][i] += TOP_WALL;
    }
    
    while (mposR != ROWS - 1 || mposC != COLS - 1)
    {
        
        //1  dopln si co vies
        
        int mynum;
        printf("napiscislo[");
        printf("%d", mposR);
        printf("][");
        printf("%d", mposC);
        printf("]");       
        scanf("%d", &mynum);
        //printf("%d",mynum);
        
        msteny[mposR][mposC] = /* steny[mposR][mposC];  */ mynum;
        if (hasWallRight(msteny[mposR][mposC]) == 1 && mposC < COLS - 1)
        {
            msteny[mposR][mposC + 1] |= LEFT_WALL;
        }    
        if (hasWallBottom(msteny[mposR][mposC]) == 1 && mposR < ROWS - 1)
        {
            msteny[mposR + 1][mposC] |= TOP_WALL;
        } 
        if (hasWallLeft(msteny[mposR][mposC]) == 1 && mposC != 0)
        {
            msteny[mposR][mposC - 1] |= RIGHT_WALL;
        } 
        if (hasWallTop(msteny[mposR][mposC]) == 1 && mposR != 0)
        {
            msteny[mposR - 1][mposC] |= BOTTOM_WALL;
        } 
    
        //2 compute
        compute(msteny);
        
        //3 print results
        
        for (int i = 0; i < ROWS; i++)
    	{
    	    for (int j = 0; j < COLS; j++)
    		{
    		    printf ("%5d ", dist[i][j] );
    		}
    	    printf ("\n");
    	}	
        printf("mposR=");
        printf("%d",mposR);
        printf("\n");
        printf("mposC=");
        printf("%d",mposC);
        printf("\n\n");
        
        print_map(msteny);
        
        //4 pohni sa
        
        int ww = msteny[mposR][mposC];
        if (hasWallRight(ww) == 0) 
        {
            vpravo = dist[mposR][mposC + 1];
        }
        else
        {
            vpravo = 100;        
        }
        ww = msteny[mposR][mposC];
        if (hasWallBottom(ww) == 0) 
        {
            dole = dist[mposR + 1][mposC];
        }
        else
        {
            dole = 100;        
        }
        ww = msteny[mposR][mposC];
        if (hasWallLeft(ww) == 0) 
        {
            vlavo = dist[mposR][mposC - 1];
        }
        else
        {
            vlavo = 100;        
        }
        ww = msteny[mposR][mposC];
        if (hasWallTop(ww) == 0) 
        {
            hore = dist[mposR - 1][mposC];
        }
        else
        {
            hore = 100;        
        }
        if (vpravo <= dole && vpravo <= vlavo && vpravo <= hore)
        {
            mposC++;
        }    
        else if (dole <= vpravo && dole <= vlavo && dole <= hore)
        {
            mposR++;
        } 
        else if (vlavo <= dole && vlavo <= hore && vlavo <= vpravo)
        {
            mposC--;
        } 
        else /*if (vpravo <= dole && vpravo <= vlavo && vpravo <= hore)*/
        {
            mposR--;
        } 
        
        
        
    }
    
    print_map(msteny);
    int mynum;
    printf("napiscislo\n");
    scanf("%d", &mynum);
    printf("%d",mynum);
    return 0;
}
