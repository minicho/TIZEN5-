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

#include "tabTestFormFactory.h"
#include "AppResourceId.h"
#include "CalendarViewerForm.h"



CalendarViewerForm::CalendarViewerForm(void)
	: __pSelectCalendar(null)
{

}

CalendarViewerForm::~CalendarViewerForm(void)
{
}

bool
CalendarViewerForm::Initialize(void)
{
	result r = Construct(ID_FORM_CALENDAR3);
	TryReturn(!IsFailed(r), false, "CalendarViewerForm::Initialize() failed with [%s]", GetErrorMessage(r));

	return true;
}

result
CalendarViewerForm::InitializeActionControls(void)
{
	String btnName [] = {IDC_BTN_PRV_YEAR, IDC_BTN_PRV_MONTH, IDC_BTN_NEXT_MONTH, IDC_BTN_NEXT_YEAR};
	Button* pButton = null;
	for (int i = 0; i < ACTION_BUTTON_COUNT; i++)
	{
		pButton = dynamic_cast<Button*> (GetControl(btnName[i]));
		TryReturn(pButton != null, E_SYSTEM, "Unable to get control [%ls]", btnName[i].GetPointer());

		pButton->SetActionId(ID_BUTTON_PREV_YEAR_ID + i);
		pButton->AddActionEventListener(*this);
	}

	String lblStr [] = {IDC_EDIT_FIELD_YEAR, IDC_EDIT_FIELD_MONTH};

	String value [] = {L"", L""};
	value[0].Append(__pSelectCalendar->GetTimeField(TIME_FIELD_YEAR));
	value[1].Append(__pSelectCalendar->GetTimeField(TIME_FIELD_MONTH));

	result r = E_SUCCESS;
	for (int i = 0; i < ACTION_LABEL_COUNT; i++)
	{
		__pEditFields[i] = dynamic_cast<EditField*> (GetControl(lblStr[i]));
		TryReturn(__pEditFields[i] != null, E_SYSTEM, "Unable to get control [%ls]", lblStr[i].GetPointer());

		r = __pEditFields[i]->SetText(value[i]);
		TryReturn(!IsFailed(r), r, "[%s] EditField::SetText() failed for [%ls]", GetErrorMessage(r), lblStr[i].GetPointer());

		__pEditFields[i]->AddTextEventListener(*this);
	}

	return E_SUCCESS;
}

result
CalendarViewerForm::InitializeDayNumberButtons(void)
{
	int plus = 110;
	for (int i = 0; i < DAY_TEXTBOX_COUNT; i++)
	{
		String name(L"IDC_BUTTON");
		//String name(L"IDC_")
		name.Append(i+1);
		Control* pControl = GetControl(name);
		pControl->SetEnabled(true);
		//__pButtns[i] = dynamic_cast<TextBox*>(pControl);
		__pButtons[i] = dynamic_cast<Button*>(pControl);
		TryReturn(__pButtons[i] != null, E_SYSTEM, "Unable to get control [%ls]", name.GetPointer());
		__pButtons[i]->SetActionId(plus + i);
		__pButtons[i]->AddActionEventListener(*this);
		//Tizen::Graphics::Color setColor;
		//setColor = __pButtons[i]->GetTextColor(TEXT_BOX_TEXT_COLOR_NORMAL);
		//__pButtons[i]->SetTextColor(TEXT_BOX_TEXT_COLOR_DISABLED ,setColor);
	}
	return E_SUCCESS;
}

result
CalendarViewerForm::OnInitializing(void)
{

	result r = E_SUCCESS;

	Header* pHeader = GetHeader();
	if (pHeader != null)
	{
		pHeader->SetStyle(HEADER_STYLE_TAB);

		HeaderItem headerItem1;
		headerItem1.Construct(ID_HEADER_ITEM1);
		headerItem1.SetText("캘린더");
		pHeader->AddItem(headerItem1);

		HeaderItem headerItem2;
		headerItem2.Construct(ID_HEADER_ITEM2);
		headerItem2.SetText("채팅");
		pHeader->AddItem(headerItem2);

		HeaderItem headerItem3;
		headerItem3.Construct(ID_HEADER_ITEM3);
		headerItem3.SetText("더보기");
		pHeader->AddItem(headerItem3);


		pHeader->AddActionEventListener(*this);
		r = pHeader->SetItemSelected(0);
	}


	//DateTime CurDateTime;

	SetFormBackEventListener(this);

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
	//pHeader->SetItemSelected(0);
	return E_SUCCESS;
}

