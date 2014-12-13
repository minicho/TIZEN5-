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

#include "EventDetailForm.h"
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

EventDetailForm::EventDetailForm()
	: __pSubjectLabelData(null)
	, __pStartDateLabelData(null)
	, __pEndDateLabelData(null)
	, __pLocationLabelData(null)
	, __pPriorityLabelData(null)
	, __pSensitivityLabelData(null)
	, __pStatusLabelData(null)
	, __pReminderLabelData(null)
	, __pRecurrenceLabelData(null)
	, __pDescriptionLabelData(null)
	, __pCalendarbook(null)
	, __pCalEventInstance(null)
	, __pDeleteRecurringEventPopup(null)
{
}

EventDetailForm::~EventDetailForm()
{
}

bool
EventDetailForm::Initialize(void)
{
	result r = E_SUCCESS;

	__pCalendarbook = new (std::nothrow) Calendarbook();
	TryReturn(__pCalendarbook != null, false, "[%s] Failed to instantiate calendarbook.");

	r = __pCalendarbook->Construct();
	TryReturn(!IsFailed(r), false, "[%s] Failed to construct the calendarbook.", GetErrorMessage(r));

	r = Construct(FORM_STYLE_NORMAL | FORM_STYLE_FOOTER | FORM_STYLE_HEADER | FORM_STYLE_PORTRAIT_INDICATOR);
	TryReturn(!IsFailed(r), false, "[%s] Failed to construct the form.", GetErrorMessage(r));

	return true;
}

