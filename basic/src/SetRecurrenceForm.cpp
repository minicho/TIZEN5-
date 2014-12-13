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
#include <FGraphics.h>
#include <FLocales.h>
#include <FSystem.h>

#include "SetRecurrenceForm.h"
#include "AppResourceId.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Graphics;
using namespace Tizen::Locales;
using namespace Tizen::Social;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;

static const int SECTION_COUNT = 2;
static const int SECTION_ITEM_COUNT_TYPE = 6;
static const int SECTION_ITEM_COUNT_UNTIL = 2;
static const int H_ITEM = 128;
static const int FONT_SIZE_REPEAT_UNTIL_TYPE = 44;

static const unsigned int COLOR_ITEM_BACKGROUND = 0x0000000;
static const unsigned int COLOR_TEXT_REPEAT = 0xFF000000;

enum
{
	SECTION_TYPE_REPEAT_TYPE = 0,
	SECTION_TYPE_REPEAT_UNTIL
};

SetRecurrenceForm::SetRecurrenceForm(void)
	: __pTableView(null)
	, __pUntilDateEditDate(null)
	, __pRepeatCountEditField(null)
	, __repeatType(REPEAT_TYPE_NONE)
	, __repeatUntilType(REPEAT_UNTIL_TYPE_COUNT)
	, __calDayOfWeek(CAL_SUNDAY)
	, __weekOfMonth(1)
	, __month(1)
	, __day(1)
	, __count(1)
{
}

SetRecurrenceForm::~SetRecurrenceForm(void)
{
}

bool
SetRecurrenceForm::Initialize(void)
{
	result r = E_SUCCESS;

	r = Construct(FORM_STYLE_NORMAL | FORM_STYLE_FOOTER | FORM_STYLE_HEADER | FORM_STYLE_PORTRAIT_INDICATOR);
	TryReturn(!IsFailed(r), false, "[%s] Failed to construct the form.", GetErrorMessage(r));

	return true;
}

result
SetRecurrenceForm::OnInitializing(void)
{
	result r = E_SUCCESS;

	Header* pHeader = GetHeader();
	AppAssert(pHeader);
	pHeader->SetStyle(HEADER_STYLE_TITLE);
	pHeader->SetTitleText(L"Recurrence");

	Footer* pFooter = GetFooter();
	AppAssert(pFooter);
	pFooter->SetStyle(FOOTER_STYLE_BUTTON_TEXT);

	FooterItem footerSave;
	footerSave.Construct(ID_FOOTER_DONE);
	footerSave.SetText(L"Done");
	pFooter->AddItem(footerSave);

	pFooter->AddActionEventListener(*this);
	SetFormBackEventListener(this);

	Rectangle bounds = GetClientAreaBounds();
	bounds.x = 0;
	bounds.y = 0;

	__pTableView = new (std::nothrow) SectionTableView();
	__pTableView->Construct(bounds, true, TABLE_VIEW_SCROLL_BAR_STYLE_FADE_OUT);
	__pTableView->SetItemProvider(this);
	__pTableView->AddSectionTableViewItemEventListener(*this);
	__pTableView->SetGroupedLookEnabled(true);
	AddControl(__pTableView);

	return r;
}

result
SetRecurrenceForm::OnTerminating(void)
{
	return E_SUCCESS;
}

void
SetRecurrenceForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	switch (actionId)
	{
	case ID_FOOTER_DONE:
	{
		ArrayList* pList = new (std::nothrow) ArrayList();
		pList->Construct();

		Recurrence* pRecurrence = CreateRecurrenceN();

		if (pRecurrence != null)
		{
			pList->Add(*(new (std::nothrow) Recurrence(*pRecurrence)));
		}

		pSceneManager->GoBackward(BackwardSceneTransition(), pList);

	}
	break;
	}
}

int
SetRecurrenceForm::GetSectionCount(void)
{
	return SECTION_COUNT;
}

int
SetRecurrenceForm::GetItemCount(int sectionIndex)
{
	if (sectionIndex == SECTION_TYPE_REPEAT_TYPE)
	{
		return SECTION_ITEM_COUNT_TYPE;
	}
	else //(sectionIndex == SECTION_TYPE_UNTILE_TYPE)
	{
		return SECTION_ITEM_COUNT_UNTIL;
	}
}

