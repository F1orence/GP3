#include "Component.h"
#include "Physics.h"
#include "CollisionShape.h"

class RigidBody : public Component
{
private:
	CollisionShape* m_shape;
	btRigidBody* m_rigidBody;
	btMotionState* m_mState = nullptr;
	float m_mass;
	bool m_gravityEnabled = true;

public:
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnAttach() override;

	void Init(CollisionShape* shape, float mass = 1.f, const glm::vec3 localInertia = glm::vec3(1));
	virtual void UpdateParent();
	virtual void UpdateRigidBody();
	virtual void AddForce(glm::vec3 force);
	btRigidBody* Get() { return m_rigidBody; }
	void RemoveIntertia();
	void ToggleGravity();
	void ApplyDamping(float strength);
};
