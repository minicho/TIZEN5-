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
#include <FApp.h>
#include <FSystem.h>

#include "EventListForm.h"
#include "AppResourceId.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Graphics;
using namespace Tizen::Locales;
using namespace Tizen::Social;
using namespace Tizen::System;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

static const int GROUP_COUNT = 2;
static const int H_GROUP_ITEM = 48;
static const int H_ITEM = 80;
static const unsigned int COLOR_ITEM_BACKGROUND = 0x0000000;

enum
{
	GROUP_INDEX_ALL_DAY_EVENT = 0,
	GROUP_INDEX_NON_ALL_DAY_EVENT
};

EventListForm::EventListForm()
	: __pAlldayEventsList(null)
	, __pNonAlldayEventsList(null)
	, __pLocaleCalendar(null)
	, __pCalendarbook(null)
	, __isDaily(true)
{
}

EventListForm::~EventListForm()
{
}

bool
EventListForm::Initialize(void)
{
	result r = E_SUCCESS;

	r = Construct(FORM_STYLE_NORMAL | FORM_STYLE_FOOTER | FORM_STYLE_HEADER | FORM_STYLE_PORTRAIT_INDICATOR);
	TryReturn(!IsFailed(r), false, "[%s] Failed to construct the form.", GetErrorMessage(r));

	__pCalendarbook = new (std::nothrow) Calendarbook();
	TryReturn(__pCalendarbook != null, false, "Failed to instantiate calendarbook.");

	r = __pCalendarbook->Construct(*this);
	if(IsFailed(r))
	{
		__pCalendarbook->Construct();
		__pCalendarbook->GetLatestVersion();
		r = GetLastResult();

		MessageBox messageBox;

		if(r == E_USER_NOT_CONSENTED)
		{
			messageBox.Construct(L"Error", L"The calendar privacy should be enabled.", MSGBOX_STYLE_OK);
		}
		else
		{
			messageBox.Construct(L"Error", L"Failed to construct the calendarbook.", MSGBOX_STYLE_OK);
		}

		int doModal;
		messageBox.ShowAndWait(doModal);

		UiApp* pApp = UiApp::GetInstance();
		AppAssert(pApp);
		pApp->Terminate();
	}

	return true;
}

result
EventListForm::OnInitializing(void)
{
	result r = E_SUCCESS;

	Header* pHeader = GetHeader();
	AppAssert(pHeader);

	DateTime today;
	String formattedString;

	SystemTime::GetCurrentTime(WALL_TIME, today);

	__pLocaleCalendar = Tizen::Locales::Calendar::CreateInstanceN(CALENDAR_GREGORIAN);
	__pLocaleCalendar->SetTime(today);
	DateTimeFormatter* pDateFormatter = DateTimeFormatter::CreateDateFormatterN(DATE_TIME_STYLE_DEFAULT);

	String customizedPattern = L"dd MMM yyyy";
	pDateFormatter->ApplyPattern(customizedPattern);
	pDateFormatter->Format(*__pLocaleCalendar, formattedString);

	HeaderItem headerDaily;
	headerDaily.Construct(ID_HEADER_DAILY);
	headerDaily.SetText(L"일");

	HeaderItem headerMonthly;
	headerMonthly.Construct(ID_HEADER_MONTHLY);
	headerMonthly.SetText(L"월");

	pHeader->SetStyle(HEADER_STYLE_SEGMENTED_WITH_TITLE);
	pHeader->SetTitleText(formattedString);
	pHeader->AddItem(headerDaily);
	pHeader->AddItem(headerMonthly);
	pHeader->AddActionEventListener(*this);

	Footer* pFooter = GetFooter();
	AppAssert(pFooter);
	pFooter->SetStyle(FOOTER_STYLE_BUTTON_TEXT);

	FooterItem footerCreate;
	footerCreate.Construct(ID_FOOTER_CREATE);
	footerCreate.SetText(L"일정 생성");
	pFooter->AddItem(footerCreate);

	SetFormBackEventListener(this);
	pFooter->AddActionEventListener(*this);

	Tizen::Ui::Controls::Button* pPreviousButton = new (std::nothrow) Button();
	pPreviousButton->Construct(Rectangle(0, 0, GetClientAreaBounds().width / 2, 72), L"이전");
	pPreviousButton->SetActionId(ID_BUTTON_PREV);
	pPreviousButton->AddActionEventListener(*this);
	AddControl(pPreviousButton);

	Tizen::Ui::Controls::Button* pNextButton = new (std::nothrow) Button();
	pNextButton->Construct(Rectangle(GetClientAreaBounds().width / 2, 0, GetClientAreaBounds().width / 2, 72), L"다음");
	pNextButton->SetActionId(ID_BUTTON_NEXT);
	pNextButton->AddActionEventListener(*this);
	AddControl(pNextButton);

	__pGroupedListView = new (std::nothrow) GroupedListView();
	__pGroupedListView->Construct(Rectangle(0, 72, GetClientAreaBounds().width, GetClientAreaBounds().height - 72), GROUPED_LIST_VIEW_STYLE_INDEXED, true, SCROLL_STYLE_FADE_OUT);
	__pGroupedListView->SetTextOfEmptyList(L"일정 없음");
	__pGroupedListView->SetItemProvider(*this);
	__pGroupedListView->AddGroupedListViewItemEventListener(*this);
	__pGroupedListView->SetItemDividerColor(Tizen::Graphics::Color::GetColor(COLOR_ID_BLACK));
	AddControl(__pGroupedListView);

	LocaleManager localeManager;
	localeManager.Construct();
	__timeZone = localeManager.GetSystemTimeZone();

	return r;
}

