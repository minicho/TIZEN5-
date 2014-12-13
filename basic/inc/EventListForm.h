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

#ifndef _EVENT_LIST_FORM_H_
#define _EVENT_LIST_FORM_H_

#include <FBase.h>
#include <FLocales.h>
#include <FSocial.h>
#include <FUi.h>

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

class EventListForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Social::ICalendarbookEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
	, public Tizen::Ui::Scenes::ISceneEventListener
	, public Tizen::Ui::Controls::IGroupedListViewItemEventListener
	, public Tizen::Ui::Controls::IGroupedListViewItemProvider
{
public:
	EventListForm();
	virtual ~EventListForm();

	bool Initialize(void);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId, const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId, const Tizen::Ui::Scenes::SceneId& nextSceneId);

	virtual void OnGroupedListViewItemStateChanged(Tizen::Ui::Controls::GroupedListView& listView, int groupIndex, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status);
	virtual void OnGroupedListViewItemSwept(Tizen::Ui::Controls::GroupedListView& listView, int groupIndex, int index, Tizen::Ui::Controls::SweepDirection direction);
	virtual void OnGroupedListViewContextItemStateChanged(Tizen::Ui::Controls::GroupedListView& listView, int groupIndex, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus status);

	virtual Tizen::Ui::Controls::GroupItem* CreateGroupItem(int groupIndex, int itemWidth);
	virtual Tizen::Ui::Controls::ListItemBase* CreateItem(int groupIndex, int index, int itemWidth);
	virtual bool DeleteItem(int groupIndex, int itemIndex, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth);
	virtual bool DeleteGroupItem(int groupIndex, Tizen::Ui::Controls::GroupItem* pItem, int itemWidth);
	virtual int GetGroupCount(void);
	virtual int GetItemCount(int groupIndex);

	virtual void OnCalendarEventsChanged(const Tizen::Base::Collection::IList& eventChangeInfoList);
	virtual void OnCalendarTodosChanged(const Tizen::Base::Collection::IList& todoChangeInfoList){}

private:
	void GetEventList(void);
	void LoadAllDayEvents(Tizen::Base::DateTime& eventDate);
	void LoadNonAllDayEvents(Tizen::Base::DateTime& eventDate);

	Tizen::Social::CalEventInstance* GetAllDayEventAt(int index);
	Tizen::Social::CalEventInstance* GetNonAllDayEventAt(int index);

private:
	static const int ID_HEADER_DAILY = 100;
	static const int ID_HEADER_MONTHLY = 101;
	static const int ID_FOOTER_CREATE = 200;
	static const int ID_BUTTON_PREV = 300;
	static const int ID_BUTTON_NEXT = 301;

	Tizen::Base::Collection::IList* __pAlldayEventsList;
	Tizen::Base::Collection::IList* __pNonAlldayEventsList;
	Tizen::Locales::TimeZone __timeZone;
	Tizen::Locales::Calendar* __pLocaleCalendar;
	Tizen::Social::Calendarbook* __pCalendarbook;
	Tizen::Ui::Controls::GroupedListView* __pGroupedListView;

	bool __isDaily;
	String __pString;
	int __pInt;
};

#endif // _EVENT_LIST_FORM_H_
