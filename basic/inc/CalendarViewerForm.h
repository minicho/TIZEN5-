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

#ifndef _CALENDAR_VIEWER_FORM_H_
#define _CALENDAR_VIEWER_FORM_H_

#include <FUi.h>
#include <FBase.h>
#include <FLocales.h>
#include <time.h>

#include <FApp.h>
#include <FSystem.h>
#include <FAppUiApp.h>
#include <FWeb.h>
#include <FGraphics.h>
#include <FUiIme.h>
#include <gl.h>

#include <FNet.h>


using namespace Tizen::App;
using namespace Tizen::System;
using namespace Tizen::Base;
using namespace Tizen::Base::Runtime;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;
using namespace Tizen::Media;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Locales;
using namespace Tizen::Base::Collection;


class CalendarViewerForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::ITextEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
	, public Tizen::Ui::Scenes::ISceneEventListener
{
public:
	CalendarViewerForm(void);
	virtual ~CalendarViewerForm(void);

	bool Initialize(void);
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
	virtual void OnTextValueChanged(const Tizen::Ui::Control& source);
	virtual void OnTextValueChangeCanceled(const Tizen::Ui::Control& source);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);
    virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                           const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);

    virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId, const Tizen::Ui::Scenes::SceneId& nextSceneId);


private:
	result InitializeActionControls(void);
	result InitializeDayNumberButtons(void);
	bool UpdateCalendarDisplay(void);

public:
	enum ButtonActionId
	{
		ID_BUTTON_PREV_YEAR_ID = 105,
		ID_BUTTON_PREV_MONTH_ID,
		ID_BUTTON_NEXT_MONTH_ID,
		ID_BUTTON_NEXT_YEAR_ID,
	};

	static const int ACTION_BUTTON_COUNT = 4;
	static const int ACTION_LABEL_COUNT = 2;
	static const int DAY_TEXTBOX_COUNT = 42;

	static const int ID_CHATMAIN = 1;
	static const int ID_LOGIN = 2;

	static const int ID_HEADER_ITEM1 = 101;
	static const int ID_HEADER_ITEM2 = 102;
	static const int ID_HEADER_ITEM3 = 103;

	DateTime CurDateTime;

private:
	Tizen::Locales::Calendar* __pSelectCalendar;
	Tizen::Ui::Controls::EditField* __pEditFields[ACTION_LABEL_COUNT];
	Tizen::Ui::Controls::TextBox* __pTextBoxDay[DAY_TEXTBOX_COUNT];
	Tizen::Ui::Controls::Button* __pButtons[DAY_TEXTBOX_COUNT];


};

#endif // _CALENDAR_VIEWER_FORM_H_
