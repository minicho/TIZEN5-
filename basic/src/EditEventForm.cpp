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

#include "EditEventForm.h"
#include "AppResourceId.h"
#include "basicAppFrame.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Graphics;
using namespace Tizen::Locales;
using namespace Tizen::Social;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

EditEventForm::EditEventForm()
	: __pCalendarbook(null)
	, __pCalEvent(null)
	, __pRecurrence(null)
	, __selectedReminder(REMINDER_TIME_UNIT_MINUTE)
	, __selectedPriority(EVENT_PRIORITY_LOW)
	, __selectedSensitivity(SENSITIVITY_PUBLIC)
	, __selectedStatus(EVENT_STATUS_NONE)
{
}

EditEventForm::~EditEventForm()
{
}

bool
EditEventForm::Initialize(void)
{
	result r = E_SUCCESS;

	r = Construct(FORM_STYLE_NORMAL | FORM_STYLE_FOOTER | FORM_STYLE_HEADER | FORM_STYLE_PORTRAIT_INDICATOR);
	TryReturn(!IsFailed(r), false, "[%s] Failed to construct the form.", GetErrorMessage(r));

	__pCalendarbook = new (std::nothrow) Calendarbook();
	TryReturn(__pCalendarbook != null, false, "[%s] Failed to instantiate calendarbook.");

	r = __pCalendarbook->Construct();
	TryReturn(!IsFailed(r), false, "[%s] Failed to construct the calendarbook.", GetErrorMessage(r));

	return true;
}

