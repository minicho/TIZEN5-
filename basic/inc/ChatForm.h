/*
 * ChatForm.h
 *
 *  Created on: Dec 8, 2014
 *      Author: ajou
 */

#ifndef CHATFORM_H_
#define CHATFORM_H_

#include <new>
#include <FBase.h>
#include <FNet.h>
#include <FApp.h>
#include <FUi.h>

using namespace Tizen::Net;
using namespace Tizen::Net::Sockets;
using namespace Tizen::Base;

class ChatForm
    : public Tizen::Ui::Controls::Form
    , public Tizen::Ui::IActionEventListener
    , public Tizen::Net::Sockets::ISocketEventListener
    , public Tizen::Ui::Controls::IFormBackEventListener
    , public Tizen::Ui::Scenes::ISceneEventListener
{
public:
	ChatForm();
    ChatForm(Ip4Address* pAddress, int port, bool isServer, bool isTcp, Tizen::Ui::Controls::Form* pPreviousForm);

	virtual ~ChatForm();

	   bool Initialize(void);

	    virtual result OnInitializing(void);

	    virtual result OnTerminating(void);

	    virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

	    result StartChat(void);

	    virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);

	    virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
	                                           const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);

	    virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId, const Tizen::Ui::Scenes::SceneId& nextSceneId);

	    void EnableControl(bool val);

	    virtual void OnSocketAccept(Socket& socket);

	    virtual void OnSocketClosed(Socket& socket1, NetSocketClosedReason reason);

	    virtual void OnSocketConnected(Socket& socket1);

	    virtual void OnSocketReadyToReceive(Socket& socket);

	    virtual void OnSocketReadyToSend(Socket& socket1);

	private:

	    result SendData(void);

	    result ReceiveData(void);

	    void OnClose(void);

	    result ReceiveDataUDP(void);

	    result ReceiveDataTCP(void);

	    result DisplayText(ByteBuffer& txBuffer, unsigned long buflen);

	    result SendDataCommand(String command);

	    result SocketServer(void);

	    result SocketClient(void);

	    result CreateSocket(NetSocketType type, NetSocketProtocol protocol);

	    void ShowPopup(void);

	    void HidePopup(void);

	    void WaitAndHidePopup(void);

	private:
	    Tizen::Ui::Controls::Form* __pForm;
	    Tizen::Ui::Controls::Label* __pLabelWait;
	    Tizen::Ui::Controls::EditArea* __pEditAreaData;
	    Tizen::Ui::Controls::EditField* __pEditFieldtext;
	    Tizen::Ui::Controls::EditField* __pEditUsId;
	    Tizen::Net::Ip4Address* __pClientIp;
	    int __port;
	    bool __isTcp;                                // true = tcp false = udp
	    bool __isServer;
	    bool __isConnected;
	    bool __isConstructed;

	    Tizen::Net::Sockets::Socket* __pSocket;
	    Tizen::Net::Sockets::Socket* __pTcpSocket;
	    Tizen::Net::NetEndPoint* __pUdpEndpoint;
	    Tizen::Ui::Controls::Form* __pPreviousForm;
	    Tizen::Ui::Controls::Popup* __pPopup;
	    Tizen::Ui::Controls::Footer* __pFooter;

};

#endif /* CHATFORM_H_ */
