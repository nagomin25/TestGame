#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const int WIDTH = 1920, HEIGHT = 1080; // �E�B���h�E�̕��ƍ����̃s�N�Z����

    SetWindowText("�Ȃ��݂̑�`��"); // �E�B���h�E�̃^�C�g��
    SetGraphMode(WIDTH, HEIGHT, 32); // �E�B���h�E�̑傫���ƃJ���[�r�b�g���̎w��
    ChangeWindowMode(TRUE); // �E�B���h�E���[�h�ŋN��
    if (DxLib_Init() == -1) return -1; // ���C�u���������� �G���[���N������I��
    SetBackgroundColor(0, 0, 0); // �w�i�F�̎w��
    SetDrawScreen(DX_SCREEN_BACK); // �`��ʂ𗠉�ʂɂ���

    int bgX = 0;
    int imgBG = LoadGraph("image/bg.png");

    while (1) // ���C�����[�v
    {
        ClearDrawScreen(); // ��ʂ��N���A����

        bgX = bgX - 10;
        if (bgX <= -WIDTH) {
            bgX = bgX + WIDTH;
        };

        DrawGraph(bgX + WIDTH, 0, imgBG, FALSE);
        DrawGraph(bgX, 0, imgBG, FALSE);

        ScreenFlip(); // ����ʂ̓��e��\��ʂɔ��f������
        WaitTimer(16); // ��莞�ԑ҂�
        if (ProcessMessage() == -1) break; // Windows��������󂯎��G���[���N������I��
        if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESC�L�[�������ꂽ��I��
    }

    DxLib_End(); // �c�w���C�u�����g�p�̏I������
    return 0; // �\�t�g�̏I��
}