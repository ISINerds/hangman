#define RAYGUI_IMPLEMENTATION
#include "includes/utils/src/raylib.h"
#include "includes/utils/src/raygui.h"

#include "includes/utils/words-handler.h"
#include "includes/utils/rankings-handler.h"
#include "includes/utils/audio.h"
#include "includes/utils/particles.h"
#include "includes/utils/snow.h"
#include "includes/data-structures/dictionary.h"
#include<ctype.h>
// --- GLOBAL STYLE VARIABLES

// #define textPadding 10
// #define borderRadius 0.1
// #define gap 10
// #define padding 5
// #define textSize 30
#define letterMargin 5
#define letterSize w * 0.05

const int screenWidth = 800;
const int screenHeight = 450;
int w, h; //width and height for responsiveness

//Back Arrow on the top left
Image image;
Texture2D texture;

Color backgroundColor = (Color){0, 0, 20, 255} ;

// For the LOST and WIN pages 
float gameResultMessageY = -100.0f;

#define MAX_INPUT_CHARS     9
char userName[MAX_INPUT_CHARS + 1] = "\0";      // NOTE: One extra space required for null terminator char '\0'
int letterCount = 0;

Rectangle userNameTextBox;
bool mouseOnText = false;

int framesCounter = 0;

// --- GLOBAL VARIABLES
bool dropDown1 = false;
bool dropDown2 = false;
char* singlePlayerOptions = "Hangman;Guess The Word";
char* twoPlayersOptions = "Dual Hangman;Sudden Death";
char* difficultyOptions = "Easy;Medium;Hard";

typedef enum {
    HANGMAN=0,
    GUESS_THE_WORD=1,
    DUAL_HANGMAN=2,
    SUDDEN_DEATH=3,
} SELECTED_OPTION;
SELECTED_OPTION selectedOption = 0;

// which page to display
typedef enum {
    WELCOME_PAGE=0,
    SIGNLE_PLAYER_PAGE =1,
    HANGMAN_PAGE = 2,
    GUESS_THE_WORD_PAGE = 3,
    TWO_PLAYER_PAGE = 4,
    DUAL_HANGMAN_PAGE = 5,
    SUDDEN_DEATH_PAGE = 6,
    RANKINGS = 7,
    WIN_PAGE = 8,
    LOST_PAGE=9,
} PAGE_NUMBER;
PAGE_NUMBER pageNumber = WELCOME_PAGE;

Level selectedDifficulty;

// Define the camera to look into our 3d world
// Camera camera = { { -5.0f, 10.0f, 10.0f }, { -5.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
Camera camera = { { -5.1f, 4.0f, 15.6f }, { -5.9f, 3.6f, -0.1f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

// Hangman pieces number
const int piecesNumber = 10;
int nb = 0;
// Hangman pieces
Vector3 hangmanPiecesPositions[11] = {{-0.3f, 0.3f, 0.8f}, {-2.0f, 2.7f, 0.9f}, {-1.0f, 5.0f, 0.9f}, {0.0f, 4.5f, 0.9f}, {0.0f, 4.2f, 0.9f}, {0.0f, 3.2f, 0.9f}, {0.5f, 3.2f, 0.9f}, {-0.5f, 3.2f, 0.9f}, {-0.2f, 2.2f, 0.9f}, {0.2f, 2.2f, 0.9f}};
Vector3 hangmanPiecesSizes[11] = {{4.0f, 0.3f, 2.0f}, {0.2f, 4.5f, 0.2f}, {3.0f, 0.2f, 1.3f}, {0.1f, 0.8f, 0.2f}, {0.5f, 0.6f, 0.7f}, {0.8f, 1.4f, 0.7f}, {0.2f, 1.0f, 0.2f}, {0.2f, 1.0f, 0.2f}, {0.2f, 1.0f, 0.2f}, {0.2f, 1.0f, 0.2f}};

char clicked[26] = "__________________________";
char* wordToGuess = NULL;
char wordToShow[] = "_________________________________";


Words words;
Level level;
Dictionary* dic = NULL;

Sound success;
Sound fail;
Sound win;
Sound lost; 
Sound backgroundMusic;
int backgroundMusicStarted = 0; // false
int lostSound = 0;
// Variables for timer for the background music
double startTime;
double elapsedTime = 0;
double delayTime = 120.0; // 2 minutes delay ( background music is 2 minutes and 4 seconds)
void welcomePage() {
    drawParticles();
    int textWidth = MeasureText("Welcome To Hangman Game", w * 0.03);
    DrawText("Welcome To Hangman Game", w / 2 - textWidth / 2, h * 0.1, w * 0.03, GRAY);
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.4 ,w * 0.4, h * 0.1},"1 Player Mode")) {
        pageNumber = SIGNLE_PLAYER_PAGE;
    }
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.5 + 5 ,w * 0.4, h * 0.1},"2 Player Mode")) {
        pageNumber = TWO_PLAYER_PAGE;
    }
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.6 + 10 ,w * 0.4, h * 0.1},"Rankings")) {
        pageNumber = RANKINGS;
    }
}

