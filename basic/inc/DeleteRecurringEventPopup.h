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

#ifndef _DELETE_RECURRING_EVENT_POPUP_H_
#define _DELETE_RECURRING_EVENT_POPUP_H_

#include <FUi.h>

class DeleteRecurringEventPopup
	: public Tizen::Ui::Controls::Popup
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::IPropagatedKeyEventListener
{
public:
	DeleteRecurringEventPopup(void);
	virtual ~DeleteRecurringEventPopup(void);

public:
	result Construct(const Tizen::Ui::Controls::Form* pParentForm);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

	virtual bool OnKeyPressed(Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo);
	virtual bool OnKeyReleased(Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo);
	virtual bool OnPreviewKeyPressed(Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo);
	virtual bool OnPreviewKeyReleased(Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo);
	virtual bool TranslateKeyEventInfo(Control& source, Tizen::Ui::KeyEventInfo& keyEventInfo);

public:
	static const int ID_BUTTON_SINGLE_INSTANCE = 100;
	static const int ID_BUTTON_ALL_INSTANCES = 101;
	static const int ID_BUTTON_CANCEL = 102;

private:
	Tizen::Ui::Controls::Form* __pParentForm;

};
#endif // _DELETE_RECURRING_EVENT_POPUP_H_
