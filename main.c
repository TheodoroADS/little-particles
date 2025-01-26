#include "include/raylib.h"
#include <math.h>
#include <omp.h>
#include "particle.h"
#include "vec_utils.h"

#define screenWidth 800
#define screenHeight 600

#define NB_PARTICLES 10

#define ball_color GRAY

#define ball_radius_upper 30
#define ball_radius_lower 1

#define COLLISION_FORCE_SPRING_CONSTANT 0.1f // spring-like collision force constant

BallParticle PARTICLES[NB_PARTICLES];

Color BALL_COLORS[] = {
    LIGHTGRAY, 
    YELLOW, 
    BLUE, 
    PURPLE, 
    GREEN, 
    WHITE, 
    LIGHTGRAY, 
    LIME, 
    SKYBLUE, 
    MAGENTA, 
    PINK, 
    GOLD, 
    BEIGE
};

#define BALL_COLORS_SIZE ((int) sizeof(BALL_COLORS)/sizeof(Color))

float random_radius() {

    return (rand() % (ball_radius_upper - ball_radius_lower + 1) + ball_radius_lower);

}

Color random_color() {
    return BALL_COLORS[rand() % BALL_COLORS_SIZE];
}


Force attraction_force(BallParticle particle, Vector2 dest, float force_intensity) {

    Vector2 origin = particle.position;
    Vector2 force = {dest.x - origin.x, dest.y - origin.y};
    float norm = sqrtf(force.x*force.x + force.y*force.y);
    force_intensity = force_intensity*particle.mass; // = G*m1*m2
    force.x *= force_intensity/(norm*norm*norm); // normalize and then devide by norm^2
    force.y *= force_intensity/(norm*norm*norm);

    return force;
}


void init_particles() { 
    // #pragma omp parallel for
    for (int i = 0; i < NB_PARTICLES; i++) {
        PARTICLES[i] = ball_particle_init_random_pos(screenWidth, screenHeight, 10, random_radius(), random_color()); 
    }
}

int main() {
    
    srand(time(NULL));  
    init_particles();
    InitWindow(screenWidth, screenHeight, "Some particles");
    SetTargetFPS(60);

    const float main_force_intensity = 70000.0f;
    const float particle_interaction_intensity = .05f;
    const float collision_tresh = 0.5f;

    while (!WindowShouldClose()) {
        
        Vector2 force_source = GetMousePosition();
        float dt = GetFrameTime();
        
        if (GetKeyPressed() == KEY_R) {
            // reset simulation
            init_particles();            
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // compute forces, update acceleration, position and draw
        for (int i = 0; i < NB_PARTICLES; i++) {

            BallParticle* particle = PARTICLES + i;

            // gravity interaction with other particles
            Force other_particles_pull = {0.0, 0.0};
            // collision interaction with other particles
            Force collision_push = {0.0, 0.0}; 

            // interactions with other particles
            for (int j = 0; j < NB_PARTICLES; j++) {
                
                // skipping the interactions between the particle and itself
                if (i == j) continue;
                
                BallParticle* other_particle = PARTICLES + j;

                // gravity pull between the particles
                other_particles_pull = vec_add(
                    other_particles_pull,
                     attraction_force(
                        *particle,
                        other_particle->position,
                        particle_interaction_intensity*other_particle->mass
                     )
                );

                if (ball_particle_collided(particle, other_particle, collision_tresh)) {

                    collision_push = vec_add(
                        collision_push,
                        resolve_collision_force(
                            COLLISION_FORCE_SPRING_CONSTANT, 
                            particle, 
                            other_particle
                        )
                    );             
                }
            }
    
            Force resultant_force = sum_forces(
                // attraction force with the mouse
                attraction_force(*particle, force_source, main_force_intensity),
                other_particles_pull,
                collision_push
            );

            // update acceleration
            ball_particle_update_acceleration(particle, resultant_force);
            ball_particle_update_pos(particle, dt);

            DrawCircleV(particle->position,particle->radius,particle->color);
        }


        EndDrawing();
    }

  CloseWindow();
  return 0;
}