result
EventListForm::OnTerminating(void)
{
	if (__pCalendarbook != null)
	{
		delete __pCalendarbook;
	}

	if (__pLocaleCalendar != null)
	{
		delete __pLocaleCalendar;
	}

	if (__pAlldayEventsList != null)
	{
		__pAlldayEventsList->RemoveAll(true);
		delete __pAlldayEventsList;
	}

	if (__pNonAlldayEventsList != null)
	{
		__pNonAlldayEventsList->RemoveAll(true);
		delete __pNonAlldayEventsList;
	}

	return E_SUCCESS;
}

void
EventListForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
	String formattedString;
	DateTimeFormatter* pDateFormatter = DateTimeFormatter::CreateDateFormatterN(DATE_TIME_STYLE_DEFAULT);
	String customizedPattern;

	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	switch (actionId)
	{
	case ID_FOOTER_CREATE:
		pSceneManager->GoForward(ForwardSceneTransition(SCENE_EVENT_CREATION));
		break;

	case ID_HEADER_DAILY:
	{
		customizedPattern = L"dd MMM yyyy";
		pDateFormatter->ApplyPattern(customizedPattern);
		pDateFormatter->Format(*__pLocaleCalendar, formattedString);
		GetHeader()->SetTitleText(formattedString);
		GetHeader()->Invalidate(false);
		__isDaily = true;
		GetEventList();
		__pGroupedListView->UpdateList();
	}
	break;

	case ID_HEADER_MONTHLY:
	{
		customizedPattern = L"MMM yyyy";
		pDateFormatter->ApplyPattern(customizedPattern);
		pDateFormatter->Format(*__pLocaleCalendar, formattedString);
		GetHeader()->SetTitleText(formattedString);
		GetHeader()->Invalidate(false);
		__isDaily = false;
		GetEventList();
		__pGroupedListView->UpdateList();
	}
	break;

	case ID_BUTTON_PREV:
	{
		if (__isDaily == true)
		{
			__pLocaleCalendar->AddTimeField(TIME_FIELD_DAY_OF_MONTH, -1);
			customizedPattern = L"dd MMM yyyy";
			pDateFormatter->ApplyPattern(customizedPattern);
			pDateFormatter->Format(*__pLocaleCalendar, formattedString);
			GetHeader()->SetTitleText(formattedString);
			GetHeader()->Invalidate(false);
			GetEventList();
			__pGroupedListView->UpdateList();
		}
		else
		{
			__pLocaleCalendar->AddTimeField(TIME_FIELD_MONTH, -1);
			customizedPattern = L"MMM yyyy";
			pDateFormatter->ApplyPattern(customizedPattern);
			pDateFormatter->Format(*__pLocaleCalendar, formattedString);
			GetHeader()->SetTitleText(formattedString);
			GetHeader()->Invalidate(false);
			GetEventList();
			__pGroupedListView->UpdateList();
		}
	}
	break;

	case ID_BUTTON_NEXT:
	{
		if (__isDaily == true)
		{
			__pLocaleCalendar->AddTimeField(TIME_FIELD_DAY_OF_MONTH, 1);
			customizedPattern = L"dd MMM yyyy";
			pDateFormatter->ApplyPattern(customizedPattern);
			pDateFormatter->Format(*__pLocaleCalendar, formattedString);
			GetHeader()->SetTitleText(formattedString);
			GetHeader()->Invalidate(false);
			GetEventList();
			__pGroupedListView->UpdateList();
		}
		else
		{
			__pLocaleCalendar->AddTimeField(TIME_FIELD_MONTH, 1);
			customizedPattern = L"MMM yyyy";
			pDateFormatter->ApplyPattern(customizedPattern);
			pDateFormatter->Format(*__pLocaleCalendar, formattedString);
			GetHeader()->SetTitleText(formattedString);
			GetHeader()->Invalidate(false);
			GetEventList();
			__pGroupedListView->UpdateList();
		}
	}
	break;
	}
}

