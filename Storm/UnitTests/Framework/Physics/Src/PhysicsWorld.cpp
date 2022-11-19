#include "Precompiled.h"
#include "PhysicsWorld.h"

using namespace Storm;
using namespace Storm::Physics;

namespace
{
	Math::Vector3 oa{ +0.0f, 0.8165f, +0.0000f };
	Math::Vector3 ob{ -0.5f, 0.0000f, -0.2887f };
	Math::Vector3 oc{ +0.5f, 0.0000f, -0.2887f };
	Math::Vector3 od{ +0.0f, 0.0000f, +0.5774f };
}
void PhysicsWorld::Initialize(Settings settings)
{
	mSettings = std::move(settings);
}

void PhysicsWorld::Update(float deltaTime)
{
	if (mPuse)
	{
		return;
	}
	mTimer += deltaTime;
	while (mTimer >= mSettings.timeStep)
	{
		mTimer -= mSettings.timeStep;
		AccumulateForces();
		Integrate();
		SatisfyConstraints();
	}

}

void PhysicsWorld::DebugDraw() const
{
	using namespace Graphics;

	for (auto& c : mConstraints)
	{
		c->DebugDraw();
	}

	for (auto& p : mParticles)
	{
		auto color = Math::Lerp(Colors::Yellow, Colors::Red, Math::Clamp(p->radius / 0.1f, 0.0f, 1.0f));
		SimpleDraw::AddSphere(p->position, p->radius, color, 3, 4);
	}
	for (auto& obb : mOBBs)
	{
		SimpleDraw::AddOBB(obb, Colors::Blue);
	}

}

