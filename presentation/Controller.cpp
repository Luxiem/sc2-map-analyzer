#include "Controller.h"
#include "Common.h" // for IncPath
#include "UV/UIVerse/List.h"
#include "UV/UIVerse/UVTab.h"
#include "UV/UIVerse/Combo.h"
#include "UV/UIVerse/UVLabel.h"
#include "SC2Map.hpp"


void SaveImageFile();
void LoadMapFile();


namespace MainUI
{
	bool s_showResourceLayer = true;
	bool s_showHeightLayer = true;
	bool s_showOpennessLayer = true;
	bool s_showPathingLayer = false;
	bool s_showOverlay = true;
	bool s_showBases = false;
	bool s_showPhotonOvercharge = false;
  bool s_showLoS = true;
	
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
	
	
	static void TogglePathingLayer(void*, UV::EventArgs&)
	{
		s_showPathingLayer = !s_showPathingLayer;
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
}


Controller::~Controller()
{
}


void Controller::Init()
{
  if (m_core) return;

	// Create UI Core insance
	m_core = new UV::Core();

  UV::Page* page = m_core->CreatePage();
  UV::Page* pagePath = m_core->CreatePage();
  UV::Page* page2 = m_core->CreatePage();

	using namespace UV;
	
	VStack* layoutMain = new VStack();
	page->Add(layoutMain);
	layoutMain->SetPosition(16, 40);

  VStack* layoutCol2 = new VStack();
	page->Add(layoutCol2);
	layoutCol2->SetPosition(128, 40);
	
  //layoutMain->SetBorder(1, 0xff505050);

	// Overlay Layer
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = MainUI::ToggleOverlayLayer;
		Switch* m_switch1 = new Switch(buttonDecl);
		layoutMain->Add(m_switch1);
		m_switch1->SetText("Watchtowers", 0xff000000);
	}

  layoutMain->Space(20);

