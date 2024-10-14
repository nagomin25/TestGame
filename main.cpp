#include "DxLib.h"
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

// �E�B���h�E�̃T�C�Y
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 600;

// �u���b�N�̃T�C�Y
const int BLOCK_SIZE = 30;

// �O���b�h�̃T�C�Y
const int GRID_WIDTH = SCREEN_WIDTH / BLOCK_SIZE;
const int GRID_HEIGHT = SCREEN_HEIGHT / BLOCK_SIZE;

// �v���C���[�̕�������
struct Player {
    double x, y;
    double vx, vy;
    bool isJumping;
    int width, height;
};

Player player = { 90, 300, 0, 0, false, BLOCK_SIZE, BLOCK_SIZE };
int cameraX = 0;

const double GRAVITY = 0.5;
const double JUMP_FORCE = -10;
const double MOVE_SPEED = 5;


int clamp(int value, int low, int high) {
    return (value < low) ? low : ((value > high) ? high : value);
}


std::vector<std::string> gameMap = {
    "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
    "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
    "0000000000300000000000300000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
    "0000000000000030000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003300000000000000000000000000000330000000000000000000000000000000",
    "0000000000003000000000000000000000000000000000000000000000000000000000000000000000000000033000000000000000000000000000000000000000000000000000000000000000000000000000000",
    "0000040000000000000000000000004000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000330000000000000000000000000000000",
    "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000330000000000000000000000000030000000000000000000000000000000000000000000000000",
    "0000000000040000000000000000000000004000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
    "0000000000000000004000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
    "0000000000000000000000004000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
    "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
    "0002000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
    "0022200000000000000000400000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000040000000000000000",
    "0222220000000000000000000000001110000000000000000000000000000000000444400000000000000000000000000000000000000000000000000000000000000000000000000000000440000000000000000",
    "2222220000000000000000000000011111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004440000000000000000",
    "2222222000000000000000000000111111100000000000000000000000000000000000000000000000000440000000000000000000000000000000000000000000000000000000000000044440000000000000000",
    "1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111001111111111111111111111111111111111111111111111111111111111111111111",
    "1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111001111111111111111111111111111111111111111111111111111111111111111111",
    "1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111001111111111111111111111111111111111111111111111111111111111111111111",
    "1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111001111111111111111111111111111111111111111111111111111111111111111111"
};

void UpdateCamera()
{
    int targetCameraX = static_cast<int>(player.x) - SCREEN_WIDTH / 2;
    int maxCameraX = static_cast<int>(gameMap[0].length()) * BLOCK_SIZE - SCREEN_WIDTH;
    cameraX = clamp(targetCameraX, 0, maxCameraX);
}

bool IsCollision(double x, double y, int width, int height)
{

    int left = static_cast<int>(std::floor(x));
    int right = static_cast<int>(std::ceil(x + width));
    int top = static_cast<int>(std::floor(y));
    int bottom = static_cast<int>(std::ceil(y + height));

    for (int checkY = top; checkY < bottom; checkY++) {
        for (int checkX = left; checkX < right; checkX++) {
            int gridX = checkX / BLOCK_SIZE;
            int gridY = checkY / BLOCK_SIZE;

            if (gridY >= 0 && gridY < static_cast<int>(gameMap.size()) &&
                gridX >= 0 && gridX < static_cast<int>(gameMap[0].size())) {
                if (gameMap[gridY][gridX] == '1' || gameMap[gridY][gridX] == '4') {
                    return true;
                }
            }
        }
    }

    return false;
}
bool IsOnGround(const Player& p)
{
    if (p.y + p.height >= gameMap.size() * BLOCK_SIZE) {
        // �v���C���[���}�b�v�̉��[�𒴂��Ă���
        return false;
    }
    return IsCollision(p.x, p.y + p.height + 0.1, p.width, 0.1);
}

void ResetPlayer()
{
    player = { 90, 300, 0, 0, false, BLOCK_SIZE, BLOCK_SIZE };
}



void UpdatePlayer()
{
    // ���������̈ړ�
    if (CheckHitKey(KEY_INPUT_A)) player.vx = -MOVE_SPEED;
    else if (CheckHitKey(KEY_INPUT_D)) player.vx = MOVE_SPEED;
    else player.vx = 0;

    // �W�����v
    bool onGround = IsOnGround(player);
    if (CheckHitKey(KEY_INPUT_SPACE) && onGround) {
        player.vy = JUMP_FORCE;
        player.isJumping = true;
    }

    // �d�͂̓K�p
    if (!onGround) {
        player.vy += GRAVITY;
    }
    else if (player.vy > 0) {
        player.vy = 0;
    }

    // X�����̈ړ��ƏՓ˔���
    double newX = player.x + player.vx;
    if (!IsCollision(newX, player.y, player.width, player.height) &&
        newX >= 0 && newX + player.width <= gameMap[0].length() * BLOCK_SIZE) {
        player.x = newX;
    }

    // Y�����̈ړ��ƏՓ˔���
    double newY = player.y + player.vy;
    if (!IsCollision(player.x, newY, player.width, player.height)) {
        player.y = newY;
        player.isJumping = !onGround;
    }
    else {
        if (player.vy > 0) {  // �������ւ̏Փˁi���n�j
            player.y = std::floor(player.y / BLOCK_SIZE) * BLOCK_SIZE;
            player.vy = 0;
            player.isJumping = false;
        }
        else {  // ������ւ̏Փ�
            player.y = std::ceil((newY + player.height) / BLOCK_SIZE) * BLOCK_SIZE - player.height;
            player.vy = 0;
        }
    }

    // ��ʉ��ɗ������ꍇ�̃Q�[���I�[�o�[����
    if (player.y > SCREEN_HEIGHT) {
        ResetPlayer();
    }
}

