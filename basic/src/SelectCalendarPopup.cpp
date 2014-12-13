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
#include <FSocial.h>

#include "SelectCalendarPopup.h"
#include "AppResourceId.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Runtime;
using namespace Tizen::Base::Collection;
using namespace Tizen::Graphics;
using namespace Tizen::Social;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

SelectCalendarPopup::SelectCalendarPopup()
	: __pCalendarList(null)
	, __pListView(null)
	, __pParentForm(null)
	, __selectedCalendarId(null)
{
}

SelectCalendarPopup::~SelectCalendarPopup()
{
	if (__pCalendarList != null)
	{
		__pCalendarList->RemoveAll(true);
		delete __pCalendarList;
	}
}

result
SelectCalendarPopup::Construct(const Form* pParentForm, RecordId calendarId)
{
	result r = E_SUCCESS;

	r = Popup::Construct(true, Dimension(650, 800));
	TryReturn(!IsFailed(r), r, "[%s] Failed to construct popup.", GetErrorMessage(r));

	SetTitleText(L"달력 선택");
	__pParentForm = const_cast< Form* >(pParentForm);
	if (__pParentForm == null)
	{
		return E_FAILURE;
	}

	__selectedCalendarId = calendarId;

	__pListView = new (std::nothrow) ListView();
	__pListView->Construct(Rectangle(20, 0, GetClientAreaBounds().width - 40, GetClientAreaBounds().height - 89));
	__pListView->SetItemProvider(*this);
	__pListView->AddListViewItemEventListener(*this);

	AddControl(__pListView);

	Button* pButtonCancel = new (std::nothrow) Button();
	pButtonCancel->Construct(Rectangle((GetClientAreaBounds().width - 180) / 2, GetClientAreaBounds().height - 89, 180, 74), L"Cancel");
	pButtonCancel->SetActionId(ID_BUTTON_CANCEL_POPUP);
	pButtonCancel->AddActionEventListener(*this);
	AddControl(pButtonCancel);

	r = GetCalendarlist();
	TryReturn(!IsFailed(r), r, "[%s] Failed to get the calendar list.", GetErrorMessage(r));

	SetPropagatedKeyEventListener(this);

	return r;
}

void
SelectCalendarPopup::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
	switch (actionId)
	{
	case ID_BUTTON_CANCEL_POPUP:
		__pParentForm->SendUserEvent(0, null);
		break;

	default:
		break;
	}
}

bool
SelectCalendarPopup::OnKeyPressed(Control& source, const KeyEventInfo& keyEventInfo)
{
	return false;
}

bool
SelectCalendarPopup::OnKeyReleased(Control& source, const KeyEventInfo& keyEventInfo)
{
	if (keyEventInfo.GetKeyCode() == KEY_ESC || keyEventInfo.GetKeyCode() == KEY_BACK)
	{
		__pParentForm->SendUserEvent(0, null);
	}

	return false;
}

bool
SelectCalendarPopup::OnPreviewKeyPressed(Control& source, const KeyEventInfo& keyEventInfo)
{
	return false;
}

bool
SelectCalendarPopup::OnPreviewKeyReleased(Control& source, const KeyEventInfo& keyEventInfo)
{
	return false;
}

bool
SelectCalendarPopup::TranslateKeyEventInfo(Control& source, KeyEventInfo& keyEventInfo)
{
	return false;
}

void
SelectCalendarPopup::OnListViewItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListItemStatus status)
{
	if (status == LIST_ITEM_STATUS_SELECTED)
	{
		ArrayList* pList = new (std::nothrow) ArrayList();
		pList->Construct();

		Calendar* pCalendar = static_cast< Calendar* >(__pCalendarList->GetAt(index));
		pList->Add(*new (std::nothrow) Integer(pCalendar->GetRecordId()));

		__pParentForm->SendUserEvent(0, pList);
	}

}

void
SelectCalendarPopup::OnListViewItemSwept(Tizen::Ui::Controls::ListView& listView, int index, Tizen::Ui::Controls::SweepDirection direction)
{
}

void
SelectCalendarPopup::OnListViewContextItemStateChanged(Tizen::Ui::Controls::ListView& listView, int index, int elementId, Tizen::Ui::Controls::ListContextItemStatus state)
{
}

void
SelectCalendarPopup::OnItemReordered(Tizen::Ui::Controls::ListView& view, int oldIndex, int newIndex)
{
}

void
SelectCalendarPopup::OnListViewItemLongPressed(Tizen::Ui::Controls::ListView& listView, int index, int elementId, bool& invokeListViewItemCallback)
{
}

int
SelectCalendarPopup::GetItemCount(void)
{
	if (__pCalendarList != null)
	{
		return __pCalendarList->GetCount();
	}
	else
	{
		return 0;
	}

}

Tizen::Ui::Controls::ListItemBase*
SelectCalendarPopup::CreateItem(int index, int itemWidth)
{
	CustomItem* pItem = new (std::nothrow) CustomItem();
	Calendar* pCalendar = null;
	String calendarName;

	pCalendar = static_cast< Calendar* >(__pCalendarList->GetAt(index));
	TryReturn(pCalendar != null, null, "Failed to get a calendar");

	calendarName = pCalendar->GetName();
	if (calendarName.IsEmpty() == true)
	{
		calendarName = L"(Calendar ID: ";
		calendarName.Append(pCalendar->GetRecordId());
		calendarName.Append(L")");
	}
	pItem->Construct(Dimension(itemWidth, 100), LIST_ANNEX_STYLE_NORMAL);
	pItem->AddElement(Rectangle(32, 0, GetClientAreaBounds().width - 64, 112), 1, calendarName, true);
	if (__selectedCalendarId == pCalendar->GetRecordId())
	{
		__pListView->SetItemChecked(index, true);
	}
	return pItem;
}

bool
SelectCalendarPopup::DeleteItem(int index, Tizen::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	return true;
}

result
SelectCalendarPopup::GetCalendarlist(void)
{
	result r = E_SUCCESS;

	Calendarbook calendarbook;
	r = calendarbook.Construct();
	TryReturn(!IsFailed(r), false, "[%s] Failed to construct the calendarbook.", GetErrorMessage(r));

	CalendarbookFilter filter(CB_FI_TYPE_CALENDAR);
	filter.AppendInt(FI_CONJ_OP_NONE, CALENDAR_FI_PR_ITEM_TYPE, FI_CMP_OP_EQUAL, CALENDAR_ITEM_TYPE_EVENT_ONLY);
	filter.AppendInt(FI_CONJ_OP_OR, CALENDAR_FI_PR_ITEM_TYPE, FI_CMP_OP_EQUAL, CALENDAR_ITEM_TYPE_EVENT_AND_TODO);

	IList* pList = calendarbook.SearchN(filter, CALENDAR_FI_PR_CALENDAR_ID, SORT_ORDER_ASCENDING);

	if (IsFailed(GetLastResult()) || pList == null)
	{
		AppLogException("[%s] Failed to get the calendar list", GetErrorMessage(GetLastResult()));
	}

	__pCalendarList = pList;

	return r;
}