	// Resource
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleResourceLayer;
		Switch* m_switch4 = new Switch(buttonDecl);
		layoutMain->Add(m_switch4);
		m_switch4->SetText("Destructible", 0xff000000);
		m_switch4->SetToggle(true);
	}
  
  layoutMain->Space(20);	
  
  // LoS
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleLoSLayer;
		Switch* m_switchLoS = new Switch(buttonDecl);
		layoutMain->Add(m_switchLoS);
		m_switchLoS->SetText("LoS Blocker", 0xff000000);
		m_switchLoS->SetToggle(true);
	}

  layoutMain->Space(20);
	
	// Height Layer
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleHeightLayer;
		Switch* m_switch2 = new Switch(buttonDecl);
		layoutMain->Add(m_switch2);
		m_switch2->SetText("Height", 0xff000000);
		
		s0 = new Slider(Slider::HORIZONTAL);
		s0->SetSize(100, 14);
		s0->SetDataRange(0.0, 1.0);
		s0->SetValue(1.0);
		layoutMain->Add(s0);
		s0->m_callback = &MainUI::SliderMoved;
	}

  layoutMain->Space(20);
	
	// Openness Layer
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleOpennessLayer;
		Switch* m_switch3 = new Switch(buttonDecl);
		layoutMain->Add(m_switch3);
		m_switch3->SetText("Openness", 0xff000000);
		
		s1 = new Slider(Slider::HORIZONTAL);
		s1->SetSize(100, 14);
		s1->SetDataRange(0.0, 1.0);
		s1->SetValue(1.0);
		layoutMain->Add(s1);
		s1->m_callback = &MainUI::SliderMoved;
	}
	
  layoutMain->Space(20);
			
	// Bases
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleBasesLayer;
		Switch* m_switch5 = new Switch(buttonDecl);
		layoutCol2->Add(m_switch5);
		m_switch5->SetText("Bases", 0xff000000);
		m_switch5->SetToggle(false);
	}

  layoutCol2->Space(20);
	
	// Photon Overcharge
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::TogglePhotonOverchargeLayer;
		Switch* m_switch6 = new Switch(buttonDecl);
		layoutCol2->Add(m_switch6);
		m_switch6->SetText("Photon Overcharge", 0xff000000);
		m_switch6->SetToggle(false);
	}

  layoutCol2->Space(20);
	
	// Siege Maps
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleBlinkMap;
		MainUI::m_switchSiegeBlink = new Switch(buttonDecl);
		layoutCol2->Add(MainUI::m_switchSiegeBlink);
		MainUI::m_switchSiegeBlink->SetText("Blink Map", 0xff000000);
		MainUI::m_switchSiegeBlink->SetToggle(false);
	}
	
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleTankMap;
		MainUI::m_switchSiegeTank = new Switch(buttonDecl);
		layoutCol2->Add(MainUI::m_switchSiegeTank);
		MainUI::m_switchSiegeTank->SetText("Siege Map", 0xff000000);
		MainUI::m_switchSiegeTank->SetToggle(false);
	}
	
	{
		s2 = new Slider(Slider::HORIZONTAL);
		s2->SetSize(100, 14);
		s2->SetDataRange(0.0, 1.0);
		s2->SetValue(1.0);
		layoutCol2->Add(s2);
		s2->m_callback = &MainUI::SliderMoved;
	}
	
  layoutCol2->Space(20);

	// Reload config
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ReloadConfig;
		Button* m_configReload = new Button(buttonDecl);
		layoutMain->Add(m_configReload);
		m_configReload->SetText("Reload Config", 0xff000000);
		m_configReload->SetSize(182, 28);
	}

	// Load
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::LoadMap;
		Button* m_buttonLoad = new Button(buttonDecl);
		layoutMain->Add(m_buttonLoad);
		m_buttonLoad->SetText("Load Map...", 0xff000000);
		m_buttonLoad->SetSize(182, 28);
	}

	// Save
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::SaveImage;
		Button* m_buttonSave = new Button(buttonDecl);
		layoutMain->Add(m_buttonSave);
		m_buttonSave->SetText("Save Image...", 0xff000000);
		m_buttonSave->SetSize(182, 28);
	}	

  // Pathing Layer
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::TogglePathingLayer;
		Switch* m_switchPath = new Switch(buttonDecl);
		pagePath->Add(m_switchPath);
		m_switchPath->SetText("Pathing", 0xff000000);
		m_switchPath->SetToggle(false);

    m_switchPath->SetPosition(16, 60);
	}

  {
    m_basePathStart = new Combo();
    pagePath->Add(m_basePathStart);

    m_basePathStart->SetCallback(new TCallback<Controller>(this, &Controller::PathCallback1));
    m_basePathStart->SetPosition(16, 90);
    m_basePathStart->SetSize(68, 22); 
  }

  {
    Combo* combo = new Combo();
    pagePath->Add(combo);

    combo->AddItem("main");
    combo->AddItem("nat");
    combo->AddItem("3rd");

    combo->Select(0);

    combo->SetCallback(new TCallback<Controller>(this, &Controller::PathCallback2));
    combo->SetPosition(78, 90);
  }

  {
    Label* label = new Label();
    label->SetText("to", 0xff000000);
    label->SetPosition(124, 90 + 9);
    pagePath->Add(label);
  }

  {
    m_basePathEnd = new Combo();
    pagePath->Add(m_basePathEnd);

    m_basePathEnd->SetCallback(new TCallback<Controller>(this, &Controller::PathCallback3));
    m_basePathEnd->SetPosition(140, 90);
    m_basePathEnd->SetSize(68, 22);
  }

  {
    Combo* combo = new Combo();
    pagePath->Add(combo);

    combo->AddItem("main");
    combo->AddItem("nat");
    combo->AddItem("3rd");

    combo->Select(0);

    combo->SetCallback(new TCallback<Controller>(this, &Controller::PathCallback4));
    combo->SetPosition(202, 90);
  }



  {
    Tab* tab = new Tab();

    tab->AddTab("Controls", page);
    tab->AddTab("Path", pagePath);
    tab->AddTab("Log", page2);

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


float Controller::GetAlpha1()
{
  return s0->GetValue();
}


float Controller::GetAlpha2()
{
  return s1->GetValue();
}


float Controller::GetAlpha3()
{
  return s2->GetValue();
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

  for (std::list<Base*>::iterator i = a_map->bases.begin(); i != a_map->bases.end(); ++i)
	{
    if ((*i)->numExpo == 1)
    {
      m_basePathStart->AddItem( (*i)->sl->name );
      m_basePathEnd->AddItem( (*i)->sl->name );
    }
  }

  m_basePathStart->Select(0);
  m_pathSpawnA = 0;

  m_basePathEnd->Select(1);
  m_pathSpawnB = 1;

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


void Controller::OnWindowSizeChanged(int a_width, int a_height)
{
  m_log->SetSize(240, a_height - 72);
  m_core->OnClientAreaChanged(a_width, a_height);
}
