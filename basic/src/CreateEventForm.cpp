//
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
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
#include <FLocales.h>

#include "CreateEventForm.h"
#include "basicAppFrame.h"
#include "AppResourceId.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Graphics;
using namespace Tizen::Locales;
using namespace Tizen::Social;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

CreateEventForm::CreateEventForm()
	: __pCalendarbook(null)
	, __pCalendar(null)
	, __pRecurrence(null)
	, __selectedReminder(REMINDER_TIME_UNIT_MINUTE)
	, __selectedPriority(EVENT_PRIORITY_LOW)
	, __selectedSensitivity(SENSITIVITY_PUBLIC)
	, __selectedStatus(EVENT_STATUS_NONE)
	, __pSelectCalendarPopup(null)
{
}

CreateEventForm::~CreateEventForm()
{
}

bool
CreateEventForm::Initialize(void)
{
	result r = E_SUCCESS;

	r = Construct(FORM_STYLE_NORMAL | FORM_STYLE_FOOTER | FORM_STYLE_HEADER | FORM_STYLE_PORTRAIT_INDICATOR);
	TryReturn(!IsFailed(r), false, "[%s] Failed to construct the form.", GetErrorMessage(r));

	__pCalendarbook = new (std::nothrow) Calendarbook();
	TryReturn(__pCalendarbook != null, false, "Failed to instantiate calendarbook");

	r = __pCalendarbook->Construct();
	TryReturn(!IsFailed(r), false, "[%s] Failed to construct the calendarbook.", GetErrorMessage(r));

	return true;
}

