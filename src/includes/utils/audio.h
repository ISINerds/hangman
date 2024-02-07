void playSuccessSound(Sound success){
    success = LoadSound("assets/success.mp3");
    PlaySound(success);
}
void playFailSound(Sound fail){
    fail = LoadSound("assets/fail.mp3");
    PlaySound(fail);
}
void playWinSound(Sound win){
    win = LoadSound("assets/win.mp3");
    PlaySound(win);
}
void playLostSound(Sound lost){
    lost = LoadSound("assets/lost.wav");
    PlaySound(lost);
}
void playBackgroundMusic(Sound backgroundMusic){
    backgroundMusic = LoadSound("assets/background-music.mp3");
    PlaySound(backgroundMusic);
}