result
EditEventForm::OnInitializing(void)
{
	result r = E_SUCCESS;

	Header* pHeader = GetHeader();
	AppAssert(pHeader);
	pHeader->SetStyle(HEADER_STYLE_TITLE);
	pHeader->SetTitleText(L"Edit event");

	Footer* pFooter = GetFooter();
	AppAssert(pFooter);
	pFooter->SetStyle(FOOTER_STYLE_BUTTON_TEXT);

	FooterItem footerSave;
	footerSave.Construct(ID_FOOTER_SAVE);
	footerSave.SetText(L"Save");
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

	// Subject
	__pSubjectEditField = new (std::nothrow) EditField();
	__pSubjectEditField->Construct(Rectangle(UI_X_POSITION_GAP, yPosition, UI_WIDTH, UI_HEIGHT), EDIT_FIELD_STYLE_NORMAL, INPUT_STYLE_FULLSCREEN, EDIT_FIELD_TITLE_STYLE_TOP);
	__pSubjectEditField->SetTitleText(L"Subject");
	__pSubjectEditField->SetGuideText(L"Enter the subject");
	pScrollPanel->AddControl(__pSubjectEditField);


	int minYear = Calendarbook::GetMinDateTime().GetYear() + 1;
	int maxYear = Calendarbook::GetMaxDateTime().GetYear() - 1;

	// Start Date
	Label* pStartDateLabel = new (std::nothrow) Label();
	pStartDateLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT + UI_SPACE, UI_WIDTH, UI_HEIGHT), L"Start");
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
	pEndDateLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT, UI_WIDTH, UI_HEIGHT), L"End");
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
	pTimeZoneLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT, UI_WIDTH, UI_HEIGHT), L"TimeZone");
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
	__pIsAllDayButton->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT + UI_SPACE, UI_WIDTH, UI_HEIGHT), CHECK_BUTTON_STYLE_ONOFF_SLIDING, BACKGROUND_STYLE_DEFAULT, false, L"All day Event");
	__pIsAllDayButton->SetActionId(ID_CHKBUTTON_ISALLDAY_CHECKED, ID_CHKBUTTON_ISALLDAY_UNCHECKED);
	__pIsAllDayButton->SetTextColor(COLOR_TITLE_LABEL);
	__pIsAllDayButton->AddActionEventListener(*this);
	pScrollPanel->AddControl(__pIsAllDayButton);

	// Location
	__pLocationEditField = new (std::nothrow) EditField();
	__pLocationEditField->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT + UI_SPACE, UI_WIDTH, UI_HEIGHT), EDIT_FIELD_STYLE_NORMAL, INPUT_STYLE_FULLSCREEN, EDIT_FIELD_TITLE_STYLE_TOP);
	__pLocationEditField->SetTitleText(L"Location");
	__pLocationEditField->SetGuideText(L"Enter the location");
	pScrollPanel->AddControl(__pLocationEditField);

	// Description
	__pDescriptionEditField = new (std::nothrow) EditField();
	__pDescriptionEditField->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT, UI_WIDTH, UI_HEIGHT), EDIT_FIELD_STYLE_NORMAL, INPUT_STYLE_FULLSCREEN, EDIT_FIELD_TITLE_STYLE_TOP);
	__pDescriptionEditField->SetTitleText(L"Description");
	__pDescriptionEditField->SetGuideText(L"Enter the description");
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
	__pReminderEditField->SetName(L"Reminder");
	__pReminderEditField->SetTextAlignment(ALIGNMENT_CENTER);
	pScrollPanel->AddControl(__pReminderEditField);

	__pReminderContextButton = new (std::nothrow) Button();
	__pReminderContextButton->Construct(Rectangle(UI_X_POSITION_MIDDLE * 2 + UI_X_POSITION_GAP, yPosition + 19, UI_WIDTH / 2, BUTTON_HEIGHT), L"Minute(s)");
	__pReminderContextButton->SetActionId(ID_BUTTON_REMINDER);
	__pReminderContextButton->AddActionEventListener(*this);
	pScrollPanel->AddControl(__pReminderContextButton);

	__pReminderContextMenu = new (std::nothrow) ContextMenu();
	__pReminderContextMenu->Construct(Point(UI_X_POSITION_GAP + UI_WIDTH * 3 / 4, yPosition + UI_HEIGHT * 2), CONTEXT_MENU_STYLE_LIST, CONTEXT_MENU_ANCHOR_DIRECTION_UPWARD);
	__pReminderContextMenu->AddItem(L"Minute(s)", ID_CONTEXT_REMINDER_MINUTE);
	__pReminderContextMenu->AddItem(L"Hour(s)", ID_CONTEXT_REMINDER_HOUR);
	__pReminderContextMenu->AddItem(L"Day(s)", ID_CONTEXT_REMINDER_DAY);
	__pReminderContextMenu->AddItem(L"Week(s)", ID_CONTEXT_REMINDER_WEEK);
	__pReminderContextMenu->AddActionEventListener(*this);

	// Recurrence
	Label* pRecurrenceLabel = new (std::nothrow) Label();
	pRecurrenceLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT, UI_WIDTH, UI_HEIGHT), L"Recurrence");
	pRecurrenceLabel->SetTextConfig(29, LABEL_TEXT_STYLE_NORMAL);
	pRecurrenceLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pRecurrenceLabel->SetTextColor(COLOR_TITLE_LABEL);
	pRecurrenceLabel->SetBackgroundColor(Color(COLOR_BACKGROUND_LABEL));
	pScrollPanel->AddControl(pRecurrenceLabel);

	__pSetRecurrenceButton = new (std::nothrow) Button();
	__pSetRecurrenceButton->Construct(Rectangle(UI_X_POSITION_MIDDLE + UI_X_POSITION_GAP, yPosition + 19, UI_WIDTH * 3 / 4, BUTTON_HEIGHT), L"None");
	__pSetRecurrenceButton->SetActionId(ID_BUTTON_RECURRENCE);
	__pSetRecurrenceButton->AddActionEventListener(*this);
	pScrollPanel->AddControl(__pSetRecurrenceButton);

	// Priority
	Label* pPriorityLabel = new (std::nothrow) Label();
	pPriorityLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT + UI_SPACE, UI_WIDTH, UI_HEIGHT), L"Priority");
	pPriorityLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pPriorityLabel->SetTextColor(COLOR_TITLE_LABEL);
	pPriorityLabel->SetBackgroundColor(COLOR_BACKGROUND_LABEL);
	pScrollPanel->AddControl(pPriorityLabel);

	__pPriorityContextButton = new (std::nothrow) Button();
	__pPriorityContextButton->Construct(Rectangle(UI_X_POSITION_MIDDLE + UI_X_POSITION_GAP, yPosition + 19, UI_WIDTH * 3 / 4, BUTTON_HEIGHT), L"Low");
	__pPriorityContextButton->SetActionId(ID_BUTTON_PRIORITY);
	__pPriorityContextButton->AddActionEventListener(*this);
	pScrollPanel->AddControl(__pPriorityContextButton);

	__pPriorityContextMenu = new (std::nothrow) ContextMenu();
	__pPriorityContextMenu->Construct(Point(UI_X_POSITION_GAP + UI_WIDTH * 5 / 8, yPosition + UI_HEIGHT * 2), CONTEXT_MENU_STYLE_LIST, CONTEXT_MENU_ANCHOR_DIRECTION_UPWARD);
	__pPriorityContextMenu->AddItem(L"High", ID_CONTEXT_PRIORITY_HIGH);
	__pPriorityContextMenu->AddItem(L"Normal", ID_CONTEXT_PRIORITY_NORMAL);
	__pPriorityContextMenu->AddItem(L"Low", ID_CONTEXT_PRIORITY_LOW);
	__pPriorityContextMenu->AddActionEventListener(*this);

	// Sensitivity
	Label* pSensitivityLabel = new (std::nothrow) Label();
	pSensitivityLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT, UI_WIDTH, UI_HEIGHT), L"Sensitivity");
	pSensitivityLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pSensitivityLabel->SetTextColor(COLOR_TITLE_LABEL);
	pSensitivityLabel->SetBackgroundColor(COLOR_BACKGROUND_LABEL);
	pScrollPanel->AddControl(pSensitivityLabel);

	__pSensitivityContextButton = new (std::nothrow) Button();
	__pSensitivityContextButton->Construct(Rectangle(UI_X_POSITION_MIDDLE + UI_X_POSITION_GAP, yPosition + 19, UI_WIDTH * 3 / 4, BUTTON_HEIGHT), L"Public");
	__pSensitivityContextButton->SetActionId(ID_BUTTON_SENSITIVITY);
	__pSensitivityContextButton->AddActionEventListener(*this);
	pScrollPanel->AddControl(__pSensitivityContextButton);

	__pSensitivityContextMenu = new (std::nothrow) ContextMenu();
	__pSensitivityContextMenu->Construct(Point(UI_X_POSITION_GAP + UI_WIDTH * 5 / 8, yPosition + UI_HEIGHT * 2), CONTEXT_MENU_STYLE_LIST, CONTEXT_MENU_ANCHOR_DIRECTION_UPWARD);
	__pSensitivityContextMenu->AddItem(L"Public", ID_CONTEXT_SENSITIVITY_PUBLIC);
	__pSensitivityContextMenu->AddItem(L"Private", ID_CONTEXT_SENSITIVITY_PRIVATE);
	__pSensitivityContextMenu->AddItem(L"Confidential", ID_CONTEXT_SENSITIVITY_CONFIDENTIAL);
	__pSensitivityContextMenu->AddActionEventListener(*this);

	// Status
	Label* pStatusLabel = new (std::nothrow) Label();
	pStatusLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT, UI_WIDTH, UI_HEIGHT), L"Status");
	pStatusLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pStatusLabel->SetTextColor(COLOR_TITLE_LABEL);
	pStatusLabel->SetBackgroundColor(COLOR_BACKGROUND_LABEL);
	pScrollPanel->AddControl(pStatusLabel);

	__pStatusContextButton = new (std::nothrow) Button();
	__pStatusContextButton->Construct(Rectangle(UI_X_POSITION_MIDDLE + UI_X_POSITION_GAP, yPosition + 19, UI_WIDTH * 3 / 4, BUTTON_HEIGHT), L"None");
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
EditEventForm::OnTerminating(void)
{
	if (__pCalendarbook != null)
	{
		delete __pCalendarbook;
	}

	if (__pCalEvent != null)
	{
		delete __pCalEvent;
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
EditEventForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
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
		__pReminderContextButton->SetText(L"Minute(s)");
		__pReminderContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_REMINDER_HOUR:
		__selectedReminder = REMINDER_TIME_UNIT_HOUR;
		__pReminderContextButton->SetText(L"Hour(s)");
		__pReminderContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_REMINDER_DAY:
		__selectedReminder = REMINDER_TIME_UNIT_DAY;
		__pReminderContextButton->SetText(L"Day(s)");
		__pReminderContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_REMINDER_WEEK:
		__selectedReminder = REMINDER_TIME_UNIT_WEEK;
		__pReminderContextButton->SetText(L"Week(s)");
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
		__pPriorityContextButton->SetText(L"High");
		__pPriorityContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_PRIORITY_NORMAL:
		__selectedPriority = EVENT_PRIORITY_NORMAL;
		__pPriorityContextButton->SetText(L"Normal");
		__pPriorityContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_PRIORITY_LOW:
		__selectedPriority = EVENT_PRIORITY_LOW;
		__pPriorityContextButton->SetText(L"Low");
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
		__pSensitivityContextButton->SetText(L"Public");
		__pSensitivityContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_SENSITIVITY_PRIVATE:
		__selectedSensitivity = SENSITIVITY_PRIVATE;
		__pSensitivityContextButton->SetText(L"Private");
		__pSensitivityContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_SENSITIVITY_CONFIDENTIAL:
		__selectedSensitivity = SENSITIVITY_CONFIDENTIAL;
		__pSensitivityContextButton->SetText(L"Confidential");
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
		__pStatusContextButton->SetText(L"None");
		__pStatusContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_STATUS_CONFIRMED:
		__selectedStatus = EVENT_STATUS_CONFIRMED;
		__pStatusContextButton->SetText(L"Confirmed");
		__pStatusContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_STATUS_CANCELLED:
		__selectedStatus = EVENT_STATUS_CANCELLED;
		__pStatusContextButton->SetText(L"Cancelled");
		__pStatusContextButton->Invalidate(false);
		break;

	case ID_CONTEXT_STATUS_TENTATIVE:
		__selectedStatus = EVENT_STATUS_TENTATIVE;
		__pStatusContextButton->SetText(L"Tentative");
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

	case ID_FOOTER_SAVE:
		if (__pSubjectEditField->GetText().IsEmpty())
		{
			MessageBox messageBox;
			messageBox.Construct(L"Error", "Please enter the subject", MSGBOX_STYLE_OK, 0);
			int doModal;
			messageBox.ShowAndWait(doModal);
		}
		else
		{
			if (__pCalEvent != null)
			{
				r = UpdateEvent();
				TryReturnVoid(!IsFailed(r), "[%s] Failed to edit event.", GetErrorMessage(r));
			}
			else
			{
				MessageBox messageBox;
				messageBox.Construct(L"Error", "Failed to edit the event.", MSGBOX_STYLE_OK, 0);
				int doModal;
				messageBox.ShowAndWait(doModal);
			}

			pSceneManager->GoBackward(BackwardSceneTransition(SCENE_EVENT_LIST));
		}
		break;

	}

}

