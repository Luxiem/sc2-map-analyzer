#include "Controller.h"
#include "Common.h" // for IncPath
#include "UV/UIVerse/List.h"
#include "UV/UIVerse/UVTab.h"
#include "UV/UIVerse/Combo.h"
#include "UV/UIVerse/UVLabel.h"
#include "UV/UIVerse/ImageButton.h"
#include "SC2Map.hpp"


void SaveImageFile();
void LoadMapFile();


namespace MainUI
{
	bool s_showResourceLayer = true;
	bool s_showHeightLayer = true;
	bool s_showOpennessLayer = true;
	bool s_showPathingLayer = false;
    bool s_showInfluenceLayer = false;
	bool s_showOverlay = true;
	bool s_showBases = false;
	bool s_showPhotonOvercharge = false;
    bool s_showLoS = true;
	bool s_showVespeneBalance = false;
	
	unsigned char s_ResourceLayerOpacity = 255;
	unsigned char s_OpennessLayerOpacity = 255;
	unsigned char s_PathingLayerOpacity = 255;
	int s_SiegeMapType = 0;
	
	bool s_redraw = true;
}


namespace MainUI
{
	UV::Switch* m_switchSiegeBlink = NULL;
	UV::Switch* m_switchSiegeTank = NULL;
	
	
	static void ToggleOverlayLayer(void*, UV::EventArgs&)
	{
		s_showOverlay = !s_showOverlay;
		s_redraw = true;
	}
	
	
	static void ToggleHeightLayer(void*, UV::EventArgs&)
	{
		s_showHeightLayer = !s_showHeightLayer;
		s_redraw = true;
	}
	
	
	static void ToggleOpennessLayer(void*, UV::EventArgs&)
	{
		s_showOpennessLayer = !s_showOpennessLayer;
		s_redraw = true;
	}
	
	
	static void ToggleResourceLayer(void*, UV::EventArgs&)
	{
		s_showResourceLayer = !s_showResourceLayer;
		s_redraw = true;
	}
	
	
	static void ToggleBasesLayer(void*, UV::EventArgs&)
	{
		s_showBases = !s_showBases;
		s_redraw = true;
	}
	
	
	static void TogglePhotonOverchargeLayer(void*, UV::EventArgs&)
	{
		s_showPhotonOvercharge = !s_showPhotonOvercharge;
		s_redraw = true;
	}


  static void ToggleLoSLayer(void*, UV::EventArgs&)
	{
		s_showLoS = !s_showLoS;
		s_redraw = true;
	}

  static void ToggleVespeneBalanceLayer(void*, UV::EventArgs&)
  {		
	  s_showVespeneBalance = !s_showVespeneBalance;
	  s_redraw = true;
  }
	
	
	static void TogglePathingLayer(void*, UV::EventArgs&)
	{
		s_showPathingLayer = !s_showPathingLayer;
		s_redraw = true;
	}
	
    
    static void ToggleInfluenceLayer(void*, UV::EventArgs&)
	{
		s_showInfluenceLayer = !s_showInfluenceLayer;
		s_redraw = true;
	}
	
	static void ToggleBlinkMap(void*, UV::EventArgs&)
	{
		if (!m_switchSiegeBlink->GetToggle())
		{
			m_switchSiegeTank->SetToggle(false);
			s_SiegeMapType = 1;
		}
		else s_SiegeMapType = 0;
		s_redraw = true;
	}
	
	
	static void ToggleTankMap(void*, UV::EventArgs&)
	{
		if (!m_switchSiegeTank->GetToggle())
		{
			m_switchSiegeBlink->SetToggle(false);
			s_SiegeMapType = 2;
		}
		else s_SiegeMapType = 0;
		s_redraw = true;
	}
	
	
	
	static void SliderMoved(void*, UV::EventArgs&)
	{
		s_redraw = true;
	}
	
	
	static void SaveImage(void*, UV::EventArgs&)
	{
        SaveImageFile();
	}


    static void LoadMap(void*, UV::EventArgs&)
	{
        LoadMapFile();
	}


    static void ReloadConfig(void*, UV::EventArgs&)
	{
        Common::ReloadConfig();
        s_redraw = true;
	}
}






Controller::Controller()
{
  m_core = 0;
  s0 = 0;
  s1 = 0;
  s2 = 0;

  m_pathSpawnA = 0;
  m_pathSpawnB = 0;
  m_pathBaseA = 0;
  m_pathBaseB = 0;

  m_mainMenu = 0;
}


Controller::~Controller()
{
}


