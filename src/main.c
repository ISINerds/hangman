#define RAYGUI_IMPLEMENTATION
#include "includes/utils/src/raylib.h"
#include "includes/utils/src/raygui.h"

#include "includes/utils/words-handler.h"
#include "includes/utils/rankings-handler.h"
#include "includes/utils/audio.h"
#include "includes/utils/particles.h"
#include "includes/utils/snow.h"
#include "includes/data-structures/dictionary.h"
// --- GLOBAL STYLE VARIABLES
#define letterMargin 5
#define letterSize w * 0.05
const int screenWidth = 800;
const int screenHeight = 450;
int w, h; //width and height for responsiveness

//Back Arrow on the top left
Image image;
Texture2D texture;
Font font;
Color backgroundColor = (Color){0, 0, 20, 255} ;

// For the LOST and WIN pages 
float gameResultMessageY = -100.0f;
// For The input in the win page
#define MAX_INPUT_CHARS 9
char userName[MAX_INPUT_CHARS + 1] = "\0";      // NOTE: One extra space required for null terminator char '\0'
int letterCount = 0;
Rectangle userNameTextBox;
bool mouseOnText = false;
int framesCounter = 0;

// --- GLOBAL VARIABLES
bool dropDown1 = false;
bool dropDown2 = false;
char* difficultyOptions = "Easy;Medium;Hard";


// which page to display
typedef enum {
    WELCOME_PAGE=0,
    SIGNLE_PLAYER_PAGE =1,
    HANGMAN_PAGE = 2,
    TWO_PLAYER_PAGE = 3,
    DUAL_HANGMAN_PAGE = 4,
    RANKINGS = 5,
    WIN_PAGE = 6,
    LOST_PAGE=7,
    WIN_PAGE_TWO_PLAYERS = 8,
} PAGE_NUMBER;
PAGE_NUMBER pageNumber = WELCOME_PAGE;

Level selectedDifficulty;
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
bool playerOneTurn = true;

Words words;
Level level;
Dictionary* dic = NULL;
Rankings rankings = {NULL, 0};
float rankingsScroll = 0;

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
    Vector2 textWidth = MeasureTextEx(font,"Welcome To Hangman Game", w * 0.05,0);
    DrawTextEx(font,"Welcome To Hangman Game",(Vector2){w / 2 - textWidth.x / 2, h * 0.1},w*0.05,0,LIGHTGRAY);

    // One Player Mode
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.4 ,w * 0.4, h * 0.1},"1 Player Mode")) {
        pageNumber = SIGNLE_PLAYER_PAGE; 
    }
    // Two Player Mode 
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.5 + 7 ,w * 0.4, h * 0.1},"2 Player Mode")) {
        pageNumber = TWO_PLAYER_PAGE;
    }
    // Display The Rankings
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.6 + 14 ,w * 0.4, h * 0.1},"Rankings")) {
        pageNumber = RANKINGS;
        rankingsScroll = 0;
        rankings = parserRankings("src/rankings.txt");
        // Sort The Rankings In Decreasing Order Based On their Score 
        sortRankings(rankings);
    }
}

void singlePlayerPage() {
    // Draw The particles 
    drawParticles();

    Vector2 textWidth = MeasureTextEx(font,"Select The Difficulty Level", w * 0.05,0);
    DrawTextEx(font,"Select The Difficulty Level",(Vector2){w / 2 - textWidth.x / 2, h * 0.15},w*0.05,0,WHITE);


    //  Back To Welcome Page 
   if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.6 + 15 ,w * 0.4, h * 0.1},"Back")  && !dropDown2) {
        pageNumber = WELCOME_PAGE;
    }
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.5 + 10 ,w * 0.4, h * 0.1},"Start") && !dropDown2) {
        // Initialize All The Used Variables In case Player Want To Replay
        gameResultMessageY = -100.0f;
        if(wordToGuess != NULL) free(wordToGuess);
        wordToGuess = NULL;
        for(int i=0;i<strlen(wordToShow);i++) wordToShow[i] = '_';
        for(int i=0;i<26;i++) clicked[i] = '_';
        nb = 0;
        pageNumber = HANGMAN_PAGE;
        // The Level Selected By The User
        if(selectedDifficulty == EASY) level = EASY;
        else if(selectedDifficulty == MEDIUM) level = MEDIUM;
        else level = HARD;
        wordToGuess = randomWord(words, level);
        printf("word to guess: %s\n", wordToGuess);   
    }
    // Display The Difficulty Options
    if( GuiDropdownBox((Rectangle){w / 2 - w * 0.2, h * 0.4 + 5 ,w * 0.4, h * 0.1}, difficultyOptions, &selectedDifficulty, dropDown2)) {
        dropDown2 = !dropDown2;
    }
}

