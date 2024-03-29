// Particles
#define MAX_PARTICLES 250
#define PARTICLE_SIZE 2
#define PARTICLE_SPEED 3
typedef struct {
    Vector2 position;
    Color color;
    Vector2 speed;
} Particle;
Particle particles[MAX_PARTICLES];


void initializeParticles(){
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].position.x = GetRandomValue(0, GetScreenWidth());
        particles[i].position.y = GetRandomValue(0, GetScreenHeight());
        particles[i].color = WHITE;
        particles[i].speed.x = GetRandomValue(-PARTICLE_SPEED, PARTICLE_SPEED);
        particles[i].speed.y = GetRandomValue(-PARTICLE_SPEED, PARTICLE_SPEED);
        if(particles[i].speed.x == 0){
            particles[i].speed.x++;
        }
        if(particles[i].speed.y == 0){
            particles[i].speed.y++;
        }
    }
}
void drawParticles(){
   for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].position.x += particles[i].speed.x;
        particles[i].position.y += particles[i].speed.y;

        // Bounce particles off the window edges
        if (particles[i].position.x >= GetScreenWidth() || particles[i].position.x <= 0) {
            particles[i].speed.x = -particles[i].speed.x;
        }

        if (particles[i].position.y >= GetScreenHeight() || particles[i].position.y <= 0) {
            particles[i].speed.y = -particles[i].speed.y;
        }
    }
   for (int i = 0; i < MAX_PARTICLES; i++) {
        DrawCircleV(particles[i].position, PARTICLE_SIZE, particles[i].color);
   }
}

