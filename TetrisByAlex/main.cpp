#include <iostream>
#include <Windows.h>
#include <thread>
#include <vector>
#include <cstdio>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <conio.h>
#include <cstdlib>

//#define arraySize 4
const unsigned char arraySize = 4;

std::wstring tetromino[8];
int nFieldWidth = 20;
int nFieldHeight = 18;
unsigned char *pField = nullptr;


int nScreenWidth = 80;           // Console Screen Size X ( columns )
int nScreenHeight = 30;           // Console Screen Size Y ( rows )

int Rotate(int px, int py, int r = 0)
{
    switch (r % 4)
    {
    case 0: return py * 4 + px;          // 0 degrees
    case 1: return 12 + py - (px * 4);   // 90 degrees
    case 2: return 15 - (py * 4) - px;   // 180 degrees
    case 3: return 3 - py + (px * 4);    // 270 degrees
    }
    return 0;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
    for (int px = 0; px < 4; px++)
        for (int py = 0; py < 4; py++)
        {
            // Get index into piece
            int pi = Rotate(px, py, nRotation);

            // Get index into field
            int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

            if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
            {
                if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
                {
                    if (tetromino[nTetromino][pi] == L'X' && pField[fi] != 0)
                        return false; // fail on first hit
                }
            }
        }

    return true;
}

