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

#ifndef _EVENT_DETAIL_FORM_H_
#define _EVENT_DETAIL_FORM_H_

#include <FBase.h>
#include <FUi.h>
#include <FSocial.h>

#include "DeleteRecurringEventPopup.h"

class EventDetailForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
	, public Tizen::Ui::Scenes::ISceneEventListener
{
public:
	EventDetailForm();
	virtual ~EventDetailForm();

	bool Initialize(void);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);
	virtual void OnUserEventReceivedN(RequestId requestId, Tizen::Base::Collection::IList* pArgs);

	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId, const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId, const Tizen::Ui::Scenes::SceneId& nextSceneId);

private:
	void UpdateEventView(void);

	void ShowDeleteRecurringEventPopup(void);
	void CloseDeleteRecurringEventPopup(void);
	void ShowDeleteEventMessageBox(void);
	result DeleteSingleInstance(void);
	result DeleteEvent(void);

private:
	static const int ID_FOOTER_EDIT = 100;
	static const int ID_FOOTER_DELETE = 101;
	static const int ID_FOOTER_BACK = 102;
	static const int ID_BUTTON_YES_POPUP = 200;
	static const int ID_BUTTON_CANCEL_POPUP = 201;

	Tizen::Ui::Controls::Label* __pSubjectLabelData;
	Tizen::Ui::Controls::Label* __pStartDateLabelData;
	Tizen::Ui::Controls::Label* __pEndDateLabelData;
	Tizen::Ui::Controls::Label* __pLocationLabelData;
	Tizen::Ui::Controls::Label* __pPriorityLabelData;
	Tizen::Ui::Controls::Label* __pSensitivityLabelData;
	Tizen::Ui::Controls::Label* __pStatusLabelData;
	Tizen::Ui::Controls::Label* __pReminderLabelData;
	Tizen::Ui::Controls::Label* __pRecurrenceLabelData;
	Tizen::Ui::Controls::Label* __pDescriptionLabelData;
	Tizen::Social::Calendarbook* __pCalendarbook;
	Tizen::Social::CalEventInstance* __pCalEventInstance;
	DeleteRecurringEventPopup* __pDeleteRecurringEventPopup;

};

#endif // _EVENT_DETAIL_FORM_H_