void singlePlayerPage() {
    drawParticles();
//    if(selectedOption!=GUESS_THE_WORD && selectedOption!=HANGMAN){
//       selectedOption = GUESS_THE_WORD; // default value
//    }
   if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.6 + 15 ,w * 0.4, h * 0.1},"Back") && !dropDown1 && !dropDown2) {
        pageNumber = WELCOME_PAGE;
    }
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.5 + 10 ,w * 0.4, h * 0.1},"Start") && !dropDown1 && !dropDown2) {
        if(selectedOption == HANGMAN) {
            if(wordToGuess != NULL) free(wordToGuess);
            wordToGuess = NULL;
            for(int i=0;i<strlen(wordToShow);i++) wordToShow[i] = '_';
            // wordToShow = "_________________________________";
            for(int i=0;i<26;i++) clicked[i] = '_';
            // clicked = "__________________________";
            nb = 0;
            pageNumber = HANGMAN_PAGE;
            if(selectedDifficulty == EASY) level = EASY;
            else if(selectedDifficulty == MEDIUM) level = MEDIUM;
            else level = HARD;
            wordToGuess = randomWord(words, level);
            // printf("word to guess: %s\n", wordToGuess);
        }
        else {
            pageNumber = GUESS_THE_WORD_PAGE;
        }
    }
    if(!dropDown1 && GuiDropdownBox((Rectangle){w / 2 - w * 0.2, h * 0.4 + 5 ,w * 0.4, h * 0.1}, difficultyOptions, &selectedDifficulty, dropDown2)) {
        dropDown2 = !dropDown2;
    }
    if(GuiDropdownBox((Rectangle){w / 2 - w * 0.2, h * 0.3 ,w * 0.4, h * 0.1}, singlePlayerOptions, &selectedOption, dropDown1)) {
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
    drawParticles();
//    if(selectedOption!=DUAL_HANGMAN && selectedOption!=SUDDEN_DEATH){
//       selectedOption = DUAL_HANGMAN; // default value
//    }
//    if(selectedOption > 1) selectedOption -= 2;
   if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.55 + 10 ,w * 0.4, h * 0.1},"Back") && !dropDown1 && !dropDown2) {
        pageNumber = WELCOME_PAGE;
    }
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.45 + 5 ,w * 0.4, h * 0.1},"Start") && !dropDown1) {
        // TODO : Fix the selected option for the 2 player mode (create another enum or try another solution)
        if(selectedOption == HANGMAN) {
            pageNumber = DUAL_HANGMAN_PAGE;
        }
        else {
            pageNumber = SUDDEN_DEATH_PAGE;
        }
    }
    if(GuiDropdownBox((Rectangle){w / 2 - w * 0.2, h * 0.35,w * 0.4, h * 0.1}, twoPlayersOptions,  &selectedOption, dropDown1)) {
        dropDown1 = !dropDown1;
    }
}

