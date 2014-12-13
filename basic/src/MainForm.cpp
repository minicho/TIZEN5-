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

#include "tabTestFormFactory.h"
#include "AppResourceId.h"
#include "MainForm.h"



MainForm::MainForm(void)

{

}

MainForm::~MainForm(void)
{
}

bool
MainForm::Initialize(void)
{
	result r = Form::Construct(IDF_MAINFORM);
	//result r = Form::Construct(ID_FORM_CALENDAR3);
	TryReturn(r == E_SUCCESS, false, "Failed to construct form");

	return true;

}
/*
result
MainForm::CreateImagePanel(void)
{
	ImagePanel* pImagePanel = new (std::nothrow) ImagePanel();
	result r = pImagePanel->Initialize(Rectangle(0, 0, GetClientAreaBounds().width, GetClientAreaBounds().height));
	AddOrientationEventListener(*pImagePanel);
	r = AddControl(pImagePanel);
	__pPanel[2] = pImagePanel;

	return r;
}
*/

result
MainForm::OnInitializing(void)
{
/*
	result r = E_SUCCESS;

	Rectangle clientRect = GetClientAreaBounds();
	Rectangle rect(0, 0, clientRect.width, clientRect.height);


	rtScreen = GetClientAreaBounds();

	// Create header
	Header* pHeader = GetHeader();
	if (pHeader != null)
	{
		pHeader->SetStyle(HEADER_STYLE_TAB);

		HeaderItem headerItem1;
		headerItem1.Construct(ID_HEADER_ITEM1);
		headerItem1.SetText(L"Button");
		pHeader->AddItem(headerItem1);

		HeaderItem headerItem2;
		headerItem2.Construct(ID_HEADER_ITEM2);
		headerItem2.SetText(L"Calendar");
		pHeader->AddItem(headerItem2);

		HeaderItem headerItem3;
		headerItem3.Construct(ID_HEADER_ITEM3);
		headerItem3.SetText(L"Chat");
		pHeader->AddItem(headerItem3);

		pHeader->AddActionEventListener(*this);
		pHeader->SetItemSelected(2);
	}

	SetFormBackEventListener(this);

	// Create the Button panel
	ButtonPanel* pButtonPanel = new (std::nothrow) ButtonPanel();
	pButtonPanel->Initialize(rect);
	AddControl(pButtonPanel);
	__pPanel[0] = pButtonPanel;



	//__pPanel[1] = static_cast<Panel *>(GetControl(IDC_ORIENTATIONPANEL));

	__pPanel[1] = static_cast<Panel *>(GetControl(IDC_PANEL2));
	if (__pPanel[1] != null)
	{
		__pButtonOrientation  = static_cast<Button *>(GetControl(IDC_BUTTON44, true));
		if (__pButtonOrientation != null)
		{
			__pButtonOrientation->SetActionId(ID_ORIENTATION);
			__pButtonOrientation->AddActionEventListener(*this);
		}

		DateTime CurDateTime;

		r = SystemTime::GetCurrentTime(CurDateTime);
		TryReturn(!IsFailed(r), r, "SystemTime::GetCurrentTime() failed with [%s]", GetErrorMessage(r));

		r = CurDateTime.SetValue(CurDateTime.GetYear(), CurDateTime.GetMonth(), 1);
		TryReturn(!IsFailed(r), r, "DateTime::SetValue() failed with [%s]", GetErrorMessage(r));

		__pSelectCalendar = Tizen::Locales::Calendar::CreateInstanceN();
		TryReturn(__pSelectCalendar != null, E_SYSTEM, "Unable to get Create Instance");

		r = __pSelectCalendar->SetTime(CurDateTime);
		TryReturn(!IsFailed(r), r, "Calendar::SetTime() failed with [%s]", GetErrorMessage(r));

		AppLog("GetCurrentTime : %d/%d/%d"
			  , __pSelectCalendar->GetTimeField(TIME_FIELD_YEAR)
			  , __pSelectCalendar->GetTimeField(TIME_FIELD_MONTH)
			  , __pSelectCalendar->GetTimeField(TIME_FIELD_DAY_OF_MONTH));


		r = InitializeActionControls();
		TryReturn(!IsFailed(r), r, "InitializeActionControls() failed with [%s]", GetErrorMessage(r));

		r = InitializeDayNumberButtons();
		TryReturn(!IsFailed(r), r, "InitializeDayNumberButtons() failed with [%s]", GetErrorMessage(r));

		TryReturn(UpdateCalendarDisplay() == true, E_SYSTEM, "UpdateCalendarDisplay() failed");

	}


	// Set the current panel as selected in the header and display it on the form
	if (pHeader)
	{
		pHeader->SetItemSelected(__panelId);

		if(__panelId == 2)
		{
			if (__pPanel[2] == null)
			{
				CreateImagePanel();
			}
			SetOrientation(ORIENTATION_AUTOMATIC);
		}

		if (__pPanel[0] != null)
		{
			__pPanel[0]->SetShowState(false);
		}
		if (__pPanel[1] != null)
		{
			__pPanel[1]->SetShowState(false);
		}

		__pPanel[__panelId]->SetShowState(true);
	}


	//return E_SUCCESS;


	Invalidate(true);
	return r;
*/
	rtScreen = GetClientAreaBounds();
	result r = E_SUCCESS;

	Header* pHeader = GetHeader();
	if (pHeader != null)
	{
		pHeader->SetStyle(HEADER_STYLE_TAB);

		HeaderItem headerItem1;
		headerItem1.Construct(ID_HEADER_ITEM1);
		headerItem1.SetText(L"날씨/지도");
		pHeader->AddItem(headerItem1);

		HeaderItem headerItem2;
		headerItem2.Construct(ID_HEADER_ITEM2);
		headerItem2.SetText(L"일정 등록");
		pHeader->AddItem(headerItem2);

		HeaderItem headerItem3;
		headerItem3.Construct(ID_HEADER_ITEM3);
		headerItem3.SetText(L"일정 보기");
		pHeader->AddItem(headerItem3);

		pHeader->AddActionEventListener(*this);
		pHeader->SetItemSelected(2);
	}

	SetFormBackEventListener(this);

	return r;
}

