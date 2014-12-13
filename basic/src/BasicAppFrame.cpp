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

#include "BasicAppFrame.h"
#include "tabTestFormFactory.h"
#include "tabTestPanelFactory.h"
#include "AppResourceId.h"
#include "CalendarViewerForm.h"

using namespace Tizen::Base;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;


BasicAppFrame::BasicAppFrame(void)
	: __panelId(0)
{
}

BasicAppFrame::~BasicAppFrame(void)
{
}

result
BasicAppFrame::OnInitializing(void)
{
	// Create a form
/*	MainForm* pMainForm = new (std::nothrow) MainForm();
	result r = pMainForm->Initialize(__panelId);

	// Add the form to the frame
	AddControl(pMainForm);

	// Set the current form
	SetCurrentForm(pMainForm);

	// Draw the form
	pMainForm->Invalidate(true);

	return r;
*/


	// Prepare Scene management.
	SceneManager* pSceneManager = SceneManager::GetInstance();
	static tabTestFormFactory formFactory;
	static tabTestPanelFactory panelFactory;
	pSceneManager->RegisterFormFactory(formFactory);
	pSceneManager->RegisterPanelFactory(panelFactory);
	//pSceneManager->RegisterScene(L"workflow");

	pSceneManager->RegisterScene(SCENE_CALENDAR_FORM, FORM_CALENDAR, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_CHATMAIN, FORM_CHATMAIN, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_CHAT, FORM_CHAT, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_WEBLINK,FORM_WEBLINK, PANEL_BLANK);

	pSceneManager->RegisterScene(SCENE_EVENT_LIST, FORM_EVENT_LIST, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_EVENT_DETAIL, FORM_EVENT_DETAIL, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_EVENT_CREATION, FORM_EVENT_CREATION, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_EVENT_EDITION, FORM_EVENT_EDITION, PANEL_BLANK);
	pSceneManager->RegisterScene(SCENE_EVENT_SETRECURRENCE, FORM_EVENT_RECURRENCE, PANEL_BLANK);

	// Go to the scene.
	// result r = pSceneManager->GoForward(ForwardSceneTransition(SCENE_CALENDAR_FORM));
	result r = pSceneManager->GoForward(ForwardSceneTransition(SCENE_CALENDAR_FORM));
	// TODO: Add your frame initialization code here.
	return r;

}

result
BasicAppFrame::OnTerminating(void)
{
	result r = E_SUCCESS;

	return r;
}