result
CreateEventForm::OnInitializing(void)
{
	result r = E_SUCCESS;

	Header* pHeader = GetHeader();
	AppAssert(pHeader);
	pHeader->SetStyle(HEADER_STYLE_TITLE);
	pHeader->SetTitleText(L"일정 추가");

	Footer* pFooter = GetFooter();
	AppAssert(pFooter);
	pFooter->SetStyle(FOOTER_STYLE_BUTTON_TEXT);

	FooterItem footerSave;
	footerSave.Construct(ID_FOOTER_SAVE);
	footerSave.SetText(L"저장");
	pFooter->AddItem(footerSave);

	pFooter->AddActionEventListener(*this);
	SetFormBackEventListener(this);

	static const unsigned int COLOR_BACKGROUND_LABEL = 0xFFEFEDE5;
	static const unsigned int COLOR_TITLE_LABEL = 0xFF808080;
	static const unsigned int COLOR_TIMEZONE_DATA = 0xFF444444;

	static const int UI_X_POSITION_GAP = 20;
	static const int UI_WIDTH = GetClientAreaBounds().width - 40;
	static const int UI_X_POSITION_MIDDLE = UI_WIDTH / 4;
	static const int UI_HEIGHT = 112;
	static const int BUTTON_HEIGHT = 74;
	static const int UI_SPACE = 32;
	static const int FONT_SIZE = 36;

	int yPosition = 0;

	ScrollPanel* pScrollPanel = new (std::nothrow) ScrollPanel();
	pScrollPanel->Construct(Rectangle(0, 0, GetClientAreaBounds().width, GetClientAreaBounds().height));

	// Calendar
	Label* pCalendarLabel = new (std::nothrow) Label();
	pCalendarLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition, UI_WIDTH, UI_HEIGHT), L"달력 종류");
	pCalendarLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pCalendarLabel->SetTextColor(COLOR_TITLE_LABEL);
	pCalendarLabel->SetBackgroundColor(Color(COLOR_BACKGROUND_LABEL));
	pScrollPanel->AddControl(pCalendarLabel);

	__pSelectCalendarButton = new (std::nothrow) Button();
	__pSelectCalendarButton->Construct(Rectangle(UI_X_POSITION_MIDDLE + UI_X_POSITION_GAP, yPosition + 19, UI_WIDTH * 3 / 4, BUTTON_HEIGHT), L"Default event calendar");
	__pSelectCalendarButton->SetActionId(ID_BUTTON_CALENDAR);
	__pSelectCalendarButton->AddActionEventListener(*this);
	pScrollPanel->AddControl(__pSelectCalendarButton);

	// Subject
	__pSubjectEditField = new (std::nothrow) EditField();
	__pSubjectEditField->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT + UI_SPACE, UI_WIDTH, UI_HEIGHT), EDIT_FIELD_STYLE_NORMAL, INPUT_STYLE_FULLSCREEN, EDIT_FIELD_TITLE_STYLE_TOP);
	__pSubjectEditField->SetTitleText(L"제목");
	__pSubjectEditField->SetGuideText(L"");
	pScrollPanel->AddControl(__pSubjectEditField);


	int minYear = Calendarbook::GetMinDateTime().GetYear() + 1;
	int maxYear = Calendarbook::GetMaxDateTime().GetYear() - 1;

	// Start Date
	Label* pStartDateLabel = new (std::nothrow) Label();
	pStartDateLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT + UI_SPACE, UI_WIDTH, UI_HEIGHT), L"시작일");
	pStartDateLabel->SetTextVerticalAlignment(ALIGNMENT_TOP);
	pStartDateLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pStartDateLabel->SetTextColor(COLOR_TITLE_LABEL);
	pStartDateLabel->SetBackgroundColor(COLOR_BACKGROUND_LABEL);
	pScrollPanel->AddControl(pStartDateLabel);

	__pStartEditDate = new (std::nothrow) EditDate();
	__pStartEditDate->Construct(Point(UI_X_POSITION_GAP, yPosition + 10));
	__pStartEditDate->SetCurrentDate();
	__pStartEditDate->SetYearRange(minYear, maxYear);
	__pStartEditDate->AddDateChangeEventListener(*this);
	pScrollPanel->AddControl(__pStartEditDate);

	__pStartEditTime = new (std::nothrow) EditTime();
	__pStartEditTime->Construct(Point(UI_X_POSITION_MIDDLE * 2 + UI_SPACE, yPosition + 10));
	__pStartEditTime->SetCurrentTime();
	__pStartEditTime->AddTimeChangeEventListener(*this);
	pScrollPanel->AddControl(__pStartEditTime);

	// End Date
	Label* pEndDateLabel = new (std::nothrow) Label();
	pEndDateLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT, UI_WIDTH, UI_HEIGHT), L"종료일");
	pEndDateLabel->SetTextVerticalAlignment(ALIGNMENT_TOP);
	pEndDateLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pEndDateLabel->SetTextColor(COLOR_TITLE_LABEL);
	pEndDateLabel->SetBackgroundColor(COLOR_BACKGROUND_LABEL);
	pScrollPanel->AddControl(pEndDateLabel);

	__pEndEditDate = new (std::nothrow) EditDate();
	__pEndEditDate->Construct(Point(UI_X_POSITION_GAP, yPosition + 10));
	__pEndEditDate->SetCurrentDate();
	__pEndEditDate->SetYearRange(minYear, maxYear);
	__pEndEditDate->AddDateChangeEventListener(*this);
	pScrollPanel->AddControl(__pEndEditDate);

	DateTime endTime;
	endTime = __pStartEditTime->GetTime();
	endTime.AddHours(1);

	__pEndEditTime = new (std::nothrow) EditTime();
	__pEndEditTime->Construct(Point(UI_X_POSITION_MIDDLE * 2 + UI_SPACE, yPosition + 10));
	__pEndEditTime->SetTime(endTime);
	__pEndEditTime->AddTimeChangeEventListener(*this);
	pScrollPanel->AddControl(__pEndEditTime);

	// TimeZone
	Label* pTimeZoneLabel = new (std::nothrow) Label();
	pTimeZoneLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT, UI_WIDTH, UI_HEIGHT), L"타임존");
	pTimeZoneLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pTimeZoneLabel->SetTextColor(COLOR_TITLE_LABEL);
	pTimeZoneLabel->SetBackgroundColor(COLOR_BACKGROUND_LABEL);
	pScrollPanel->AddControl(pTimeZoneLabel);

	Label* pTimeZoneDataLabel = new (std::nothrow) Label();
	pTimeZoneDataLabel->Construct(Rectangle(UI_X_POSITION_GAP + UI_X_POSITION_MIDDLE, yPosition, UI_WIDTH * 3 / 4, UI_HEIGHT), L"");
	pTimeZoneDataLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pTimeZoneDataLabel->SetText(GetTimezoneString());
	pTimeZoneDataLabel->SetTextColor(COLOR_TIMEZONE_DATA);
	pTimeZoneDataLabel->SetTextConfig(FONT_SIZE, LABEL_TEXT_STYLE_NORMAL);
	pTimeZoneDataLabel->SetBackgroundColor(COLOR_BACKGROUND_LABEL);
	pScrollPanel->AddControl(pTimeZoneDataLabel);

	// All day Event
	__pIsAllDayButton = new (std::nothrow) CheckButton();
	__pIsAllDayButton->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT + UI_SPACE, UI_WIDTH, UI_HEIGHT), CHECK_BUTTON_STYLE_ONOFF_SLIDING, BACKGROUND_STYLE_DEFAULT, false, L"종일 반복");
	__pIsAllDayButton->SetActionId(ID_CHKBUTTON_ISALLDAY_CHECKED, ID_CHKBUTTON_ISALLDAY_UNCHECKED);
	__pIsAllDayButton->SetTextColor(COLOR_TITLE_LABEL);
	__pIsAllDayButton->AddActionEventListener(*this);
	pScrollPanel->AddControl(__pIsAllDayButton);

	// Location
	__pLocationEditField = new (std::nothrow) EditField();
	__pLocationEditField->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT + UI_SPACE, UI_WIDTH, UI_HEIGHT), EDIT_FIELD_STYLE_NORMAL, INPUT_STYLE_FULLSCREEN, EDIT_FIELD_TITLE_STYLE_TOP);
	__pLocationEditField->SetTitleText(L"위치");
	__pLocationEditField->SetGuideText(L"");
	pScrollPanel->AddControl(__pLocationEditField);

	// Description
	__pDescriptionEditField = new (std::nothrow) EditField();
	__pDescriptionEditField->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT, UI_WIDTH, UI_HEIGHT), EDIT_FIELD_STYLE_NORMAL, INPUT_STYLE_FULLSCREEN, EDIT_FIELD_TITLE_STYLE_TOP);
	__pDescriptionEditField->SetTitleText(L"설명");
	__pDescriptionEditField->SetGuideText(L"");
	pScrollPanel->AddControl(__pDescriptionEditField);

	// Reminder
	Label* pReminderLabel = new (std::nothrow) Label();
	pReminderLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT + UI_SPACE, UI_WIDTH, UI_HEIGHT), L"Reminder");
	pReminderLabel->SetTextColor(COLOR_TITLE_LABEL);
	pReminderLabel->SetBackgroundColor(Color(COLOR_BACKGROUND_LABEL));
	pReminderLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pScrollPanel->AddControl(pReminderLabel);

	__pReminderEditField = new (std::nothrow) EditField();
	__pReminderEditField->Construct(Rectangle(UI_X_POSITION_GAP + UI_X_POSITION_MIDDLE, yPosition, UI_WIDTH / 4, UI_HEIGHT), EDIT_FIELD_STYLE_NUMBER, INPUT_STYLE_FULLSCREEN, false, 5);
	__pReminderEditField->SetGuideText(L"0 ~ ");
	__pReminderEditField->SetName(L"알림");
	__pReminderEditField->SetTextAlignment(ALIGNMENT_CENTER);
	pScrollPanel->AddControl(__pReminderEditField);

	__pReminderContextButton = new (std::nothrow) Button();
	__pReminderContextButton->Construct(Rectangle(UI_X_POSITION_MIDDLE * 2 + UI_X_POSITION_GAP, yPosition + 19, UI_WIDTH / 2, BUTTON_HEIGHT), L"분");
	__pReminderContextButton->SetActionId(ID_BUTTON_REMINDER);
	__pReminderContextButton->AddActionEventListener(*this);
	pScrollPanel->AddControl(__pReminderContextButton);

	__pReminderContextMenu = new (std::nothrow) ContextMenu();
	__pReminderContextMenu->Construct(Point(UI_X_POSITION_GAP + UI_WIDTH * 3 / 4, yPosition + UI_HEIGHT * 2), CONTEXT_MENU_STYLE_LIST, CONTEXT_MENU_ANCHOR_DIRECTION_UPWARD);
	__pReminderContextMenu->AddItem(L"분", ID_CONTEXT_REMINDER_MINUTE);
	__pReminderContextMenu->AddItem(L"시간", ID_CONTEXT_REMINDER_HOUR);
	__pReminderContextMenu->AddItem(L"일", ID_CONTEXT_REMINDER_DAY);
	__pReminderContextMenu->AddItem(L"주", ID_CONTEXT_REMINDER_WEEK);
	__pReminderContextMenu->AddActionEventListener(*this);

	// Recurrence
	Label* pRecurrenceLabel = new (std::nothrow) Label();
	pRecurrenceLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT, UI_WIDTH, UI_HEIGHT), L"반복");
	pRecurrenceLabel->SetTextConfig(29, LABEL_TEXT_STYLE_NORMAL);
	pRecurrenceLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pRecurrenceLabel->SetTextColor(COLOR_TITLE_LABEL);
	pRecurrenceLabel->SetBackgroundColor(Color(COLOR_BACKGROUND_LABEL));
	pScrollPanel->AddControl(pRecurrenceLabel);

	__pSetRecurrenceButton = new (std::nothrow) Button();
	__pSetRecurrenceButton->Construct(Rectangle(UI_X_POSITION_MIDDLE + UI_X_POSITION_GAP, yPosition + 19, UI_WIDTH * 3 / 4, BUTTON_HEIGHT), L"없음");
	__pSetRecurrenceButton->SetActionId(ID_BUTTON_RECURRENCE);
	__pSetRecurrenceButton->AddActionEventListener(*this);
	pScrollPanel->AddControl(__pSetRecurrenceButton);

	// Priority
	Label* pPriorityLabel = new (std::nothrow) Label();
	pPriorityLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT + UI_SPACE, UI_WIDTH, UI_HEIGHT), L"중요도");
	pPriorityLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pPriorityLabel->SetTextColor(COLOR_TITLE_LABEL);
	pPriorityLabel->SetBackgroundColor(COLOR_BACKGROUND_LABEL);
	pScrollPanel->AddControl(pPriorityLabel);

	__pPriorityContextButton = new (std::nothrow) Button();
	__pPriorityContextButton->Construct(Rectangle(UI_X_POSITION_MIDDLE + UI_X_POSITION_GAP, yPosition + 19, UI_WIDTH * 3 / 4, BUTTON_HEIGHT), L"하");
	__pPriorityContextButton->SetActionId(ID_BUTTON_PRIORITY);
	__pPriorityContextButton->AddActionEventListener(*this);
	pScrollPanel->AddControl(__pPriorityContextButton);

	__pPriorityContextMenu = new (std::nothrow) ContextMenu();
	__pPriorityContextMenu->Construct(Point(UI_X_POSITION_GAP + UI_WIDTH * 5 / 8, yPosition + UI_HEIGHT * 2), CONTEXT_MENU_STYLE_LIST, CONTEXT_MENU_ANCHOR_DIRECTION_UPWARD);
	__pPriorityContextMenu->AddItem(L"상", ID_CONTEXT_PRIORITY_HIGH);
	__pPriorityContextMenu->AddItem(L"중", ID_CONTEXT_PRIORITY_NORMAL);
	__pPriorityContextMenu->AddItem(L"하", ID_CONTEXT_PRIORITY_LOW);
	__pPriorityContextMenu->AddActionEventListener(*this);

	// Sensitivity
	Label* pSensitivityLabel = new (std::nothrow) Label();
	pSensitivityLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT, UI_WIDTH, UI_HEIGHT), L"공개 여부");
	pSensitivityLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pSensitivityLabel->SetTextColor(COLOR_TITLE_LABEL);
	pSensitivityLabel->SetBackgroundColor(COLOR_BACKGROUND_LABEL);
	pScrollPanel->AddControl(pSensitivityLabel);

	__pSensitivityContextButton = new (std::nothrow) Button();
	__pSensitivityContextButton->Construct(Rectangle(UI_X_POSITION_MIDDLE + UI_X_POSITION_GAP, yPosition + 19, UI_WIDTH * 3 / 4, BUTTON_HEIGHT), L"전체 공개");
	__pSensitivityContextButton->SetActionId(ID_BUTTON_SENSITIVITY);
	__pSensitivityContextButton->AddActionEventListener(*this);
	pScrollPanel->AddControl(__pSensitivityContextButton);

	__pSensitivityContextMenu = new (std::nothrow) ContextMenu();
	__pSensitivityContextMenu->Construct(Point(UI_X_POSITION_GAP + UI_WIDTH * 5 / 8, yPosition + UI_HEIGHT * 2), CONTEXT_MENU_STYLE_LIST, CONTEXT_MENU_ANCHOR_DIRECTION_UPWARD);
	__pSensitivityContextMenu->AddItem(L"전체 공개", ID_CONTEXT_SENSITIVITY_PUBLIC);
	__pSensitivityContextMenu->AddItem(L"친구 공개", ID_CONTEXT_SENSITIVITY_PRIVATE);
	__pSensitivityContextMenu->AddItem(L"나만 보기", ID_CONTEXT_SENSITIVITY_CONFIDENTIAL);
	__pSensitivityContextMenu->AddActionEventListener(*this);

	// Status
	Label* pStatusLabel = new (std::nothrow) Label();
	pStatusLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT, UI_WIDTH, UI_HEIGHT), L"상태");
	pStatusLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pStatusLabel->SetTextColor(COLOR_TITLE_LABEL);
	pStatusLabel->SetBackgroundColor(COLOR_BACKGROUND_LABEL);
	pScrollPanel->AddControl(pStatusLabel);

	__pStatusContextButton = new (std::nothrow) Button();
	__pStatusContextButton->Construct(Rectangle(UI_X_POSITION_MIDDLE + UI_X_POSITION_GAP, yPosition + 19, UI_WIDTH * 3 / 4, BUTTON_HEIGHT), L"없음");
	__pStatusContextButton->SetActionId(ID_BUTTON_STATUS);
	__pStatusContextButton->AddActionEventListener(*this);
	pScrollPanel->AddControl(__pStatusContextButton);

	__pStatusContextMenu = new (std::nothrow) ContextMenu();
	__pStatusContextMenu->Construct(Point(UI_X_POSITION_GAP + UI_WIDTH * 5 / 8, yPosition + UI_HEIGHT * 2), CONTEXT_MENU_STYLE_LIST, CONTEXT_MENU_ANCHOR_DIRECTION_UPWARD);
	__pStatusContextMenu->AddItem(L"None", ID_CONTEXT_STATUS_NONE);
	__pStatusContextMenu->AddItem(L"Confirmed", ID_CONTEXT_STATUS_CONFIRMED);
	__pStatusContextMenu->AddItem(L"Cancelled", ID_CONTEXT_STATUS_CANCELLED);
	__pStatusContextMenu->AddItem(L"Tentative", ID_CONTEXT_STATUS_TENTATIVE);
	__pStatusContextMenu->AddActionEventListener(*this);

	AddControl(pScrollPanel);

	return r;
}