void
EditEventForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	pSceneManager->GoBackward(BackwardSceneTransition());
}

void
EditEventForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId, const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
	result r = E_SUCCESS;

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
	else if (previousSceneId == SCENE_EVENT_DETAIL)
	{
		if (pArgs != null)
		{
			Integer* pInteger = static_cast< Integer* >(pArgs->GetAt(0));
			int eventId = pInteger->ToInt();
			__pCalEvent = __pCalendarbook->GetEventN(eventId);

			r = GetLastResult();
			if (IsFailed(r))
			{
				MessageBox messageBox;
				messageBox.Construct(L"Error", "Failed to get event instance", MSGBOX_STYLE_OK, 0);
				int doModal;
				messageBox.ShowAndWait(doModal);

				AppLogException("[%s] Failed to get the Event.", GetErrorMessage(r));
			}
			else
			{
				LoadEvent();
			}

			pArgs->RemoveAll(false);
			delete pArgs;
		}
	}
}

void
EditEventForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId, const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
}

void
EditEventForm::OnDateChanged(const Tizen::Ui::Control& source, int year, int month, int day)
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
EditEventForm::OnDateChangeCanceled(const Tizen::Ui::Control& source)
{
}

void
EditEventForm::OnTimeChanged(const Control& source, int hour, int minute)
{
	if ((__pStartEditDate->GetDate() == __pEndEditDate->GetDate()) && (__pStartEditTime->GetTime() > __pEndEditTime->GetTime()))
	{
		__pEndEditTime->SetTime(__pStartEditTime->GetTime());
		__pEndEditTime->Invalidate(false);
	}
}