void Controller::Init()
{
    if (m_core) return;

	// Create UI Core insance
	m_core = new UV::Core();

    UV::Page* pageResource = m_core->CreatePage();
    UV::Page* pageLayer = m_core->CreatePage();

    UV::Page* pagePath = m_core->CreatePage();
    UV::Page* page2 = m_core->CreatePage();

	using namespace UV;

    m_mainMenu = new Container();
	m_mainMenu->SetBackgroundColour(0xffefefef);
    
	VStack* layoutLayer = new VStack();
	pageLayer->Add(layoutLayer);
	layoutLayer->SetPosition(16, 40);

    VStack* layoutPath = new VStack();
	pagePath->Add(layoutPath);
	layoutPath->SetPosition(16, 40);

	VStack* layoutResource = new VStack();
	pageResource->Add(layoutResource);
	layoutResource->SetPosition(16, 40);
   
	// Reload config
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ReloadConfig;
		ImageButton* m_configReload = new ImageButton(buttonDecl);
		m_mainMenu->Add(m_configReload);
		m_configReload->SetText("Reload Config", 0xff000000);
        m_configReload->SetImageFile("png/list.png");
		m_configReload->SetSize(72, 72);
		m_configReload->SetPosition(188, 12);
	}
    
	// Load
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::LoadMap;
		ImageButton* m_buttonLoad = new ImageButton(buttonDecl);
		m_mainMenu->Add(m_buttonLoad);
		m_buttonLoad->SetText("Load Map...", 0xff000000);
		m_buttonLoad->SetImageFile("png/open.png");
		m_buttonLoad->SetSize(72, 72);
		m_buttonLoad->SetPosition(12, 12);
	}
    
	// Save
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::SaveImage;
		ImageButton* m_buttonSave = new ImageButton(buttonDecl);
		m_mainMenu->Add(m_buttonSave);
		m_buttonSave->SetText("Save Image...", 0xff000000);
        m_buttonSave->SetImageFile("png/save.png");
		m_buttonSave->SetSize(72, 72);
		m_buttonSave->SetPosition(101, 12);
	}	
    
	int space_size = 16;

	// Page 1
    
	layoutLayer->Space(space_size);
	
	// Height Layer
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleHeightLayer;
		Switch* m_switch2 = new Switch(buttonDecl);
		layoutLayer->Add(m_switch2);
		m_switch2->SetText("Height", 0xff000000);
		
		//s0 = new Slider(Slider::HORIZONTAL);
		//s0->SetSize(100, 14);
		//s0->SetDataRange(0.0, 1.0);
		//s0->SetValue(1.0);
		//layoutMain->Add(s0);
		//s0->m_callback = &MainUI::SliderMoved;
	}

    layoutLayer->Space(space_size);
	
	// Openness Layer
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleOpennessLayer;
		Switch* m_switch3 = new Switch(buttonDecl);
		layoutLayer->Add(m_switch3);
		m_switch3->SetText("Openness", 0xff000000);
		
		//s1 = new Slider(Slider::HORIZONTAL);
		//s1->SetSize(100, 14);
		//s1->SetDataRange(0.0, 1.0);
		//s1->SetValue(1.0);
		//layoutMain->Add(s1);
		//s1->m_callback = &MainUI::SliderMoved;
	}
	
    layoutLayer->Space(space_size);
			
	// Bases
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleBasesLayer;
		Switch* m_switch5 = new Switch(buttonDecl);
		layoutLayer->Add(m_switch5);
		m_switch5->SetText("Bases", 0xff000000);
		m_switch5->SetToggle(false);
	}

    layoutLayer->Space(space_size);
	
	// Siege Maps
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleBlinkMap;
		MainUI::m_switchSiegeBlink = new Switch(buttonDecl);
		layoutLayer->Add(MainUI::m_switchSiegeBlink);
		MainUI::m_switchSiegeBlink->SetText("Blink Map", 0xff000000);
		MainUI::m_switchSiegeBlink->SetToggle(false);
	}
	
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleTankMap;
		MainUI::m_switchSiegeTank = new Switch(buttonDecl);
		layoutLayer->Add(MainUI::m_switchSiegeTank);
		MainUI::m_switchSiegeTank->SetText("Siege Map", 0xff000000);
		MainUI::m_switchSiegeTank->SetToggle(false);
	}
	
	{
		//s2 = new Slider(Slider::HORIZONTAL);
		//s2->SetSize(100, 14);
		//s2->SetDataRange(0.0, 1.0);
		//s2->SetValue(1.0);
		//layoutMain->Add(s2);
		//s2->m_callback = &MainUI::SliderMoved;
	}

	layoutLayer->Space(space_size);
		
    // Influence Layer
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleInfluenceLayer;
		Switch* m_switchInfluence = new Switch(buttonDecl);
		layoutLayer->Add(m_switchInfluence);
		m_switchInfluence->SetText("Influence", 0xff000000);
		m_switchInfluence->SetToggle(false);
    }
    
    {
        m_influenceLocation = new Combo();
        layoutLayer->Add(m_influenceLocation);
        
        m_influenceLocation->SetCallback(new TCallback<Controller>(this, &Controller::InfluenceCallback));
        //m_influenceLocation->SetPosition(16, 90);
        m_influenceLocation->SetSize(68, 22);
    }
	
	// Page 2

	layoutPath->Space(space_size);

    // Pathing Layer
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::TogglePathingLayer;
		Switch* m_switchPath = new Switch(buttonDecl);
		layoutPath->Add(m_switchPath);
		m_switchPath->SetText("Show Pathing", 0xff000000);
		m_switchPath->SetToggle(false);
        
        //m_switchPath->SetPosition(16, 60);
	}
    

    {
        m_basePathStart = new Combo();
        layoutPath->Add(m_basePathStart);

        m_basePathStart->SetCallback(new TCallback<Controller>(this, &Controller::PathCallback1));
        m_basePathStart->SetPosition(16, 110);
        m_basePathStart->SetSize(68, 22);
    }

    {
        Combo* combo = new Combo();
        layoutPath->Add(combo);

        combo->AddItem("main");
        combo->AddItem("nat");
        combo->AddItem("3rd");

        combo->Select(0);

        combo->SetCallback(new TCallback<Controller>(this, &Controller::PathCallback2));
        combo->SetPosition(78, 110);
    }

    {
        Label* label = new Label();
        label->SetText("to", 0xff000000);
        label->SetPosition(124, 110 + 9);
        pagePath->Add(label);
    }

    {
      m_basePathEnd = new Combo();
      layoutPath->Add(m_basePathEnd);

      m_basePathEnd->SetCallback(new TCallback<Controller>(this, &Controller::PathCallback3));
      m_basePathEnd->SetPosition(140, 110);
      m_basePathEnd->SetSize(68, 22);
    }

    {
      Combo* combo = new Combo();
      layoutPath->Add(combo);

      combo->AddItem("main");
      combo->AddItem("nat");
      combo->AddItem("3rd");

      combo->Select(0);

      combo->SetCallback(new TCallback<Controller>(this, &Controller::PathCallback4));
      combo->SetPosition(202, 110);
    }

	// Page 3

	layoutResource->Space(space_size);
	    	
	// Photon Overcharge
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::TogglePhotonOverchargeLayer;
		Switch* m_switch6 = new Switch(buttonDecl);
		layoutResource->Add(m_switch6);
		m_switch6->SetText("Photon Overcharge", 0xff000000);
		m_switch6->SetToggle(false);
	}
	
	layoutResource->Space(space_size);

	// Watchtowers
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = MainUI::ToggleOverlayLayer;
		Switch* m_switch1 = new Switch(buttonDecl);
		layoutResource->Add(m_switch1);
		m_switch1->SetText("Watchtowers", 0xff000000);
	}

    layoutResource->Space(space_size);

	// Resource
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleResourceLayer;
		Switch* m_switch4 = new Switch(buttonDecl);
		layoutResource->Add(m_switch4);
		m_switch4->SetText("Destructible", 0xff000000);
		m_switch4->SetToggle(true);
	}
  
    layoutResource->Space(space_size);
  
    // LoS
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleLoSLayer;
		Switch* m_switchLoS = new Switch(buttonDecl);
		layoutResource->Add(m_switchLoS);
		m_switchLoS->SetText("LoS Blocker", 0xff000000);
		m_switchLoS->SetToggle(true);
	}

	layoutResource->Space(space_size);

	// Vespene
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleVespeneBalanceLayer;
		Switch* m_switchVespene = new Switch(buttonDecl);
		layoutResource->Add(m_switchVespene);
		m_switchVespene->SetText("Vespene Balance", 0xff000000);
		m_switchVespene->SetToggle(MainUI::s_showVespeneBalance);
	}
    
    
  {
    Tab* tab = new Tab();

    tab->AddTab("Layers", pageLayer);
    tab->AddTab("Path", pagePath);
	tab->AddTab("Resource", pageResource);
    //tab->AddTab("Log", page2);

    tab->SetPosition(0, 0);
    tab->SetSize(270, 42);

    m_core->Add(tab);
  }
 
  // List
  {
    m_log = new List();
    page2->Add(m_log);

    m_log->SetSize(182, 258);
    m_log->SetPosition(16, 60);
  }

  // Main menu added last
  m_core->Add(m_mainMenu);

}


