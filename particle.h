#ifndef PARTICLE_H
#define PARTICLE_H

#include "include/raylib.h"
#include <memory.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "vec_utils.h"


typedef Vector2 Force;

// something close to the earth's density
#define BALL_DENSITY 5500

#define sqrt2over2 0.70710678118

typedef struct {

    Vector2 position;
    Vector2 acceleration;
    float mass;
    float radius;
    Color color;
} BallParticle;


Force _sum_forces(int nb_forces, ...) {
    va_list args;
    Force resultant_force = {0.0, 0.0};
    va_start(args, nb_forces);
    
    for (int i = 0; i < nb_forces; i++) {
        Force force = va_arg(args, Force);
        resultant_force.x += force.x;
        resultant_force.y += force.y;
    }
    va_end(args);
    return resultant_force;
}

#define NUM_FORCES(...) (sizeof((Force[]){__VA_ARGS__})/sizeof(Force))
#define sum_forces(...) _sum_forces(NUM_FORCES(__VA_ARGS__), __VA_ARGS__)


BallParticle ball_particle_init_random_pos(int screenwidth, int screenheight, int padding, float radius, Color color) {

    float randomx = rand() % (screenwidth - padding + 1) + padding;
    float randomy = rand() % (screenheight - padding + 1) + padding;  

    BallParticle res =  {
        {randomx,randomy},
        {0,0},
        BALL_DENSITY * PI * radius*radius,
        radius,
        color
    };

    return res;

}


void ball_particle_update_pos(BallParticle* particle, float dt) {

    particle->position.x = particle->position.x + particle->acceleration.x*dt*dt;
    particle->position.y = particle->position.y + particle->acceleration.y*dt*dt;

}

void ball_particle_update_acceleration(BallParticle* particle, Force attraction_force) {

    particle->acceleration.x = particle->acceleration.x + attraction_force.x/particle->mass;
    particle->acceleration.y = particle->acceleration.y + attraction_force.y/particle->mass;
}

float ball_particle_dist(BallParticle* a, BallParticle* b) {

    Vector2 v = {a->position.x - b->position.x, a->position.y - b->position.y};
    return vec_norm(v);

}

int ball_particle_collided(BallParticle* a, BallParticle* b, float tresh) {

    float dist = ball_particle_dist(a,b);
    return dist - (a->radius + b->radius) < tresh;
}

// for this implementation, a spring like force (proportional to the distance of the particles)
// K is the coefficient of the spring force
Force resolve_collision_force(float K, BallParticle* particle, BallParticle* otherParticle) {
    
    Vector2 delta_position = vec_diff(particle->position, otherParticle->position);
    Force collision_force;
    
    float distance = vec_norm(delta_position);
    float sum_radii  = particle->radius + otherParticle->radius;
    float dif = sum_radii - distance;
    float coef = K * dif * sqrt2over2/distance;
    // here we take the rotated direction by 45 degrees
    collision_force.x = coef * (delta_position.x - delta_position.y); 
    collision_force.y = coef * (delta_position.x + delta_position.y); 

    return collision_force;
}

#endif // PARTICLE_H