TableViewItem*
SetRecurrenceForm::CreateItem(int sectionIndex, int itemIndex, int itemWidth)
{
	TableViewItem* pItem = new (std::nothrow) TableViewItem();
	pItem->Construct(Dimension(itemWidth, H_ITEM), TABLE_VIEW_ANNEX_STYLE_RADIO);
	pItem->SetBackgroundColor(Color(COLOR_ITEM_BACKGROUND), TABLE_VIEW_ITEM_DRAWING_STATUS_NORMAL);
	pItem->SetBackgroundColor(Color(COLOR_ITEM_BACKGROUND), TABLE_VIEW_ITEM_DRAWING_STATUS_PRESSED);
	pItem->SetBackgroundColor(Color(COLOR_ITEM_BACKGROUND), TABLE_VIEW_ITEM_DRAWING_STATUS_HIGHLIGHTED);

	Label* pLabel = new (std::nothrow) Label();
	Rectangle itemBoundType;
	itemBoundType.SetBounds(0, 0, GetClientAreaBounds().width * 0.7, H_ITEM);
	Rectangle itemBoundUntil;
	itemBoundUntil.SetBounds(0, 0, GetClientAreaBounds().width * 0.3, H_ITEM);
	Rectangle dataBoundUntil;
	dataBoundUntil.SetBounds(GetClientAreaBounds().width * 0.3, 0, GetClientAreaBounds().width * 0.3, H_ITEM);

	if (sectionIndex == SECTION_TYPE_REPEAT_TYPE)
	{
		String repeatString;

		switch (itemIndex)
		{
		case REPEAT_TYPE_NONE:
			repeatString = L"None";
			break;

		case REPEAT_TYPE_DAILY:
			repeatString = L"Daily";
			break;

		case REPEAT_TYPE_WEEKLY:
			//format: Weekly (Every Monday)
			repeatString = L"Weekly (Every ";
			repeatString.Append(GetCalDayOfWeekString(__calDayOfWeek));
			repeatString.Append(L")");
			break;

		case REPEAT_TYPE_MONTHLY:
			//format: Monthly (Every 3st Monday)
			repeatString = L"Monthly (Every ";
			repeatString.Append(__weekOfMonth);
			switch (__weekOfMonth)
			{
			case 1:
				repeatString.Append(L"st ");
				break;

			case 2:
				repeatString.Append(L"nd ");
				break;

			case 3:
				repeatString.Append(L"rd ");
				break;

			default:
				repeatString.Append(L"th ");
				break;
			}
			repeatString.Append(GetCalDayOfWeekString(__calDayOfWeek));
			repeatString.Append(L")");
			break;

		case REPEAT_TYPE_MONTHLY_DATE:
			//format: Monthly (On day 15)
			repeatString = L"Monthly (On day";
			repeatString.Append(__day);
			repeatString.Append(L")");
			break;

		case REPEAT_TYPE_YEARLY_DATE:
			//format: Yearly (On Jan. 15)
			repeatString = L"Yearly (On ";
			repeatString.Append(GetMonthString(__month));
			repeatString.Append(__day);
			repeatString.Append(L")");
			break;

		default:
			break;
		}
		pLabel->Construct(itemBoundType, repeatString);
	}
	else //(sectionIndex == SECTION_TYPE_REPEATE_UNTIL)
	{
		bool isEnabled = false;

		switch (itemIndex)
		{
		case REPEAT_UNTIL_TYPE_DATE:
		{
			pLabel->Construct(itemBoundUntil, L"Until");

			int minYear = Calendarbook::GetMinDateTime().GetYear() + 1;
			int maxYear = Calendarbook::GetMaxDateTime().GetYear() - 1;

			__pUntilDateEditDate = new (std::nothrow) EditDate();
			__pUntilDateEditDate->Construct(Point(GetClientAreaBounds().width / 4, 0));
			__pUntilDateEditDate->SetDate(__untilDate);
			__pUntilDateEditDate->SetYearRange(minYear, maxYear);

			if (__repeatType != REPEAT_TYPE_NONE && __repeatUntilType == REPEAT_UNTIL_TYPE_DATE)
			{
				isEnabled = true;
			}
			__pUntilDateEditDate->SetEnabled(isEnabled);
			pItem->AddControl(__pUntilDateEditDate);
			pItem->SetIndividualSelectionEnabled(__pUntilDateEditDate, true);
		}
		break;

		case REPEAT_UNTIL_TYPE_COUNT:
			pLabel->Construct(itemBoundUntil, L"Count");
			__pRepeatCountEditField = new (std::nothrow) EditField();
			__pRepeatCountEditField->Construct(dataBoundUntil, EDIT_FIELD_STYLE_NUMBER, INPUT_STYLE_FULLSCREEN, false, 3);

			if (__count > 0)
			{
				__pRepeatCountEditField->SetText(Integer::ToString(__count));
			}

			__pRepeatCountEditField->SetTextAlignment(ALIGNMENT_CENTER);

			if (__repeatType != REPEAT_TYPE_NONE && __repeatUntilType == REPEAT_UNTIL_TYPE_COUNT)
			{
				isEnabled = true;
			}

			__pRepeatCountEditField->SetEnabled(isEnabled);
			pItem->AddControl(__pRepeatCountEditField);
			pItem->SetIndividualSelectionEnabled(__pRepeatCountEditField, true);
			break;

		default:
			break;
		}
	}

	pLabel->SetTextConfig(FONT_SIZE_REPEAT_UNTIL_TYPE, LABEL_TEXT_STYLE_NORMAL);
	pLabel->SetTextColor(Color(COLOR_TEXT_REPEAT));
	pLabel->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pItem->AddControl(pLabel);

	return pItem;
}