result
CalendarViewerForm::OnTerminating(void)
{
	delete __pSelectCalendar;
	return E_SUCCESS;
}

bool
CalendarViewerForm::UpdateCalendarDisplay(void)
{
	DateTime oldValue = __pSelectCalendar->GetTime();
	result r = E_SUCCESS;
	for (int i = 0; i < DAY_TEXTBOX_COUNT; i++)
	{
		//r =
		__pButtons[i]->SetShowState(false);
		//TryReturn(!IsFailed(r), false, "Button::SetShowState() failed with [%s]", GetErrorMessage(r));
	}

	r = __pSelectCalendar->SetTimeField(TIME_FIELD_DAY_OF_MONTH, 1);
	int offset = __pSelectCalendar->GetTimeField(TIME_FIELD_DAY_OF_WEEK) - 1;
	int date = __pSelectCalendar->GetTimeField(TIME_FIELD_DAY_OF_MONTH);

	int i = offset;
	do
	{
		String value(L"");
		value.Append(date);
		__pButtons[i]->SetText(value);

		//r =
		__pButtons[i]->SetShowState(true);
		TryReturn(!IsFailed(r), false, "Button::SetShowState() failed with [%s]", GetErrorMessage(r));

		r = __pSelectCalendar->AddTimeField(TIME_FIELD_DAY_OF_MONTH, 1);
		TryReturn(!IsFailed(r), false, "Calendar::AddTimeField failed with [%s]", GetErrorMessage(r));

		date = __pSelectCalendar->GetTimeField(TIME_FIELD_DAY_OF_MONTH);

		i++;
	} while (date != 1 && i < DAY_TEXTBOX_COUNT);

	__pSelectCalendar->SetTime(oldValue);

	String year = L"";
	year.Append(__pSelectCalendar->GetTimeField(TIME_FIELD_YEAR));
	__pEditFields[0]->SetText(year);

	String month = L"";
	month.Append(__pSelectCalendar->GetTimeField(TIME_FIELD_MONTH));
	__pEditFields[1]->SetText(month);

	SetFocus();
	RequestRedraw(true);

	return true;
}

void
CalendarViewerForm::OnActionPerformed(const Control& source, int actionId)
{
	int year = __pSelectCalendar->GetTimeField(TIME_FIELD_YEAR);
	int month = __pSelectCalendar->GetTimeField(TIME_FIELD_MONTH);
	String s = "a";


	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);


    ArrayList* pList = new (std::nothrow) ArrayList;
    AppAssert(pList);

    //pList->Add(*(new (std::nothrow) String(L"calendar")));
    //Header* pHeader = GetHeader();

	switch(actionId)
	{
	case ID_HEADER_ITEM1:
		//pSceneManager->GoForward(ForwardSceneTransition(SCENE_CALENDAR_FORM), pList);
		//pHeader->SetItemSelected(0);

		break;
	case ID_HEADER_ITEM2:
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_CHATMAIN), pList);
		//pHeader->SetItemSelected(1);
		break;
	case ID_HEADER_ITEM3:
		//webLink(actionId);
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_WEBLINK), pList);
		break;

    //pSceneManager->GoForward(ForwardSceneTransition(SCENE_CHAT_MENU), pList);
	case ID_BUTTON_PREV_MONTH_ID:
		TryReturnVoid((year > 1) || (month > 1), "At first page");
		__pSelectCalendar->AddTimeField(TIME_FIELD_MONTH, -1);
		break;

	case ID_BUTTON_NEXT_MONTH_ID:
		TryReturnVoid((year < 9999) || (month < 12), "At last page");
		__pSelectCalendar->AddTimeField(TIME_FIELD_MONTH, 1);
		break;

	case ID_BUTTON_PREV_YEAR_ID:
		TryReturnVoid(year > 1, "At starting year");
		__pSelectCalendar->AddTimeField(TIME_FIELD_YEAR, -1);
		break;

	case ID_BUTTON_NEXT_YEAR_ID:
		TryReturnVoid(year < 9999, "At starting year");
		__pSelectCalendar->AddTimeField(TIME_FIELD_YEAR, 1);
		break;

	default:
		break;
	}
	if(actionId >= 110 && actionId < 200){
		AppLog("actionId : %d", actionId);
		//method of web link, hyperText!
		//webLink(actionId);
		//pSceneManager->GoForward(ForwardSceneTransition(SCENE_LIST), pList);
		//pList->Add(*(new (std::nothrow) String()));
		//Tizen::Base::Object::_Exit(1);
		//pList->Add(*(new (std::nothrow) DateTime(CurDateTime)));
		pList->Add(*__pSelectCalendar);
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_EVENT_LIST), pList);
		pList->RemoveAll();
		delete pList;
	}

	UpdateCalendarDisplay();