void
EventListForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	 SceneManager* pSceneManager = SceneManager::GetInstance();
	 AppAssert(pSceneManager);

	 pSceneManager->GoBackward(BackwardSceneTransition(SCENE_CALENDAR_FORM));
}

void
EventListForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId, const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
    if (pArgs != null)
    {
        if (pArgs->GetCount() == 2)
        {
            String* pString = dynamic_cast<String*>(pArgs->GetAt(0));
            //TryCatch(pServer != null, , "Failed to retrieve the client/server from the Array list.");

            __pString = *pString;
            if(__pString != null)
            {
            	//int* pInt = dynamic_cast<int*>(pArgs->GetAt(1));

            	//__pInt = pInt;

            	//Tizen::Locales::Calendar* cal = dynamic_cast<Tizen::Locales::Calendar*>(pArgs->GetAt(1));
            	Tizen::Locales::Calendar* cal = dynamic_cast<Tizen::Locales::Calendar*>(pArgs->GetAt(1));
            	__pLocaleCalendar = cal;
            }
        }
    	//pArgs->RemoveAll();
    	//delete pArgs;
    }

	GetEventList();
	__pGroupedListView->UpdateList();

//CATCH:
	//pArgs->RemoveAll(true);
	//pArgs->RemoveAll();
	//delete pArgs;
}

void
EventListForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId, const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
}

void
EventListForm::OnGroupedListViewItemStateChanged(Tizen::Ui::Controls::GroupedListView& listView, int groupIndex, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status)
{
	if (status == LIST_ITEM_STATUS_SELECTED)
	{
		CalEventInstance* pCalEventInstance = null;

		if (groupIndex == GROUP_INDEX_ALL_DAY_EVENT)
		{
			pCalEventInstance = GetAllDayEventAt(index);
		}
		else
		{
			pCalEventInstance = GetNonAllDayEventAt(index);
		}

		if (pCalEventInstance != null)
		{
			SceneManager* pSceneManager = SceneManager::GetInstance();
			AppAssert(pSceneManager);

			ArrayList* pList = new (std::nothrow) ArrayList();
			pList->Construct();
			pList->Add(*(new (std::nothrow) CalEventInstance(*pCalEventInstance)));
			pSceneManager->GoForward(ForwardSceneTransition(SCENE_EVENT_DETAIL), pList);
		}
	}
}

void
EventListForm::OnGroupedListViewItemSwept(Tizen::Ui::Controls::GroupedListView& listView, int groupIndex, int index, Tizen::Ui::Controls::SweepDirection direction)
{
}