void
EditEventForm::OnTimeChangeCanceled(const Control& source)
{
}

String
EditEventForm::GetTimezoneString(void) const
{
	LocaleManager localeManager;
	localeManager.Construct();
	TimeZone timeZone = localeManager.GetSystemTimeZone();

	return timeZone.GetId();
}

String
EditEventForm::GetRecurrenceString(void) const
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
			recurString.Append(L"day");
			break;

		case FREQ_WEEKLY:
			recurString.Append(L"week");
			break;

		case FREQ_MONTHLY:
			recurString.Append(L"month");
			break;

		case FREQ_YEARLY:
			recurString.Append(L"year");
			break;

		default:
			break;
		}
	}
	return recurString;
}

void
EditEventForm::LoadEvent(void)
{
	result r = E_SUCCESS;

	// Loads the Subject
	__pSubjectEditField->SetText(__pCalEvent->GetSubject());

	// Loads the Time
	DateTime startDate = __pCalEvent->GetStartTime();
	DateTime endDate = __pCalEvent->GetEndTime();

	if (__pCalEvent->IsAllDayEvent() == true)
	{
		__pIsAllDayButton->SetSelected(true);

		// Start Date
		__pStartEditDate->SetDate(startDate);
		__pStartEditTime->SetShowState(false);

		// End Date
		if(startDate < endDate)
		{
			endDate.AddDays(-1);
		}
		__pEndEditDate->SetDate(endDate);
		__pEndEditTime->SetShowState(false);
	}
	else
	{
		// Convert UTC time to local time
		LocaleManager localeManager;
		r = localeManager.Construct();
		TryReturnVoid(!IsFailed(r), "[%s] Failed to construct localeManager.", GetErrorMessage(r));

		TimeZone timeZone = localeManager.GetSystemTimeZone();
		startDate = timeZone.UtcTimeToWallTime(startDate);
		endDate = timeZone.UtcTimeToWallTime(endDate);

		// Start Date
		__pStartEditDate->SetDate(startDate);
		__pStartEditTime->SetTime(startDate);

		// End Date
		__pEndEditDate->SetDate(endDate);
		__pEndEditTime->SetTime(endDate);
	}

	// Loads the Location
	__pLocationEditField->SetText(__pCalEvent->GetLocation());

	// Loads the Priority
	__selectedPriority = __pCalEvent->GetPriority();
	switch (__selectedPriority)
	{
	case EVENT_PRIORITY_LOW:
		__pPriorityContextButton->SetText(L"Low");
		break;

	case EVENT_PRIORITY_NORMAL:
		__pPriorityContextButton->SetText(L"Normal");
		break;

	case EVENT_PRIORITY_HIGH:
		__pPriorityContextButton->SetText(L"High");
		break;

	default:
		break;
	}

	// Loads the Sensitivity
	__selectedSensitivity = __pCalEvent->GetSensitivity();
	switch (__selectedSensitivity)
	{
	case SENSITIVITY_PUBLIC:
		__pSensitivityContextButton->SetText(L"Public");
		break;

	case SENSITIVITY_PRIVATE:
		__pSensitivityContextButton->SetText(L"Private");
		break;

	case SENSITIVITY_CONFIDENTIAL:
		__pSensitivityContextButton->SetText(L"Confidential");
		break;

	default:
		break;
	}

	// Loads the Status
	__selectedStatus = __pCalEvent->GetStatus();
	switch (__selectedStatus)
	{
	case EVENT_STATUS_NONE:
		__pStatusContextButton->SetText(L"None");
		break;

	case EVENT_STATUS_CONFIRMED:
		__pStatusContextButton->SetText(L"Confirmed");
		break;

	case EVENT_STATUS_CANCELLED:
		__pStatusContextButton->SetText(L"Cancelled");
		break;

	case EVENT_STATUS_TENTATIVE:
		__pStatusContextButton->SetText(L"Tentative");
		break;

	default:
		break;
	}

	// Loads the Reminder
	const IList& pReminderList = __pCalEvent->GetAllReminders();
	const Reminder* pReminder = static_cast< const Reminder* >(pReminderList.GetAt(0));

	if (pReminder != null)
	{
		int timeOffset = pReminder->GetTimeOffset();

		switch (pReminder->GetTimeUnit())
		{
		case REMINDER_TIME_UNIT_MINUTE:
			__pReminderContextButton->SetText(L"minutes(s)");
			break;

		case REMINDER_TIME_UNIT_HOUR:
			__pReminderContextButton->SetText(L"hour(s)");
			break;

		case REMINDER_TIME_UNIT_DAY:
			__pReminderContextButton->SetText(L"day(s)");
			break;

		case REMINDER_TIME_UNIT_WEEK:
			__pReminderContextButton->SetText(L"week(s)");
			break;
		case REMINDER_TIME_UNIT_NONE:
		default:
			break;
		}

		__pReminderEditField->SetText(Integer::ToString(timeOffset));
	}

	// Loads the Recurrence
	const Recurrence* pRecurrence = __pCalEvent->GetRecurrence();
	String RecurrenceValue;

	if (pRecurrence != null)
	{
		__pRecurrence = new (std::nothrow) Recurrence(*pRecurrence);
	}
	else
	{
		__pRecurrence = null;
	}

	__pSetRecurrenceButton->SetText(GetRecurrenceString());

	// Loads the Description
	__pDescriptionEditField->SetText(__pCalEvent->GetDescription());

}

