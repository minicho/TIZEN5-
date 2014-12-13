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

#ifndef _MAIN_FORM_H_
#define _MAIN_FORM_H_

#include <FUi.h>
#include <FBase.h>
#include <FLocales.h>

#include <FApp.h>
#include <FSystem.h>
#include <FAppUiApp.h>
#include <FWeb.h>
#include <FGraphics.h>
#include <FNet.h>

#include <FUiIme.h>
#include <gl.h>

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;
using namespace Tizen::System;
using namespace Tizen::Locales;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Web;
using namespace Tizen::Net::Http;
using namespace Tizen::Web::Controls;


class MainForm 
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
	, public Tizen::Web::Controls::ILoadingListener
	, public Tizen::Ui::Scenes::ISceneEventListener

{
public:
	MainForm(void);
	virtual ~MainForm(void);
	bool Initialize(void);

	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

    virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                           const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);

    virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId, const Tizen::Ui::Scenes::SceneId& nextSceneId);

    virtual void webLink(int id);

	virtual void OnEstimatedProgress (int progress) {}
	virtual void OnHttpAuthenticationCanceled (void) {}
	virtual bool OnHttpAuthenticationRequestedN (const Tizen::Base::String &host, const Tizen::Base::String &realm,
				const Tizen::Web::Controls::AuthenticationChallenge &authentication);
	virtual void OnLoadingCanceled (void) {}
	virtual void OnLoadingCompleted (void);
	virtual void OnLoadingErrorOccurred (LoadingErrorType error, const Tizen::Base::String &reason);
	virtual bool OnLoadingRequested (const Tizen::Base::String &url, WebNavigationType type);
	virtual void OnLoadingStarted (void) {}
	virtual void OnPageTitleReceived (const Tizen::Base::String &title) {}
	virtual DecisionPolicy OnWebDataReceived (const Tizen::Base::String &mime, const Tizen::Net::Http::HttpHeader &httpHeader);


private:
	static const int ID_HEADER_ITEM1 = 101;
	static const int ID_HEADER_ITEM2 = 102;
	static const int ID_HEADER_ITEM3 = 103;

	Rectangle rtScreen, rtWeb;

protected:
	Tizen::Web::Controls::Web* __pWeb;
};

#endif