bool
SetRecurrenceForm::DeleteItem(int sectionIndex, int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem)
{
	delete pItem;
	return true;
}

void
SetRecurrenceForm::UpdateItem(int sectionIndex, int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem)
{
}

String
SetRecurrenceForm::GetSectionHeader(int sectionIndex)
{
	switch (sectionIndex)
	{
	case SECTION_TYPE_REPEAT_TYPE:
		return L"Repeat";
	case SECTION_TYPE_REPEAT_UNTIL:
		return L"Repeat until";
	default:
		break;
	}
	return L"";

}

bool
SetRecurrenceForm::HasSectionHeader(int sectionIndex)
{
	return true;
}

String
SetRecurrenceForm::GetSectionFooter(int sectionIndex)
{
	return L"";
}

bool
SetRecurrenceForm::HasSectionFooter(int sectionIndex)
{
	return false;
}

int
SetRecurrenceForm::GetDefaultItemHeight(void)
{
	return H_ITEM;
}

void
SetRecurrenceForm::OnSectionTableViewContextItemActivationStateChanged(SectionTableView& tableView, int sectionIndex, int itemIndex, TableViewContextItem* pContextItem, bool activated)
{
}

void
SetRecurrenceForm::OnSectionTableViewItemStateChanged(SectionTableView& tableView, int sectionIndex, int itemIndex, TableViewItem* pItem, TableViewItemStatus status)
{
	if (status == TABLE_VIEW_ITEM_STATUS_CHECKED)
	{
		if (sectionIndex == SECTION_TYPE_REPEAT_TYPE)
		{
			tableView.SetItemChecked(sectionIndex, (int) __repeatType, false);
			__repeatType = (RepeatType) itemIndex;

			if (__repeatType == REPEAT_TYPE_NONE)
			{
				tableView.SetItemChecked(SECTION_TYPE_REPEAT_UNTIL, (int) __repeatUntilType, false);
				__pUntilDateEditDate->SetEnabled(false);
				__pRepeatCountEditField->SetEnabled(false);
				__repeatUntilType = REPEAT_UNTIL_TYPE_COUNT;
			}
			else
			{
				tableView.SetItemChecked(SECTION_TYPE_REPEAT_UNTIL, (int) __repeatUntilType, true);
				if (__repeatUntilType == REPEAT_UNTIL_TYPE_DATE)
				{
					__pUntilDateEditDate->SetEnabled(true);
					__pRepeatCountEditField->SetEnabled(false);
				}
				else // __repeatUntilType == REPEAT_UNTIL_TYPE_COUNT
				{
					__pUntilDateEditDate->SetEnabled(false);
					__pRepeatCountEditField->SetEnabled(true);
				}
			}
		}
		else // (groupIndex == SECTION_TYPE_REPEAT_UNTIL)
		{
			if (__repeatType != REPEAT_TYPE_NONE)
			{
				tableView.SetItemChecked(sectionIndex, (int) __repeatUntilType, false);
				__repeatUntilType = (RepeatUntilType) itemIndex;

				switch (itemIndex)
				{
				case REPEAT_UNTIL_TYPE_DATE:
				{
					__pUntilDateEditDate->SetEnabled(true);
					__pRepeatCountEditField->SetEnabled(false);
				}
				break;

				case REPEAT_UNTIL_TYPE_COUNT:
				{
					__pUntilDateEditDate->SetEnabled(false);
					__pRepeatCountEditField->SetEnabled(true);
				}
				break;

				default:
					break;
				}
			}
			else
			{
				__pTableView->SetItemChecked(SECTION_TYPE_REPEAT_UNTIL, itemIndex, false);
			}
		}
		tableView.Invalidate(true);
	}
	else if (status == TABLE_VIEW_ITEM_STATUS_UNCHECKED)
	{
		tableView.SetItemChecked(sectionIndex, itemIndex, true);
		tableView.Invalidate(true);
	}
}

