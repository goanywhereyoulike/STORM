#pragma once

namespace Storm
{
	class GameObject;
	using MakeOverride = std::function<bool(const char*, const rapidjson::Value&, GameObject&)>;
	namespace GameObjectFactory
	{
		void SetMakeOverride(MakeOverride makeOveride);
		void Make(std::filesystem::path templateFile, GameObject& gameObject);
	};

}