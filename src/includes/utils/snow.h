#define NUM_SNOWFLAKES 100
#define SNOWFLAKE_SIZE 1
#define SNOWFLAKE_SPEED 2

typedef struct {
    Vector2 position;
    Color color;
} Snowflake;
Snowflake snowflakes[NUM_SNOWFLAKES];

void initializeSnowflakes(){
    for (int i = 0; i < NUM_SNOWFLAKES; i++) {
        snowflakes[i].position.x = GetRandomValue(0, GetScreenWidth());
        snowflakes[i].position.y = GetRandomValue(0, GetScreenHeight());
        snowflakes[i].color = WHITE;
    }
}

void drawSnowflaskes(float w , float h){
    for (int i = 0; i < NUM_SNOWFLAKES; i++) {
        snowflakes[i].position.y += SNOWFLAKE_SPEED;

        // Reset snowflake to the top if it goes beyond the screen
        if (snowflakes[i].position.y > h) {
            snowflakes[i].position.y = 0;
            snowflakes[i].position.x = GetRandomValue(0, w);
        }
    }

    // Draw snowflakes
        for (int i = 0; i < NUM_SNOWFLAKES; i++) {
            DrawCircleV(snowflakes[i].position, SNOWFLAKE_SIZE, snowflakes[i].color);
        }
}
