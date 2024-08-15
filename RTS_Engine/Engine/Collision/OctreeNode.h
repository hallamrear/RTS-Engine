#pragma once
#include <cmath>
#include <World/Entity.h>
#include <System/ServiceLocator.h>
#include <System/Assets/AssetManager.h>
#include <Collision/CollisionDetection.h>
#include <Collision/Collider/AABBCollider.h>

#define OCTREE_CHILD_NODE_COUNT 8
#define OCTREE_NODE_MAX_ENTITY_COUNT 8
#define OCTREE_MAX_NODE_BOX_SIZE 4096
#define OCTREE_MAX_TREE_DEPTH 128

namespace Bennett
{
	template<class T>
	class BENNETT_ENGINE Node
	{
	private:
		inline static int m_MaxDepth = OCTREE_MAX_TREE_DEPTH;
		inline static int m_MaxSize = OCTREE_MAX_NODE_BOX_SIZE;
		inline static int m_MaxDataObjectsPerNode = OCTREE_NODE_MAX_ENTITY_COUNT;

	protected:
		int m_Depth = -1;
		bool m_HasSplit = false;
		Entity* m_NodeEntity = nullptr;
		std::vector<T> m_DataObjects;
		unsigned int m_Size = INT_MAX;
		Transform m_Centre = Transform();

		Node* m_ParentNode = nullptr;
		Node* m_ChildNodes[OCTREE_CHILD_NODE_COUNT];

	public:

		inline static void IncreaseMaxDepth()
		{
			m_MaxDepth++;
			Log(LOG_MINIMAL, "Max Depth: %i\n", m_MaxDepth);
		};

		inline static void DecreaseMaxDepth()
		{
			if (m_MaxDepth - 1 <= 1)
				m_MaxDepth = 1;
			else
				m_MaxDepth--;

			Log(LOG_MINIMAL, "Max Depth: %i\n", m_MaxDepth);
		};

		AABBCollider* m_Collider = nullptr;

		enum OCTREE_QUADRANT
		{
			POSX_POSY_POSZ = 0,
			POSX_POSY_NEGZ,
			NEGX_POSY_NEGZ,
			NEGX_POSY_POSZ,
			POSX_NEGY_POSZ,
			POSX_NEGY_NEGZ,
			NEGX_NEGY_NEGZ,
			NEGX_NEGY_POSZ,
		};

		inline Node<T>(Node* parent, const int& depth, const OCTREE_QUADRANT& quadrant)
		{
			m_ParentNode = parent;
			m_Depth = depth;
			m_Size = m_MaxSize / m_Depth;
			m_Centre = Transform();

			if (m_Depth > 1)
			{
				int halfSize = m_Size / 2;
				int quarterSize = halfSize / 2;

				switch (quadrant)
				{
				case POSX_POSY_POSZ: { m_Centre.SetPosition(m_ParentNode->m_Centre.GetPosition() + glm::vec3(+halfSize, +halfSize, +halfSize)); } break;
				case POSX_POSY_NEGZ: { m_Centre.SetPosition(m_ParentNode->m_Centre.GetPosition() + glm::vec3(+halfSize, +halfSize, -halfSize)); } break;
				case NEGX_POSY_NEGZ: { m_Centre.SetPosition(m_ParentNode->m_Centre.GetPosition() + glm::vec3(-halfSize, +halfSize, -halfSize)); } break;
				case NEGX_POSY_POSZ: { m_Centre.SetPosition(m_ParentNode->m_Centre.GetPosition() + glm::vec3(-halfSize, +halfSize, +halfSize)); } break;
				case POSX_NEGY_POSZ: { m_Centre.SetPosition(m_ParentNode->m_Centre.GetPosition() + glm::vec3(+halfSize, -halfSize, +halfSize)); } break;
				case POSX_NEGY_NEGZ: { m_Centre.SetPosition(m_ParentNode->m_Centre.GetPosition() + glm::vec3(+halfSize, -halfSize, -halfSize)); } break;
				case NEGX_NEGY_NEGZ: { m_Centre.SetPosition(m_ParentNode->m_Centre.GetPosition() + glm::vec3(-halfSize, -halfSize, -halfSize)); } break;
				case NEGX_NEGY_POSZ: { m_Centre.SetPosition(m_ParentNode->m_Centre.GetPosition() + glm::vec3(-halfSize, -halfSize, +halfSize)); } break;
				default:
					throw;
					break;
				}

				m_Collider = new AABBCollider(m_Centre, glm::vec3((float)halfSize));
			}

			for (size_t i = 0; i < OCTREE_CHILD_NODE_COUNT; i++)
			{
				m_ChildNodes[i] = nullptr;
			}

			std::string name = "N_" + std::to_string(depth) + "_" + std::to_string(quadrant);
			m_NodeEntity = new Entity(name.c_str());
			m_NodeEntity->GetTransform().SetPosition(m_Centre.GetPosition());
			m_NodeEntity->GetTransform().SetScale(glm::vec3((float)m_Size));
		}