result
CreateEventForm::OnTerminating(void)
{
	if (__pCalendarbook != null)
	{
		delete __pCalendarbook;
	}

	if (__pCalendar != null)
	{
		delete __pCalendar;
	}

	if (__pRecurrence != null)
	{
		delete __pRecurrence;
	}

	__pReminderContextMenu->Destroy();
	__pPriorityContextMenu->Destroy();
	__pSensitivityContextMenu->Destroy();
	__pStatusContextMenu->Destroy();

	return E_SUCCESS;
}

void
CreateEventForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
	result r = E_SUCCESS;

	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	ScrollPanel* pScrollPanel = static_cast< ScrollPanel* >(GetControl(0));
	AppAssert(pScrollPanel);

	static const int CONTEXT_POSITION = 200;

	switch (actionId)
	{
	case ID_BUTTON_REMINDER:
		__pReminderContextMenu->SetAnchorPosition(Point(__pReminderContextMenu->GetAnchorPosition().x, source.GetBounds().y + CONTEXT_POSITION - pScrollPanel->GetScrollPosition()));
		__pReminderContextMenu->SetFocusable(true);
		__pReminderContextMenu->SetShowState(true);
		__pReminderContextMenu->Show();
		break;

	case ID_CONTEXT_REMINDER_MINUTE:
		__selectedReminder = REMINDER_TIME_UNIT_MINUTE;
		__pReminderContextButton->SetText(L"분");
		__pReminderContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_REMINDER_HOUR:
		__selectedReminder = REMINDER_TIME_UNIT_HOUR;
		__pReminderContextButton->SetText(L"시간");
		__pReminderContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_REMINDER_DAY:
		__selectedReminder = REMINDER_TIME_UNIT_DAY;
		__pReminderContextButton->SetText(L"일");
		__pReminderContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_REMINDER_WEEK:
		__selectedReminder = REMINDER_TIME_UNIT_WEEK;
		__pReminderContextButton->SetText(L"주");
		__pReminderContextButton->Invalidate(false);
		break;

	case ID_BUTTON_RECURRENCE:
	{
		ArrayList* pList = new (std::nothrow) ArrayList();
		pList->Construct();

		DateTime startDate = __pStartEditDate->GetDate();
		DateTime endDate = __pEndEditDate->GetDate();

		pList->Add(*(new (std::nothrow) DateTime(startDate)));
		pList->Add(*(new (std::nothrow) DateTime(endDate)));

		if (__pRecurrence != null)
		{
			pList->Add(*(new (std::nothrow) Recurrence(*__pRecurrence)));
		}

		pSceneManager->GoForward(ForwardSceneTransition(SCENE_EVENT_SETRECURRENCE), pList);
	}
	break;

	case ID_BUTTON_PRIORITY:
		__pPriorityContextMenu->SetAnchorPosition(Point(__pPriorityContextMenu->GetAnchorPosition().x, source.GetBounds().y + CONTEXT_POSITION - pScrollPanel->GetScrollPosition()));
		__pPriorityContextMenu->SetFocusable(true);
		__pPriorityContextMenu->SetShowState(true);
		__pPriorityContextMenu->Show();
		break;

	case ID_CONTEXT_PRIORITY_HIGH:
		__selectedPriority = EVENT_PRIORITY_HIGH;
		__pPriorityContextButton->SetText(L"상");
		__pPriorityContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_PRIORITY_NORMAL:
		__selectedPriority = EVENT_PRIORITY_NORMAL;
		__pPriorityContextButton->SetText(L"중");
		__pPriorityContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_PRIORITY_LOW:
		__selectedPriority = EVENT_PRIORITY_LOW;
		__pPriorityContextButton->SetText(L"하");
		__pPriorityContextButton->Invalidate(false);
		break;

	case ID_BUTTON_SENSITIVITY:
		__pSensitivityContextMenu->SetAnchorPosition(Point(__pSensitivityContextMenu->GetAnchorPosition().x, source.GetBounds().y + CONTEXT_POSITION - pScrollPanel->GetScrollPosition()));
		__pSensitivityContextMenu->SetFocusable(true);
		__pSensitivityContextMenu->SetShowState(true);
		__pSensitivityContextMenu->Show();
		break;

	case ID_CONTEXT_SENSITIVITY_PUBLIC:
		__selectedSensitivity = SENSITIVITY_PUBLIC;
		__pSensitivityContextButton->SetText(L"전체 공개");
		__pSensitivityContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_SENSITIVITY_PRIVATE:
		__selectedSensitivity = SENSITIVITY_PRIVATE;
		__pSensitivityContextButton->SetText(L"친구 공개");
		__pSensitivityContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_SENSITIVITY_CONFIDENTIAL:
		__selectedSensitivity = SENSITIVITY_CONFIDENTIAL;
		__pSensitivityContextButton->SetText(L"나만 보기");
		__pSensitivityContextButton->Invalidate(false);
		break;

	case ID_BUTTON_STATUS:
		__pStatusContextMenu->SetAnchorPosition(Point(__pStatusContextMenu->GetAnchorPosition().x, source.GetBounds().y + CONTEXT_POSITION - pScrollPanel->GetScrollPosition()));
		__pStatusContextMenu->SetFocusable(true);
		__pStatusContextMenu->SetShowState(true);
		__pStatusContextMenu->Show();
		break;

	case ID_CONTEXT_STATUS_NONE:
		__selectedStatus = EVENT_STATUS_NONE;
		__pStatusContextButton->SetText(L"없음");
		__pStatusContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_STATUS_CONFIRMED:
		__selectedStatus = EVENT_STATUS_CONFIRMED;
		__pStatusContextButton->SetText(L"완료됨");
		__pStatusContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_STATUS_CANCELLED:
		__selectedStatus = EVENT_STATUS_CANCELLED;
		__pStatusContextButton->SetText(L"취소됨");
		__pStatusContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_STATUS_TENTATIVE:
		__selectedStatus = EVENT_STATUS_TENTATIVE;
		__pStatusContextButton->SetText(L"보류중");
		__pStatusContextButton->Invalidate(false);
		break;

	case ID_CHKBUTTON_ISALLDAY_CHECKED:
	{
		if (__pStartEditTime != null)
		{
			__pStartEditTime->SetShowState(false);
		}

		if (__pEndEditTime != null)
		{
			__pEndEditTime->SetShowState(false);
		}
	}
	break;

	case ID_CHKBUTTON_ISALLDAY_UNCHECKED:
	{
		if (__pStartEditTime != null)
		{
			__pStartEditTime->SetShowState(true);
		}

		if (__pEndEditTime != null)
		{
			__pEndEditTime->SetShowState(true);
		}
	}
	break;

	case ID_BUTTON_CALENDAR:
		ShowSelectCalendarPopup();
		break;

	case ID_FOOTER_SAVE:
		if (__pSubjectEditField->GetText().IsEmpty())
		{
			MessageBox messageBox;
			messageBox.Construct(L"Error", "제목을 입력해 주세요", MSGBOX_STYLE_OK, 0);
			int doModal;
			messageBox.ShowAndWait(doModal);
		}
		else
		{
			r = AddEvent();
			TryReturnVoid(!IsFailed(r), "[%s] Failed to add event.", GetErrorMessage(r));
			pSceneManager->GoBackward(BackwardSceneTransition());
		}
		break;

	}

}