void
SetRecurrenceForm::OnSceneActivatedN(const SceneId& previousSceneId, const SceneId& currentSceneId, IList* pArgs)
{
	if (pArgs != null)
	{
		DateTime* pStartTime = null;
		DateTime* pEndTime = null;
		Recurrence* pRecurrence = null;

		pStartTime = static_cast< DateTime* >(pArgs->GetAt(0));
		TryReturnVoid(pStartTime != null, "pArgs(0)  is invalid.");
		pEndTime = static_cast< DateTime* >(pArgs->GetAt(1));
		TryReturnVoid(pEndTime != null, "pArgs(1) is invalid.");

		if (pArgs->GetCount() == 3)
		{
			pRecurrence = static_cast< Recurrence* >(pArgs->GetAt(2));
			TryReturnVoid(pRecurrence != null, "pArgs(2) is invalid.");
		}

		if (pRecurrence != null)
		{
			RecurFrequency frequency = pRecurrence->GetFrequency();
			switch (frequency)
			{
			case FREQ_DAILY:
				__repeatType = REPEAT_TYPE_DAILY;
				break;

			case FREQ_WEEKLY:
				__repeatType = REPEAT_TYPE_WEEKLY;
				break;

			case FREQ_MONTHLY:
				if (pRecurrence->GetDayOfMonth() == 0)
				{
					__repeatType = REPEAT_TYPE_MONTHLY;
				}
				else
				{
					__repeatType = REPEAT_TYPE_MONTHLY_DATE;
				}
				break;

			case FREQ_YEARLY:
				__repeatType = REPEAT_TYPE_YEARLY_DATE;
				break;

			default:
				__repeatType = REPEAT_TYPE_NONE;
				break;
			}

			if (pRecurrence->GetUntil() != null)
			{
				__repeatUntilType = REPEAT_UNTIL_TYPE_DATE;
				__untilDate = *(pRecurrence->GetUntil());
			}
			else
			{
				__repeatUntilType = REPEAT_UNTIL_TYPE_COUNT;
				__untilDate = *pEndTime;
				__count = pRecurrence->GetCounts();
			}
		}
		else
		{
			__repeatType = REPEAT_TYPE_NONE;
			__repeatUntilType = REPEAT_UNTIL_TYPE_COUNT;
			__untilDate = *pEndTime;
			__count = 1;
		}

		Tizen::Locales::GregorianCalendar gregorianCalendar;
		gregorianCalendar.Construct(*pStartTime);
		int dayOfWeek = gregorianCalendar.GetTimeField(TIME_FIELD_DAY_OF_WEEK);
		__calDayOfWeek = (CalDayOfWeek) (1 << (dayOfWeek - 1));
		__weekOfMonth = gregorianCalendar.GetTimeField(TIME_FIELD_WEEK_OF_MONTH);
		__month = pStartTime->GetMonth();
		__day = pStartTime->GetDay();

		__pTableView->UpdateTableView();
		__pTableView->SetItemChecked(SECTION_TYPE_REPEAT_TYPE, (int) __repeatType, true);
		if (__repeatType == REPEAT_TYPE_NONE)
		{
			__pTableView->SetItemChecked(SECTION_TYPE_REPEAT_UNTIL, (int) __repeatUntilType, false);
		}
		else
		{
			__pTableView->SetItemChecked(SECTION_TYPE_REPEAT_UNTIL, (int) __repeatUntilType, true);
		}

	}

	pArgs->RemoveAll(true);
	delete pArgs;
}