int main()
{
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    int nRekord = 0;
    int nRekordOld = 0;
    int nScoreOld = 0;

    std::string line;
    std::ifstream myfile("Tetris.txt");
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            std::stringstream geek(line);
            int line = 0;
            geek >> line;

            nRekordOld = line;
        }
        myfile.close();
    }

    do {
        wsprintf(&screen[10 * nScreenWidth + 18], L"TTTTTT  EEEEEE   TTTTTT  RRRRRRRR  II  SSSSSS");
        wsprintf(&screen[11 * nScreenWidth + 18], L"TTTTTT  EE       TTTTTT  RR    RR  II  SS    ");
        wsprintf(&screen[12 * nScreenWidth + 18], L"  TT    EE         TT    RR    RR  II  SS    ");
        wsprintf(&screen[13 * nScreenWidth + 18], L"  TT    EE         TT    RR    RR  II  SS    ");
        wsprintf(&screen[14 * nScreenWidth + 18], L"  TT    EEEEE      TT    RRRRRRRR  II  SSSSSS");
        wsprintf(&screen[15 * nScreenWidth + 18], L"  TT    EE         TT    RR RR     II      SS");
        wsprintf(&screen[16 * nScreenWidth + 18], L"  TT    EE         TT    RR  RR    II      SS");
        wsprintf(&screen[17 * nScreenWidth + 18], L"  TT    EE         TT    RR   RR   II      SS");
        wsprintf(&screen[18 * nScreenWidth + 18], L"  TT    EEEEEE     TT    RR    RR  II  SSSSSS");
        wsprintf(&screen[20 * nScreenWidth + 24], L"BBBB  Y   Y   AAAA L    EEEE X   X");
        wsprintf(&screen[21 * nScreenWidth + 24], L"B   B  Y Y    A  A L    E     X X ");
        wsprintf(&screen[22 * nScreenWidth + 24], L"BBBB    Y     AAAA L    EEE    X  ");
        wsprintf(&screen[23 * nScreenWidth + 24], L"B   B   Y     A  A L    E     X X ");
        wsprintf(&screen[24 * nScreenWidth + 24], L"BBBB    Y     A  A LLLL EEEE X   X");
        wsprintf(&screen[29 * nScreenWidth + 30], L"PRESS 'SPACE' TO PLAY");

        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);

    } while ((0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) == 0);

    for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';

    srand(time(NULL));

    // Create assets
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

    tetromino[7].append(L"....");
    tetromino[7].append(L"..X.");
    tetromino[7].append(L"....");
    tetromino[7].append(L"....");

    while (1)
    {
        pField = new unsigned char[nFieldWidth * nFieldHeight]; // Create play field buffer
        for (int x = 0; x < nFieldWidth; x++) // Board Boundary
            for (int y = 0; y < nFieldHeight; y++)
                pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;

        int nScore = 0;
        bool bGameOver = false, bKeyPause = false;
        int nCurrentPiece = 0;
        int nCurrentRotation = 0;
        int nCurrentX = nFieldWidth / 2;
        int nCurrentY = 0;
        int nCurrent = 0;

        bool bKey[arraySize];
        bool bRotateHold = false;
        bool bPause = false;
        bool bDead = false;
        bool bKeyExit = false;

        std::vector<int> vLines;
        int nSpeed = 20;
        int nSpeedCount = 0;
        bool bForceDown = false;
        int nPieceCount = 0;

        while(!bGameOver)
        {
            //Timing =================================================
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Game Tick
            nSpeedCount++;
            bForceDown = (nSpeedCount == nSpeed);

            //Input ==================================================
            for (int k = 0; k < arraySize; k++)
                bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28\x41\x44"[k]))) != 0;
            
            // GAME LOGIC ============================================

            // Handle Player Movement
            nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
            nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
            nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

            // Rotate, but latch to stop wild spinning
            if (bKey[3])
            {
                nCurrentRotation += (!bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
                bRotateHold = true;
            }
            else
            {
                bRotateHold = false;
            }

            WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);

            // Force the piece down the playfield if it's time
            if (bForceDown)
            {

                // Update difficulty every 50 piecies
                nSpeedCount = 0;
                nPieceCount++;
                if (nPieceCount % 10 == 0)
                {
                    if (nSpeed >= 10)
                    {
                        nSpeed--;
                    }
                }

                // Test if pece can be moved down
                if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
                {
                    nCurrentY++; // It can, so do it!
                }
                else
                {
                    // It can't! Lock the piece in place
                    for (int px = 0; px < 4; px++)
                        for (int py = 0; py < 4; py++)
                            if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
                            {
                                pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;
                                nPieceCount++;
                            }

                    // Check for lines
                    for (int py = 0; py < 4; py++)
                        if (nCurrentY + py < nFieldHeight - 1)
                        {
                            bool bLine = true;
                            for (int px = 1; px < nFieldWidth - 1; px++)
                                bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

                            if (bLine)
                            {
                                // Remove Line, set to =
                                for (int px = 1; px < nFieldWidth - 1; px++)
                                    pField[(nCurrentY + py) * nFieldWidth + px] = 8;
                                vLines.push_back(nCurrentY + py);
                            }
                        }

                    // Score
                    nScore += 25;
                    if (!vLines.empty()) nScore += (1 << vLines.size()) * 50;

                    nScoreOld = nScore;
                    if (nScore > nRekordOld)
                    {
                        nRekordOld = nScore;
                        nRekord = nScore;
                        if (line < std::to_string(nRekord))
                        {
                            std::ofstream myfile("Tetris.txt");
                            if (myfile.is_open())
                            {
                                myfile << nRekord;
                                myfile.close();
                            }
                        }
                    }
                    else
                    {
                        nRekord = nRekordOld;
                    }

                    // Pick New Piece
                    nCurrentX = nFieldWidth / 2;
                    nCurrentY = 0;
                    nCurrentRotation = 0;
                    nCurrentPiece = rand() % 8;

                    // If piece does not fit straight away, game over!
                    bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
                }
                WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
            }

            // Display =============================

            // Draw Field
            for (int x = 0; x < nFieldWidth; x++)
                for (int y = 0; y < nFieldHeight; y++)
                    screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFGH█"[pField[y * nFieldWidth + x]];

            // Draw Current Piece
            for (int px = 0; px < 4; px++)
                for (int py = 0; py < 4; py++)
                    if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                        screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;

            // Draw Score
            swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", nScore);
            WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);

            // Animate Line Completion
            if (!vLines.empty())
            {
                // Display Frame (cheekily to draw lines)
                WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);

                for (auto& v : vLines)
                    for (int px = 1; px < nFieldWidth - 1; px++)
                    {
                        for (int py = v; py > 0; py--)
                            pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
                        pField[px] = 0;
                    }
                vLines.clear();
            }

            // Display Frame
            WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);

            bKeyPause = (0x8000 & GetAsyncKeyState((unsigned char)('\x50'))) != 0;
            bKeyExit = (0x8000 & GetAsyncKeyState((unsigned char)('\x58'))) != 0;

            if (bKeyPause)
            {
                bPause = true;
            }

            bForceDown = false;


            if (bGameOver)
            {
                wsprintf(&screen[10 * nScreenWidth + 40], L"     GAME OVER");
                wsprintf(&screen[12 * nScreenWidth + 40], L"     Score:%d", nScore);
                wsprintf(&screen[13 * nScreenWidth + 40], L"     Rekord:%d", nRekord);
                wsprintf(&screen[15 * nScreenWidth + 35], L"PRESS    'SPACE' TO PLAY AGAIN");
                wsprintf(&screen[17 * nScreenWidth + 35], L"PRESS    'X' TO EXIT");
            }

            if (bPause)
            {

                wsprintf(&screen[10 * nScreenWidth + 58], L" PAUSE ");
                wsprintf(&screen[12 * nScreenWidth + 51], L"Score:%d", nScore);
                wsprintf(&screen[13 * nScreenWidth + 51], L"Record:%d", nRekord);
                wsprintf(&screen[15 * nScreenWidth + 50], L"PRESS   'R' TO RESUME");
                WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);

                while ((0x8000 & GetAsyncKeyState((unsigned char)('\x52'))) == 0);
                bPause = false;
                for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
            }

            // Display Frame
            WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
        }

        do {
            bKeyExit = (0x8000 & GetAsyncKeyState((unsigned char)('\x58'))) != 0;
            if (bKeyExit)
            {
                return EXIT_SUCCESS;
            }
        } while ((0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) == 0);
        for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';

    }
    return 0;
}
