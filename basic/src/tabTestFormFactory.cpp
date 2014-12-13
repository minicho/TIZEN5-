#include <new>
#include "tabTestFormFactory.h"
#include "AppResourceId.h"
#include "CalendarViewerForm.h"
#include "MainForm.h"
#include "ChatMain.h"
#include "ChatForm.h"
#include "EventListForm.h"
#include "CreateEventForm.h"
#include "EventDetailForm.h"
#include "EditEventForm.h"
#include "SetRecurrenceForm.h"

using namespace Tizen::Ui::Scenes;


tabTestFormFactory::tabTestFormFactory(void)
{
}

tabTestFormFactory::~tabTestFormFactory(void)
{
}

Tizen::Ui::Controls::Form*
tabTestFormFactory::CreateFormN(const Tizen::Base::String& formId, const Tizen::Ui::Scenes::SceneId& sceneId)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);
	Tizen::Ui::Controls::Form* pNewForm = null;


	if (formId == FORM_CALENDAR)
	{
		CalendarViewerForm* pForm = new (std::nothrow) CalendarViewerForm();
		TryReturn(pForm != null, null, "The memory is insufficient.");
		pForm->Initialize();
		pNewForm = pForm;
	}

	else if (formId == FORM_CHATMAIN)
	{
		ChatMain* pMainFrm = new (std::nothrow) ChatMain();
		pMainFrm->Initialize();
		pNewForm = pMainFrm;
	}
	else if (formId == FORM_CHAT)
	{
		AppLog("FORM CHAT MENU! \n");

		ChatForm* pChatForm = new (std::nothrow) ChatForm();
		if (pChatForm == null)
		{
			int result;
			MessageBox* pMsgBox = new (std::nothrow) MessageBox();
			pMsgBox->Construct(L"Error", L"Error in allocating Memory", MSGBOX_STYLE_OK);
			pMsgBox->ShowAndWait(result);
			delete pMsgBox;
		}

		pChatForm->Initialize();
		AppLog("Initialize :Exit");
		pSceneManager->AddSceneEventListener(sceneId, *pChatForm);
		pNewForm = pChatForm;

	}
	else if(formId == FORM_WEBLINK)
	{
		MainForm* pForm = new (std::nothrow) MainForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}

	else if (formId == FORM_EVENT_LIST)
	{
		EventListForm* pForm = new (std::nothrow) EventListForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}
	else if (formId == FORM_EVENT_CREATION)
	{
		CreateEventForm* pForm = new (std::nothrow) CreateEventForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}
	else if (formId == FORM_EVENT_DETAIL)
	{
		EventDetailForm* pForm = new (std::nothrow) EventDetailForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}
	else if (formId == FORM_EVENT_EDITION)
	{
		EditEventForm* pForm = new (std::nothrow) EditEventForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}
	else if (formId == FORM_EVENT_RECURRENCE)
	{
		SetRecurrenceForm* pForm = new (std::nothrow) SetRecurrenceForm();
		pForm->Initialize();
		pSceneManager->AddSceneEventListener(sceneId, *pForm);
		pNewForm = pForm;
	}
	// TODO: Add your form creation code here

	return pNewForm;
}
