#include <Nazara/Graphics.hpp>
#include <Nazara/Renderer.hpp>
#include <Nazara/Utility.hpp>
#include <NDK/Application.hpp>
#include <NDK/Components.hpp>
#include <NDK/Systems.hpp>
#include <NDK/World.hpp>
#include <iostream>

#include <Nazara/Graphics/TileMap.hpp>
#include <Nazara/Platform/EventHandler.hpp>
#include <NDK/Canvas.hpp>

int main()
{
	//Déclaration des fonctions
	Nz::Rectui GetSelectedTiles(Nz::Vector2ui& clic, Nz::Vector2ui& realease);
	//bool IsOn(const Nz::Rectui rect, const Nz::Vector2ui& pos);

	//Création de la fenêtre
	Ndk::Application application;

	Nz::RenderWindow& mainWindow = application.AddWindow<Nz::RenderWindow>();
	mainWindow.Create(Nz::VideoMode(800, 600, 32)/*::GetDesktopMode()*/, "Test"/*,Nz::WindowStyle_Fullscreen*/);

	//Variables de fonctionnement
	Nz::EventHandler& eventHandler = mainWindow.GetEventHandler();
	Nz::Rectui rectSelectedTiles(0, 0, 0, 0);
	Nz::Vector2ui posOnClick(0, 0);
	Nz::Vector2ui posOnRelease(0, 0);
	Nz::MaterialRef tileSetMaterial = Nz::Material::New();
	tileSetMaterial->LoadFromFile("D:/Programmation_2018/NazaraProject/NazaraProject/Ressources/Tilesets/32x32/Village.png");
	const Nz::Vector2f tileSize{ 32,32 };
	float tileSetRatioX = 0.f;
	float tileSetRatioY = 0.f;

	//Worlds
	///tileSet
	Ndk::World& tileSetWorld = application.AddWorld();
	tileSetWorld.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());
	///Selection du tileSet
	Ndk::World& selectionWorld = application.AddWorld();
	selectionWorld.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());
	///Map
	Ndk::World& mapWorld = application.AddWorld();
	mapWorld.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());
	
	//Sprite Selection
	Nz::SpriteRef selectionSprite = Nz::Sprite::New();
	selectionSprite->GetMaterial()->Configure("Translucent2D");
	Nz::Color color(220, 120, 120, 100);
	selectionSprite->SetColor(color);
	Ndk::EntityHandle spriteEntity = tileSetWorld.CreateEntity();
	Ndk::NodeComponent& nCompSpriteSelection = spriteEntity->AddComponent<Ndk::NodeComponent>();
	Ndk::GraphicsComponent& gCompSpriteSelection = spriteEntity->AddComponent<Ndk::GraphicsComponent>();
	gCompSpriteSelection.Attach(selectionSprite);

	//Caméras
	///Caméra du tileSet
	Ndk::EntityHandle viewTileSet = tileSetWorld.CreateEntity();
	Ndk::NodeComponent& nodeCompViewTileSet = viewTileSet->AddComponent<Ndk::NodeComponent>();
	Ndk::CameraComponent& tileSetCamera = viewTileSet->AddComponent<Ndk::CameraComponent>();
	tileSetCamera.SetTarget(&mainWindow);
	tileSetCamera.SetProjectionType(Nz::ProjectionType_Orthogonal);
	tileSetRatioX = float(tileSetMaterial->GetDiffuseMap()->GetSize().x) / float(mainWindow.GetSize().x);
	std::cout << "taillefenetre X : " << mainWindow.GetSize().x << " , taillefenetre Y : " << mainWindow.GetSize().y << std::endl;
	std::cout << "tileSetRatio X : " << tileSetRatioX << std::endl;
	Nz::Rectf tileSetRectOnWindow(0.f, 0.f, tileSetRatioX, 0.8);
	tileSetCamera.SetTargetRegion(tileSetRectOnWindow);
	///Caméra de la sélection du tileSet
	Ndk::EntityHandle viewSelection = selectionWorld.CreateEntity();
	Ndk::NodeComponent& nodeCompViewSelection = viewSelection->AddComponent<Ndk::NodeComponent>();
	Ndk::CameraComponent& selectionCamera = viewSelection->AddComponent<Ndk::CameraComponent>();
	selectionCamera.SetTarget(&mainWindow);
	selectionCamera.SetProjectionType(Nz::ProjectionType_Orthogonal);
	Nz::Rectf selectionRectOnWindow(0.f, 0.81, tileSetRatioX, 1.f);
	selectionCamera.SetTargetRegion(selectionRectOnWindow);
	///Caméra de la map
	Ndk::EntityHandle viewMap = mapWorld.CreateEntity();
	viewMap->AddComponent<Ndk::NodeComponent>();
	Ndk::CameraComponent& mapCamera = viewMap->AddComponent<Ndk::CameraComponent>();
	mapCamera.SetTarget(&mainWindow);
	mapCamera.SetProjectionType(Nz::ProjectionType_Orthogonal);
	Nz::Rectf mapRectOnWindow(tileSetRatioX + 0.1, 1.f, 1.f, 1.f);
	mapCamera.SetTargetRegion(mapRectOnWindow);

	//TileMap
	///TileSet
	Nz::Vector2ui tileSetSize( tileSetMaterial->GetDiffuseMap()->GetSize().x / 32, tileSetMaterial->GetDiffuseMap()->GetSize().y / 32 );
	
	Nz::TileMap tileSet( tileSetSize , tileSize );
	tileSet.SetMaterial(0, tileSetMaterial);

	Ndk::EntityHandle tileSetEntity = tileSetWorld.CreateEntity();
	Ndk::NodeComponent& tileSetNodeComponent = tileSetEntity->AddComponent<Ndk::NodeComponent>();
	Ndk::GraphicsComponent& tileSetGraphicsComponent = tileSetEntity->AddComponent<Ndk::GraphicsComponent>();
	tileSetGraphicsComponent.Attach(&tileSet);

	std::vector<Nz::Vector2ui> posTiles;
	std::vector<Nz::Rectui> textureRectTiles;
	for (unsigned int y = 0; y < tileSetSize.y; ++y)
	{
		for (unsigned int x = 0; x < tileSetSize.x; ++x)
		{
			posTiles.emplace_back(Nz::Vector2ui(x, y));
			textureRectTiles.emplace_back(Nz::Rectui(x*32, y*32, 32, 32));
			tileSet.EnableTile(posTiles.back(), textureRectTiles.back());
		}
	}
	///Selection du tileSet
	Nz::Vector2ui tileMapSelectionSize(tileSetSize.x, 4);///Nb tile séléctionnables dans les deux dimensions

	std::cout << "taille selection x : " << tileMapSelectionSize.x << " , taille y : " << tileMapSelectionSize.y << std::endl;
	Nz::TileMap selection(tileMapSelectionSize, tileSize);
	selection.SetMaterial(0, tileSetMaterial);
	std::vector<Nz::Vector2ui> posTilesSelection;
	std::vector<Nz::Rectui> textureRectTilesSelection;

	Ndk::EntityHandle selectionEntity = selectionWorld.CreateEntity();
	Ndk::NodeComponent& selectionNodeComponent = selectionEntity->AddComponent<Ndk::NodeComponent>();
	Ndk::GraphicsComponent& selectionGraphicsComponent = selectionEntity->AddComponent<Ndk::GraphicsComponent>();
	selectionGraphicsComponent.Attach(&selection);

	//Gestion des évenements
	///MOLETTE
	eventHandler.OnMouseWheelMoved.Connect([&](const Nz::EventHandler*, Nz::WindowEvent::MouseWheelEvent e)
	{
		if (nodeCompViewTileSet.GetPosition().y - e.delta * 32 > 
			tileSetMaterial->GetDiffuseMap()->GetSize().y - tileSetCamera.GetTargetRegion().height * mainWindow.GetSize().y)
		{
			nodeCompViewTileSet.SetPosition(0, static_cast<float>(tileSetMaterial->GetDiffuseMap()->GetSize().y - tileSetCamera.GetTargetRegion().height * mainWindow.GetSize().y), 0);
		}
		else if (nodeCompViewTileSet.GetPosition().y - e.delta * 32 < 0 )
		{
			nodeCompViewTileSet.SetPosition(0, 0, 0);
		}
		else
		{
			nodeCompViewTileSet.Move(0, -e.delta * 32, 0);
		}
	});

	///CLIC GAUCHE
	eventHandler.OnMouseButtonPressed.Connect([&](const Nz::EventHandler*, Nz::WindowEvent::MouseButtonEvent e)
	{
		
		posOnClick.x = e.x + nodeCompViewTileSet.GetPosition().x;
		posOnClick.y = e.y + nodeCompViewTileSet.GetPosition().y;

		if (viewTileSet->GetComponent<Ndk::CameraComponent>().GetTargetRegion().Contains(static_cast<float>(e.x) / mainWindow.GetSize().x, static_cast<float>(e.y) / mainWindow.GetSize().y))
		{
			
		}

	});
		
	eventHandler.OnMouseButtonReleased.Connect([&](const Nz::EventHandler*, Nz::WindowEvent::MouseButtonEvent e)
	{
		posOnRelease.x = e.x + nodeCompViewTileSet.GetPosition().x;
		posOnRelease.y = e.y + nodeCompViewTileSet.GetPosition().y;
		
		if (viewTileSet->GetComponent<Ndk::CameraComponent>().GetTargetRegion().Contains(static_cast<float>(e.x) / mainWindow.GetSize().x, static_cast<float>(e.y) / mainWindow.GetSize().y))
		{
			rectSelectedTiles = GetSelectedTiles(posOnClick, posOnRelease);
			std::cout << rectSelectedTiles.x << " , " << rectSelectedTiles.y << " , " << rectSelectedTiles.width << " , " << rectSelectedTiles.height << std::endl;

			///on clear la selection
			posTilesSelection.clear();
			textureRectTilesSelection.clear();
			selection.DisableTiles();

			Nz::Vector2ui nbSelectedTiles;
			nbSelectedTiles.x = 1 + rectSelectedTiles.width - rectSelectedTiles.x;
			nbSelectedTiles.y = 1 + rectSelectedTiles.height - rectSelectedTiles.y;
			
			/*if (nbSelectedTiles.x > selection.GetMapSize().x)///Si la selection est plus grande que la tileMap
				nbSelectedTiles.x = selection.GetMapSize().x;/// on ramène la taille de la selection à la taille de la tileMap
			if (nbSelectedTiles.y > selection.GetMapSize().y)
				nbSelectedTiles.y = selection.GetMapSize().y;

			for (unsigned int y = 0 ; y < nbSelectedTiles.y; ++y)
			{
				for (unsigned int x = 0 ; x < nbSelectedTiles.x; ++x)
				{
					posTilesSelection.emplace_back(Nz::Vector2ui(x, y));
					textureRectTilesSelection.emplace_back(Nz::Rectui((x + rectSelectedTiles.x) * 32, (y + rectSelectedTiles.y) * 32, 32, 32));
					selection.EnableTile(posTilesSelection.back(), textureRectTilesSelection.back());
				}
			}*/

			nCompSpriteSelection.SetPosition(float(rectSelectedTiles.x) * 32.f, float(rectSelectedTiles.y) * 32.f);
			selectionSprite->SetSize(float(nbSelectedTiles.x) * 32.f, float(nbSelectedTiles.y) * 32.f);
		}
		else
		{
			
		}
		
	});
	///Redimensionnement de l'écran
	eventHandler.OnResized.Connect([&](const Nz::EventHandler*, Nz::WindowEvent::SizeEvent e)
	{
		//Redéfinition du ratio des éléments
		tileSetRatioX = float(tileSetMaterial->GetDiffuseMap()->GetSize().x) / float(mainWindow.GetSize().x);
		std::cout << "taillefenetre X : " << mainWindow.GetSize().x << " , taillefenetre Y : " << mainWindow.GetSize().y << std::endl;
		std::cout << "tileSetRatio X : " << tileSetRatioX << std::endl;
		///Redéfinition de la target région de la caméra du tileSet
		tileSetRectOnWindow = { 0.f, 0.f, tileSetRatioX, float(0.8) };
		tileSetCamera.SetTargetRegion(tileSetRectOnWindow);
		///Redéfinition de la target région de la caméra de la sélection
		selectionRectOnWindow = { 0.f, float(0.81), tileSetRatioX, 1.f };
		selectionCamera.SetTargetRegion(selectionRectOnWindow);
		///Redéfinition de la target région de la caméra de la map
		mapRectOnWindow = { tileSetRatioX + float(0.1), 1.f, 1.f, 1.f };
		mapCamera.SetTargetRegion(mapRectOnWindow);
	});

	while (application.Run())
	{
		mainWindow.Display();
	}

	return EXIT_SUCCESS;
}

Nz::Rectui GetSelectedTiles(Nz::Vector2ui& clic, Nz::Vector2ui& realease)
{
	return { std::min(clic.x, realease.x) / 32,
		std::min(clic.y, realease.y) / 32,
		std::max(clic.x, realease.x) / 32,
		std::max(clic.y, realease.y) / 32 };
}

/*bool IsOn(const Nz::Rectui rect, const Nz::Vector2ui& pos)
{
	return pos.x >= rect.x && pos.x <= rect.width && pos.y >= rect.y && pos.y <= rect.height ? true : false;
}*/