void twoPlayersPage() {
    // Draw Particles
    drawParticles();
    Vector2 textWidth = MeasureTextEx(font,"Select The Difficulty Level", w * 0.05,0);
    DrawTextEx(font,"Select The Difficulty Level",(Vector2){w / 2 - textWidth.x / 2, h * 0.15},w*0.05,0,WHITE);
    //  Back To Welcome Page
   if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.6 + 15 ,w * 0.4, h * 0.1},"Back") && !dropDown2) {
        pageNumber = WELCOME_PAGE;
    }
    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.5 + 10 ,w * 0.4, h * 0.1},"Start")  && !dropDown2) {
        // Initialize All The Variables In Case User Wants To Replay
        gameResultMessageY = -100.0f;
        playerOneTurn = true;
        if(wordToGuess != NULL) free(wordToGuess);
        wordToGuess = NULL;
        for(int i=0;i<strlen(wordToShow);i++) wordToShow[i] = '_';
        for(int i=0;i<26;i++) clicked[i] = '_';
        nb = 0;
        pageNumber = DUAL_HANGMAN_PAGE;
        if(selectedDifficulty == EASY) level = EASY;
        else if(selectedDifficulty == MEDIUM) level = MEDIUM;
        else level = HARD;
        wordToGuess = randomWord(words, level);
        printf("word to guess: %s\n", wordToGuess);
    }
    // Display The Difficulty Options
    if(GuiDropdownBox((Rectangle){w / 2 - w * 0.2, h * 0.4 + 5 ,w * 0.4, h * 0.1}, difficultyOptions, &selectedDifficulty, dropDown2)) {
        dropDown2 = !dropDown2;
    }
}