void
CreateEventForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId, const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
	if (previousSceneId == SCENE_EVENT_SETRECURRENCE)
	{
		if (pArgs != null)
		{
			if (__pRecurrence != null)
			{
				delete __pRecurrence;
				__pRecurrence = null;
			}

			if (pArgs->GetCount() > 0)
			{
				__pRecurrence = static_cast< Recurrence* >(pArgs->GetAt(0));
			}

			__pSetRecurrenceButton->SetText(GetRecurrenceString());

			pArgs->RemoveAll(false);
			delete pArgs;
		}
	}
}

void
CreateEventForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	pSceneManager->GoBackward(BackwardSceneTransition());
}

void
CreateEventForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId, const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
}

void
CreateEventForm::OnDateChanged(const Tizen::Ui::Control& source, int year, int month, int day)
{
	if (__pStartEditDate->GetDate() > __pEndEditDate->GetDate())
	{
		__pEndEditDate->SetDate(__pStartEditDate->GetDate());
		__pEndEditDate->Invalidate(false);
	}
	if (__pRecurrence != null)
	{
		delete __pRecurrence;
		__pRecurrence = null;

		__pSetRecurrenceButton->SetText(GetRecurrenceString());
		__pSetRecurrenceButton->Invalidate(false);
	}
}