result
EditEventForm::UpdateEvent(void)
{
	result r = E_SUCCESS;

	// Sets the Subject
	String strValue = __pSubjectEditField->GetText();
	if (strValue.IsEmpty() == false)
	{
		__pCalEvent->SetSubject(strValue);
	}

	// Sets the Location
	__pCalEvent->SetLocation(__pLocationEditField->GetText());

	// Sets the Description
	__pCalEvent->SetDescription(__pDescriptionEditField->GetText());

	// Sets the Priority
	__pCalEvent->SetPriority(__selectedPriority);

	// Sets the Sensitivity
	__pCalEvent->SetSensitivity(__selectedSensitivity);

	// Sets the Status
	__pCalEvent->SetStatus(__selectedStatus);

	// Gets the Time zone.
	LocaleManager localeManager;
	localeManager.Construct();

	TimeZone timeZone = localeManager.GetSystemTimeZone();
	__pCalEvent->SetTimeZone(timeZone);

	DateTime startDate;
	DateTime endDate;

	__pCalEvent->SetRecurrence(null);

	if (__pIsAllDayButton->IsSelected() == true)
	{
		__pCalEvent->SetAllDayEvent(true);

		startDate = __pStartEditDate->GetDate();
		endDate = __pEndEditDate->GetDate();
		endDate.AddDays(1);

		__pCalEvent->SetStartAndEndTime(startDate, endDate);
	}
	else
	{
		__pCalEvent->SetAllDayEvent(false);
		startDate.SetValue(__pStartEditDate->GetYear(), __pStartEditDate->GetMonth(), __pStartEditDate->GetDay(), __pStartEditTime->GetHour(), __pStartEditTime->GetMinute(), 0);
		endDate.SetValue(__pEndEditDate->GetYear(), __pEndEditDate->GetMonth(), __pEndEditDate->GetDay(), __pEndEditTime->GetHour(), __pEndEditTime->GetMinute(), 0);
		__pCalEvent->SetStartAndEndTime(timeZone.WallTimeToUtcTime(startDate), timeZone.WallTimeToUtcTime(endDate));
	}

	// Sets the Reminder
	const IList& pReminderList = __pCalEvent->GetAllReminders();
	if (pReminderList.GetCount() > 0)
	{
		__pCalEvent->RemoveReminderAt(0);
	}

	strValue = __pReminderEditField->GetText();
	if (strValue.IsEmpty() == false)
	{
		int timeOffset = 0;
		Integer::Parse(strValue, timeOffset);
		if (timeOffset > 0)
		{
			Reminder reminder;
			reminder.SetTimeOffset(__selectedReminder, timeOffset);
			__pCalEvent->AddReminder(reminder);
		}
	}

	// Sets the Recurrence
	if (__pRecurrence != null)
	{
		const DateTime* pUntilDate = __pRecurrence->GetUntil();
		if (pUntilDate && __pCalEvent->IsAllDayEvent() == false)
		{
			DateTime utcTime = timeZone.WallTimeToUtcTime(*pUntilDate);
			__pRecurrence->SetUntil(&utcTime);
		}
		r = __pCalEvent->SetRecurrence(__pRecurrence);
		TryCatch(!IsFailed(r), , "[%s] Failed to set recurrence to event.", GetErrorMessage(r));
	}

	r = __pCalendarbook->UpdateEvent(*__pCalEvent);
	TryCatch(!IsFailed(r), , "[%s] Failed to update the event.", GetErrorMessage(r));

	return r;

CATCH:
	int doModal;
	MessageBox messageBox;
	messageBox.Construct(L"Error", "Failed to edit the event.", MSGBOX_STYLE_OK, 0);
	messageBox.ShowAndWait(doModal);

	return r;
}