// Image button control, returns true when clicked
bool GuiImageButtonEx(Rectangle bounds, const char *text, Texture2D texture, Rectangle texSource){
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
bool GuiImageButton(Rectangle bounds, const char *text, Texture2D texture){
    return GuiImageButtonEx(bounds, text, texture, RAYGUI_CLITERAL(Rectangle){ 0, 0, (float)texture.width, (float)texture.height });
}

void rankingsPage() {
    // Back To Welcome Page
    if(GuiImageButton((Rectangle){ 10, 10, w * 0.05, w * 0.05 }, "", texture)) {
        pageNumber = WELCOME_PAGE;
    }
    // For The Scrolling Behaviour
    Rectangle boundry = {w / 8, h * 0.15, 3 * w / 4, h * 0.8};
    if(CheckCollisionPointRec(GetMousePosition(), boundry)) {
        if(GetMouseWheelMove()){
            rankingsScroll += 5 * GetMouseWheelMove() * boundry.width * GetFrameTime();
        }
    }
    // Display The Table Header
    rankingsScroll = fmin(0, rankingsScroll);
    rankingsScroll = fmax(-(h * 0.13 + h * 0.18 + h * 0.075 * rankings.numberOfPlayers - h), rankingsScroll);
    Vector2 textWidth = MeasureTextEx(font,"Rankings", w * 0.04,0);
    DrawTextEx(font,"Rankings",(Vector2){w / 2 - textWidth.x / 2, h * 0.05},w*0.04,0,GRAY);
    BeginScissorMode(w / 8, h * 0.15, 3 * w / 4, h * 0.8);
    GuiButton((Rectangle){w / 8, h * 0.15 + rankingsScroll, w / 4 - h * 0.0025, h * 0.1}, "Rank");
    GuiButton((Rectangle){3*w / 8 + h * 0.0025, h * 0.15 + rankingsScroll, w / 4 - h * 0.0025, h * 0.1}, "Player");
    GuiButton((Rectangle){5*w / 8 + h * 0.0050, h * 0.15 + rankingsScroll, w / 4 - h * 0.0025, h * 0.1}, "Score");
    // Display The Table Content 
    for(int i=0;i<rankings.numberOfPlayers;i++) {
        GuiButton((Rectangle){w / 8 , h * 0.18 + h * 0.075 * (i+1) + rankingsScroll, w / 4 - h * 0.0025, h * 0.07}, TextFormat("%d",i+1));
        GuiButton((Rectangle){3*w / 8 + h * 0.0025, h * 0.18 + h * 0.075 * (i+1) + rankingsScroll, w / 4 - h * 0.0025, h * 0.07}, TextFormat("%s",rankings.players[i].username));
        GuiButton((Rectangle){5*w / 8 + h * 0.0050 , h * 0.18 + h * 0.075 * (i+1) + rankingsScroll, w / 4 - h * 0.0025, h * 0.07}, TextFormat("%.2f",rankings.players[i].score));
    }
    EndScissorMode();
}
// To Diplay Lines Under The Word Letters To Guess
void initLines(int lettersNumber) {
    int totalLength = lettersNumber * letterSize + (lettersNumber - 1) * letterMargin;
    int start = w / 2 - totalLength / 2;
    for(int i = 0;i<lettersNumber;i++) {
        DrawRectangle(start + i * (letterSize + letterMargin), h * 0.3, letterSize, 5, DARKGRAY);
    }
}
// Display The Correct Letters Guessed By The User
void drawGuessedLetter(char* letter, int position, int lettersNumber) {
    int totalLength = lettersNumber * letterSize + (lettersNumber - 1) * letterMargin;
    int start = w / 2 - totalLength / 2;
    DrawTextEx(font,letter, (Vector2){start + position * (letterSize + letterMargin) + w * 0.01, h * 0.3 - letterSize}, letterSize,0, DARKGRAY);
}
// Check If Word Was Guessed By The User
int checkWord(char * wordToGuess , char * wordToShow){

    for(int i=0 ; i<strlen(wordToGuess);i++){
        if(tolower(wordToShow[i]) != wordToGuess[i]){
            return 0; // False
        }
    }
    return 1; // True
}
void hangman() {
    // Back To Welcome Page
    if(GuiImageButton((Rectangle){ 10, 10, w * 0.05, w * 0.05 }, "", texture)) {
        pageNumber = WELCOME_PAGE;
    }
        
    initLines(strlen(wordToGuess));
    for(int i=0;i<strlen(wordToGuess);i++) {
        if(wordToShow[i] != '_') {
            char str[] = {wordToShow[i], '\0'};
            drawGuessedLetter(str, i, strlen(wordToGuess));
        }
    }
    // Draw The Keyboard
    for(int i=0;i<26;i++) {
        char str[] = {'A' + i, '\0'};
        // Use the strchr function to search for the character
        char* foundChar = strchr(clicked, 'A' + i);
        bool test = true; // Check If The Button Was Not Clicked
        // Check if the character was clicked
        if (foundChar != NULL) {
            test = false;
        }
        //  For A To J
        if(i < 10) {
            if(test) {
                if(GuiButton((Rectangle){letterMargin * (4 + i) + letterSize * i, h * 0.5, letterSize, letterSize},str) || IsKeyPressed(qwertyToAzerty('A'+i))) {
                    clicked[i] = 'A' + i;
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
                            wordToShow[letterPositionsInWordToGuess[j]] = 'A' + i;
                        }
                        free(letterPositionsInWordToGuess);
                    }
                }
            }
            else {
                if(strchr(wordToGuess,'a'+i)!=NULL){
                    DrawRectangle(letterMargin * (4 + i) + letterSize * i, h * 0.5, letterSize, letterSize, (Color){0, 128, 0, 255});
                }
                else{
                    DrawRectangle(letterMargin * (4 + i) + letterSize * i, h * 0.5, letterSize, letterSize, RED);
                }
                DrawTextEx(font,str, (Vector2) {letterMargin * (4 + i) + letterSize * i + letterSize / 2 - MeasureTextEx(font,str, h * 0.05,0).x / 2, h * 0.5 + letterSize / 4}, h * 0.05,0, LIGHTGRAY);
            }
        }
        //  For K To T
        else if(i < 20) {
            if(test) {
                if(GuiButton((Rectangle){letterMargin * (4 + i - 10) + letterSize * (i - 10), h * 0.5 + letterMargin + letterSize, letterSize, letterSize},str)|| IsKeyPressed(qwertyToAzerty('A'+i))) {
                    clicked[i] = 'A' + i;
                    int * letterPositionsInWordToGuess = searchLetter(dic, wordToGuess, 'a' + i);
                    if(letterPositionsInWordToGuess == NULL || letterPositionsInWordToGuess[0] == -1) {
                        playFailSound(fail);
                        nb++;
                        if(letterPositionsInWordToGuess != NULL) free(letterPositionsInWordToGuess);
                    }
                    else {
                        playSuccessSound(success);

                        for(int j=0;j < strlen(wordToGuess)&&letterPositionsInWordToGuess[j]!=-1;j++) {
                            wordToShow[letterPositionsInWordToGuess[j]] = 'A' + i;
                        }
                        free(letterPositionsInWordToGuess);
                    }
                }
            }
            else {
                if(strchr(wordToGuess,'a'+i)!=NULL){
                    DrawRectangle(letterMargin * (4 + i - 10) + letterSize * (i - 10), h * 0.5 + (letterMargin + letterSize), letterSize, letterSize, (Color){0, 128, 0, 255});
                }
                else{
                    DrawRectangle(letterMargin * (4 + i - 10) + letterSize * (i - 10), h * 0.5 + (letterMargin + letterSize), letterSize, letterSize, RED);
                }
                DrawTextEx(font,str,(Vector2) {letterMargin * (4 + i - 10) + letterSize * (i - 10) + letterSize / 2 - MeasureTextEx(font,str, h * 0.05,0).x / 2, h * 0.5 + (letterMargin + letterSize) + letterSize / 4}, h * 0.05,0, LIGHTGRAY);
            }
        }
        // For U To Z
        else {
            if(test) {
                if(GuiButton((Rectangle){letterMargin * (4 + i - 18) + letterSize * (i - 18), h * 0.5 + (letterMargin + letterSize) * 2, letterSize, letterSize},str)|| IsKeyPressed(qwertyToAzerty('A'+i))) {
                    clicked[i] = 'A' + i;
                    int * letterPositionsInWordToGuess = searchLetter(dic, wordToGuess, 'a' + i);
                    if(letterPositionsInWordToGuess == NULL || letterPositionsInWordToGuess[0] == -1) {
                        playFailSound(fail);
                        nb++;
                        if(letterPositionsInWordToGuess != NULL) free(letterPositionsInWordToGuess);
                    }
                    else {
                        playSuccessSound(success);
                        for(int j=0;j < strlen(wordToGuess)&&letterPositionsInWordToGuess[j]!=-1;j++) {
                            wordToShow[letterPositionsInWordToGuess[j]] = 'A' + i;
                        }
                        free(letterPositionsInWordToGuess);
                    }
                }
            }
            else {
                if(strchr(wordToGuess,'a'+i)!=NULL){
                    DrawRectangle(letterMargin * (4 + i - 18) + letterSize * (i - 18), h * 0.5 + (letterMargin + letterSize) * 2, letterSize, letterSize, (Color){0, 128, 0, 255});
                }
                else{
                    DrawRectangle(letterMargin * (4 + i - 18) + letterSize * (i - 18), h * 0.5 + (letterMargin + letterSize) * 2, letterSize, letterSize, RED);
                }
                DrawTextEx(font,str,(Vector2){ letterMargin * (4 + i - 18) + letterSize * (i - 18) + letterSize / 2 - MeasureTextEx(font,str, h * 0.05,0).x / 2, h * 0.5 + (letterMargin + letterSize) * 2 + letterSize / 4}, h * 0.05,0, LIGHTGRAY);
            }
        }
        }
        BeginMode3D(camera);
        // Draw Hangman
        for(int i=0;i<nb;i++) {
            DrawCube(hangmanPiecesPositions[i], hangmanPiecesSizes[i].x, hangmanPiecesSizes[i].y, hangmanPiecesSizes[i].z, LIGHTGRAY);
            DrawCubeWires(hangmanPiecesPositions[i], hangmanPiecesSizes[i].x, hangmanPiecesSizes[i].y, hangmanPiecesSizes[i].z, BLACK);
        }
        // Draw The Grid Under Hangman
        DrawGrid(7, 1);        
        EndMode3D();
        // Check If The User Lost
        if(nb>=piecesNumber){
            pageNumber = LOST_PAGE;
        }
        // Check If The User Guessed The Word 
        if(nb<piecesNumber && checkWord(wordToGuess,wordToShow)){
            pageNumber = WIN_PAGE;
        }
}