// Image button control, returns true when clicked
bool GuiImageButtonEx(Rectangle bounds, const char *text, Texture2D texture, Rectangle texSource)
{
    GuiControl state = guiState;
    bool clicked = false;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked)
    {
        Vector2 mousePoint = GetMousePosition();

        // Check button state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) clicked = true;
            else state = STATE_FOCUSED;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(BUTTON, BORDER_WIDTH), Fade(GetColor(GuiGetStyle(BUTTON, BORDER + (state*3))), guiAlpha), Fade(GetColor(GuiGetStyle(BUTTON, BASE + (state*3))), guiAlpha));

    GuiDrawText(text, GetTextBounds(BUTTON, bounds), GuiGetStyle(BUTTON, TEXT_ALIGNMENT), Fade(GetColor(GuiGetStyle(BUTTON, TEXT + (state*3))), guiAlpha));
    if (texture.id > 0) DrawTextureRec(texture, texSource, RAYGUI_CLITERAL(Vector2){ bounds.x + bounds.width/2 - texSource.width/2, bounds.y + bounds.height/2 - texSource.height/2 }, Fade(GetColor(GuiGetStyle(BUTTON, TEXT + (state*3))), guiAlpha));
    //------------------------------------------------------------------

    return clicked;
}

// Image button control, returns true when clicked
bool GuiImageButton(Rectangle bounds, const char *text, Texture2D texture)
{
    return GuiImageButtonEx(bounds, text, texture, RAYGUI_CLITERAL(Rectangle){ 0, 0, (float)texture.width, (float)texture.height });
}

void rankingsPage() {
    if(GuiImageButton((Rectangle){ 10, 10, w * 0.05, w * 0.05 }, "", texture)) {
        pageNumber = WELCOME_PAGE;
    }
    // if(GuiButton((Rectangle){20, 20 ,w * 0.05, w * 0.05},"")) {
    //     pageNumber = WELCOME_PAGE;
    // }
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.4 ,w * 0.4, h * 0.1},"Not Implemented Yet")) {
    }
}
// function that draws lines under the word the user has to guess
void initLines(int lettersNumber) {
    int totalLength = lettersNumber * letterSize + (lettersNumber - 1) * letterMargin;
    int start = w / 2 - totalLength / 2;
    for(int i = 0;i<lettersNumber;i++) {
        DrawRectangle(start + i * (letterSize + letterMargin), h * 0.3, letterSize, 5, GRAY);
    }
}

void drawGuessedLetter(char* letter, int position, int lettersNumber) {
    int totalLength = lettersNumber * letterSize + (lettersNumber - 1) * letterMargin;
    int start = w / 2 - totalLength / 2;
    DrawText(letter, start + position * (letterSize + letterMargin) + w * 0.01, h * 0.3 - letterSize, letterSize, DARKGRAY);
}

