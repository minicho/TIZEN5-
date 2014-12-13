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

#include "DeleteRecurringEventPopup.h"

using namespace Tizen::Graphics;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;

DeleteRecurringEventPopup::DeleteRecurringEventPopup(void)
{
}

DeleteRecurringEventPopup::~DeleteRecurringEventPopup(void)
{
}

result
DeleteRecurringEventPopup::Construct(const Form* pParentForm)
{
	result r = E_SUCCESS;

	r = Popup::Construct(true, Dimension(650, 500));
	TryReturn(!IsFailed(r), r, "[%s] Failed to construct popup.", GetErrorMessage(r));

	SetTitleText(L"반복 일정 삭제");
	__pParentForm = const_cast< Form* >(pParentForm);
	if (__pParentForm == null)
	{
		return E_FAILURE;
	}

	Button* pSingleInstanceButton = new (std::nothrow) Button();
	pSingleInstanceButton->Construct(Rectangle((GetClientAreaBounds().width - 550) / 2, 50, 550, 74), L"현재 일정만");
	pSingleInstanceButton->SetActionId(ID_BUTTON_SINGLE_INSTANCE);
	pSingleInstanceButton->AddActionEventListener(*this);
	AddControl(pSingleInstanceButton);

	Button* pAllInstanceButton = new (std::nothrow) Button();
	pAllInstanceButton->Construct(Rectangle((GetClientAreaBounds().width - 550) / 2, 150, 550, 74), L"모든 일정");
	pAllInstanceButton->SetActionId(ID_BUTTON_ALL_INSTANCES);
	pAllInstanceButton->AddActionEventListener(*this);
	AddControl(pAllInstanceButton);

	Button* pCancelButton = new (std::nothrow) Button();
	pCancelButton->Construct(Rectangle((GetClientAreaBounds().width - 550) / 2, 250, 550, 74), L"취소");
	pCancelButton->SetActionId(ID_BUTTON_CANCEL);
	pCancelButton->AddActionEventListener(*this);
	AddControl(pCancelButton);

	SetPropagatedKeyEventListener(this);

	return r;
}

void
DeleteRecurringEventPopup::OnActionPerformed(const Control& source, int actionId)
{
	switch (actionId)
	{
	case ID_BUTTON_SINGLE_INSTANCE:
		__pParentForm->SendUserEvent(ID_BUTTON_SINGLE_INSTANCE, null);
		break;

	case ID_BUTTON_ALL_INSTANCES:
		__pParentForm->SendUserEvent(ID_BUTTON_ALL_INSTANCES, null);
		break;

	case ID_BUTTON_CANCEL:
		__pParentForm->SendUserEvent(ID_BUTTON_CANCEL, null);
		break;

	default:
		break;
	}
}

bool
DeleteRecurringEventPopup::OnKeyPressed(Control& source, const KeyEventInfo& keyEventInfo)
{
	return false;
}

bool
DeleteRecurringEventPopup::OnKeyReleased(Control& source, const KeyEventInfo& keyEventInfo)
{
	if (keyEventInfo.GetKeyCode() == KEY_ESC || keyEventInfo.GetKeyCode() == KEY_BACK)
	{
		__pParentForm->SendUserEvent(ID_BUTTON_CANCEL, null);
	}

	return false;
}

bool
DeleteRecurringEventPopup::OnPreviewKeyPressed(Control& source, const KeyEventInfo& keyEventInfo)
{
	return false;
}

bool
DeleteRecurringEventPopup::OnPreviewKeyReleased(Control& source, const KeyEventInfo& keyEventInfo)
{
	return false;
}

bool
DeleteRecurringEventPopup::TranslateKeyEventInfo(Control& source, KeyEventInfo& keyEventInfo)
{
	return false;
}