int qwertyToAzerty(int key){
    switch (key){
    case KEY_A:  return KEY_Q;
    case KEY_Q : return KEY_A;
    case KEY_SEMICOLON : return KEY_M;
    case KEY_Z : return KEY_W;
    case KEY_W : return KEY_Z;
    default:return key;
    }
}
void dualHangMan() {
    Color playerOneColor, playerTwoColor;
    if(playerOneTurn) {
        playerOneColor = BLUE;
        playerTwoColor = DARKGRAY;
    }
    else {
        playerOneColor = DARKGRAY;
        playerTwoColor = RED;
    }
    DrawTextEx(font,"Player 1", (Vector2){ w - MeasureText("Player 1", w * 0.04) - h * 0.05, h * 0.05}, w * 0.04,0, playerOneColor);
    DrawTextEx(font,"Player 2", (Vector2) {w - MeasureText("Player 1", w * 0.04) - h * 0.05, h * 0.12}, w * 0.04,0, playerTwoColor);
    if(GuiImageButton((Rectangle){ 10, 10, w * 0.05, w * 0.05 }, "", texture)) {
        pageNumber = WELCOME_PAGE;
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
                if(GuiButton((Rectangle){letterMargin * (4 + i) + letterSize * i, h * 0.5, letterSize, letterSize},str) || IsKeyPressed(qwertyToAzerty('A'+i))) {
                    clicked[i] = 'A' + i;

                    int * letterPositionsInWordToGuess = searchLetter(dic, wordToGuess, 'a' + i);
                    if(letterPositionsInWordToGuess == NULL || letterPositionsInWordToGuess[0] == -1) {
                        playerOneTurn = !playerOneTurn;
                        playFailSound(fail);
                        nb++;
                        if(letterPositionsInWordToGuess != NULL) free(letterPositionsInWordToGuess);
                    }
                    else {
                        playSuccessSound(success);
                        for(int j=0;j < strlen(wordToGuess)&&letterPositionsInWordToGuess[j]!=-1;j++) {
                            wordToShow[letterPositionsInWordToGuess[j]] = 'A' + i;
                        }
                        free(letterPositionsInWordToGuess);
                    }
                }
            }
            else {
                if(strchr(wordToGuess,'a'+i)!=NULL){
                    DrawRectangle(letterMargin * (4 + i) + letterSize * i, h * 0.5, letterSize, letterSize, (Color){0, 128, 0, 255});
                }
                else{
                    DrawRectangle(letterMargin * (4 + i) + letterSize * i, h * 0.5, letterSize, letterSize, RED);
                }
                DrawTextEx(font,str,(Vector2) {letterMargin * (4 + i) + letterSize * i + letterSize / 2 - MeasureText(str, h * 0.05) / 2, h * 0.5 + letterSize / 4}, h * 0.05,0, LIGHTGRAY);
            }
        }
        else if(i < 20) {
            if(test) {
                if(GuiButton((Rectangle){letterMargin * (4 + i - 10) + letterSize * (i - 10), h * 0.5 + letterMargin + letterSize, letterSize, letterSize},str)|| IsKeyPressed(qwertyToAzerty('A'+i))) {
                    clicked[i] = 'A' + i;
                    int * letterPositionsInWordToGuess = searchLetter(dic, wordToGuess, 'a' + i);
                    if(letterPositionsInWordToGuess == NULL || letterPositionsInWordToGuess[0] == -1) {
                        playerOneTurn = !playerOneTurn;
                        playFailSound(fail);
                        nb++;
                        if(letterPositionsInWordToGuess != NULL) free(letterPositionsInWordToGuess);
                    }
                    else {
                        playSuccessSound(success);

                        for(int j=0;j < strlen(wordToGuess)&&letterPositionsInWordToGuess[j]!=-1;j++) {
                            wordToShow[letterPositionsInWordToGuess[j]] = 'A' + i;
                        }
                        free(letterPositionsInWordToGuess);
                    }
                }
            }
            else {
                if(strchr(wordToGuess,'a'+i)!=NULL){
                    DrawRectangle(letterMargin * (4 + i - 10) + letterSize * (i - 10), h * 0.5 + (letterMargin + letterSize), letterSize, letterSize, (Color){0, 128, 0, 255});
                }
                else{
                    DrawRectangle(letterMargin * (4 + i - 10) + letterSize * (i - 10), h * 0.5 + (letterMargin + letterSize), letterSize, letterSize, RED);
                }                
                DrawTextEx(font,str, (Vector2) {letterMargin * (4 + i - 10) + letterSize * (i - 10) + letterSize / 2 - MeasureText(str, h * 0.05) / 2, h * 0.5 + (letterMargin + letterSize) + letterSize / 4}, h * 0.05,0, LIGHTGRAY);
            }
        }
        else {
            if(test) {
                if(GuiButton((Rectangle){letterMargin * (4 + i - 18) + letterSize * (i - 18), h * 0.5 + (letterMargin + letterSize) * 2, letterSize, letterSize},str)|| IsKeyPressed(qwertyToAzerty('A'+i))) {
                    clicked[i] = 'A' + i;
                    int * letterPositionsInWordToGuess = searchLetter(dic, wordToGuess, 'a' + i);
                    if(letterPositionsInWordToGuess == NULL || letterPositionsInWordToGuess[0] == -1) {
                        playerOneTurn = !playerOneTurn;
                        playFailSound(fail);
                        nb++;
                        if(letterPositionsInWordToGuess != NULL) free(letterPositionsInWordToGuess);
                    }
                    else {
                        playSuccessSound(success);
                        for(int j=0;j < strlen(wordToGuess)&&letterPositionsInWordToGuess[j]!=-1;j++) {
                            wordToShow[letterPositionsInWordToGuess[j]] = 'A' + i;
                        }
                        free(letterPositionsInWordToGuess);
                    }
                }
            }
            else {
                if(strchr(wordToGuess,'a'+i)!=NULL){
                    DrawRectangle(letterMargin * (4 + i - 18) + letterSize * (i - 18), h * 0.5 + (letterMargin + letterSize) * 2, letterSize, letterSize, (Color){0, 128, 0, 255});
                }
                else{
                    DrawRectangle(letterMargin * (4 + i - 18) + letterSize * (i - 18), h * 0.5 + (letterMargin + letterSize) * 2, letterSize, letterSize, RED);
                }                
                DrawTextEx(font,str, (Vector2) {letterMargin * (4 + i - 18) + letterSize * (i - 18) + letterSize / 2 - MeasureText(str, h * 0.05) / 2, h * 0.5 + (letterMargin + letterSize) * 2 + letterSize / 4}, h * 0.05,0, LIGHTGRAY);
            }
        }
        }
        BeginMode3D(camera);

        for(int i=0;i<nb;i++) {
            DrawCube(hangmanPiecesPositions[i], hangmanPiecesSizes[i].x, hangmanPiecesSizes[i].y, hangmanPiecesSizes[i].z, GRAY);
            DrawCubeWires(hangmanPiecesPositions[i], hangmanPiecesSizes[i].x, hangmanPiecesSizes[i].y, hangmanPiecesSizes[i].z, DARKGRAY);
        }
        DrawGrid(7, 1);        
        EndMode3D();
        if(nb>=piecesNumber){
            pageNumber = LOST_PAGE;
        }
        if(nb<piecesNumber && checkWord(wordToGuess,wordToShow)){
            pageNumber = WIN_PAGE_TWO_PLAYERS;
        }
}