int checkWord(char * wordToGuess , char * wordToShow){

    for(int i=0 ; i<strlen(wordToGuess);i++){
        if(tolower(wordToShow[i]) != wordToGuess[i]){
            return 0; // False
        }
    }
    return 1; // True
}
void hangman() {
    if(GuiImageButton((Rectangle){ 10, 10, w * 0.05, w * 0.05 }, "", texture)) {
        pageNumber = WELCOME_PAGE;
    }
    // if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) nb++;
    nb %= piecesNumber+1;
    if(false){
    // if (IsKeyDown(KEY_UP)) camera.position.y +=0.1;
    // else if (IsKeyDown(KEY_DOWN)) camera.position.y-=0.1;
    // else if (IsKeyDown(KEY_RIGHT)) camera.position.z+=0.1;
    // else if (IsKeyDown(KEY_LEFT)) camera.position.z-=0.1;
    // else if (IsKeyDown(KEY_J)) camera.position.x+=0.1;
    // else if (IsKeyDown(KEY_K)) camera.position.x-=0.1;
    // if (IsKeyDown(KEY_W)) camera.target.y +=0.1;
    // else if (IsKeyDown(KEY_S)) camera.target.y-=0.1;
    // else if (IsKeyDown(KEY_D)) camera.target.z+=0.1;
    // else if (IsKeyDown(KEY_A)) camera.target.z-=0.1;
    // else if (IsKeyDown(KEY_R)) camera.target.x+=0.1;
    // else if (IsKeyDown(KEY_T)) camera.target.x-=0.1;

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
    }
    
        
    initLines(strlen(wordToGuess));
    for(int i=0;i<strlen(wordToGuess);i++) {
        if(wordToShow[i] != '_') {
            char str[] = {wordToShow[i], '\0'};
            drawGuessedLetter(str, i, strlen(wordToGuess));
        }
    }

    for(int i=0;i<26;i++) {
        char str[] = {'A' + i, '\0'};
        // Use the strchr function to search for the character
        char* foundChar = strchr(clicked, 'A' + i);
        bool test = true;
        // Check if the character was clicked
        if (foundChar != NULL) {
            test = false;
        }
        if(i < 10) {
            if(test) {
                if(GuiButton((Rectangle){letterMargin * (4 + i) + letterSize * i, h * 0.5, letterSize, letterSize},str)) {
                    clicked[i] = 'A' + i;
                    // printf("clicked: %s\n", clicked);
                    // printf("wordtoguess: %s\n", wordToGuess);
                    // printf("wordtoshow: %s\n", wordToShow);
                    int * letterPositionsInWordToGuess = searchLetter(dic, wordToGuess, 'a' + i);
                    // for(int i=0;i < strlen(wordToGuess);i++) {
                        // printf("%d ", letterPositionsInWordToGuess[i]);
                        // drawGuessedLetter(str, letterPositionsInWordToGuess[i], strlen(wordToGuess));
                    // }
                    // printf("\n");
                    if(letterPositionsInWordToGuess == NULL || letterPositionsInWordToGuess[0] == -1) {
                        //draw hangman
                        playFailSound(fail);
                        nb++;
                        if(letterPositionsInWordToGuess != NULL) free(letterPositionsInWordToGuess);
                    }
                    else {
                        playSuccessSound(success);
                        for(int j=0;j < strlen(wordToGuess)&&letterPositionsInWordToGuess[j]!=-1;j++) {
                            // printf("%d %d\n", j, letterPositionsInWordToGuess[j]);
                            // drawGuessedLetter(str, letterPositionsInWordToGuess[i], strlen(wordToGuess));
                            wordToShow[letterPositionsInWordToGuess[j]] = 'A' + i;
                        }
                        free(letterPositionsInWordToGuess);
                    }
                }
            }
            else {
                DrawRectangle(letterMargin * (4 + i) + letterSize * i, h * 0.5, letterSize, letterSize, GRAY);
                DrawText(str, letterMargin * (4 + i) + letterSize * i + letterSize / 2 - MeasureText(str, h * 0.05) / 2, h * 0.5 + letterSize / 4, h * 0.05, LIGHTGRAY);
            }
        }
        else if(i < 20) {
            if(test) {
                if(GuiButton((Rectangle){letterMargin * (4 + i - 10) + letterSize * (i - 10), h * 0.5 + letterMargin + letterSize, letterSize, letterSize},str)) {
                    clicked[i] = 'A' + i;
                    // printf("clicked: %s\n", clicked);
                    int * letterPositionsInWordToGuess = searchLetter(dic, wordToGuess, 'a' + i);
                    if(letterPositionsInWordToGuess == NULL || letterPositionsInWordToGuess[0] == -1) {
                        //draw hangman
                        playFailSound(fail);
                        nb++;
                        if(letterPositionsInWordToGuess != NULL) free(letterPositionsInWordToGuess);
                    }
                    else {
                        playSuccessSound(success);

                        for(int j=0;j < strlen(wordToGuess)&&letterPositionsInWordToGuess[j]!=-1;j++) {
                            // printf("%d %d\n", j, letterPositionsInWordToGuess[j]);
                            // drawGuessedLetter(str, letterPositionsInWordToGuess[i], strlen(wordToGuess));
                            wordToShow[letterPositionsInWordToGuess[j]] = 'A' + i;
                        }
                        // printf("\n");
                        free(letterPositionsInWordToGuess);
                    }
                }
            }
            else {
                DrawRectangle(letterMargin * (4 + i - 10) + letterSize * (i - 10), h * 0.5 + (letterMargin + letterSize), letterSize, letterSize, GRAY);
                DrawText(str, letterMargin * (4 + i - 10) + letterSize * (i - 10) + letterSize / 2 - MeasureText(str, h * 0.05) / 2, h * 0.5 + (letterMargin + letterSize) + letterSize / 4, h * 0.05, LIGHTGRAY);
            }
        }
        else {
            if(test) {
                if(GuiButton((Rectangle){letterMargin * (4 + i - 18) + letterSize * (i - 18), h * 0.5 + (letterMargin + letterSize) * 2, letterSize, letterSize},str)) {
                    clicked[i] = 'A' + i;
                    // printf("clicked: %s\n", clicked);
                    int * letterPositionsInWordToGuess = searchLetter(dic, wordToGuess, 'a' + i);
                    if(letterPositionsInWordToGuess == NULL || letterPositionsInWordToGuess[0] == -1) {
                        //draw hangman
                        playFailSound(fail);
                        nb++;
                        if(letterPositionsInWordToGuess != NULL) free(letterPositionsInWordToGuess);
                    }
                    else {
                        playSuccessSound(success);
                        for(int j=0;j < strlen(wordToGuess)&&letterPositionsInWordToGuess[j]!=-1;j++) {
                            // printf("%d %d\n", j, letterPositionsInWordToGuess[j]);
                            // drawGuessedLetter(str, letterPositionsInWordToGuess[i], strlen(wordToGuess));
                            wordToShow[letterPositionsInWordToGuess[j]] = 'A' + i;
                        }
                        // printf("\n");
                        free(letterPositionsInWordToGuess);
                    }
                }
            }
            else {
                DrawRectangle(letterMargin * (4 + i - 18) + letterSize * (i - 18), h * 0.5 + (letterMargin + letterSize) * 2, letterSize, letterSize, GRAY);
                DrawText(str, letterMargin * (4 + i - 18) + letterSize * (i - 18) + letterSize / 2 - MeasureText(str, h * 0.05) / 2, h * 0.5 + (letterMargin + letterSize) * 2 + letterSize / 4, h * 0.05, LIGHTGRAY);
            }
        }
        }
        // for(int i=0;i<ran;i++) {
        //     int ranChar = random() % 32;
        //     char str[2] = {ranChar + 65, '\0'};
            // drawGuessedLetter(str, i, ran);
        // }
        BeginMode3D(camera);

        for(int i=0;i<nb;i++) {
            DrawCube(hangmanPiecesPositions[i], hangmanPiecesSizes[i].x, hangmanPiecesSizes[i].y, hangmanPiecesSizes[i].z, GRAY);
            DrawCubeWires(hangmanPiecesPositions[i], hangmanPiecesSizes[i].x, hangmanPiecesSizes[i].y, hangmanPiecesSizes[i].z, DARKGRAY);
        }
        DrawGrid(7, 1);        // Draw a grid
        EndMode3D();
        // Test if the user still has attempts or not
        if(nb>=piecesNumber){
            printf("You lost\n");
            pageNumber = LOST_PAGE;
        }
        printf("%s",wordToGuess);
        // Test if the user guessed the word (complete the condition)
        if(nb<piecesNumber && checkWord(wordToGuess,wordToShow)){
            pageNumber = WIN_PAGE;
            printf("You win\n");
        }
}

