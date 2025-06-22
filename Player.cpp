#include "Player.h"
#include "ColliderManager.h"


Player::Player(const Transform& transform, ObjFileModel* mesh) 
	: GameObject(transform, nullptr, mesh), m_ForwardSpeed{ 1.2f }, m_LaneSpeed{ 2.0f }, m_Score { 0 }, m_Camera{ nullptr }, m_IsChangingLanes{ false }
	, m_CurrentLane{ MIDDLE_LANE }, m_TargetLane{ MIDDLE_LANE }, m_LaneTransitionProgress{ 0.0f }, m_LaneWidth{ 2.5f }, m_LaneTransitionDuration{ 0.0005f }
	, m_LastLaneChangeTime{ steady_clock::now() }, m_LaneChangeCooldown{ 1.0f }
{
	float zOffset = 5.0f;
	float z = XMVectorGetZ(transform.position) + zOffset;
	float x = XMVectorGetX(transform.position);
	float y = XMVectorGetY(transform.position);

	m_CameraOffset = { x, y, z };
	m_Camera = new Camera(m_CameraOffset, transform.rotation);

}

Player::~Player()
{
	if (m_Camera != nullptr) { delete m_Camera; m_Camera = nullptr; };
}

void Player::Update(const double delta_Time)
{
	/*if (!m_IsMoving)
	{
	}*/
	//else
	//{
	//	m_Speed = 0.0f;
	//}
		//m_IsMoving = true;

	//auto currentKeyPressTime = steady_clock::now();
	//static auto threshold = 0.09s;

	//if (keyboardTracker.pressed.A)
	//{
	//	if (currentKeyPressTime - m_LastKeyPressTime > threshold) 
	//	{
	//		Move(delta_Time);
	//		m_LastKeyPressTime = currentKeyPressTime;
	//	}

	//	cout << "AAAAAAAAAAAAAAAAAAAAAA key pressed" << "\n";
	//}
	//else if (keyboardTracker.pressed.D)
	//{
	//	if (currentKeyPressTime - m_LastKeyPressTime > threshold)
	//	{
	//		Move(delta_Time);
	//		m_LastKeyPressTime = currentKeyPressTime;
	//	}
	//	cout << "DDDDDDDDDDDDDDDDDDDDDDD key pressed" << "\n";
	//}


	m_Transform.position += { 0.0f, 0.0f, m_ForwardSpeed* (float)delta_Time, 0.0f};

	if (m_IsChangingLanes)
	{
		//m_LaneTransitionProgress += float(delta_Time)/m_LaneSpeed;

		auto elapsed = duration_cast<milliseconds>(steady_clock::now() - m_LastLaneChangeTime).count() / 1000.0f;

		if (m_LaneTransitionDuration)
		elapsed /= m_LaneTransitionDuration;

		float target = m_TargetLane * m_LaneWidth;

		float current = m_CurrentLane * m_LaneWidth;

		float moveAmount = (target - current) * (float)delta_Time;

		float newX = Lerp(current, target, elapsed);

		if (elapsed >= 1)
		{
			m_IsChangingLanes = false;
			m_CurrentLane = m_TargetLane;
			m_Transform.position = { target, XMVectorGetY(m_Transform.position), XMVectorGetZ(m_Transform.position), 0.0f };
		}
		else
		{
			m_Transform.position = { newX, XMVectorGetY(m_Transform.position), XMVectorGetZ(m_Transform.position), 0.0f };
		}
	}
	cout << "Lane Transition Progress: " << m_LaneTransitionProgress << "\n";
	m_CameraOffset = { XMVectorGetX(m_Transform.position), XMVectorGetY(m_Transform.position), XMVectorGetZ(m_Transform.position) + 5.0f };
	
	m_Camera->GetTransform().position = { m_CameraOffset };
	
	ColliderManager::SetColliderPosition(m_CameraOffset, 0);

}

void Player::SetSpeed(const float speed)
{
	m_ForwardSpeed = speed;
}

void Player::SetCamera(Camera& camera)
{
	m_Camera = &camera;
}

void Player::SetScore(int score)
{
	m_Score = score;
}

void Player::IncreaseScore(unsigned int incrementValue)
{
	m_Score += incrementValue;
}

void Player::HandleInput(const double delta_Time)
{
	auto keyboardState = Keyboard::Get().GetState();
	auto mouseState = Mouse::Get().GetState();

	keyboardTracker.Update(keyboardState);
	keyboardTracker2.Update(keyboardState);
	mouseStateTracker.Update(mouseState);

	float sensitivity = XM_2PI * 0.05f; // Needs to be in the camera

	float yaw = m_Camera->GetYaw(), pitch = m_Camera->GetPitch(); // Player

	// Do thsi on camera udated
	m_Camera->SetYaw(yaw + mouseState.x * sensitivity * (float)delta_Time); // Needs to be in the camera
	m_Camera->SetPitch(pitch + mouseState.y * sensitivity * (float)delta_Time); // Needs to be in the camera

	auto currentTime = steady_clock::now();

	//double timeSinceLastLaneChange = duration<double>(currentTime - m_LastLaneChangeTime).count();

	///*if (timeSinceLastLaneChange < m_LaneChangeCooldown || m_IsChangingLanes)
	//{
	//	return;
	//}*/

	if (keyboardTracker.pressed.A) 
	{
		BeginLaneChange(m_CurrentLane-1);
	}

	if (keyboardTracker2.pressed.D)
	{
		BeginLaneChange(m_CurrentLane+1);
	}
}

void Player::BeginLaneChange(int targetLane)
{
	if (targetLane < LEFT_LANE || targetLane > RIGHT_LANE) 
	{
		return;
	}

	//if (m_IsChangingLanes)
	//{
	//	return;
	//}

	m_IsChangingLanes = true;
	//m_LaneTransitionProgress = 0.0f;
	m_TargetLane = targetLane;
	m_LastLaneChangeTime = steady_clock::now();
}

int Player::GetScore()
{
	return m_Score;
}

void Player::IncrementScore()
{
	m_Score++;
}

Camera* Player::GetCamera()
{
	return m_Camera;
}

float Player::Smoothstep(float edge0, float edge1, float x) {
	// Scale, and clamp x to 0..1 range
	x = Clamp((x - edge0) / (edge1 - edge0));

	return x * x * (3.0f - 2.0f * x);
}

float Player::Clamp(float x, float lowerlimit, float upperlimit) {
	if (x < lowerlimit) return lowerlimit;
	if (x > upperlimit) return upperlimit;
	return x;
}


float Player::Lerp(float a, float b, float f)
{
	float lerpValue = a * (1.0f - f) + (b * f);
	return lerpValue;
}


float Player::GetSpeed()
{
	return m_ForwardSpeed;
}
