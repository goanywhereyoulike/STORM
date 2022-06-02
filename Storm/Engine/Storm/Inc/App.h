#pragma once
namespace Storm
{
	class AppState;
	struct AppConfig
	{
		std::wstring appName = L"Hello DirectX";
		uint32_t windowWidth = 1280;
		uint32_t windowHeight = 720;
		uint32_t debugDrawLimit = 64000;
		bool useEscToQuit = true;


	};

	class App
	{
	public:
		template <class StateType> 
		void AddState(std::string key)
		{

			//ASSERT(mAppStates.find(key) == mAppStates.end());
			//mAppStates.insert
			static_assert(std::is_base_of_v<AppState, StateType>, "App -- 'StateType' must be derived from 'AppState'.");
			auto [iter, result] = mAppStates.try_emplace(std::move(key), nullptr);
			if (result)
			{
				auto& ptr = iter->second;
				ptr = std::make_unique<StateType>();
				if (mCurrentState == nullptr)
				{
					LOG("App -- Starting state: %s", iter->first.c_str());
					mCurrentState = ptr.get();
				}
			}
		}

		void ChangeState(const std::string& statename);
		void Run(const AppConfig& appConfig);
		void Quit();


	private:
		using AppStateMap = std::map<std::string, std::unique_ptr<AppState>>;
		AppStateMap mAppStates;
		AppState* mCurrentState = nullptr;
		AppState* mNextState = nullptr;
		bool mRunning = false;


	};

}