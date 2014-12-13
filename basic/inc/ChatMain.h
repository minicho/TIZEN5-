/*
 * ChatMain.h
 *
 *  Created on: Dec 8, 2014
 *      Author: ajou
 */

#ifndef CHATMAIN_H_
#define CHATMAIN_H_

#include <new>
#include <FBase.h>
#include <FNet.h>
#include <FUi.h>
#include "ChatForm.h"

class ChatMain
	: public Tizen::Ui::Controls::Form
    , public Tizen::Ui::Controls::IFormBackEventListener
    , public Tizen::Ui::IActionEventListener
    , public Tizen::Ui::Scenes::ISceneEventListener
{
public:
	ChatMain();
	virtual ~ChatMain();

    bool Initialize(void);

    virtual result OnInitializing(void);

    virtual result OnTerminating(void);

    virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

    virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

    virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                           const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);

    virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId, const Tizen::Ui::Scenes::SceneId& nextSceneId);


    void StartChat(void);

    void SetDefaultValues(void);

private:
    Tizen::Ui::Controls::CheckButton* __pButtonServer;
    Tizen::Ui::Controls::CheckButton* __pButtonTcp;
    Tizen::Ui::Controls::CheckButton* __pButtonUdp;
    Tizen::Ui::Controls::EditField* __pEditIPAddress;
    Tizen::Ui::Controls::EditField* __pEditPort;

	static const int ID_HEADER_ITEM1 = 101;
	static const int ID_HEADER_ITEM2 = 102;
	static const int ID_HEADER_ITEM3 = 103;

    Tizen::Net::Ip4Address* __pClientIp;
    int __iport;
    bool __isProtocol;                                // true = tcp false = udp
    bool __isServer;
    ChatForm* __pChatForm;
    Tizen::Ui::Controls::FooterItem __footerItem_Left;
};

#endif /* CHATMAIN_H_ */

