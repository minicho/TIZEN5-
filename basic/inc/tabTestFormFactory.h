#ifndef _TAB_TEST_FORM_FACTORY_H_
#define _TAB_TEST_FORM_FACTORY_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <gl.h>

#include "CalendarViewerForm.h"
#include "MainForm.h"
#include "ChatMain.h"
#include "ChatForm.h"
#include "EventListForm.h"
#include "CreateEventForm.h"
#include "EventDetailForm.h"
#include "EditEventForm.h"
#include "SetRecurrenceForm.h"

//extern const wchar_t* FORM_MAIN_MENU;
//extern const wchar_t* FORM_CHATMAIN_MENU;
//extern const wchar_t* FORM_MAIN_2;

class tabTestFormFactory
	: public Tizen::Ui::Scenes::IFormFactory
{
public:
	tabTestFormFactory(void);
	virtual ~tabTestFormFactory(void);

	virtual Tizen::Ui::Controls::Form* CreateFormN(const Tizen::Base::String& formId, const Tizen::Ui::Scenes::SceneId& sceneId);
};

#endif // _TAB_TEST_FORM_FACTORY_H_