result
EventDetailForm::OnInitializing(void)
{
	result r = E_SUCCESS;

	Header* pHeader = GetHeader();
	AppAssert(pHeader);
	pHeader->SetStyle(HEADER_STYLE_TITLE);
	pHeader->SetTitleText(L"상세 보기");

	Footer* pFooter = GetFooter();
	AppAssert(pFooter);
	pFooter->SetStyle(FOOTER_STYLE_BUTTON_TEXT);

	FooterItem footerEdit;
	footerEdit.Construct(ID_FOOTER_EDIT);
	footerEdit.SetText(L"수정");
	pFooter->AddItem(footerEdit);

	FooterItem footerDelete;
	footerDelete.Construct(ID_FOOTER_DELETE);
	footerDelete.SetText(L"삭제");
	pFooter->AddItem(footerDelete);

	pFooter->AddActionEventListener(*this);
	SetFormBackEventListener(this);

	static const unsigned int COLOR_BACKGROUND_LABEL = 0xFFEFEDE5;
	static const unsigned int COLOR_TITLE_LABEL = 0xFF808080;

	static const int UI_X_POSITION_GAP = 20;
	static const int UI_WIDTH = GetClientAreaBounds().width - 40;
	static const int UI_X_POSITION_MIDDLE = UI_WIDTH / 4 + UI_X_POSITION_GAP;
	static const int UI_HEIGHT = 112;
	static const int UI_SPACE = 26;
	int yPosition = 0;

	ScrollPanel* pScrollPanel = new (std::nothrow) ScrollPanel();
	pScrollPanel->Construct(Rectangle(0, 0, GetClientAreaBounds().width, GetClientAreaBounds().height));

	// Subject
	Label* pSubjectLabel = new (std::nothrow) Label();
	pSubjectLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition, UI_WIDTH, UI_HEIGHT), L"제목");
	pSubjectLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pSubjectLabel->SetTextColor(COLOR_TITLE_LABEL);
	pSubjectLabel->SetBackgroundColor(Color(COLOR_BACKGROUND_LABEL));
	pScrollPanel->AddControl(pSubjectLabel);

	__pSubjectLabelData = new (std::nothrow) Label();
	__pSubjectLabelData->Construct(Rectangle(UI_X_POSITION_MIDDLE, yPosition, UI_WIDTH * 3 / 4, UI_HEIGHT), L"");
	__pSubjectLabelData->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pScrollPanel->AddControl(__pSubjectLabelData);

	// Start Date
	Label* pStartDateLabel = new (std::nothrow) Label();
	pStartDateLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT + UI_SPACE, UI_WIDTH, UI_HEIGHT), L"시작일");
	pStartDateLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pStartDateLabel->SetTextColor(COLOR_TITLE_LABEL);
	pStartDateLabel->SetBackgroundColor(Color(COLOR_BACKGROUND_LABEL));
	pScrollPanel->AddControl(pStartDateLabel);

	__pStartDateLabelData = new (std::nothrow) Label();
	__pStartDateLabelData->Construct(Rectangle(UI_X_POSITION_MIDDLE, yPosition, UI_WIDTH * 3 / 4, UI_HEIGHT), L"");
	__pStartDateLabelData->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pScrollPanel->AddControl(__pStartDateLabelData);

	// End Date
	Label* pEndDateLabel = new (std::nothrow) Label();
	pEndDateLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT, UI_WIDTH, UI_HEIGHT), L"종료일");
	pEndDateLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pEndDateLabel->SetTextColor(COLOR_TITLE_LABEL);
	pEndDateLabel->SetBackgroundColor(Color(COLOR_BACKGROUND_LABEL));
	pScrollPanel->AddControl(pEndDateLabel);

	__pEndDateLabelData = new (std::nothrow) Label();
	__pEndDateLabelData->Construct(Rectangle(UI_X_POSITION_MIDDLE, yPosition, UI_WIDTH * 3 / 4, UI_HEIGHT), L"");
	__pEndDateLabelData->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pScrollPanel->AddControl(__pEndDateLabelData);

	// Location
	Label* pLocationLabel = new (std::nothrow) Label();
	pLocationLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT + UI_SPACE, UI_WIDTH, UI_HEIGHT), L"위치");
	pLocationLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pLocationLabel->SetTextColor(COLOR_TITLE_LABEL);
	pLocationLabel->SetBackgroundColor(Color(COLOR_BACKGROUND_LABEL));
	pScrollPanel->AddControl(pLocationLabel);

	__pLocationLabelData = new (std::nothrow) Label();
	__pLocationLabelData->Construct(Rectangle(UI_X_POSITION_MIDDLE, yPosition, UI_WIDTH * 3 / 4, UI_HEIGHT), L"");
	__pLocationLabelData->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pScrollPanel->AddControl(__pLocationLabelData);

	// Description
	Label* pDescriptionLabel = new (std::nothrow) Label();
	pDescriptionLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT, UI_WIDTH, UI_HEIGHT), L"설명");
	pDescriptionLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pDescriptionLabel->SetTextColor(COLOR_TITLE_LABEL);
	pDescriptionLabel->SetBackgroundColor(Color(COLOR_BACKGROUND_LABEL));
	pScrollPanel->AddControl(pDescriptionLabel);

	__pDescriptionLabelData = new (std::nothrow) Label();
	__pDescriptionLabelData->Construct(Rectangle(UI_X_POSITION_MIDDLE, yPosition, UI_WIDTH * 3 / 4, UI_HEIGHT), L"");
	__pDescriptionLabelData->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pScrollPanel->AddControl(__pDescriptionLabelData);

	// Reminder
	Label* pReminderLabel = new (std::nothrow) Label();
	pReminderLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT + UI_SPACE, UI_WIDTH, UI_HEIGHT), L"알림");
	pReminderLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pReminderLabel->SetTextColor(COLOR_TITLE_LABEL);
	pReminderLabel->SetBackgroundColor(Color(COLOR_BACKGROUND_LABEL));
	pScrollPanel->AddControl(pReminderLabel);

	__pReminderLabelData = new (std::nothrow) Label();
	__pReminderLabelData->Construct(Rectangle(UI_X_POSITION_MIDDLE, yPosition, UI_WIDTH * 3 / 4, UI_HEIGHT), L"");
	__pReminderLabelData->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pScrollPanel->AddControl(__pReminderLabelData);

	// Recurrence
	Label* pRecurrenceLabel = new (std::nothrow) Label();
	pRecurrenceLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT, UI_WIDTH, UI_HEIGHT), L"반복");
	pRecurrenceLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pRecurrenceLabel->SetTextColor(COLOR_TITLE_LABEL);
	pRecurrenceLabel->SetBackgroundColor(Color(COLOR_BACKGROUND_LABEL));
	pScrollPanel->AddControl(pRecurrenceLabel);

	__pRecurrenceLabelData = new (std::nothrow) Label();
	__pRecurrenceLabelData->Construct(Rectangle(UI_X_POSITION_MIDDLE, yPosition, UI_WIDTH * 3 / 4, UI_HEIGHT), L"");
	__pRecurrenceLabelData->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pScrollPanel->AddControl(__pRecurrenceLabelData);

	// Priority
	Label* pPriorityLabel = new (std::nothrow) Label();
	pPriorityLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT + UI_SPACE, UI_WIDTH, UI_HEIGHT), L"중요도");
	pPriorityLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pPriorityLabel->SetTextColor(COLOR_TITLE_LABEL);
	pPriorityLabel->SetBackgroundColor(Color(COLOR_BACKGROUND_LABEL));
	pScrollPanel->AddControl(pPriorityLabel);

	__pPriorityLabelData = new (std::nothrow) Label();
	__pPriorityLabelData->Construct(Rectangle(UI_X_POSITION_MIDDLE, yPosition, UI_WIDTH * 3 / 4, UI_HEIGHT), L"");
	__pPriorityLabelData->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pScrollPanel->AddControl(__pPriorityLabelData);

	// Sensitivity
	Label* pSensitivityLabel = new (std::nothrow) Label();
	pSensitivityLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT, UI_WIDTH, UI_HEIGHT), L"공개 여부");
	pSensitivityLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pSensitivityLabel->SetTextColor(COLOR_TITLE_LABEL);
	pSensitivityLabel->SetBackgroundColor(Color(COLOR_BACKGROUND_LABEL));
	pScrollPanel->AddControl(pSensitivityLabel);

	__pSensitivityLabelData = new (std::nothrow) Label();
	__pSensitivityLabelData->Construct(Rectangle(UI_X_POSITION_MIDDLE, yPosition, UI_WIDTH * 3 / 4, UI_HEIGHT), L"");
	__pSensitivityLabelData->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pScrollPanel->AddControl(__pSensitivityLabelData);

	// Status
	Label* pStatusLabel = new (std::nothrow) Label();
	pStatusLabel->Construct(Rectangle(UI_X_POSITION_GAP, yPosition += UI_HEIGHT, UI_WIDTH, UI_HEIGHT), L"상태");
	pStatusLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pStatusLabel->SetTextColor(COLOR_TITLE_LABEL);
	pStatusLabel->SetBackgroundColor(Color(COLOR_BACKGROUND_LABEL));
	pScrollPanel->AddControl(pStatusLabel);

	__pStatusLabelData = new (std::nothrow) Label();
	__pStatusLabelData->Construct(Rectangle(UI_X_POSITION_MIDDLE, yPosition, UI_WIDTH * 3 / 4, UI_HEIGHT), L"");
	__pStatusLabelData->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pScrollPanel->AddControl(__pStatusLabelData);

	AddControl(pScrollPanel);

	return r;
}