void guessTheWord() {
    if(GuiImageButton((Rectangle){ 10, 10, w * 0.05, w * 0.05 }, "", texture)) {
        pageNumber = WELCOME_PAGE;
    }
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.55 + 10 ,w * 0.4, h * 0.1},"Guess the Word")) {
    }
}

void dualHangMan() {
    if(GuiImageButton((Rectangle){ 10, 10, w * 0.05, w * 0.05 }, "", texture)) {
        pageNumber = WELCOME_PAGE;
    }
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.55 + 10 ,w * 0.4, h * 0.1},"Dual HangMan")) {
    }
}

void suddenDeath() {
    if(GuiImageButton((Rectangle){ 10, 10, w * 0.05, w * 0.05 }, "", texture)) {
        pageNumber = WELCOME_PAGE;
    }
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.55 + 10 ,w * 0.4, h * 0.1},"Sudden Death")) {
    }
}
void displayUsernameInput(){

    userNameTextBox =(Rectangle) { w/2.0f - 100*1.7, h*0.5, 225*1.7, 50 };
    if (CheckCollisionPointRec(GetMousePosition(), userNameTextBox)) mouseOnText = true;
    else mouseOnText = false;

    if (mouseOnText){
            // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            // Get char pressed (unicode character) on the queue
            int key = GetCharPressed();
            // Check if more characters have been pressed on the same frame
            while (key > 0){
                // NOTE: Only allow keys in range [32..125]
                if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS)){
                    userName[letterCount] = (char)key;
                    userName[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                    letterCount++;
                }
                key = GetCharPressed();  // Check next character in the queue
            }
            // Remove a letter
            if (IsKeyPressed(KEY_BACKSPACE)){
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                userName[letterCount] = '\0';
            }
        }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (mouseOnText) framesCounter++;
    else framesCounter = 0;


    DrawText("Enter your username !",  w / 2 - MeasureText("Enter your username !", w*0.03) / 2 , h*0.4, w*0.03, WHITE);
    DrawRectangleRec(userNameTextBox, WHITE);
    
    if (mouseOnText) DrawRectangleLines((int)userNameTextBox.x, (int)userNameTextBox.y, (int)userNameTextBox.width, (int)userNameTextBox.height, BLUE);
    else DrawRectangleLines((int)userNameTextBox.x, (int)userNameTextBox.y, (int)userNameTextBox.width, (int)userNameTextBox.height, DARKGRAY);

    DrawText(userName, (int)userNameTextBox.x + 5, (int)userNameTextBox.y + 8, 40, GRAY);
    
    DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, MAX_INPUT_CHARS), w / 2 - MeasureText("INPUT CHARS: %i/%i", 20) / 2, h*0.5 + 60, 20, DARKGRAY);

    if (mouseOnText){
        if (letterCount < MAX_INPUT_CHARS){
            // Draw blinking underscore char
            if (((framesCounter/20)%2) == 0) DrawText("_", (int)userNameTextBox.x + 8 + MeasureText(userName, 40), (int)userNameTextBox.y + 12, 40, BLUE);
        }
        else DrawText("Press BACKSPACE to delete chars...", w / 2 - MeasureText("Press BACKSPACE to delete chars...", 20) / 2, h*0.5 + 90, 20, GRAY);
    }
    if (IsKeyPressed(KEY_ENTER)) {
        addRanking("src/rankings.txt",userName,evaluateWord(wordToGuess)-nb);
            // Print the entered name to the console (you can replace this with your desired action)
            printf("Entered name: %s\n", userName);
            pageNumber = WELCOME_PAGE;
            // Intialize variables to 0 or null
    }
}
// Check if any key is pressed
// NOTE: We limit keys check to keys between 32 (KEY_SPACE) and 126
// bool IsAnyKeyPressed()
// {
//     bool keyPressed = false;
//     int key = GetKeyPressed();

