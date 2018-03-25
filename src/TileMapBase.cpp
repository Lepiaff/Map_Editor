#include "TileMapBase.h"

namespace ME
{
	TileMapBase::TileMapBase(TileMapParams params) :
		m_world(Ndk::Application::Instance()->AddWorld()),
		m_tileSize(params.tileSize),
		m_window(params.window),
		m_selection(m_world, m_tileSize),
		m_displayZone(params.displayZone)
	{
		m_size.x = params.size.x;
		m_size.y = params.size.y;
		m_world.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());

		m_entityCamera = m_world.CreateEntity();
		m_entityCamera->AddComponent<Ndk::NodeComponent>();
		m_entityCamera->AddComponent<Ndk::GraphicsComponent>();

		Ndk::CameraComponent& camera = m_entityCamera->AddComponent<Ndk::CameraComponent>();
		camera.SetTarget(m_window);
		camera.SetProjectionType(Nz::ProjectionType_Orthogonal);
		
		camera.SetTargetRegion(m_displayZone);
	}

	void TileMapBase::UpdateCamera(const float delta)
	{
		Ndk::NodeComponent& view = m_entityCamera->GetComponent<Ndk::NodeComponent>();
		Ndk::CameraComponent& camera = m_entityCamera->GetComponent<Ndk::CameraComponent>();
		if (view.GetPosition().y - delta * m_tileSize.y  >
			m_size.y * m_tileSize.y - camera.GetTargetRegion().height * m_window->GetSize().y)
		{
			view.SetPosition(0, static_cast<float>(m_size.y * m_tileSize.y - camera.GetTargetRegion().height * m_window->GetSize().y), 0);
		}
		else if (view.GetPosition().y - delta * m_tileSize.y  < 0)
		{
			view.SetPosition(0, 0, 0);
		}
		else
		{
			view.Move(0, -delta * m_tileSize.y, 0);
		}
	}

	void TileMapBase::SetTargetRegion(Nz::Rectf region)
	{
		m_displayZone = region;
		m_entityCamera->GetComponent<Ndk::CameraComponent>().SetTargetRegion(m_displayZone);
	}

	void TileMapBase::UpdateSelection(Nz::Rectui selection)
	{
		//Mise à jour des position sur Y en fonction de la position de la caméra
		Nz::Vector3f posCamera = m_entityCamera->GetComponent<Ndk::NodeComponent>().GetPosition();
		selection.x += posCamera.x;
		selection.y += posCamera.y;
		selection.width += posCamera.x;
		selection.height += posCamera.y;

		m_selection.Update(GetSelectedTiles(std::move(selection)));
	}

	Nz::Rectui TileMapBase::GetSelectedTiles(Nz::Rectui selection, bool ordonned)
	{
		if (ordonned)
			return { std::min(selection.x, selection.width) / static_cast<unsigned int>(m_tileSize.x),
			std::min(selection.y, selection.height) / static_cast<unsigned int>(m_tileSize.y),
			std::max(selection.x, selection.width) / static_cast<unsigned int>(m_tileSize.x),
			std::max(selection.y, selection.height) / static_cast<unsigned int>(m_tileSize.y) };

		return { selection.x / static_cast<unsigned int>(m_tileSize.x),
			selection.y / static_cast<unsigned int>(m_tileSize.y),
			selection.width / static_cast<unsigned int>(m_tileSize.x),
			selection.height / static_cast<unsigned int>(m_tileSize.y) };
	}
}