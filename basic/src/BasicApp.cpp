//
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
//
// Licensed under the Flora License, Version 1.1 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://floralicense.org/license/
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <new>
#include "BasicApp.h"
#include "BasicAppFrame.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::System;
using namespace Tizen::Ui::Controls;

BasicApp::BasicApp(void)
{
}

BasicApp::~BasicApp(void)
{
}

Application*
BasicApp::CreateInstance(void)
{
	// Create the instance through the constructor.
	return new (std::nothrow) BasicApp();
}

bool
BasicApp::OnAppInitialized(void)
{
	// Create the application frame.
	BasicAppFrame* pBasicAppFrame = new (std::nothrow) BasicAppFrame;
	TryReturn(pBasicAppFrame != null, false, "The memory is insufficient.");
	pBasicAppFrame->Construct();
	pBasicAppFrame->SetName(L"BasicApp");
	AddFrame(*pBasicAppFrame);

	return true;
}

bool
BasicApp::OnAppInitializing(AppRegistry& appRegistry)
{
/*
	int lastPanelId = 1;
	String lastPanelName(L"");
	String panelIDkey(L"AppLastPanelId");
	String panelNamekey(L"AppLastPanelName");

	result r = appRegistry.Get(panelIDkey, lastPanelId);
	if (r == E_KEY_NOT_FOUND)
	{
		lastPanelId = 1;
		appRegistry.Add(panelIDkey, lastPanelId);
	}

	r = appRegistry.Get(panelNamekey, lastPanelName);
	if (r == E_KEY_NOT_FOUND)
	{
		appRegistry.Add(panelNamekey, L"Panel1");
	}

	// Create a Frame
	BasicAppFrame* pBasicAppFrame = new (std::nothrow) BasicAppFrame();
	pBasicAppFrame->Initialize(lastPanelId);
	pBasicAppFrame->SetName(L"BasicApp");
	AddFrame(*pBasicAppFrame);
*/
	return true;
}
bool
BasicApp::OnAppWillTerminate(void)
{
	// TODO: Deallocate or release resources in devices that have the END key.
	return true;
}

bool
BasicApp::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination)
{
	return true;
}

void
BasicApp::OnForeground(void)
{
}

void
BasicApp::OnBackground(void)
{
}

void
BasicApp::OnLowMemory(void)
{
}

void
BasicApp::OnBatteryLevelChanged(BatteryLevel batteryLevel)
{
}

void
BasicApp::OnScreenOn (void)
{
}

void
BasicApp::OnScreenOff (void)
{
}