//     if ((key >= 32) && (key <= 126)) keyPressed = true;

//     return keyPressed;
// }
void lostPage(){
    if(!lostSound){
    playLostSound(lost);
    lostSound = 1;
    }
    int textWidth = MeasureText(TextFormat("You Lost! The word was: %s ",wordToGuess), w * 0.04) ;
    DrawText( TextFormat("You Lost! The word was: %s ",wordToGuess), w / 2 - textWidth / 2, h * 0.1, w * 0.04, RED);

    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.4  ,w * 0.4, h * 0.1},"Try again")) {
        pageNumber = WELCOME_PAGE;
        lostSound =0;
    }
}
void winPage(){
    if(gameResultMessageY == -100.0f){
        playWinSound(win);
        UnloadSound(win);
    }
    drawSnowflaskes(w,h);

        gameResultMessageY += 2.0f;
        DrawText("Congratulations, You Win!", w / 2 - MeasureText("Congratulations, You Win!", w*0.05) / 2, gameResultMessageY, w*0.05, BLUE);
        if(gameResultMessageY >h*0.25){
            gameResultMessageY = h*0.25;
            displayUsernameInput();
        }

}
void previewScreen() {
    w = GetRenderWidth();
    h = GetRenderHeight();
    ClearBackground(backgroundColor);
    BeginDrawing();
    // For the first time
    if(!backgroundMusicStarted){
        playBackgroundMusic(backgroundMusic);
        backgroundMusicStarted =1;
    }
    elapsedTime = GetTime() - startTime;
    // Check if the specified delay has passed ( after 2 minutes will restart the audio again)
    if (elapsedTime >= delayTime){
        playBackgroundMusic(backgroundMusic);
        // Reset the start time for the next minute
        startTime = GetTime();
    }

        // DrawText("Move player with cursors to collide", 220, 40, 20, GRAY);

        //     // DrawFPS(10, 10);
        //     hangman();
            switch(pageNumber) {
                case WELCOME_PAGE:   welcomePage(); break;
                case SIGNLE_PLAYER_PAGE:  singlePlayerPage(); break;
                case HANGMAN_PAGE : hangman(); break;
                case GUESS_THE_WORD_PAGE : guessTheWord(); break;
                case TWO_PLAYER_PAGE:  twoPlayersPage(); break;
                case DUAL_HANGMAN_PAGE :  dualHangMan(); break;
                case SUDDEN_DEATH_PAGE : suddenDeath(); break;
                case RANKINGS:  rankingsPage(); break;
                case WIN_PAGE : winPage() ; break;
                case LOST_PAGE : lostPage(); break;
            }
        EndDrawing();

}

int main(void) {    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Hangman");
    SetTargetFPS(60);
    initializeParticles();
    // Load image and create texture
    image = LoadImage("assets/340.png");
    Color c = GetImageColor(image, 200, 200);
    // printf("color: %d %d %d", c.r, c.g, c.b);
    ImageColorReplace(&image, BLACK, LIGHTGRAY);
    ImageResize(&image, 30, 30);
    texture = LoadTextureFromImage(image);
    // Unload image as it's already in texture
    UnloadImage(image);
    words = parser("src/words.txt");
    dic = AddAll(dic, "src/words.txt");
    double startTime = GetTime();
    InitAudioDevice();      // Initialize audio device
    while (!WindowShouldClose()) {
            initializeSnowflakes();
      GuiSetStyle(DEFAULT, TEXT_SIZE, h*0.05); // Set the font size to 20
        previewScreen();
    }
    UnloadSound(success);
    UnloadSound(fail);
    UnloadSound(backgroundMusic);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
