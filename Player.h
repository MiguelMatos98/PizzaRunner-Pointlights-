#pragma once
#include "GameObject.h"
#include "Camera.h"
#include <chrono>
#include <Mouse.h>
#include <Keyboard.h>

// Player should have a camera class with all the 
// functionality 
using namespace std::chrono_literals;
using namespace std::chrono;

//
//enum class MoveDirection
//{
//	Left,
//	Right,
//	Forward
//};

class Player : public GameObject
{
private:
	static constexpr int LEFT_LANE = -1;
	static constexpr int MIDDLE_LANE = 0;
	static constexpr int RIGHT_LANE = 1;
	int m_Score;
	float m_ForwardSpeed;
	float m_LaneSpeed;
	Camera* m_Camera;
	XMVECTOR m_CameraOffset;
	bool m_IsChangingLanes;
	int m_CurrentLane;
	int m_TargetLane;
	float m_LaneTransitionProgress;
	float m_LaneWidth;
	float m_LaneTransitionDuration;
	steady_clock::time_point m_LastLaneChangeTime;
	float m_LaneChangeCooldown;
	Keyboard::KeyboardStateTracker keyboardTracker2;
	Keyboard::KeyboardStateTracker keyboardTracker;
	Mouse::ButtonStateTracker mouseStateTracker;
private:
	void IncrementScore();
public:
	Player(const Transform& transform, ObjFileModel* model = nullptr); // Add camera to player here
	~Player();
	int GetScore();
	void SetScore(int score);
	float GetSpeed();
	void SetSpeed(const float speed);
	Camera* GetCamera();
	void SetCamera(Camera& camera);
	void Update(const double delta_Time) override; // Fucntion pointer to this
	void IncreaseScore(unsigned int incrementValue);
	void HandleInput(const double delta_Time);
	void BeginLaneChange(int targetLane);
	float Smoothstep(float edge0, float edge1, float x);
	float Lerp(float a, float b, float f);
	float Clamp(float x, float lowerlimit = 0.0f, float upperlimit = 1.0f);
};



