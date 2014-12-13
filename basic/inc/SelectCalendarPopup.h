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

#ifndef _SELECT_CALENDAR_POPUP_H_
#define _SELECT_CALENDAR_POPUP_H_

#include <FBase.h>
#include <FSocial.h>
#include <FUi.h>

class SelectCalendarPopup
	: public Tizen::Ui::Controls::Popup
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::IPropagatedKeyEventListener
	, public Tizen::Ui::Controls::IListViewItemEventListener
	, public Tizen::Ui::Controls::IListViewItemProvider
{
public:
	SelectCalendarPopup();
	virtual ~SelectCalendarPopup();

	result Construct(const Tizen::Ui::Controls::Form* pParentForm, Tizen::Social::RecordId calendarId);

	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

	virtual bool OnKeyPressed(Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo);
	virtual bool OnKeyReleased(Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo);
	virtual bool OnPreviewKeyPressed(Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo);
	virtual bool OnPreviewKeyReleased(Control& source, const Tizen::Ui::KeyEventInfo& keyEventInfo);
	virtual bool TranslateKeyEventInfo(Control& source, Tizen::Ui::KeyEventInfo& keyEventInfo);

	virtual void OnListViewItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status);
	virtual void OnListViewItemSwept(Tizen::Ui::Controls::ListView& listView, int index, Tizen::Ui::Controls::SweepDirection direction);
	virtual void OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state);
	virtual void OnItemReordered(Tizen::Ui::Controls::ListView& view, int oldIndex, int newIndex);
	virtual void OnListViewItemLongPressed(Tizen::Ui::Controls::ListView& listView, int index, int elementId, bool& invokeListViewItemCallback);

	virtual int GetItemCount(void);
	virtual Tizen::Ui::Controls::ListItemBase* CreateItem(int index, int itemWidth);
	virtual bool DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth);

private:
	result GetCalendarlist(void);

private:
	static const int ID_BUTTON_CANCEL_POPUP = 100;

	Tizen::Base::Collection::IList* __pCalendarList;
	Tizen::Ui::Controls::ListView* __pListView;
	Tizen::Ui::Controls::Form* __pParentForm;
	Tizen::Social::RecordId __selectedCalendarId;
};

#endif // _SELECT_CALENDAR_POPUP_H_