void
CreateEventForm::OnDateChangeCanceled(const Tizen::Ui::Control& source)
{
}

void
CreateEventForm::OnTimeChanged(const Control& source, int hour, int minute)
{
	if ((__pStartEditDate->GetDate() == __pEndEditDate->GetDate()) && (__pStartEditTime->GetTime() > __pEndEditTime->GetTime()))
	{
		__pEndEditTime->SetTime(__pStartEditTime->GetTime());
		__pEndEditTime->Invalidate(false);
	}
}

void
CreateEventForm::OnTimeChangeCanceled(const Control& source)
{
}

void
CreateEventForm::OnUserEventReceivedN(RequestId requestId, IList* pArgs)
{
	if (pArgs != null)
	{
		Integer* pId = static_cast< Integer* >(pArgs->GetAt(0));
		RecordId calendarId = pId->ToInt();

		if (__pCalendar != null)
		{
			delete __pCalendar;
		}

		__pCalendar = __pCalendarbook->GetCalendarN(calendarId);

		String calendarName = __pCalendar->GetName();
		if (calendarName.IsEmpty() == true)
		{
			calendarName = L"Calendar ID: ";
			calendarName.Append(__pCalendar->GetRecordId());
		}

		__pSelectCalendarButton->SetText(calendarName);
		__pSelectCalendarButton->Invalidate(false);

		pArgs->RemoveAll(true);
		delete pArgs;
	}

	DeleteCalendarPopup();
}