		inline ~Node<T>()
		{
			if (m_HasSplit)
			{
				for (size_t i = 0; i < OCTREE_CHILD_NODE_COUNT; i++)
				{
					delete m_ChildNodes[i]; 
					m_ChildNodes[i] = nullptr;
				}
			}

			if (m_NodeEntity)
			{
				delete m_NodeEntity;
				m_NodeEntity = nullptr;
			}

			if (m_Collider)
			{
				delete m_Collider;
				m_Collider = nullptr;
			}

			m_DataObjects.clear();
			m_HasSplit = false;
		}

		inline void Update(const float& deltaTime)
		{
			if (m_NodeEntity->HasModel() == false)
			{
				m_NodeEntity->SetModel(ServiceLocator::GetAssetManager().GetModel("1x1_Cube"));
			}

			if (m_HasSplit)
			{
				for (size_t i = 0; i < OCTREE_CHILD_NODE_COUNT; i++)
				{
					m_ChildNodes[i]->Update(deltaTime);
				}
			}
		}

		inline void Render(const Renderer& renderer)
		{
			if (m_HasSplit)
			{
				for (size_t i = 0; i < OCTREE_CHILD_NODE_COUNT; i++)
				{
					m_ChildNodes[i]->Render(renderer);
				}
			}
			else
			{
				m_NodeEntity->Render(renderer);
			}				
		}

		inline void AddDataToNode(const T& data)
		{
			if (m_HasSplit)
			{
				const Entity* entity = dynamic_cast<const Entity*>(data);

				if (!entity)
					return;

				for (size_t i = 0; i < OCTREE_CHILD_NODE_COUNT; i++)
				{
					AABBCollider* aabb = m_ChildNodes[i]->m_Collider;

					if (Collision::CheckCollision(entity->GetCollider(), *aabb))
					{
						m_ChildNodes[i]->AddDataToNode(data);
						break;
					}
				}

				return;
			}

			m_DataObjects.push_back(data);

			if (m_Depth >= m_MaxDepth)
			{
				return;
			}

			if (m_DataObjects.size() > m_MaxDataObjectsPerNode)
			{
				SplitNode();
			}
		}

		inline void SplitNode()
		{
			for (size_t i = 0; i < OCTREE_CHILD_NODE_COUNT; i++)
			{
				m_ChildNodes[i] = new Node<T>(this, m_Depth * 2, (OCTREE_QUADRANT)i);
			}

			for (size_t i = 0; i < m_DataObjects.size(); i++)
			{
				T entity = m_DataObjects[i];
				Node* node = nullptr;

				for (size_t i = 0; i < OCTREE_CHILD_NODE_COUNT; i++)
				{
					node = m_ChildNodes[i];

					AABBCollider* aabb = m_ChildNodes[i]->m_Collider;

					if (Collision::CheckCollision(entity->GetCollider(), *aabb))
					{
						break;
					}
				}

				if (node != nullptr)
				{
					node->AddDataToNode(entity);
				}

				//BUG : currently if the data does not fit in any of the nodes then it will fall through and just get cleared.
			}

			m_HasSplit = true;
			m_DataObjects.clear();
		}

	public:

		/// <summary>
		/// Returns parent node of current node. If the return value is nullptr, the current node is the root node.
		/// </summary>
		/// <returns>Pointer to the parent node.</returns>
		inline Node* GetParentNode()
		{
			return m_ParentNode;
		}

		inline const int& GetDepth() { return m_Depth; };

		inline void Print()
		{
			int count = (int)log2(m_Depth);
			std::string str = "";
			for (size_t i = 0; i < count; i++)
			{
				str += '\t';
			}
			str += std::to_string(m_DataObjects.size());
			str += "\n";
			Log(LOG_MINIMAL, str.c_str());

			if (m_HasSplit)
			{
				for (size_t i = 0; i < OCTREE_CHILD_NODE_COUNT; i++)
				{
					if(m_ChildNodes[i]) m_ChildNodes[i]->Print();
				}
			}
		}

	};
}