void Controller::Done()
{
  if (m_core) delete m_core;
  m_core = 0;
}


bool Controller::ShowOpennessLayer()
{
  return MainUI::s_showOpennessLayer;
}


bool Controller::ShowLoS()
{
  return MainUI::s_showLoS;
}


bool Controller::ShowHeightLayer()
{
  return MainUI::s_showHeightLayer;
}


bool Controller::ShowResourceLayer()
{
  return MainUI::s_showResourceLayer;
}


bool Controller::ShowPhotonOvercharge()
{
  return MainUI::s_showPhotonOvercharge;
}


bool Controller::ShowWatchTower()
{
  return MainUI::s_showOverlay;
}


bool Controller::ShowBases()
{
  return MainUI::s_showBases;
}


bool Controller::ShowPathingLayer()
{
  return MainUI::s_showPathingLayer;
}


bool Controller::ShowInfluenceLayer()
{
    return MainUI::s_showInfluenceLayer;
}


bool Controller::ShowVespeneEfficiency()
{
	return MainUI::s_showVespeneBalance;
}


float Controller::GetAlpha1()
{
  return 1.0f; //return s0->GetValue();
}


float Controller::GetAlpha2()
{
  return 1.0f; //return s1->GetValue();
}


float Controller::GetAlpha3()
{
  return 1.0f; //return s2->GetValue();
}


