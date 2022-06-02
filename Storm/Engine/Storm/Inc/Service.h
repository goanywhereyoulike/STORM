#pragma once
#include "Typelds.h"

namespace Storm
{
	class GameWorld;

	class Service
	{
	public:
		Service() = default;
		virtual ~Service() = default;


		Service(const Service&) = delete;
		Service& operator=(const Service&) = delete;
		Service(Service&&) = delete;
		Service& operator=(Service&&) = delete;

		virtual uint32_t GetTypeId() const = 0;

		virtual void Initialize() {}
		virtual void Terminate() {}

		virtual void Update(float deltaTime) {}
		virtual void DebugUI() {}

		virtual void Render() {}

		GameWorld& GetWorld() { return *mWorld; }
		const GameWorld& GetWorld() const { return *mWorld; }

	private:
		friend class GameWorld;
		GameWorld* mWorld = nullptr;


	};


}