void
MainForm::webLink(int id){

	rtWeb = rtScreen;
	rtWeb.y = 85;
	__pWeb = new Web();
	__pWeb->Construct(rtWeb);
	AddControl(*__pWeb);
	__pWeb->SetLoadingListener(this);
	switch(id)
	{
	case ID_HEADER_ITEM1:
		__pWeb->LoadUrl(L"http://tmap.dothome.co.kr/index2.html");
		break;
	case ID_HEADER_ITEM2:
		__pWeb->LoadUrl(L"https://www.google.com/calendar/render?action=TEMPLATE&text=&dates=20141211T050001Z/20141211T060001Z&details=&location=&sprop=&sprop=name");
		break;
	case ID_HEADER_ITEM3:
		__pWeb->LoadUrl(L"http://tmap.dothome.co.kr/calendar.html");
		break;
	default:
		break;
	}
}

result
MainForm::OnTerminating(void)
{
	result r = E_SUCCESS;

	return r;
}

void
MainForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

    ArrayList* pList = new (std::nothrow) ArrayList;
    AppAssert(pList);

	switch(actionId)
	{
	case ID_HEADER_ITEM1:
		webLink(actionId);
		//pSceneManager->GoForward(ForwardSceneTransition(SCENE_CALENDAR_FORM), pList);
		//pHeader->SetItemSelected(0);

		break;
	case ID_HEADER_ITEM2:
		webLink(actionId);
		//pSceneManager->GoForward(ForwardSceneTransition(SCENE_CHATMAIN), pList);
		//pHeader->SetItemSelected(1);
		break;
	case ID_HEADER_ITEM3:
		webLink(actionId);
		break;
	default:
		break;
	}
}

void
MainForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	 SceneManager* pSceneManager = SceneManager::GetInstance();
	 AppAssert(pSceneManager);

	 pSceneManager->GoBackward(BackwardSceneTransition(SCENE_CALENDAR_FORM));

}

bool
MainForm::OnHttpAuthenticationRequestedN (const Tizen::Base::String &host, const Tizen::Base::String &realm,
		const Tizen::Web::Controls::AuthenticationChallenge &authentication)
{
	return false;
}

void
MainForm::OnLoadingCompleted(void)
{
}

void
MainForm::OnLoadingErrorOccurred (LoadingErrorType error, const Tizen::Base::String &reason)
{
}

bool
MainForm::OnLoadingRequested (const Tizen::Base::String &url, WebNavigationType type)
{
	return false;
}

DecisionPolicy
MainForm::OnWebDataReceived (const Tizen::Base::String &mime, const Tizen::Net::Http::HttpHeader &httpHeader)
{
	return WEB_DECISION_CONTINUE;
}

void
MainForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                       const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
	AppLog("tab3!!");
}

void
MainForm::OnSceneDeactivated(const SceneId& currentSceneId, const SceneId& nextSceneId)
{
	AppLog("tab3!!");
}
