#pragma once
#ifndef SELECTION_H
#define SELECTION_H

#include <Nazara/Math/Rect.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <NDK/World.hpp>
#include <NDK/Entity.hpp>
#include <NDK/Application.hpp>
#include <NDK/Components.hpp>

namespace ME
{

	class Selection
	{
	private:
		Nz::Rectui m_selection;
		Nz::SpriteRef m_sprite;
		Ndk::World& m_world;
		Ndk::EntityHandle m_entity;
		Nz::Vector2f m_tileSize;
	public:
		Selection(Ndk::World& world, Nz::Vector2f tileSize);
		~Selection();

		void Update(Nz::Rectui newSelection);
		
		void Clear();

		inline Nz::Rectui GetSelectionIds() { return m_selection; }
	};
}
#endif // !SELECTION_H