result
EventDetailForm::OnTerminating(void)
{
	if (__pCalendarbook != null)
	{
		delete __pCalendarbook;
	}

	if (__pCalEventInstance != null)
	{
		delete __pCalEventInstance;
	}

	return E_SUCCESS;
}

void
EventDetailForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	switch (actionId)
	{
	case ID_FOOTER_EDIT:
	{
		ArrayList* pList = new (std::nothrow) ArrayList();
		pList->Construct();
		pList->Add(*(new (std::nothrow) Integer(__pCalEventInstance->GetOriginalEventId())));
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_EVENT_EDITION, SCENE_TRANSITION_ANIMATION_TYPE_NONE, SCENE_HISTORY_OPTION_NO_HISTORY, SCENE_DESTROY_OPTION_DESTROY), pList);
	}
	break;

	case ID_FOOTER_DELETE:
	{
		if (__pCalEventInstance->IsRecurring() == true)
		{
			ShowDeleteRecurringEventPopup();
		}
		else
		{
			ShowDeleteEventMessageBox();
		}
	}
	break;

	default:
		break;
	}
}

void
EventDetailForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	pSceneManager->GoBackward(BackwardSceneTransition());
}

void
EventDetailForm::OnUserEventReceivedN(RequestId requestId, IList* pArgs)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	CloseDeleteRecurringEventPopup();
	Invalidate(true);

	switch (requestId)
	{
	case DeleteRecurringEventPopup::ID_BUTTON_SINGLE_INSTANCE:
		DeleteSingleInstance();
		pSceneManager->GoBackward(BackwardSceneTransition(SCENE_EVENT_LIST));
		break;

	case DeleteRecurringEventPopup::ID_BUTTON_ALL_INSTANCES:
		DeleteEvent();
		pSceneManager->GoBackward(BackwardSceneTransition(SCENE_EVENT_LIST));
		break;

	case DeleteRecurringEventPopup::ID_BUTTON_CANCEL:
		break;

	default:
		break;
	}
}