String
CreateEventForm::GetTimezoneString(void) const
{
	LocaleManager localeManager;
	localeManager.Construct();
	TimeZone timeZone = localeManager.GetSystemTimeZone();

	return timeZone.GetId();
}

String
CreateEventForm::GetRecurrenceString(void) const
{
	String recurString;

	if (__pRecurrence == null)
	{
		recurString = L"None";
	}
	else
	{
		recurString = L"Every ";

		switch (__pRecurrence->GetFrequency())
		{
		case FREQ_DAILY:
			recurString.Append(L"일");
			break;

		case FREQ_WEEKLY:
			recurString.Append(L"주");
			break;

		case FREQ_MONTHLY:
			recurString.Append(L"월");
			break;

		case FREQ_YEARLY:
			recurString.Append(L"년");
			break;

		default:
			break;
		}
	}
	return recurString;
}

void
CreateEventForm::ShowSelectCalendarPopup(void)
{
	DeleteCalendarPopup();
	__pSelectCalendarPopup = new (std::nothrow) SelectCalendarPopup();
	if (__pCalendar != null)
	{
		__pSelectCalendarPopup->Construct(this, __pCalendar->GetRecordId());
	}
	else
	{
		__pSelectCalendarPopup->Construct(this, INVALID_RECORD_ID);
	}

	__pSelectCalendarPopup->SetShowState(true);
	__pSelectCalendarPopup->Show();
}

