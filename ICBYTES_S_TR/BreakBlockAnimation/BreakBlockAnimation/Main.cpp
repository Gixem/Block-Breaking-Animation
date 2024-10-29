#include "icb_gui.h"
#include <windows.h>

int FRM1, BTN;
int ballX = 150, ballY = 400, ballSpeedX = 2, ballSpeedY = -2;
int paddleX = 250, paddleWidth = 100, paddleHeight = 20;
int blockWidth = 80, blockHeight = 30;
bool blockVisible[10];
int blockColors[10];
int blockX[10] = { 50, 150, 250, 350, 450, 50, 150, 250, 350, 450 };
int blockY[2] = { 100, 150 };

int ballColor = 0xffff00;
ICBYTES panel;

bool animationRunning = false; // Flag to indicate if animation is running

// GUI ayarlarý
void ICGUI_Create()
{
    ICG_MWTitle("Break Block Animation");
    ICG_MWSize(700, 700);
    ICG_SetFont(30, 12, "");
}

// Topu takip eden daha yumuþak otomatik paddle hareketi
void MovePaddleSmoothly()
{
    int targetX = ballX - paddleWidth / 2; // Topun X koordinatýný hedefler
    int speed = 3; // Paddle'in hareket hýzýný azaltmak için küçük bir deðer

    if (paddleX < targetX && paddleX + paddleWidth < 700)
        paddleX += speed; // Paddle saða yavaþça kayar
    else if (paddleX > targetX && paddleX > 0)
        paddleX -= speed; // Paddle sola yavaþça kayar
}

// Topun animasyonu ve blok kýrma mantýðý
void AnimateBall(void* lpParam)
{
    animationRunning = true; // Set the flag to true when animation starts

    // Bloklarýn baþlangýç ayarlarý
    for (int i = 0; i < 10; i++) {
        blockVisible[i] = true; // Tüm bloklar baþlangýçta görünür
        blockColors[i] = 0xff0000 + i * 0x001100; // Bloklarýn renkleri
    }

    // Animasyon döngüsü (sürekli çalýþýr)
    while (animationRunning) // Check the flag in the loop condition
    {
        MovePaddleSmoothly(); // Yumuþak paddle hareketi

        ballX += ballSpeedX;
        ballY += ballSpeedY;

        if (ballX >= 700 || ballX <= 0) ballSpeedX = -ballSpeedX;
        if (ballY <= 0) ballSpeedY = -ballSpeedY;

        // Bloklarla çarpýþma kontrolü
        for (int j = 0; j < 10; j++)
        {
            int blockRow = (j < 5) ? 0 : 1;
            if (blockVisible[j] && ballX >= blockX[j] && ballX <= blockX[j] + blockWidth &&
                ballY >= blockY[blockRow] && ballY <= blockY[blockRow] + blockHeight)
            {
                ballSpeedY = -ballSpeedY;
                blockVisible[j] = false; // Blok kýrýldý
                ballColor = blockColors[j]; // Top kýrýlan bloðun rengini alýr
            }
        }

        // Raket ile çarpýþma kontrolü
        if (ballY + 10 >= 600 && ballX >= paddleX && ballX <= paddleX + paddleWidth)
        {
            ballSpeedY = -ballSpeedY; // Topun Y eksenindeki yönünü ters çevir
        }

        // Top zemine çarparsa (oyun biter)
        if (ballY > 700) break;

        // Ekraný temizle
        FillRect(panel, 0, 0, 700, 700, 0);

        // Blok çizimi (kýrýlmamýþsa)
        for (int j = 0; j < 10; j++) {
            int blockRow = (j < 5) ? 0 : 1;
            if (blockVisible[j])
            {
                FillRect(panel, blockX[j], blockY[blockRow], blockWidth, blockHeight, blockColors[j]); // Bloklar
            }
        }

        // Topun çizimi
        FillCircle(panel, ballX, ballY, 10, ballColor); // Top (renk deðiþir)

        // Raketin çizimi
        FillRect(panel, paddleX, 600, paddleWidth, paddleHeight, 0x00ff00); // Yeþil raket

        // Görüntüyü güncelle
        DisplayImage(FRM1, panel);
        Sleep(30); // Animasyonu yavaþlat
    }

    animationRunning = false; // Reset the flag when the animation ends
}

// Ana GUI ve animasyon baþlatma butonu
void ICGUI_main()
{
    CreateImage(panel, 700, 700, ICB_UINT);
    FRM1 = ICG_FrameMedium(5, 5, 400, 200);
    FillRect(panel, 0, 0, 700, 700, 0);
    DisplayImage(FRM1, panel);
    BTN = ICG_TButton(575, 5, 100, 50, "START", AnimateBall, NULL);
}

// Function to handle closing the application
void OnClose()
{
    if (animationRunning) {
        animationRunning = false; // Stop the animation when closing
    }
}

// Main entry point or window procedure should call OnClose
// Ensure this function is called when the window is closed