void
EventDetailForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId, const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
	if (pArgs != null)
	{
		__pCalEventInstance = static_cast< CalEventInstance* >(pArgs->GetAt(0));

		UpdateEventView();
		Invalidate(true);

		pArgs->RemoveAll(false);
		delete pArgs;
	}
}

void
EventDetailForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId, const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
}

void
EventDetailForm::UpdateEventView()
{
	result r = E_SUCCESS;

	String propertyValue;
	String date;

	// Subject
	propertyValue = __pCalEventInstance->GetSubject();
	if (propertyValue.IsEmpty() == true)
	{
		propertyValue = L"제목 없음";
	}
	__pSubjectLabelData->SetText(propertyValue);

	// Convert UTC time to local time
	DateTime startDate = __pCalEventInstance->GetStartTime();
	DateTime endDate = __pCalEventInstance->GetEndTime();

	if (__pCalEventInstance->IsAllDayEvent() == true)
	{
		// Start Date
		date = startDate.ToString();
		date.SetLength(10);
		__pStartDateLabelData->SetText(date);

		// End Date
		if(startDate < endDate)
		{
			endDate.AddDays(-1);
		}
		date = endDate.ToString();
		date.SetLength(10);
		__pEndDateLabelData->SetText(date);
	}
	else
	{
		LocaleManager localeManager;
		r = localeManager.Construct();
		TryReturnVoid(!IsFailed(r), "[%s] Failed to construct localeManager.", GetErrorMessage(r));

		TimeZone timeZone = localeManager.GetSystemTimeZone();
		startDate = timeZone.UtcTimeToWallTime(startDate);
		endDate = timeZone.UtcTimeToWallTime(endDate);

		// Start Date
		date = startDate.ToString();
		__pStartDateLabelData->SetText(date);

		// End Date
		date = endDate.ToString();
		__pEndDateLabelData->SetText(date);
	}

	// Location
	propertyValue = __pCalEventInstance->GetLocation();
	if (propertyValue.IsEmpty() == true)
	{
		propertyValue = L"위치 없음";
	}
	__pLocationLabelData->SetText(propertyValue);

	// Priority
	switch (__pCalEventInstance->GetPriority())
	{
	case EVENT_PRIORITY_LOW:
		__pPriorityLabelData->SetText(L"하");
		break;

	case EVENT_PRIORITY_NORMAL:
		__pPriorityLabelData->SetText(L"중");
		break;

	case EVENT_PRIORITY_HIGH:
		__pPriorityLabelData->SetText(L"상");
		break;

	default:
		break;
	}

	// Sensitivity
	switch (__pCalEventInstance->GetSensitivity())
	{
	case SENSITIVITY_PUBLIC:
		__pSensitivityLabelData->SetText(L"전체 공개");
		break;

	case SENSITIVITY_PRIVATE:
		__pSensitivityLabelData->SetText(L"친구 공개");
		break;

	case SENSITIVITY_CONFIDENTIAL:
		__pSensitivityLabelData->SetText(L"나만 보기");
		break;

	default:
		break;
	}

	// Status
	switch (__pCalEventInstance->GetStatus())
	{
	case EVENT_STATUS_NONE:
		__pStatusLabelData->SetText(L"없음");
		break;

	case EVENT_STATUS_CONFIRMED:
		__pStatusLabelData->SetText(L"완료됨");
		break;

	case EVENT_STATUS_CANCELLED:
		__pStatusLabelData->SetText(L"취소됨");
		break;

	case EVENT_STATUS_TENTATIVE:
		__pStatusLabelData->SetText(L"보류중");
		break;

	default:
		break;
	}

	// Reminder
	if (__pCalEventInstance->HasReminder() == true)
	{
		propertyValue = L"Yes";
	}
	else
	{
		propertyValue = L"No";
	}
	__pReminderLabelData->SetText(propertyValue);

	// Recurrence
	if (__pCalEventInstance->IsRecurring() == true)
	{
		propertyValue = L"Yes";
	}
	else
	{
		propertyValue = L"No";
	}
	__pRecurrenceLabelData->SetText(propertyValue);

	// Description
	propertyValue = __pCalEventInstance->GetDescription();
	if (propertyValue.IsEmpty() == true)
	{
		propertyValue = L"설명 없음";
	}
	__pDescriptionLabelData->SetText(propertyValue);

}

