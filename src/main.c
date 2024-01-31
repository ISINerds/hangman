/*******************************************************************************************
*
*   raylib [models] example - Detect basic 3d collisions (box vs sphere vs box)
*
*   Example originally created with raylib 1.3, last time updated with raylib 3.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#define RAYGUI_IMPLEMENTATION
#include "includes/utils/src/raylib.h"
#include "includes/utils/src/raygui.h"

#define textPadding 10
#define borderRadius 0.1
#define gap 10
#define padding 5
#define textSize 30

const int screenWidth = 800;
const int screenHeight = 450;

//width and height for responsiveness
int w, h;
bool dropDown1 = false;
bool dropDown2 = false;
char* singlePlayerOptions = "Hangman;Guess The Word";
char* twoPlayersOptions = "Dual Hangman;Sudden Death";
int selectedOption = 0;
char* difficultyOptions = "Easy;Medium;Hard";
int selectedDifficulty = 0;
// Define the camera to look into our 3d world
// Camera camera = { { -5.0f, 10.0f, 10.0f }, { -5.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
Camera camera = { { -5.1f, 4.0f, 15.6f }, { -5.9f, 3.6f, -0.1f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
// hangman pieces number
const int piecesNumber = 10;
int nb = 0;
// hangman pieces
Vector3 hangmanPiecesPositions[11] = {{-0.3f, 0.3f, 0.8f}, {-2.0f, 2.7f, 0.9f}, {-1.0f, 5.0f, 0.9f}, {0.0f, 4.5f, 0.9f}, {0.0f, 4.2f, 0.9f}, {0.0f, 3.2f, 0.9f}, {0.5f, 3.2f, 0.9f}, {-0.5f, 3.2f, 0.9f}, {-0.2f, 2.2f, 0.9f}, {0.2f, 2.2f, 0.9f}};
Vector3 hangmanPiecesSizes[11] = {{4.0f, 0.3f, 2.0f}, {0.2f, 4.5f, 0.2f}, {3.0f, 0.2f, 1.3f}, {0.1f, 0.8f, 0.2f}, {0.5f, 0.6f, 0.7f}, {0.8f, 1.4f, 0.7f}, {0.2f, 1.0f, 0.2f}, {0.2f, 1.0f, 0.2f}, {0.2f, 1.0f, 0.2f}, {0.2f, 1.0f, 0.2f}};
// which page to display
int pageNumber = 0;

void welcomePage() {
    int textWidth = MeasureText("Welcome To Hangman Game", w * 0.03);
    DrawText("Welcome To Hangman Game", w / 2 - textWidth / 2, h * 0.1, w * 0.03, GRAY);
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.4 ,w * 0.4, h * 0.1},"1 Player Mode")) {
        pageNumber = 1;
    }
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.5 + 5 ,w * 0.4, h * 0.1},"2 Player Mode")) {
        pageNumber = 4;
    }
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.6 + 10 ,w * 0.4, h * 0.1},"Rankings")) {
        pageNumber = 7;
    }
}

void singlePlayerPage() {
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.6 + 10 ,w * 0.4, h * 0.1},"Start") && !dropDown1 && !dropDown2) {
        if(selectedOption == 0) {
            pageNumber = 2;
        }
        else {
            pageNumber = 3;
        }
    }
    if(!dropDown1 && GuiDropdownBox((Rectangle){w / 2 - w * 0.2, h * 0.5 + 5 ,w * 0.4, h * 0.1}, difficultyOptions, &selectedDifficulty, dropDown2)) {
        dropDown2 = !dropDown2;
    }
    if(GuiDropdownBox((Rectangle){w / 2 - w * 0.2, h * 0.4 ,w * 0.4, h * 0.1}, singlePlayerOptions, &selectedOption, dropDown1)) {
        dropDown1 = !dropDown1;
        // printf("%d %d\n", selectedOption, dropDown);
    }
    // if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.4 ,w * 0.4, h * 0.1},"Hangman")) {
    //     pageNumber = 2;
    // }
    // if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.5 + 5 ,w * 0.4, h * 0.1},"Guess The Word")) {
    //     pageNumber = 3;
    // }
}

void twoPlayersPage() {
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.5 + 5 ,w * 0.4, h * 0.1},"Start") && !dropDown1) {
        if(selectedOption == 0) {
            pageNumber = 5;
        }
        else {
            pageNumber = 6;
        }
    }
    if(GuiDropdownBox((Rectangle){w / 2 - w * 0.2, h * 0.4,w * 0.4, h * 0.1}, twoPlayersOptions, &selectedOption, dropDown1)) {
        dropDown1 = !dropDown1;
    }
}

void rankingsPage() {
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.4 ,w * 0.4, h * 0.1},"Not Implemented Yet")) {
    }
}

void hangman() {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) nb++;
        nb %= piecesNumber+1;
    
    if (IsKeyDown(KEY_UP)) camera.position.y +=0.1;
    else if (IsKeyDown(KEY_DOWN)) camera.position.y-=0.1;
    else if (IsKeyDown(KEY_RIGHT)) camera.position.z+=0.1;
    else if (IsKeyDown(KEY_LEFT)) camera.position.z-=0.1;
    else if (IsKeyDown(KEY_J)) camera.position.x+=0.1;
    else if (IsKeyDown(KEY_K)) camera.position.x-=0.1;
    if (IsKeyDown(KEY_W)) camera.target.y +=0.1;
    else if (IsKeyDown(KEY_S)) camera.target.y-=0.1;
    else if (IsKeyDown(KEY_D)) camera.target.z+=0.1;
    else if (IsKeyDown(KEY_A)) camera.target.z-=0.1;
    else if (IsKeyDown(KEY_R)) camera.target.x+=0.1;
    else if (IsKeyDown(KEY_T)) camera.target.x-=0.1;

    // if (IsKeyDown(KEY_UP)) hangmanPiecesPositions[nb].z+=0.1;
    // else if (IsKeyDown(KEY_DOWN)) hangmanPiecesPositions[nb].z-=0.1;
    // else if (IsKeyDown(KEY_RIGHT)) hangmanPiecesPositions[nb].x+=0.1;
    // else if (IsKeyDown(KEY_LEFT)) hangmanPiecesPositions[nb].x-=0.1;
    // else if (IsKeyDown(KEY_T)) hangmanPiecesPositions[nb].y+=0.1;
    // else if (IsKeyDown(KEY_R)) hangmanPiecesPositions[nb].y-=0.1;
    // printf("x y z: %f %f %f\n", hangmanPiecesPositions[nb].x, hangmanPiecesPositions[nb].y, hangmanPiecesPositions[nb].z);
    // if (IsKeyDown(KEY_UP)) hangmanPiecesSizes[nb].z+=0.1;
    // else if (IsKeyDown(KEY_DOWN)) hangmanPiecesSizes[nb].z-=0.1;
    // else if (IsKeyDown(KEY_RIGHT)) hangmanPiecesSizes[nb].x+=0.1;
    // else if (IsKeyDown(KEY_LEFT)) hangmanPiecesSizes[nb].x-=0.1;
    // else if (IsKeyDown(KEY_T)) hangmanPiecesSizes[nb].y+=0.1;
    // else if (IsKeyDown(KEY_R)) hangmanPiecesSizes[nb].y-=0.1;
    // printf("x y z: %f %f %f\n", hangmanPiecesSizes[nb].x, hangmanPiecesSizes[nb].y, hangmanPiecesSizes[nb].z);
    // printf("position: x y z; target: x y z %f %f %f %f %f %f\n", camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z);
    // UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        // UpdateCamera(&camera, CAMERA_ORBITAL);
        
        BeginMode3D(camera);

        for(int i=0;i<nb;i++) {
            DrawCube(hangmanPiecesPositions[i], hangmanPiecesSizes[i].x, hangmanPiecesSizes[i].y, hangmanPiecesSizes[i].z, GRAY);
            DrawCubeWires(hangmanPiecesPositions[i], hangmanPiecesSizes[i].x, hangmanPiecesSizes[i].y, hangmanPiecesSizes[i].z, DARKGRAY);
        }
        


        

        
            DrawGrid(7, 1);        // Draw a grid

        EndMode3D();
        
}



void previewScreen() {
    w = GetRenderWidth();
    h = GetRenderHeight();
    ClearBackground(RAYWHITE);
    BeginDrawing();

        // DrawText("Move player with cursors to collide", 220, 40, 20, GRAY);

        //     // DrawFPS(10, 10);
        //     hangman();
            switch(pageNumber) {
                case 0: welcomePage(); break;
                case 1: singlePlayerPage(); break;
                case 2: hangman(); break;
                case 4: twoPlayersPage(); break;
                case 7: rankingsPage(); break;
            }
        EndDrawing();

}

int main(void) {    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Hangman");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
      GuiSetStyle(DEFAULT, TEXT_SIZE, h*0.05); // Set the font size to 20
        previewScreen();
    }
    CloseWindow();
    return 0;
}