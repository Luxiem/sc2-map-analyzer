#include "Controller.h"
#include "Common.h" // for IncPath


void SaveImageFile();
void LoadMapFile();


namespace MainUI
{
	bool s_showResourceLayer = false;
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
	
	
	static void PathingNext(void*, UV::EventArgs&)
	{
		Common::IncPath(1);
		s_redraw = true;
	}
	
	
	static void PathingPrev(void*, UV::EventArgs&)
	{
		Common::IncPath(-1);
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

	using namespace UV;
	
	VStack* layoutMain = new VStack();
	page->Add(layoutMain);
	layoutMain->SetPosition(16, 40);
	
	// Overlay Layer
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = MainUI::ToggleOverlayLayer;
		Switch* m_switch1 = new Switch(buttonDecl);
		layoutMain->Add(m_switch1);
		m_switch1->SetText("Watchtowers", 0xff000000);
	}
	
	
	// Height Layer
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleHeightLayer;
		Switch* m_switch2 = new Switch(buttonDecl);
		layoutMain->Add(m_switch2);
		m_switch2->SetText("Height", 0xff000000);
		
		s0 = new Slider(Slider::HORIZONTAL);
		s0->SetSize(200, 14);
		s0->SetDataRange(0.0, 1.0);
		s0->SetValue(1.0);
		layoutMain->Add(s0);
		s0->m_callback = &MainUI::SliderMoved;
	}
	
	// Openness Layer
	{
		Declaration buttonDecl;
		buttonDecl.m_commandPressed = &MainUI::ToggleOpennessLayer;
		Switch* m_switch3 = new Switch(buttonDecl);
		layoutMain->Add(m_switch3);
		m_switch3->SetText("Openness", 0xff000000);
		
		s1 = new Slider(Slider::HORIZONTAL);
		s1->SetSize(200, 14);
		s1->SetDataRange(0.0, 1.0);
		s1->SetValue(1.0);
		layoutMain->Add(s1);
		s1->m_callback = &MainUI::SliderMoved;
	}
	Switch* m_switch4 = 0;
	Switch* m_switchPath = 0;
	
	// Resource
	{
		Declaration buttonDecl;
		buttonDecl.VMargin = 160;
		buttonDecl.HMargin = 16;
		buttonDecl.m_commandPressed = &MainUI::ToggleResourceLayer;
		m_switch4 = new Switch(buttonDecl);
		layoutMain->Add(m_switch4);
		m_switch4->SetText("Destructible", 0xff000000);
		m_switch4->SetToggle(false);
	}
	
	// Pathing Layer
	{
		Declaration buttonDecl;
		buttonDecl.VMargin = 200;
		buttonDecl.HMargin = 16;
		buttonDecl.m_commandPressed = &MainUI::TogglePathingLayer;
		m_switchPath = new Switch(buttonDecl);
		layoutMain->Add(m_switchPath);
		m_switchPath->SetText("Pathing", 0xff000000);
		m_switchPath->SetToggle(false);
	}
	
	{
		Declaration buttonDecl;
		buttonDecl.VMargin = 230;
		buttonDecl.HMargin = 156;
		buttonDecl.m_commandPressed = &MainUI::PathingPrev;
		Button* m_buttonPathPrev = new Button(buttonDecl);
		layoutMain->Add(m_buttonPathPrev);
		m_buttonPathPrev->SetText("<", 0xff000000);
		m_buttonPathPrev->SetSize(20, 20);
	}
	
	{
		Declaration buttonDecl;
		buttonDecl.VMargin = 230;
		buttonDecl.HMargin = 184;
		buttonDecl.m_commandPressed = &MainUI::PathingNext;
		Button* m_buttonPathNext = new Button(buttonDecl);
		layoutMain->Add(m_buttonPathNext);
		m_buttonPathNext->SetText(">", 0xff000000);
		m_buttonPathNext->SetSize(20, 20);
	}
	
	// Bases
	{
		Declaration buttonDecl;
		buttonDecl.VMargin = 240;
		buttonDecl.HMargin = 16;
		buttonDecl.m_commandPressed = &MainUI::ToggleBasesLayer;
		Switch* m_switch5 = new Switch(buttonDecl);
		layoutMain->Add(m_switch5);
		m_switch5->SetText("Bases", 0xff000000);
		m_switch5->SetToggle(false);
	}
	
	// Photon Overcharge
	{
		Declaration buttonDecl;
		buttonDecl.VMargin = 280;
		buttonDecl.HMargin = 16;
		buttonDecl.m_commandPressed = &MainUI::TogglePhotonOverchargeLayer;
		Switch* m_switch6 = new Switch(buttonDecl);
		layoutMain->Add(m_switch6);
		m_switch6->SetText("Photon Overcharge", 0xff000000);
		m_switch6->SetToggle(false);
	}

  // LoS
	{
		Declaration buttonDecl;
		buttonDecl.VMargin = 0;
		buttonDecl.HMargin = 0;
		buttonDecl.m_commandPressed = &MainUI::ToggleLoSLayer;
		Switch* m_switchLoS = new Switch(buttonDecl);
		layoutMain->Add(m_switchLoS);
		m_switchLoS->SetText("LoS Blocker", 0xff000000);
		m_switchLoS->SetToggle(true);
	}
	
	// Siege Maps
	{
		Declaration buttonDecl;
		buttonDecl.VMargin = 360;
		buttonDecl.HMargin = 16;
		buttonDecl.m_commandPressed = &MainUI::ToggleBlinkMap;
		MainUI::m_switchSiegeBlink = new Switch(buttonDecl);
		layoutMain->Add(MainUI::m_switchSiegeBlink);
		MainUI::m_switchSiegeBlink->SetText("Blink Map", 0xff000000);
		MainUI::m_switchSiegeBlink->SetToggle(false);
	}
	
	{
		Declaration buttonDecl;
		buttonDecl.VMargin = 400;
		buttonDecl.HMargin = 16;
		buttonDecl.m_commandPressed = &MainUI::ToggleTankMap;
		MainUI::m_switchSiegeTank = new Switch(buttonDecl);
		layoutMain->Add(MainUI::m_switchSiegeTank);
		MainUI::m_switchSiegeTank->SetText("Siege Map", 0xff000000);
		MainUI::m_switchSiegeTank->SetToggle(false);
	}
	
	{
		s2 = new Slider(Slider::HORIZONTAL);
		s2->SetSize(200, 14);
		s2->SetDataRange(0.0, 1.0);
		s2->SetValue(1.0);
		layoutMain->Add(s2);
		s2->m_callback = &MainUI::SliderMoved;
	}
	
	// Reload config
	{
		Declaration buttonDecl;
		buttonDecl.VMargin = 458;
		buttonDecl.HMargin = 16;
		buttonDecl.m_commandPressed = &MainUI::ReloadConfig;
		Button* m_configReload = new Button(buttonDecl);
		page->Add(m_configReload);
		m_configReload->SetText("Reload Config", 0xff000000);
		m_configReload->SetSize(182, 28);
	}

	// Load
	{
		Declaration buttonDecl;
		buttonDecl.VMargin = 494;
		buttonDecl.HMargin = 16;
		buttonDecl.m_commandPressed = &MainUI::LoadMap;
		Button* m_buttonLoad = new Button(buttonDecl);
		page->Add(m_buttonLoad);
		m_buttonLoad->SetText("Load Map...", 0xff000000);
		m_buttonLoad->SetSize(182, 28);
	}

	// Save
	{
		Declaration buttonDecl;
		buttonDecl.VMargin = 530;
		buttonDecl.HMargin = 16;
		buttonDecl.m_commandPressed = &MainUI::SaveImage;
		Button* m_buttonSave = new Button(buttonDecl);
		page->Add(m_buttonSave);
		m_buttonSave->SetText("Save Image...", 0xff000000);
		m_buttonSave->SetSize(182, 28);
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
