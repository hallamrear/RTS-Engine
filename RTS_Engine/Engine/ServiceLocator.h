#pragma once
#include "Renderer.h"
#include "AssetManager.h"

namespace Bennett
{
	class ServiceLocator
	{
	private:
		static Renderer* m_Renderer;
		static AssetManager* m_AssetManager;

	public:
		static void Initialise();
		static void Shutdown();

		inline static AssetManager& GetAssetManager() { assert(m_AssetManager != nullptr); return *m_AssetManager; };
		inline static Renderer& GetRenderer() { assert(m_Renderer != nullptr); return *m_Renderer; };
	};
}