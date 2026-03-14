#include "raylib.h"
#include "rlgl.h"
#include <stdbool.h>
#include <math.h>
int main(void) {
    // Initialization
    InitWindow(800, 450, "world");

    // Load Model and Animations
    Model model = LoadModel("s.glb");

    Vector3 modelPos = { 0, 0, 0};
    
    int animCount = 2;
    ModelAnimation *anims = LoadModelAnimations("s.glb", &animCount);

    // Animation playing variables
    unsigned int animIndex = 1;         // Current animation playing
    unsigned int animCurrentFrame = 0;  // Current animation frame
    // --- SHADER LOADING ---
    // Pass NULL for the vertex shader to use Raylib's default, 
    // or provide your own path.

bool isCaptured = true;

float yaw = 0.0f;   // Horizontal angle
    float pitch = 0.0f; // Vertical angle
    float sensitivity = 0.003f;
    float speed = 0.1f;
    Camera camera = { 0 };
    camera.position = (Vector3){ 5.0f, 1.2f, 5.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float rotation = 0.0f;
    SetTargetFPS(60);
DisableCursor();
    while (!WindowShouldClose()) {
    float dx = camera.position.x - modelPos.x;
float dz = camera.position.z - modelPos.z;
float angleRad = atan2(dx, dz); 

// 2. Calculate movement using Radians
float botSpeed = 0.03f; // Adjust as needed
float d = sqrt(dx*dx + dz*dz);
 // --- INPUT: MOUSE LOOK ---
        Vector2 mouseDelta = GetMouseDelta();
        yaw   -= mouseDelta.x * 0.003f;
        pitch -= mouseDelta.y * 0.003f;
        if (pitch > 1.5f) pitch = 1.5f;
        if (pitch < -1.5f) pitch = -1.5f;
if (d > 3) animIndex = 1;
else animIndex = 0;
animCurrentFrame = (animCurrentFrame + 1)%anims[animIndex].frameCount;
        UpdateModelAnimation(model, anims[animIndex], (float)animCurrentFrame);
        
        // First time: Declare and calculate
Vector3 direction = { cosf(pitch) * sinf(yaw), sinf(pitch), cosf(pitch) * cosf(yaw) };
camera.target.x = camera.position.x + direction.x;
camera.target.y = camera.position.y + direction.y;
camera.target.z = camera.position.z + direction.z;

        // --- INPUT: KEYBOARD MOVEMENT ---
        Vector3 forward = { sinf(yaw), 0, cosf(yaw) };
        Vector3 right = { sinf(yaw - PI/2.0f), 0, cosf(yaw - PI/2.0f) };

        if (IsKeyDown(KEY_W)) {
        	camera.position.x += forward.x *speed;
        	camera.position.z += forward.z * speed;
        }
else if (IsKeyDown(KEY_S)) {
	camera.position.x -= forward.x * speed;
	camera.position.z -= forward.z * speed;
	}
if (IsKeyDown(KEY_D)) {
	camera.position.x  += right.x * speed;
	camera.position.z  += right.z * speed;
	}
else if (IsKeyDown(KEY_A)) {
	camera.position.x  -= right.x * speed;
	camera.position.z  -= right.z * speed;
	}


        // --- CALC ROTATION: MODEL FACES CAMERA ---
        // 1. Calculate the angle in RADIANS for the math

if (d > 3) {
modelPos.x += sinf(angleRad) * botSpeed;
modelPos.z += cosf(angleRad) * botSpeed;
}
// --- DRAWING ---
BeginDrawing();
    ClearBackground(SKYBLUE);
    BeginMode3D(camera);
        DrawPlane((Vector3){ 0, 0, 0 }, (Vector2){ 50, 50 }, GRAY);
        DrawGrid(20, 1.0f);

        rlPushMatrix();
            // 3. Move the world to the BOT'S POSITION
            rlTranslatef(modelPos.x, modelPos.y, modelPos.z);
            
            // 4. Rotate using DEGREES
            rlRotatef(angleRad * RAD2DEG, 0, 1, 0);
            
            // 5. Draw at local origin
            DrawModel(model, (Vector3){ 0, 0, 0 }, 1.0f, WHITE);
        rlPopMatrix();
    EndMode3D();

    DrawText("eveexlove", 10, 10, 20, BLACK);
EndDrawing();
    }

    // Cleanup
    UnloadModel(model);
    CloseWindow();

    return 0;
}
