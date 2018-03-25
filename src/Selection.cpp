#include "Selection.h"

namespace ME
{

	Selection::Selection(Ndk::World& world, Nz::Vector2f tileSize) :
		m_world(world),
		m_sprite(Nz::Sprite::New()),
		m_entity(m_world.CreateEntity()),
		m_selection(0, 0, 0, 0),
		m_tileSize(tileSize)
	{
		m_sprite->SetSize(0, 0);
		m_sprite->GetMaterial()->Configure("Translucent2D");
		m_sprite->SetColor(Nz::Color(220, 120, 120, 100));
		m_entity->AddComponent<Ndk::NodeComponent>();
		m_entity->AddComponent<Ndk::GraphicsComponent>().Attach(m_sprite);
	}


	Selection::~Selection()
	{
	}

	void Selection::Update(Nz::Rectui newSelection)
	{
		m_selection = newSelection;
		m_entity->GetComponent<Ndk::NodeComponent>().SetPosition(m_selection.x * m_tileSize.x, m_selection.y * m_tileSize.y);
		
		Nz::Vector2ui nbSelectedTiles;
		nbSelectedTiles.x = 1 + m_selection.width - m_selection.x;
		nbSelectedTiles.y = 1 + m_selection.height - m_selection.y;

		m_sprite->SetSize(nbSelectedTiles.x * m_tileSize.x, nbSelectedTiles.y * m_tileSize.y);
	}

	void Selection::Clear()
	{
		m_selection = { 0, 0, 0, 0 };
		m_sprite->SetSize(0, 0);
	}

}