void
CreateEventForm::DeleteCalendarPopup(void)
{
	delete __pSelectCalendarPopup;
	__pSelectCalendarPopup = null;
}

result
CreateEventForm::AddEvent(void)
{
	result r = E_SUCCESS;

	CalEvent calEvent;

	// Sets the Subject
	String strValue = __pSubjectEditField->GetText();
	if (strValue.IsEmpty() == false)
	{
		calEvent.SetSubject(strValue);
	}

	// Sets the Location
	strValue = __pLocationEditField->GetText();
	if (strValue.IsEmpty() == false)
	{
		calEvent.SetLocation(strValue);
	}

	// Sets the Description
	strValue = __pDescriptionEditField->GetText();
	if (strValue.IsEmpty() == false)
	{
		calEvent.SetDescription(strValue);
	}

	// Sets the Priority
	calEvent.SetPriority(__selectedPriority);

	// Sets the Sensitivity
	calEvent.SetSensitivity(__selectedSensitivity);

	// Sets the Status
	calEvent.SetStatus(__selectedStatus);

	// Sets the Time zone
	LocaleManager localeManager;
	localeManager.Construct();

	TimeZone timeZone = localeManager.GetSystemTimeZone();
	calEvent.SetTimeZone(timeZone);

	DateTime startDate;
	DateTime endDate;

	if (__pIsAllDayButton->IsSelected() == true)
	{
		calEvent.SetAllDayEvent(true);

		startDate = __pStartEditDate->GetDate();
		endDate = __pEndEditDate->GetDate();
		endDate.AddDays(1);

		r = calEvent.SetStartAndEndTime(startDate, endDate);
		TryCatch(!IsFailed(r), , "[%s] Failed to set time.", GetErrorMessage(r));
	}
	else
	{
		calEvent.SetAllDayEvent(false);
		startDate.SetValue(__pStartEditDate->GetYear(), __pStartEditDate->GetMonth(), __pStartEditDate->GetDay(), __pStartEditTime->GetHour(), __pStartEditTime->GetMinute(), 0);
		endDate.SetValue(__pEndEditDate->GetYear(), __pEndEditDate->GetMonth(), __pEndEditDate->GetDay(), __pEndEditTime->GetHour(), __pEndEditTime->GetMinute(), 0);
		r = calEvent.SetStartAndEndTime(timeZone.WallTimeToUtcTime(startDate), timeZone.WallTimeToUtcTime(endDate));
		TryCatch(!IsFailed(r), , "[%s] Failed to set time", GetErrorMessage(r));
	}

	// Sets the Reminder
	strValue = __pReminderEditField->GetText();
	if (strValue.IsEmpty() == false)
	{
		int timeOffset = 0;
		Integer::Parse(strValue, timeOffset);
		if (timeOffset > 0)
		{
			Reminder reminder;
			reminder.SetTimeOffset(__selectedReminder, timeOffset);
			calEvent.AddReminder(reminder);
		}
	}

	// Sets the Recurrence
	if (__pRecurrence != null)
	{
		const DateTime* pUntilDate = __pRecurrence->GetUntil();
		if (pUntilDate && calEvent.IsAllDayEvent() == false)
		{
			DateTime utcTime = timeZone.WallTimeToUtcTime(*pUntilDate);
			__pRecurrence->SetUntil(&utcTime);
		}
		r = calEvent.SetRecurrence(__pRecurrence);
		TryCatch(!IsFailed(r), , "[%s] Failed to set recurrence to event.", GetErrorMessage(r));
	}

	if (__pCalendar != null)
	{
		r = __pCalendarbook->AddEvent(calEvent, __pCalendar->GetRecordId());
		TryCatch(!IsFailed(r), , "[%s] Failed to add an event.", GetErrorMessage(r));
	}
	else
	{
		r = __pCalendarbook->AddEvent(calEvent);
		TryCatch(!IsFailed(r), , "[%s] Failed to add an event.", GetErrorMessage(r));
	}

	return r;

CATCH:
	int doModal;
	MessageBox messageBox;
	messageBox.Construct(L"Error", "Failed to create an event.", MSGBOX_STYLE_OK, 0);
	messageBox.ShowAndWait(doModal);
	return r;

}
