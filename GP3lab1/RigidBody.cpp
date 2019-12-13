#include "pch.h"
#include "RigidBody.h"
#include "Physics.h"
#include "Entity.h"

void RigidBody::OnUpdate(float deltaTime)
{
}

void RigidBody::OnRender()
{
}

void RigidBody::OnAttach()
{
}

void RigidBody::Init(CollisionShape* shape, float mass, const glm::vec3 localInertia)
{
	btTransform bT = Physics::ConvertTransformToBtTransform(*m_entity->GetTransform());
	m_mState = new btDefaultMotionState(bT);
	m_shape = shape;
	btVector3 li = btVector3(localInertia.x, localInertia.y, localInertia.z);
	m_shape->Get()->calculateLocalInertia(mass, li);
	m_rigidBody = new btRigidBody(btScalar(mass), m_mState, m_shape->Get(), li);
	Physics::GetInstance()->AddRigidbody(this);
	m_rigidBody->setSleepingThresholds(0, 0);
	m_rigidBody->setFriction(1);
}

void RigidBody::UpdateParent()
{
	btTransform* newPos = &m_rigidBody->getWorldTransform();
	m_entity->GetTransform()->SetPosition(glm::vec3((float)newPos->getOrigin().getX(), (float)newPos->getOrigin().getY(), (float)newPos->getOrigin().getZ()));
	m_entity->GetTransform()->SetRotation(glm::quat(newPos->getRotation().getW(), newPos->getRotation().getX(), newPos->getRotation().getY(), newPos->getRotation().getZ()));
}

void RigidBody::UpdateRigidBody()
{
	btTransform t = Physics::ConvertTransformToBtTransform(*m_entity->GetTransform());
	m_rigidBody->setWorldTransform(t);
	m_rigidBody->getMotionState()->setWorldTransform(t);
}

void RigidBody::AddForce(glm::vec3 force) // this is horrible and needs to be reworked
{
	btVector3 tempVec = btVector3(0, 0, 0); // m_entity->GetTransform()->GetPosition().x, m_entity->GetTransform()->GetPosition().y, m_entity->GetTransform()->GetPosition().z);
	btVector3* tempPoint = &tempVec;
	m_rigidBody->applyForce(btVector3(force.x, force.y, force.z), *tempPoint);
}

void RigidBody::RemoveIntertia()
{
	m_rigidBody->setLinearVelocity(btVector3(0, 0, 0));
}