void displayUsernameInput(){


    DrawTextEx(font,"Enter your username !",(Vector2) { w / 2 - MeasureTextEx(font,"Enter your username !", w*0.03,0).x / 2 , h*0.4}, w*0.03,0, WHITE);
    Rectangle input_box={
        (w-0.3*w)/2,
        1.3*h/4+h*0.2,
        w*0.3,
        h*0.07
    };
    GuiTextBox(input_box, userName, MAX_INPUT_CHARS+1, true);
    DrawTextEx(font,TextFormat("INPUT CHARS: %i/%i", strlen(userName), MAX_INPUT_CHARS), (Vector2) {w / 2 - MeasureTextEx(font,"INPUT CHARS: 5/9", 20,0).x / 2, h*0.6}, 20,0, DARKGRAY);
    // If The User Pressed Enter Key , Submit His Username That Should not Be Empty
    if (IsKeyPressed(KEY_ENTER)) {
        if(userName[0]!='\0'){
            addRanking("src/rankings.txt",userName,evaluateWord(wordToGuess)-nb);
            pageNumber = WELCOME_PAGE;
            strcpy(userName,"\0");
        }
    }


}

void lostPage(){
    if(!lostSound){
    playLostSound(lost);
    lostSound = 1;
    }
    Vector2 textWidth = MeasureTextEx(font,TextFormat("You Lost! The word was: %s ",wordToGuess), w * 0.04,0) ;
    DrawTextEx(font,TextFormat("You Lost! The word was: %s ",wordToGuess),(Vector2) {w / 2 - textWidth.x / 2, h * 0.1}, w * 0.04,0, RED);

    if(GuiButton((Rectangle){w / 2 - w * 0.2, h * 0.4  ,w * 0.4, h * 0.1},"Try again")) {
        pageNumber = WELCOME_PAGE;
        lostSound =0;
    }
}
void winPage(){
    initializeSnowflakes();
    if(gameResultMessageY == -100.0f){
        playWinSound(win);
        UnloadSound(win);
    }
    drawSnowflaskes(w,h);

    gameResultMessageY += 2.0f;
    DrawTextEx(font,"Congratulations, You Win!", (Vector2){w / 2 - MeasureTextEx(font,"Congratulations, You Win!", w*0.05,0).x / 2,gameResultMessageY}, w*0.05,0, BLUE);
    if(gameResultMessageY >h*0.25){
        gameResultMessageY = h*0.25;
        displayUsernameInput();
    }
}
void winPageTwoPlayers(){
    initializeSnowflakes();
    if(gameResultMessageY == -100.0f){
        playWinSound(win);
        UnloadSound(win);
    }
    drawSnowflaskes(w,h);

    gameResultMessageY += 2.0f;
    DrawTextEx(font,TextFormat("Congratulations Player %d, You Win!", 2 - playerOneTurn), (Vector2) {w / 2 - MeasureTextEx(font,"Congratulations Player 2, You Win!", w*0.05,0).x / 2, gameResultMessageY}, w*0.05,0, BLUE);
    DrawTextEx(font,"Press Enter To Continue", (Vector2) {w / 2 - MeasureText("Press Enter To Continue", 20) / 2, gameResultMessageY + 100}, 20,0, DARKGRAY);
    if(gameResultMessageY >h*0.25){
        gameResultMessageY = h*0.25;
    }
    if (IsKeyPressed(KEY_ENTER)) {
        pageNumber = WELCOME_PAGE;
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

    switch(pageNumber) {
        case WELCOME_PAGE:   welcomePage(); break;
        case SIGNLE_PLAYER_PAGE:  singlePlayerPage(); break;
        case HANGMAN_PAGE : hangman(); break;
        case TWO_PLAYER_PAGE:  twoPlayersPage(); break;
        case DUAL_HANGMAN_PAGE :  dualHangMan(); break;
        case RANKINGS:  rankingsPage(); break;
        case WIN_PAGE : winPage() ; break;
        case LOST_PAGE : lostPage(); break;
        case WIN_PAGE_TWO_PLAYERS: winPageTwoPlayers(); break;
    }

    EndDrawing();
}

int main(void) {    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Hangman");
    SetTargetFPS(60);
    initializeParticles();

    font = LoadFont("assets/Montserrat-ExtraBold.ttf");
    GuiSetFont(font);
    // Change The background Of The buttons
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 1);   
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_NORMAL, 1);   
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_FOCUSED, 0);  



    // Load image and create texture
    image = LoadImage("assets/arrow.png");
    Color c = GetImageColor(image, 200, 200);
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
        GuiSetStyle(DEFAULT, TEXT_SIZE, h*0.05); // Set the font size to 20
        previewScreen();
    }
    // Unload All The Audio And Free Memory
    UnloadSound(success);
    UnloadSound(fail);
    UnloadSound(backgroundMusic);
    freePlayersArray(rankings);
    freeWordsArray(words);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}