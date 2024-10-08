#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const int WIDTH = 1920, HEIGHT = 1080; // ウィンドウの幅と高さのピクセル数

    SetWindowText("なごみの大冒険"); // ウィンドウのタイトル
    SetGraphMode(WIDTH, HEIGHT, 32); // ウィンドウの大きさとカラービット数の指定
    ChangeWindowMode(TRUE); // ウィンドウモードで起動
    if (DxLib_Init() == -1) return -1; // ライブラリ初期化 エラーが起きたら終了
    SetBackgroundColor(0, 0, 0); // 背景色の指定
    SetDrawScreen(DX_SCREEN_BACK); // 描画面を裏画面にする

    int bgX = 0;
    int imgBG = LoadGraph("image/bg.png");

    while (1) // メインループ
    {
        ClearDrawScreen(); // 画面をクリアする

        bgX = bgX - 10;
        if (bgX <= -WIDTH) {
            bgX = bgX + WIDTH;
        };

        DrawGraph(bgX + WIDTH, 0, imgBG, FALSE);
        DrawGraph(bgX, 0, imgBG, FALSE);

        ScreenFlip(); // 裏画面の内容を表画面に反映させる
        WaitTimer(16); // 一定時間待つ
        if (ProcessMessage() == -1) break; // Windowsから情報を受け取りエラーが起きたら終了
        if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESCキーが押されたら終了
    }

    DxLib_End(); // ＤＸライブラリ使用の終了処理
    return 0; // ソフトの終了
}