void PhysicsWorld::DebugUI()
{
	ImGui::Begin("Physics", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::DragFloat3("Gravity", &mSettings.gravity.x, 0.1f, -10.0f, 10.0f);
	ImGui::Checkbox("Pause Simulation", &mPuse);
	ImGui::End();

}


//Particle* PhysicsWorld::AddParticle()
//{
//	return mParticles.emplace_back(std::make_unique<Particle>()).get();
//}

Tetrahedron PhysicsWorld::AddTetrahedron()
{
	Tetrahedron tetrahedron;
	tetrahedron.a = AddParticle();
	tetrahedron.b = AddParticle();
	tetrahedron.c = AddParticle();
	tetrahedron.d = AddParticle();

	tetrahedron.a->position = { +0.0f,0.8165f,+0.0000f };
	tetrahedron.b->position = { -0.5f,0.0000f,-0.2887f };
	tetrahedron.c->position = { +0.5f,0.0000f,-0.2887f };
	tetrahedron.d->position = { +0.0f,0.0000f,+0.5774f };

	tetrahedron.a->radius = 0.02f;
	tetrahedron.b->radius = 0.02f;
	tetrahedron.c->radius = 0.02f;
	tetrahedron.d->radius = 0.02f;

	tetrahedron.a->invMass = 1.0f;
	tetrahedron.b->invMass = 1.0f;
	tetrahedron.c->invMass = 1.0f;
	tetrahedron.d->invMass = 1.0f;


	AddConstraint<Spring>(tetrahedron.a, tetrahedron.b);
	AddConstraint<Spring>(tetrahedron.a, tetrahedron.c);
	AddConstraint<Spring>(tetrahedron.a, tetrahedron.d);

	AddConstraint<Spring>(tetrahedron.b, tetrahedron.c);
	AddConstraint<Spring>(tetrahedron.c, tetrahedron.d);
	AddConstraint<Spring>(tetrahedron.d, tetrahedron.b);


	return tetrahedron;
}

void PhysicsWorld::AddPlane(const Math::Plane& plane)
{
	mPlanes.push_back(plane);
}

void Storm::Physics::PhysicsWorld::AddOBB(const Math::OBB& obb)
{
	mOBBs.push_back(obb);
}

void Storm::Physics::PhysicsWorld::Clear(bool dynamicOnly)
{
	mConstraints.clear();
	mParticles.clear();
	if (!dynamicOnly)
	{
		mPlanes.clear();
		mOBBs.clear();
	}

}



void PhysicsWorld::AccumulateForces()
{
	for (auto& p : mParticles)
	{
		p->acceleration = mSettings.gravity;
	}

}

void PhysicsWorld::Integrate()
{
	constexpr float timeStepSqr = Math::Sqr(1 / 60.0f);
	for (auto& p : mParticles)
	{
		const auto newPosition = (p->position * 2.0f) - p->lastPosition + (p->acceleration * timeStepSqr);
		p->lastPosition = p->position;
		p->position = newPosition;
	}
}

void PhysicsWorld::SatisfyConstraints()
{
	for (int n = 0; n < mSettings.iterations; ++n)
	{
		for (auto& c : mConstraints)
		{
			c->Apply();
		}
	}

	for (auto& plane : mPlanes)
	{
		for (auto& p : mParticles)
		{
			float penetration = 0.0f;
			if (Math::Intersect({ p->position,p->radius }, plane, penetration))
			{
				auto velocity = p->position - p->lastPosition;
				auto velocityPerpendicular = plane.normal * Math::Dot(velocity, plane.normal);
				auto velocityParallel = velocity - velocityPerpendicular;


				float temp = 0.0f;
				if (Math::Intersect({ p->lastPosition,p->radius }, plane, temp))
				{
					const auto newVelocity = velocityParallel * (1.0f - mSettings.drag);
					const float centerDistance = Math::Dot(p->position, plane.normal);
					const float correction = plane.distance - centerDistance + p->radius;
					p->SetPosition(p->position + (plane.normal * correction));
					p->SetVelocity(newVelocity);
				}
				else
				{

					const auto newVelocity = (velocityParallel * (1.0f - mSettings.drag)) - (velocityPerpendicular * p->bounce);
					p->SetPosition(p->position - velocityPerpendicular);
					p->SetVelocity(newVelocity);
				}

			}
		}

	}

	for (auto& obb : mOBBs)
	{
		for (auto& p : mParticles)
		{
			if (Math::Intersect(p->position, obb))
			{
				auto velocity = p->position - p->lastPosition;
				auto direction = Math::Normalize(velocity);

				Math::Ray ray{ p->lastPosition, direction };
				Math::Vector3 point, normal;
				Math::Intersect(ray, obb, point, normal);

				auto velocityPerpendicular = normal * Math::Dot(velocity, normal);
				auto velocityParallel = velocity - velocityPerpendicular;
				auto newVelocity = (velocityParallel * (1.0f - mSettings.drag)) - (velocityPerpendicular * p->bounce);
				p->SetPosition(p->position - velocityPerpendicular);
				p->SetVelocity(newVelocity);
			}

		}
	}


}

Math::Vector3 Storm::Physics::Tetrahedron::GetLook() const
{
	Math::Vector3 pos = GetPosition();
	return Math::Normalize(d->position - pos);
}

Math::Vector3 Storm::Physics::Tetrahedron::GetRight() const
{

	return Math::Normalize(c->position - b->position);
}

Math::Vector3 Storm::Physics::Tetrahedron::GetUp() const
{
	Math::Vector3 pos = GetPosition();
	return Math::Normalize(a->position - pos);
}

Math::Vector3 Storm::Physics::Tetrahedron::GetPosition() const
{
	return (b->position + c->position + d->position) * 0.3333f;
}

void Storm::Physics::Tetrahedron::SetPosition(const Math::Vector3& pos)
{
	a->SetPosition(pos + oa);
	b->SetPosition(pos + ob);
	c->SetPosition(pos + oc);
	d->SetPosition(pos + od);
}

void Storm::Physics::Tetrahedron::SetVelocity(const Math::Vector3& velocity)
{

	a->SetVelocity(velocity);
	b->SetVelocity(velocity);
	c->SetVelocity(velocity);
	d->SetVelocity(velocity);
}

Math::Matrix4 Storm::Physics::Tetrahedron::GetWorldMatrix() const
{
	Math::Vector3 r = Tetrahedron::GetRight();
	Math::Vector3 u = Tetrahedron::GetUp();
	Math::Vector3 l = Tetrahedron::GetLook();
	Math::Vector3 p = Tetrahedron::GetPosition();


	return Math::Matrix4{
	r.x,r.y,r.z,0.0f,
	u.x,u.y,u.z,0.0f,
	l.x,l.y,l.z,0.0f,
	p.x,p.y,p.z,1.0f

	};
}