int Controller::SiegeMapType()
{ 
  return MainUI::s_SiegeMapType;
}


void Controller::Log(const char* a_msg)
{
  m_log->AddItem(std::string(a_msg));
}


void Controller::OnMapLoaded(SC2Map* a_map)
{
    m_basePathStart->Clear();
    m_basePathEnd->Clear();
    m_influenceLocation->Clear();

	for (list<StartLoc*>::iterator i = a_map->startLocs.begin(); i != a_map->startLocs.end(); ++i)
    {
        m_basePathStart->AddItem( (*i)->name );
        m_basePathEnd->AddItem( (*i)->name );
        m_influenceLocation->AddItem( (*i)->name );
    }

    m_basePathStart->Select(0);
    m_pathSpawnA = 0;

    m_basePathEnd->Select(1);
    m_pathSpawnB = 1;
    
    m_influenceLocation->Select(0);
    m_influenceSpawn = 0;

    Common::SetPath(m_pathSpawnA, m_pathBaseA, m_pathSpawnB, m_pathBaseB);
}


void Controller::PathCallback1(UV::Widget* a_sender, UV::EventArgs& a_args)
{
  // Spawn start
  m_pathSpawnA = ((UV::Combo*)a_sender)->GetSelectedIndex();
  MainUI::s_redraw = true;
  Common::SetPath(m_pathSpawnA, m_pathBaseA, m_pathSpawnB, m_pathBaseB);
}


void Controller::PathCallback2(UV::Widget* a_sender, UV::EventArgs& a_args)
{
  // Base # start
  m_pathBaseA = ((UV::Combo*)a_sender)->GetSelectedIndex();
  MainUI::s_redraw = true;
  Common::SetPath(m_pathSpawnA, m_pathBaseA, m_pathSpawnB, m_pathBaseB);
}


void Controller::PathCallback3(UV::Widget* a_sender, UV::EventArgs& a_args)
{
  // Spawn end
  m_pathSpawnB = ((UV::Combo*)a_sender)->GetSelectedIndex();
  MainUI::s_redraw = true;
  Common::SetPath(m_pathSpawnA, m_pathBaseA, m_pathSpawnB, m_pathBaseB);
}


void Controller::PathCallback4(UV::Widget* a_sender, UV::EventArgs& a_args)
{
  // Base # end
  m_pathBaseB = ((UV::Combo*)a_sender)->GetSelectedIndex();
  MainUI::s_redraw = true;
  Common::SetPath(m_pathSpawnA, m_pathBaseA, m_pathSpawnB, m_pathBaseB);
}


void Controller::InfluenceCallback(UV::Widget* a_sender, UV::EventArgs& a_args)
{
    // Base #
    m_influenceSpawn = ((UV::Combo*)a_sender)->GetSelectedIndex();
    MainUI::s_redraw = true;
}


void Controller::OnWindowSizeChanged(int a_width, int a_height)
{
  m_log->SetSize(240, a_height - 72);
  m_core->OnClientAreaChanged(a_width, a_height);

  // Calculate menu position
  if (m_mainMenu)
  {
	int y = a_height - 96;
	m_mainMenu->SetPosition(0, y);
  }
}
