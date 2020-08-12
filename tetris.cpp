#define UNICODE
#include "iostream"
#include "Windows.h"

using namespace std;
wstring tetromino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
int nScreenWidth = 80;
int nScreenHeight = 30;
unsigned char *pField = NULL;
int rotate(int px,int py,int r){
  switch(r % 4){
    case 0 : return (py * 4) + px;
    case 1 : return 12 + py - (px * 4);
    case 2 : return 15 - (py * 4) - px;
    case 3 : return 3 - py + (px * 4);
  }
  return 0;
}
bool DoesPieceFit(int nTetromino,int nRotation,int nPosX,int nPosY){
  for(int px=0;px<4;px++){
    for(int py=0;py<4;py++){
      int pi=rotate(nPosX,nPosY,nRotation);
      int fi=(nPosY+py)*nFieldWidth+(nPosX+px);
      if((nPosX+px)>=0 && (nPosX+px) < nFieldWidth){
        if((nPosY+py)>=0 && (nPosY+py) < nFieldHeight){
          if(tetromino[nTetromino][pi] == L'X' && pField[fi]!=0){
            return false;
          }
        }
      }
    }
  }
  return true;
}
int main(){
  cout<<"Hello World"<<endl;
  // create
  tetromino[0].append(L"..X.");
  tetromino[0].append(L"..X.");
  tetromino[0].append(L"..X.");
  tetromino[0].append(L"..X.");

  tetromino[1].append(L"..X.");
  tetromino[1].append(L".XX.");
  tetromino[1].append(L".X..");
  tetromino[1].append(L"....");

  tetromino[2].append(L".X..");
  tetromino[2].append(L".XX.");
  tetromino[2].append(L"..X.");
  tetromino[2].append(L"....");

  tetromino[3].append(L"....");
  tetromino[3].append(L".XX.");
  tetromino[3].append(L".XX.");
  tetromino[3].append(L"....");

  tetromino[4].append(L"..X.");
  tetromino[4].append(L".XX.");
  tetromino[4].append(L"..X.");
  tetromino[4].append(L"....");

  tetromino[5].append(L"....");
  tetromino[5].append(L".XX.");
  tetromino[5].append(L"..X.");
  tetromino[5].append(L"..X.");

  tetromino[6].append(L"....");
  tetromino[6].append(L".XX.");
  tetromino[6].append(L".X..");
  tetromino[6].append(L".X..");

  // Play Area

  pField  = new unsigned char[nFieldWidth*nFieldHeight];
  for(int x=0 ; x < nFieldWidth ; x++){
    for(int y=0 ; y < nFieldHeight ; y++){
      pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth-1 || y == nFieldHeight-1) ? 9 : 0 ;
    }
  }

  wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
  for(int i=0; i < nScreenWidth * nScreenHeight ; i++){
    screen[i] = L' ';
  }
  HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE , 0 , NULL , CONSOLE_TEXTMODE_BUFFER , NULL);
  SetConsoleActiveScreenBuffer(hConsole);
  DWORD dwBytesWritten = 0;
  //cout<<tetromino[0]<<endl;

	bool bGameOver = false;

  int nCurrentPiece = 1;
  int nCurrentRotation = 0;
  int nCurrentX = (nFieldWidth)/2;
  int nCurrentY = 0;
  bool bKey[4];
	while(!bGameOver){
    // Time Delay
    /*
    //std::this_thread::sleep_for(std::chrono::nanoseconds(10));
    //sleep_for(50ms);
    //delay(10);
    //usleep(0050000);
    //sleep(std::chrono::nanoseconds(50));
    //_sleep(50);
    */
    Sleep(50);

    // Input :
    for(int i=0;i<4;i++)
      bKey[i] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[i]))) != 0;
    // Game Logic
    nCurrentX=nCurrentX-((bKey[1])&&(DoesPieceFit(nCurrentPiece,nCurrentRotation,nCurrentX-1,nCurrentY)) ? 1 : 0 );
    nCurrentX=nCurrentX+((bKey[0])&&(DoesPieceFit(nCurrentPiece,nCurrentRotation,nCurrentX+1,nCurrentY)) ? 1 : 0 );
    nCurrentY=nCurrentY+((bKey[2])&&(DoesPieceFit(nCurrentPiece,nCurrentRotation,nCurrentX,nCurrentY+1)) ? 1 : 0 );
    nCurrentRotation=nCurrentRotation+((bKey[3])&&(DoesPieceFit(nCurrentPiece,nCurrentRotation+1,nCurrentX,nCurrentY+1)) ? 1 : 0 );

    // Play Field
    for(int x=0 ; x < nFieldWidth ; x++){
   		 for(int y=0 ; y < nFieldHeight ; y++){
      			screen[(y+2)*nScreenWidth+(x+2)]=L" ABCDEFG=#"[pField[(y*nFieldWidth)+x]];
    		}
  		}
    // piece
    for(int px=0 ; px < 4 ; px++){
       for(int py=0 ; py < 4 ; py++){
            if(tetromino[nCurrentPiece][rotate(px,py,nCurrentRotation)] == L'X'){
              screen[(nCurrentY + py + 2)*nScreenWidth+(nCurrentX + px + 2)] = nCurrentPiece + 65;
            }
        }
      }
    // screen
      WriteConsoleOutputCharacter(hConsole,screen,nScreenWidth*nScreenHeight,{0,0},&dwBytesWritten);
		//break;
	}
  return 0;
}
