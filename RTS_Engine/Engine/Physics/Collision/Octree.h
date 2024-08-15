#pragma once
#include <Physics/Collision/OctreeNode.h>

namespace Bennett
{
	class Entity;

	template<class T>
	class BENNETT_ENGINE Octree
	{
	private:
		Node<T>* m_RootNode;

	public:
		inline Octree()
		{
			m_RootNode = new Node<T>(nullptr, 1, (Node<T>::OCTREE_QUADRANT) - 1);
		}

		inline ~Octree()
		{
			if (m_RootNode)
			{
				delete m_RootNode;
				m_RootNode = nullptr;
			}
		}

		/// <summary>
		/// Adds a data object to the octree.
		/// </summary>
		inline void AddDataObject(const T& entity) { m_RootNode->AddDataToNode(entity); };

		/// <summary>
		/// Not Implemented
		/// </summary>
		inline void RemoveDataObject() { throw; };

		/// <summary>
		/// Not Implemented
		/// </summary>
		inline void IsWithinNode() { throw; };

		/// <summary>
		/// Completey empties the octree of all data.
		/// </summary>
		inline void Clear()
		{
			if (m_RootNode)
			{
				delete m_RootNode;
				m_RootNode = nullptr;
			}

			m_RootNode = new Node<T>(nullptr, 1, (Node<T>::OCTREE_QUADRANT) - 1);
		}

		inline void IncreaseMaxDepth() { Node<T>::IncreaseMaxDepth(); };
		inline void DecreaseMaxDepth() { Node<T>::DecreaseMaxDepth(); };

		void Update(const float& deltaTime);
		void Render(const Renderer& renderer);
		void Print();
	};

	template<class T>
	inline void Octree<T>::Update(const float& deltaTime)
	{
		if (m_RootNode)
		{
			m_RootNode->Update(deltaTime);
		}
	};

	template<class T>
	inline void Octree<T>::Render(const Renderer& renderer)
	{
		if (ENABLE_DRAW_OCTREE_OUTLINE)
		{
			if (m_RootNode)
			{
				m_RootNode->Render(renderer);
			}
		}
	};

	template<class T>
	inline void Octree<T>::Print()
	{
		if (m_RootNode)
		{
			m_RootNode->Print();
		}
	}
}