//CATCH:
//	if (pList->GetCount() != 0)
	//{
	//	pList->RemoveAll(true);
	//	delete pList;
		//pList = null;
	//}
}

void
CalendarViewerForm::OnTextValueChanged(const Control& source)
{
	int newValue = 0;
	result r = E_SUCCESS;
	const Control* pObject = &source;
	if (pObject == __pEditFields[0])
	{
		r = Integer::Parse(__pEditFields[0]->GetText(), newValue);
		if ((!IsFailed(r)) && (newValue > 0) && (newValue <= 9999))
		{
			__pSelectCalendar->SetTimeField(TIME_FIELD_YEAR, newValue);
		}
	}
	else if (pObject == __pEditFields[1])
	{
		r = Integer::Parse(__pEditFields[1]->GetText(), newValue);
		if ((!IsFailed(r)) && (newValue > 0) && (newValue <= 12))
		{
			__pSelectCalendar->SetTimeField(TIME_FIELD_MONTH, newValue);
		}
	}

	UpdateCalendarDisplay();
}
/*
void
CalendarViewerForm::webLink(int id){
	rtWeb = rtScreen;
	rtWeb.y = 115;
	__pWeb = new Web();
	__pWeb->Construct(rtWeb);
	AddControl(*__pWeb);
	__pWeb->SetLoadingListener(this);
	__pWeb->LoadUrl(L"http://tmap.dothome.co.kr/weathermap.html");
}
*/
/*
bool
CalendarViewerForm::OnHttpAuthenticationRequestedN (const Tizen::Base::String &host, const Tizen::Base::String &realm,
		const Tizen::Web::Controls::AuthenticationChallenge &authentication)
{
	return false;
}

void
CalendarViewerForm::OnLoadingCompleted(void)
{
}

void
CalendarViewerForm::OnLoadingErrorOccurred (LoadingErrorType error, const Tizen::Base::String &reason)
{
}

bool
CalendarViewerForm::OnLoadingRequested (const Tizen::Base::String &url, WebNavigationType type)
{
	return false;
}

DecisionPolicy
CalendarViewerForm::OnWebDataReceived (const Tizen::Base::String &mime, const Tizen::Net::Http::HttpHeader &httpHeader)
{
	return WEB_DECISION_CONTINUE;
}
*/
void
CalendarViewerForm::OnTextValueChangeCanceled(const Tizen::Ui::Control& source)
{
	SetFocus();
}
void
CalendarViewerForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	UiApp* pApp = UiApp::GetInstance();
	AppAssert(pApp);
	pApp->Terminate();
}

void
CalendarViewerForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                       const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
	AppLog("calendar!!");
}

void
CalendarViewerForm::OnSceneDeactivated(const SceneId& currentSceneId, const SceneId& nextSceneId)
{
	AppLog("calendar!!");
}