void
EventListForm::OnGroupedListViewContextItemStateChanged(Tizen::Ui::Controls::GroupedListView& listView, int groupIndex, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus status)
{
}

Tizen::Ui::Controls::GroupItem*
EventListForm::CreateGroupItem(int groupIndex, int itemWidth)
{
	GroupItem* pItem = new (std::nothrow) GroupItem();
	pItem->Construct(Dimension(itemWidth, H_GROUP_ITEM));
	pItem->SetBackgroundColor(Color(COLOR_ITEM_BACKGROUND));

	if (groupIndex == GROUP_INDEX_ALL_DAY_EVENT)
	{
		pItem->SetElement(L"모든 일정");
	}
	else
	{
		pItem->SetElement(L"일정 목록");
	}

	return pItem;
}

Tizen::Ui::Controls::ListItemBase*
EventListForm::CreateItem(int groupIndex, int index, int itemWidth)
{
	SimpleItem* pItem = new (std::nothrow) SimpleItem();
	pItem->Construct(Dimension(itemWidth, H_ITEM), LIST_ANNEX_STYLE_NORMAL);

	String listItemString;

	CalEventInstance* pEventInstance = null;

	switch (groupIndex)
	{
	case GROUP_INDEX_ALL_DAY_EVENT:
	{
		pEventInstance = GetAllDayEventAt(index);

		if (pEventInstance != null)
		{
			listItemString = pEventInstance->GetSubject();
			if (listItemString.IsEmpty() == true)
			{
				listItemString = L"제목 없음";
			}
		}
		else
		{
			if (index == 0)
			{
				listItemString = L"일정 없음";
			}
		}
	}
	break;

	case GROUP_INDEX_NON_ALL_DAY_EVENT:
	{
		pEventInstance = GetNonAllDayEventAt(index);
		if (pEventInstance != null)
		{
			listItemString = pEventInstance->GetSubject();
			if (listItemString.IsEmpty() == true)
			{
				listItemString = L"제목 없음";
			}
		}
		else
		{
			if (index == 0)
			{
				listItemString = L"일정 없음";
			}
		}
	}
	break;

	default:
		break;
	}
	pItem->SetElement(listItemString);
	return pItem;
}

bool
EventListForm::DeleteItem(int groupIndex, int itemIndex, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	return true;
}

bool
EventListForm::DeleteGroupItem(int groupIndex, Tizen::Ui::Controls::GroupItem* pItem, int itemWidth)
{
	delete pItem;
	return true;
}

int
EventListForm::GetGroupCount(void)
{
	return GROUP_COUNT;
}

int
EventListForm::GetItemCount(int groupIndex)
{
	int itemCount = 0;

	if (groupIndex == GROUP_INDEX_ALL_DAY_EVENT)
	{
		if (__pAlldayEventsList != null)
		{
			itemCount = __pAlldayEventsList->GetCount();
		}
		if (itemCount == 0)
		{
			itemCount = 1;
		}
	}
	else
	{
		if (__pNonAlldayEventsList != null)
		{
			itemCount = __pNonAlldayEventsList->GetCount();
		}
		if (itemCount == 0)
		{
			itemCount = 1;
		}
	}
	return itemCount;
}