void
SetRecurrenceForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	AppAssert(pSceneManager);

	pSceneManager->GoBackward(BackwardSceneTransition());
}

void
SetRecurrenceForm::OnSceneDeactivated(const SceneId& currentSceneId, const SceneId& nextSceneId)
{
}

String
SetRecurrenceForm::GetMonthString(int month)
{
	String strValue;

	switch (month)
	{
	case 1:
		strValue = L"January ";
		break;

	case 2:
		strValue = L"February ";
		break;

	case 3:
		strValue = L"March ";
		break;

	case 4:
		strValue = L"April ";
		break;

	case 5:
		strValue = L"May ";
		break;

	case 6:
		strValue = L"June ";
		break;

	case 7:
		strValue = L"July ";
		break;

	case 8:
		strValue = L"August ";
		break;

	case 9:
		strValue = L"September ";
		break;

	case 10:
		strValue = L"October ";
		break;

	case 11:
		strValue = L"November ";
		break;

	case 12:
		strValue = L"December ";
		break;

	default:
		break;
	}

	return strValue;
}

String
SetRecurrenceForm::GetCalDayOfWeekString(CalDayOfWeek calDayOfWeek)
{
	String strValue;

	switch (calDayOfWeek)
	{
	case CAL_SUNDAY:
		strValue = L"Sunday";
		break;

	case CAL_MONDAY:
		strValue = L"Monday";
		break;

	case CAL_TUESDAY:
		strValue = L"Tuesday";
		break;

	case CAL_WEDNESDAY:
		strValue = L"Wednesday";
		break;

	case CAL_THURSDAY:
		strValue = L"Thursday";
		break;

	case CAL_FRIDAY:
		strValue = L"Friday";
		break;

	case CAL_SATURDAY:
		strValue = L"Saturday";
		break;

	default:
		break;
	}

	return strValue;
}

Recurrence*
SetRecurrenceForm::CreateRecurrenceN(void)
{
	Recurrence* pRecurrence = new (std::nothrow) Recurrence();
	TryReturn(pRecurrence != null, null, "Failed to create the recurrence");

	switch (__repeatType)
	{
	case REPEAT_TYPE_NONE:
		delete pRecurrence;
		pRecurrence = null;
		break;

	case REPEAT_TYPE_DAILY:
		pRecurrence->SetFrequency(FREQ_DAILY);
		break;

	case REPEAT_TYPE_WEEKLY:
		pRecurrence->SetFrequency(FREQ_WEEKLY);
		pRecurrence->SetDayOfWeek(__calDayOfWeek);
		break;

	case REPEAT_TYPE_MONTHLY:
		pRecurrence->SetFrequency(FREQ_MONTHLY);
		pRecurrence->SetWeekOfMonth(__weekOfMonth);
		pRecurrence->SetDayOfWeek(__calDayOfWeek);
		break;

	case REPEAT_TYPE_MONTHLY_DATE:
		pRecurrence->SetFrequency(FREQ_MONTHLY);
		pRecurrence->SetDayOfMonth(__day);
		break;

	case REPEAT_TYPE_YEARLY_DATE:
		pRecurrence->SetFrequency(FREQ_YEARLY);
		pRecurrence->SetMonthOfYear(__month);
		pRecurrence->SetDayOfMonth(__day);
		break;
	}

	if (pRecurrence != null)
	{
		if (__repeatUntilType == REPEAT_UNTIL_TYPE_DATE)
		{
			DateTime untilDate;
			untilDate.SetValue(__pUntilDateEditDate->GetYear(), __pUntilDateEditDate->GetMonth(), __pUntilDateEditDate->GetDay(), 23, 59, 59);
			pRecurrence->SetUntil(&untilDate);
		}
		else // (__repeatUntilType == REPEAT_UNTIL_TYPE_UNTIL_COUNT)
		{
			int count;
			Integer::Parse(__pRepeatCountEditField->GetText(), count);
			pRecurrence->SetCounts((count > 1) ? count : 1);
		}
	}

	return pRecurrence;
}
