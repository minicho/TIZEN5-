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

#ifndef _CREATE_EVENT_FORM_H_
#define _CREATE_EVENT_FORM_H_

#include <FApp.h>
#include <FBase.h>
#include <FSocial.h>
#include <FUi.h>

#include "SelectCalendarPopup.h"

class CreateEventForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
	, public Tizen::Ui::Scenes::ISceneEventListener
	, public Tizen::Ui::IDateChangeEventListener
	, public Tizen::Ui::ITimeChangeEventListener
{
public:
	CreateEventForm();
	virtual ~CreateEventForm();

	bool Initialize(void);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId, const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId, const Tizen::Ui::Scenes::SceneId& nextSceneId);

	virtual void OnDateChanged(const Tizen::Ui::Control& source, int year, int month, int day);
	virtual void OnDateChangeCanceled(const Tizen::Ui::Control& source);

	virtual void OnTimeChanged(const Tizen::Ui::Control& source, int hour, int minute);
	virtual void OnTimeChangeCanceled(const Tizen::Ui::Control& source);

	void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);

private:
	Tizen::Base::String GetTimezoneString(void) const;
	Tizen::Base::String GetRecurrenceString(void) const;
	void ShowSelectCalendarPopup(void);
	void DeleteCalendarPopup(void);
	result AddEvent(void);

private:
	static const int ID_FOOTER_SAVE = 100;
	static const int ID_BUTTON_CALENDAR = 200;
	static const int ID_BUTTON_REMINDER = 201;
	static const int ID_BUTTON_RECURRENCE = 202;
	static const int ID_BUTTON_PRIORITY = 203;
	static const int ID_BUTTON_SENSITIVITY = 204;
	static const int ID_BUTTON_STATUS = 205;
	static const int ID_CONTEXT_REMINDER_MINUTE = 300;
	static const int ID_CONTEXT_REMINDER_HOUR = 301;
	static const int ID_CONTEXT_REMINDER_DAY = 302;
	static const int ID_CONTEXT_REMINDER_WEEK = 303;
	static const int ID_CONTEXT_PRIORITY_HIGH = 304;
	static const int ID_CONTEXT_PRIORITY_NORMAL = 305;
	static const int ID_CONTEXT_PRIORITY_LOW = 306;
	static const int ID_CONTEXT_SENSITIVITY_PUBLIC = 307;
	static const int ID_CONTEXT_SENSITIVITY_PRIVATE = 308;
	static const int ID_CONTEXT_SENSITIVITY_CONFIDENTIAL = 309;
	static const int ID_CONTEXT_STATUS_NONE = 310;
	static const int ID_CONTEXT_STATUS_CONFIRMED = 311;
	static const int ID_CONTEXT_STATUS_CANCELLED = 312;
	static const int ID_CONTEXT_STATUS_TENTATIVE = 313;
	static const int ID_CHKBUTTON_ISALLDAY_CHECKED = 400;
	static const int ID_CHKBUTTON_ISALLDAY_UNCHECKED = 401;

	Tizen::Ui::Controls::EditField* __pSubjectEditField;
	Tizen::Ui::Controls::EditField* __pLocationEditField;
	Tizen::Ui::Controls::EditField* __pDescriptionEditField;
	Tizen::Ui::Controls::EditField* __pReminderEditField;
	Tizen::Ui::Controls::EditDate* __pStartEditDate;
	Tizen::Ui::Controls::EditDate* __pEndEditDate;
	Tizen::Ui::Controls::EditTime* __pStartEditTime;
	Tizen::Ui::Controls::EditTime* __pEndEditTime;
	Tizen::Ui::Controls::Button* __pSelectCalendarButton;
	Tizen::Ui::Controls::Button* __pReminderContextButton;
	Tizen::Ui::Controls::Button* __pSetRecurrenceButton;
	Tizen::Ui::Controls::Button* __pPriorityContextButton;
	Tizen::Ui::Controls::Button* __pSensitivityContextButton;
	Tizen::Ui::Controls::Button* __pStatusContextButton;
	Tizen::Ui::Controls::ContextMenu* __pReminderContextMenu;
	Tizen::Ui::Controls::ContextMenu* __pPriorityContextMenu;
	Tizen::Ui::Controls::ContextMenu* __pSensitivityContextMenu;
	Tizen::Ui::Controls::ContextMenu* __pStatusContextMenu;
	Tizen::Ui::Controls::CheckButton* __pIsAllDayButton;
	Tizen::Ui::Controls::TableView* __pTableView;
	Tizen::Social::Calendarbook* __pCalendarbook;
	Tizen::Social::Calendar* __pCalendar;
	Tizen::Social::Recurrence* __pRecurrence;
	Tizen::Social::ReminderTimeUnit __selectedReminder;
	Tizen::Social::EventPriority __selectedPriority;
	Tizen::Social::RecordSensitivity __selectedSensitivity;
	Tizen::Social::EventStatus __selectedStatus;
	SelectCalendarPopup* __pSelectCalendarPopup;
};

#endif // _CREATE_EVENT_FORM_H_