void
EventListForm::LoadAllDayEvents(Tizen::Base::DateTime& eventDate)
{

	DateTime startTime(eventDate);
	DateTime endTime(eventDate);

	if (__isDaily)
	{
		startTime.SetValue(eventDate.GetYear(), eventDate.GetMonth(), eventDate.GetDay(), 0, 0, 0);
		endTime = startTime;
		endTime.AddDays(1);
	}
	else
	{
		startTime.SetValue(eventDate.GetYear(), eventDate.GetMonth(), 1, 0, 0, 0);
		endTime = startTime;
		endTime.AddMonths(1);
	}

	CalendarbookFilter filter(CB_FI_TYPE_ALL_DAY_EVENT_INSTANCE);
	filter.AppendDateTime(FI_CONJ_OP_NONE, EVENT_INST_FI_PR_END_TIME, FI_CMP_OP_GREATER_THAN, startTime);
	filter.AppendDateTime(FI_CONJ_OP_AND, EVENT_INST_FI_PR_START_TIME, FI_CMP_OP_LESS_THAN, endTime);

	IList* pList = __pCalendarbook->SearchN(filter, EVENT_INST_FI_PR_START_TIME, SORT_ORDER_ASCENDING);

	result r = GetLastResult();

	if (IsFailed(r))
	{
		MessageBox messageBox;
		messageBox.Construct(L"Error", "Failed to get the all day event list", MSGBOX_STYLE_OK, 0);
		int doModal;
		messageBox.ShowAndWait(doModal);

		AppLogException("[%s] Failed to get the all day event list.", GetErrorMessage(r));
	}

	if (__pAlldayEventsList != null)
	{
		__pAlldayEventsList->RemoveAll(true);
		delete __pAlldayEventsList;
	}
	__pAlldayEventsList = pList;
}

void
EventListForm::LoadNonAllDayEvents(Tizen::Base::DateTime& eventDate)
{

	DateTime startTime(eventDate);
	DateTime endTime(eventDate);

	if (__isDaily == true)
	{
		startTime.SetValue(eventDate.GetYear(), eventDate.GetMonth(), eventDate.GetDay(), 0, 0, 0);
		endTime = startTime;
		endTime.AddDays(1);
	}
	else
	{
		startTime.SetValue(eventDate.GetYear(), eventDate.GetMonth(), 1, 0, 0, 0);
		endTime = startTime;
		endTime.AddMonths(1);
	}

	DateTime utcStartTime = __timeZone.WallTimeToUtcTime(startTime);
	DateTime utcEndTime = __timeZone.WallTimeToUtcTime(endTime);

	CalendarbookFilter filter(CB_FI_TYPE_NON_ALL_DAY_EVENT_INSTANCE);
	filter.AppendDateTime(FI_CONJ_OP_NONE, EVENT_INST_FI_PR_END_TIME, FI_CMP_OP_GREATER_THAN, utcStartTime);
	filter.AppendDateTime(FI_CONJ_OP_AND, EVENT_INST_FI_PR_START_TIME, FI_CMP_OP_LESS_THAN, utcEndTime);

	IList* pList = __pCalendarbook->SearchN(filter, EVENT_INST_FI_PR_START_TIME, SORT_ORDER_ASCENDING);

	result r = GetLastResult();

	if (IsFailed(r))
	{
		MessageBox messageBox;
		messageBox.Construct(L"Error", "Failed to get the event list", MSGBOX_STYLE_OK, 0);
		int doModal;
		messageBox.ShowAndWait(doModal);

		AppLogException("[%s] Failed to get the event list.", GetErrorMessage(r));
	}

	if (__pNonAlldayEventsList != null)
	{
		__pNonAlldayEventsList->RemoveAll(true);
		delete __pNonAlldayEventsList;
	}
	__pNonAlldayEventsList = pList;
}

void
EventListForm::GetEventList(void)
{
	//local time
	DateTime eventDate = __pLocaleCalendar->GetTime();
	LoadAllDayEvents(eventDate);
	LoadNonAllDayEvents(eventDate);
}

CalEventInstance*
EventListForm::GetAllDayEventAt(int index)
{
	if (__pAlldayEventsList == null || index < 0 || index >= __pAlldayEventsList->GetCount())
	{
		return null;
	}

	return static_cast< CalEventInstance* >(__pAlldayEventsList->GetAt(index));
}

CalEventInstance*
EventListForm::GetNonAllDayEventAt(int index)
{
	if (__pNonAlldayEventsList == null || index < 0 || index >= __pNonAlldayEventsList->GetCount())
	{
		return null;
	}

	return static_cast< CalEventInstance* >(__pNonAlldayEventsList->GetAt(index));
}

void
EventListForm::OnCalendarEventsChanged(const Tizen::Base::Collection::IList& eventChangeInfoList)
{
	GetEventList();
	__pGroupedListView->UpdateList();
}