void
EventDetailForm::ShowDeleteRecurringEventPopup(void)
{
	__pDeleteRecurringEventPopup = new (std::nothrow) DeleteRecurringEventPopup();
	__pDeleteRecurringEventPopup->Construct(this);
	__pDeleteRecurringEventPopup->Show();
}

void
EventDetailForm::CloseDeleteRecurringEventPopup(void)
{
	delete __pDeleteRecurringEventPopup;
	__pDeleteRecurringEventPopup = null;
}

void
EventDetailForm::ShowDeleteEventMessageBox(void)
{
	result r = E_SUCCESS;

	MessageBox messageBox;
	r = messageBox.Construct(L"Delete event", "Are you sure you want to delete?", MSGBOX_STYLE_OKCANCEL, 0);
	TryReturnVoid(!IsFailed(r), "[%s] Failed to construct messageBox.", GetErrorMessage(r));

	int modalResult;
	messageBox.ShowAndWait(modalResult);

	if (modalResult == MSGBOX_RESULT_OK)
	{
		r = DeleteEvent();

		if (r == E_SUCCESS)
		{
			SceneManager* pSceneManager = SceneManager::GetInstance();
			AppAssert(pSceneManager);

			pSceneManager->GoBackward(BackwardSceneTransition(SCENE_EVENT_LIST));
		}
	}
}

result
EventDetailForm::DeleteSingleInstance(void)
{
	result r = E_SUCCESS;

	r = __pCalendarbook->RemoveEventInstance(*__pCalEventInstance);

	if (IsFailed(r))
	{
		MessageBox messageBox;
		messageBox.Construct(L"Error", "Failed to delete event instance", MSGBOX_STYLE_OK, 0);
		int doModal;
		messageBox.ShowAndWait(doModal);

		AppLogException("[%s] Failed to delete event instance.", GetErrorMessage(r));
	}

	return r;
}

result
EventDetailForm::DeleteEvent(void)
{
	result r = E_SUCCESS;

	r = __pCalendarbook->RemoveEvent(__pCalEventInstance->GetOriginalEventId());

	if (IsFailed(r))
	{
		MessageBox messageBox;
		messageBox.Construct(L"Error", "Failed to delete event(s)", MSGBOX_STYLE_OK, 0);
		int doModal;
		messageBox.ShowAndWait(doModal);

		AppLogException("[%s] Failed to delete event(s).", GetErrorMessage(r));
	}

	return r;
}