void DrawMap()
{
    for (size_t y = 0; y < gameMap.size(); y++)
    {
        for (size_t x = 0; x < gameMap[y].size(); x++)
        {
            int screenX = static_cast<int>(x) * BLOCK_SIZE - cameraX;

            if (screenX < -BLOCK_SIZE || screenX > SCREEN_WIDTH) continue;

            switch (gameMap[y][x])
            {
            case '0': // �󔒁i�f�o�b�O�p�ɔ����F�ŕ\���j
                DrawBox(screenX, static_cast<int>(y) * BLOCK_SIZE, screenX + BLOCK_SIZE, static_cast<int>(y + 1) * BLOCK_SIZE, GetColor(50, 50, 50), TRUE);
                break;
            case '1': // �n��
                DrawBox(screenX, static_cast<int>(y) * BLOCK_SIZE, screenX + BLOCK_SIZE, static_cast<int>(y + 1) * BLOCK_SIZE, GetColor(0, 128, 0), TRUE);
                break;
            case '2': // �R
                DrawBox(screenX, static_cast<int>(y) * BLOCK_SIZE, screenX + BLOCK_SIZE, static_cast<int>(y + 1) * BLOCK_SIZE, GetColor(139, 69, 19), TRUE);
                break;
            case '3': // �_
                DrawCircle(screenX + BLOCK_SIZE / 2, static_cast<int>(y) * BLOCK_SIZE + BLOCK_SIZE / 2, BLOCK_SIZE / 2, GetColor(255, 255, 255), TRUE);
                break;
            case '4': // �����u���b�N
                DrawBox(screenX, static_cast<int>(y) * BLOCK_SIZE, screenX + BLOCK_SIZE, static_cast<int>(y + 1) * BLOCK_SIZE, GetColor(200, 200, 200), TRUE);
                break;
            }
        }
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetOutApplicationLogValidFlag(FALSE);  // ���O�o�͂�L����

    // �E�B���h�E���[�h�ŋN��
    if (ChangeWindowMode(TRUE) == -1) {
        MessageBox(NULL, "�E�B���h�E���[�h�̐ݒ�Ɏ��s���܂����B", "�G���[", MB_OK);
        return -1;
    }

    // �E�B���h�E�^�C�g����ݒ�
    if (SetWindowText("�Ȃ��݂̑�`��") == -1) {
        MessageBox(NULL, "�E�B���h�E�^�C�g���̐ݒ�Ɏ��s���܂����B", "�G���[", MB_OK);
        return -1;
    }

    // ��ʃ��[�h�̐ݒ�
    if (SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32) == -1) {
        MessageBox(NULL, "��ʃ��[�h�̐ݒ�Ɏ��s���܂����B", "�G���[", MB_OK);
        return -1;
    }

    // DX���C�u�����̏�����
    if (DxLib_Init() == -1) {
        MessageBox(NULL, "DX���C�u�����̏������Ɏ��s���܂����B", "�G���[", MB_OK);
        return -1;
    }

    // �`����ʂ𗠉�ʂɃZ�b�g
    if (SetDrawScreen(DX_SCREEN_BACK) == -1) {
        MessageBox(NULL, "�`����ʂ̐ݒ�Ɏ��s���܂����B", "�G���[", MB_OK);
        DxLib_End();
        return -1;
    }

    // DX���C�u�����̏�������������������ɒǉ�
    ClearDrawScreen();
    DrawString(10, 10, "�����������I", GetColor(255, 255, 255));
    ScreenFlip();


    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        // �Q�[���̍X�V����
        UpdatePlayer();
        UpdateCamera();

        // �`�揈��
        DrawMap();

        // �v���C���[�̕`��
        DrawBox(static_cast<int>(player.x) - cameraX, static_cast<int>(player.y),
            static_cast<int>(player.x) - cameraX + player.width, static_cast<int>(player.y) + player.height,
            GetColor(255, 0, 0), TRUE);


        ScreenFlip();

        WaitTimer(16); // ��60FPS�ɐ���
        if (ProcessMessage() == -1) break;
        if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
    }

    DxLib_End();
    return 0;
}