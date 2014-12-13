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

#ifndef _SET_RECURRENCE_FORM_H_
#define _SET_RECURRENCE_FORM_H_

#include <FBase.h>
#include <FSocial.h>
#include <FUi.h>

class SetRecurrenceForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
	, public virtual Tizen::Ui::Controls::ISectionTableViewItemEventListener
	, public virtual Tizen::Ui::Controls::ISectionTableViewItemProvider
	, public Tizen::Ui::Scenes::ISceneEventListener
{
public:
	bool Initialize(void);

	SetRecurrenceForm(void);
	virtual ~SetRecurrenceForm(void);

	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

	virtual int GetSectionCount(void);
	virtual int GetItemCount(int sectionIndex);
	virtual Tizen::Ui::Controls::TableViewItem* CreateItem(int sectionIndex, int itemIndex, int itemWidth);
	virtual bool DeleteItem(int sectionIndex, int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem);
	virtual void UpdateItem(int sectionIndex, int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem);
	virtual Tizen::Base::String GetSectionHeader(int sectionIndex);
	virtual bool HasSectionHeader(int sectionIndex);
	virtual Tizen::Base::String GetSectionFooter(int sectionIndex);
	virtual bool HasSectionFooter(int sectionIndex);
	virtual int GetDefaultItemHeight(void);

	virtual void OnSectionTableViewItemStateChanged(Tizen::Ui::Controls::SectionTableView& tableView, int sectionIndex, int itemIndex, Tizen::Ui::Controls::TableViewItem* pItem, Tizen::Ui::Controls::TableViewItemStatus status);
	virtual void OnSectionTableViewContextItemActivationStateChanged(Tizen::Ui::Controls::SectionTableView& tableView, int sectionIndex, int itemIndex, Tizen::Ui::Controls::TableViewContextItem* pContextItem, bool activated);

	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId, const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId, const Tizen::Ui::Scenes::SceneId& nextSceneId);

private:
	Tizen::Social::Recurrence* CreateRecurrenceN(void);
	Tizen::Base::String GetMonthString(int month);
	Tizen::Base::String GetCalDayOfWeekString(Tizen::Social::CalDayOfWeek calDayOfWeek);

private:
	enum RepeatType
	{
		REPEAT_TYPE_NONE = 0,
		REPEAT_TYPE_DAILY,
		REPEAT_TYPE_WEEKLY,
		REPEAT_TYPE_MONTHLY,
		REPEAT_TYPE_MONTHLY_DATE,
		REPEAT_TYPE_YEARLY_DATE
	};

	enum RepeatUntilType
	{
		REPEAT_UNTIL_TYPE_DATE = 0,
		REPEAT_UNTIL_TYPE_COUNT
	};

private:
	static const int ID_FOOTER_DONE = 100;

	Tizen::Base::DateTime __untilDate;
	Tizen::Ui::Controls::SectionTableView* __pTableView;
	Tizen::Ui::Controls::EditDate* __pUntilDateEditDate;
	Tizen::Ui::Controls::EditField* __pRepeatCountEditField;
	RepeatType __repeatType;
	RepeatUntilType __repeatUntilType;
	Tizen::Social::CalDayOfWeek __calDayOfWeek;

	int __weekOfMonth;
	int __month;
	int __day;
	int __count;

}; // SetRecurrenceForm

#endif // _SET_RECURRENCE_FORM_H_
