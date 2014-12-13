/*
 * ChatForm.cpp
 *
 *  Created on: Dec 8, 2014
 *      Author: ajou
 */

#include "ChatForm.h"
#include "AppResourceId.h"
#include "BasicAppFrame.h"
#include "tabTestFormFactory.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Net;
using namespace Tizen::Net::Sockets;
using namespace Tizen::Graphics;

static const int ID_BUTTON_SEND = 101;
static const int ID_BUTTON_CLOSE_POPUP = 103;
static const int DIMENSION_WIDTH = 400;
static const int DIMENSION_HEIGHT = 500;
static const int BUTTON_RECTANGLE_X = 150;
static const int BUTTON_RECTANGLE_Y = 340;
static const int BUTTON_RECTANGLE_WIDTH = 100;
static const int BUTTON_RECTANGLE_HEIGHT = 60;
static const int LABEL_RECTANGLE_X = 20;
static const int LABEL_RECTANGLE_Y = 20;
static const int LABEL_RECTANGLE_WIDTH = 340;
static const int LABEL_RECTANGLE_HEIGHT = 300;
static const int THREAD_TIMEOUT = 1000;

#define COLOR_R 0xff
#define COLOR_G 0xff
#define COLOR_B 0xff
#define COLOR_A 0x00

ChatForm::ChatForm()
   : __pForm(null)
   , __pLabelWait(null)
   , __pEditAreaData(null)
   , __pEditFieldtext(null)
   , __pClientIp(null)
   , __port(0)
   , __isTcp(true)
   , __isServer(true)
   , __isConnected(false)
   , __isConstructed(false)
   , __pSocket(null)
   , __pTcpSocket(null)
   , __pUdpEndpoint(null)
   , __pPreviousForm(null)
   , __pPopup(null)
   , __pFooter(null)
   , __pEditUsId(null)
{
   // TODO Auto-generated constructor stub

}

ChatForm::~ChatForm() {
   // TODO Auto-generated destructor stub
    delete __pClientIp;
    __pClientIp = null;
}

ChatForm::ChatForm(Ip4Address* pAddress, int port, bool isServer, bool isTcp, Tizen::Ui::Controls::Form* pPreviousForm)
    : __pForm(null)
    , __pLabelWait(null)
    , __pEditAreaData(null)
    , __pEditFieldtext(null)
    , __port(port)
    , __isTcp(isTcp)
    , __isServer(isServer)
    , __isConnected(false)
    , __isConstructed(false)
    , __pSocket(null)
    , __pTcpSocket(null)
    , __pUdpEndpoint(null)
    , __pPreviousForm(pPreviousForm)
    , __pPopup(null)
    , __pFooter(null)
   , __pEditUsId(null)
{
}

bool
ChatForm::Initialize(void)
{
    // Construct an XML form
    Construct(IDF_ChatForm);

    return true;
}

result
ChatForm::OnInitializing(void)
{
    result r = E_SUCCESS;

    // Get a button via resource ID
    __pForm = this;

    __pFooter = GetFooter();
    if (__pFooter != null)
    {
        __pFooter->SetStyle(FOOTER_STYLE_BUTTON_TEXT);
        FooterItem footerItem_Left;

        footerItem_Left.Construct(ID_BUTTON_SEND);
        footerItem_Left.SetText(L"Send");
        __pFooter->AddItem(footerItem_Left);

        if (__isTcp)
        {
            __pFooter->SetItemEnabled(0, false);
        }
        else
        {
            __pFooter->SetItemEnabled(0, true);
        }

        SetFormBackEventListener(this);

        __pFooter->AddActionEventListener(*this);
    }

    __pLabelWait = static_cast< Label* >(GetControl(IDC_Wait));
    if (__pLabelWait == null)
    {
        AppLog("Can't get label Handle");
        r = E_FAILURE;
    }

    __pEditAreaData = static_cast< EditArea* >(GetControl(IDC_DATA));
    if (__pEditAreaData == null)
    {
        AppLog("Can't get edit area Handle");
        r = E_FAILURE;
    }

    __pEditFieldtext = static_cast< EditField* >(GetControl(IDC_TEXT));
    if (__pEditFieldtext == null)
    {
        AppLog("Can't get edit field Handle");
        r = E_FAILURE;
    }



    __pEditUsId = static_cast< EditField* >(GetControl(IDC_USID));
    //__pEditUsId->SetActionId(9100);
    __pEditUsId->AddActionEventListener(*this);
    __pEditUsId->SetText("");

        if (__pEditUsId == null)
        {
            AppLog("Can't get edit field Handle");
            r = E_FAILURE;
        }

    __pPopup = new (std::nothrow) Popup();
    if (__pPopup != null)
    {
        __pPopup->Construct(true, Dimension(DIMENSION_WIDTH, DIMENSION_HEIGHT));
        __pPopup->SetTitleText(L"Send");

        Button* pButtonClose = new (std::nothrow) Button();
        pButtonClose->Construct(Rectangle(BUTTON_RECTANGLE_X, BUTTON_RECTANGLE_Y, BUTTON_RECTANGLE_WIDTH, BUTTON_RECTANGLE_HEIGHT), L"Close");
        pButtonClose->SetActionId(ID_BUTTON_CLOSE_POPUP);
        pButtonClose->AddActionEventListener(*this);
        __pPopup->AddControl(*pButtonClose);

        Label* pLabelText = new (std::nothrow) Label();

        pLabelText->Construct(Rectangle(LABEL_RECTANGLE_X, LABEL_RECTANGLE_Y, LABEL_RECTANGLE_WIDTH, LABEL_RECTANGLE_HEIGHT), L"Data Send Error");
        pLabelText->SetBackgroundColor(Color(COLOR_R, COLOR_G, COLOR_B, COLOR_A));
        pLabelText->SetTextHorizontalAlignment(ALIGNMENT_CENTER);

        __pPopup->AddControl(*pLabelText);
    }

    EnableControl(false);

    return r;
}

void
ChatForm::ShowPopup(void)
{
    if (__pPopup != null)
    {
        __pPopup->SetShowState(true);
        __pPopup->Show();
    }
}

void
ChatForm::HidePopup(void)
{
    if (__pPopup != null)
    {
        __pPopup->SetShowState(false);
    }

    Show();
}

void
ChatForm::WaitAndHidePopup(void)
{
    if (__pPopup != null)
    {
        if (__pPopup->IsVisible() == true)
        {
            AppLog("Wait for Popup to Get hidden");
            Tizen::Base::Runtime::Thread::Sleep(THREAD_TIMEOUT);
            __pPopup->SetShowState(false);
            Show();
        }
    }
}

result
ChatForm::OnTerminating(void)
{
    result r = E_SUCCESS;

    delete __pPopup;
    __pPopup = null;

    AppLog("ChatForm::OnTerminating");
    OnClose();
    EnableControl(false);

    return r;
}

void
ChatForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
    AppLog("Close Button Pressed");

    SceneManager* pSceneManager = SceneManager::GetInstance();
    AppAssert(pSceneManager);

    pSceneManager->GoBackward(BackwardSceneTransition(SCENE_CHATMAIN));

    OnClose();
    EnableControl(false);
}

void
ChatForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
    result res = E_SUCCESS;

    switch (actionId)
    {
    case ID_BUTTON_SEND:
        {
            res = SendData();
            if (IsFailed(res))
            {
                AppLog("Data Send Failed.");
                ShowPopup();
            }
        }
        break;

    case ID_BUTTON_CLOSE_POPUP:
        HidePopup();
        break;

    default:
        AppLog("Someone action taken");
        break;
    }
}

result
ChatForm::CreateSocket(NetSocketType type, NetSocketProtocol protocol)
{
    unsigned long arg = 1;
    result res = E_SUCCESS;

    // Create socket
    __pSocket = new (std::nothrow) Socket();
    TryReturn(__pSocket != null, E_OUT_OF_MEMORY, "Failed to allocate Socket");

    AppLog("Constructing Socket");
    res = __pSocket->Construct(NET_SOCKET_AF_IPV4, type, protocol);
    TryReturn(res == E_SUCCESS, res, "Socket Construction failed %d", res);

    // Non-blocking mode = 1, blocking mode = 0
    res = __pSocket->Ioctl(NET_SOCKET_FIONBIO, arg);
    TryReturn(res == E_SUCCESS, res, "Socket Ioctl failed %d", res);

    // Add Socket Listener to listen to socket event
    res = __pSocket->AddSocketListener(*this);
    TryReturn(res == E_SUCCESS, res, "Socket AddSocketListener failed %d", res);

    res = __pSocket->SetSockOpt(NET_SOCKET_SOL_SOCKET, NET_SOCKET_SO_REUSEADDR, 1);
    TryReturn(res == E_SUCCESS, res, "Socket SetSockOpt failed %d", res);

    // Set the socket as select an interested event.
    res = __pSocket->AsyncSelectByListener(NET_SOCKET_EVENT_CONNECT | NET_SOCKET_EVENT_WRITE | NET_SOCKET_EVENT_READ | NET_SOCKET_EVENT_CLOSE | NET_SOCKET_EVENT_ACCEPT);
    TryReturn(res == E_SUCCESS, res, "Socket AsyncSelectByListener failed %d", res);

    return res;
}

result
ChatForm::SocketServer(void)
{
    result res = E_SUCCESS;

    // Create a NetEndPoint
    Ip4Address peerAddr(NET_SOCKET_INADDR_ANY);  // ADDR_ANY
    unsigned short peerPort = __port;            // Port

    NetEndPoint* pPeerEndPoint = new (std::nothrow) NetEndPoint(peerAddr, peerPort);
    TryReturn(pPeerEndPoint != null, E_OUT_OF_MEMORY, "Failed to allocate NetEndPoint");

    // Bind socket to SERVER_PORT
    res = __pSocket->Bind(*pPeerEndPoint);
    delete pPeerEndPoint;
    TryReturn(res == E_SUCCESS, res, "Socket Bind failed %d", res);

    if (__isTcp)
    {
        // Start listening on tcp port
        res = __pSocket->Listen(0);   // No queue allowed
        TryReturn(res == E_SUCCESS, res, "Socket Listen failed %ls", GetErrorMessage(res));
    }

    return res;
}

result
ChatForm::SocketClient(void)
{
    result res = E_SUCCESS;

    if (__isTcp)
    {
        // Create server endpoint using serverip address and port
        NetEndPoint* pPeerEndPoint = new (std::nothrow) NetEndPoint(*__pClientIp, __port);
        TryReturn(pPeerEndPoint != null, E_OUT_OF_MEMORY, "Failed to allocate NetEndPoint");

        // Connect to peer for tcp protocol
        res = __pSocket->Connect(*pPeerEndPoint);
        AppLog("Socket Return %ls", GetErrorMessage(res));
        delete pPeerEndPoint;
        return res;
    }
    else
    {
        // Send hello message to peer using udp protocol
        EnableControl(true);
        SendDataCommand(L"__HELLO__");
        __isConnected = true;
    }

    return res;
}

result
ChatForm::StartChat(void)
{
    result res = E_SUCCESS;

    if (__isTcp)
    {
        res = CreateSocket(NET_SOCKET_TYPE_STREAM, NET_SOCKET_PROTOCOL_TCP);
    }
    else
    {
        res = CreateSocket(NET_SOCKET_TYPE_DATAGRAM, NET_SOCKET_PROTOCOL_UDP);
    }

    TryReturn(res == E_SUCCESS, res, "Error in creating Socket %d", res);

    __isConstructed = true;

    if (__isServer)
    {
        res = SocketServer();
    }
    else
    {
        res = SocketClient();
    }

    return res;
}

void
ChatForm::OnSocketAccept(Socket& socket)
{
    result res = E_SUCCESS;
    AppLog("OnSocketAccept");

    if (__pTcpSocket != null)
    {
        socket.Close();
        AppLog("Already Connected with one Client");
        return;
    }

    EnableControl(true);
    __isConnected = true;
    __pFooter->SetItemEnabled(0, true);

    Socket* pSocket = const_cast< Socket* >(&socket);
    __pTcpSocket = __pSocket;

    // Accept the connection attempt.
    __pSocket = pSocket->AcceptN();
    if (__pSocket == null)
    {
        AppLog("Socket Accept Failed");
        return;
    }

    // Add Socket Listener to listen to socket event.
    res = __pSocket->AddSocketListener(*this);
    if (IsFailed(res))
    {
        AppLog("Socket AddSocketListener failed %d", res);
    }

    // Set the socket as select an interested event.
    res = __pSocket->AsyncSelectByListener(NET_SOCKET_EVENT_WRITE | NET_SOCKET_EVENT_READ | NET_SOCKET_EVENT_CLOSE);
    if (IsFailed(res))
    {
        AppLog("Socket AsyncSelectByListener failed %d", res);
    }
}

void
ChatForm::OnSocketClosed(Socket& socket, NetSocketClosedReason reason)
{
    AppLog("OnSocketClosed");
    AppLog(GetErrorMessage(GetLastResult()));
    AppLog("Reasoning %d", reason);

    OnClose();
    AppLog("Close Socket");

    EnableControl(false);

    WaitAndHidePopup();

    SceneManager* pSceneManager = SceneManager::GetInstance();
    AppAssert(pSceneManager);

    pSceneManager->GoBackward(BackwardSceneTransition(SCENE_CHATMAIN));
}

void
ChatForm::OnSocketConnected(Socket& socket)
{
    AppLog("OnSocketConnected");
    EnableControl(true);
    __isConnected = true;
    __pFooter->SetItemEnabled(0, true);
}

void
ChatForm::OnSocketReadyToReceive(Socket& socket)
{
    AppLog("OnSocketReadyToReceive");
    ReceiveData();
}

void
ChatForm::OnSocketReadyToSend(Socket& socket)
{
    AppLog("OnSocketReadyToSend");
}

result
ChatForm::SendDataCommand(String command)
{
    ByteBuffer* pTxBuffer = null;
    //ByteBuffer* pTxBuffer1 = null;
    ByteBuffer buffer;
    //ByteBuffer nickBuffer;
    result res = E_SUCCESS;

    TryReturn(__isTcp == false, E_SYSTEM, "Sending Command not required for TCP connection");

    pTxBuffer = Tizen::Base::Utility::StringUtil::StringToUtf8N(command);
   //pTxBuffer1 = Tizen::Base::Utility::StringUtil::StringToUtf8N(command);
    TryReturn(pTxBuffer != null, E_OUT_OF_MEMORY, "Failed to allocate ByteBuffer");
    //TryReturn(pTxBuffer1 != null, E_OUT_OF_MEMORY, "Failed to allocate ByteBuffer");

    buffer.Construct(1024);
    //buffer.CopyFrom(*pTxBuffer);
    //buffer.CopyFrom(*pTxBuffer1);
    delete pTxBuffer;
   // delete pTxBuffer1;

    buffer.Flip();
    //nickBuffer.Flip();
    do
    {
        if (!__isServer)
        {
            // Create a peer end point
            if (__pClientIp != null)
            {
                NetEndPoint peerEndPoint(*__pClientIp, __port);
               // res = __pSocket->SendTo(buffer, peerEndPoint);
      //          res = __pSocket->SendTo(nickBuffer, peerEndPoint);
            }
        }
        else
        {
            if (__pUdpEndpoint != null)
            {
                // Create socket
                res = __pSocket->SendTo(buffer, *__pUdpEndpoint);
            }
        }

        if (res == E_WOULD_BLOCK)
        {
            AppLog("Would block for data sending will try to retransmit");
            Tizen::Base::Runtime::Thread::Sleep(500);
        }
        else if (res != E_SUCCESS)
        {
            AppLog("Error sending data");
            break;
        }
        else
        {
            AppLog("Command successfully sent");
            break;
        }
    }
    while (1);

    AppLog("Loop completed");
    buffer.Clear();
    //nickBuffer.Clear();

    return res;
}

result
ChatForm::SendData(void)
{
    String data;
    String nickData;
    ByteBuffer* pTxBuffer = null;
    ByteBuffer* pTxBuffer1 = null;
    ByteBuffer buffer;
    ByteBuffer nickBuffer;
    result res = E_SUCCESS;
    result res1 = E_SUCCESS;

    data = __pEditFieldtext->GetText();
    nickData = __pEditUsId->GetText();

    //data.Append(L"\n");
    data.Append(L"\n");
    nickData.Append("(TIZEN PHONE):");
    AppLog("String length is %d", data.GetCapacity());
    AppLog("String length is %d", nickData.GetCapacity());
    if (data.IsEmpty())
    {
        AppLog("There is no data to send");
        return E_SUCCESS;
    }
    pTxBuffer = Tizen::Base::Utility::StringUtil::StringToUtf8N(data);
    pTxBuffer1 = Tizen::Base::Utility::StringUtil::StringToUtf8N(nickData);
    TryReturn(pTxBuffer != null, E_OUT_OF_MEMORY, "Failed to allocate ByteBuffer");
    TryReturn(pTxBuffer1 != null, E_OUT_OF_MEMORY, "Failed~")

    AppLog("Text Length %d", pTxBuffer->GetLimit());
    AppLog("Text Length %d", pTxBuffer1->GetLimit());

    buffer.Construct(pTxBuffer->GetLimit());
    nickBuffer.Construct(pTxBuffer1->GetLimit());
    buffer.CopyFrom(*pTxBuffer);
    nickBuffer.CopyFrom(*pTxBuffer1);
    buffer.Flip();
    nickBuffer.Flip();

    delete pTxBuffer;
    delete pTxBuffer1;

    if (__isTcp)
    {
       //buffer = nickData.Append("하하");
       res1 = __pSocket->Send(nickBuffer);
        res = __pSocket->Send(buffer);
    }
    else if (!__isTcp)
    {
        if (!__isServer)
        {
            // Create a peer end point
            NetEndPoint peerEndPoint(*__pClientIp, __port);
            res = __pSocket->SendTo(buffer, peerEndPoint);
            res1 = __pSocket->SendTo(nickBuffer, peerEndPoint);
        }
        else
        {
            if (__pUdpEndpoint != null)
            {
                res = __pSocket->SendTo(buffer, *__pUdpEndpoint);
                res1 = __pSocket->SendTo(nickBuffer, *__pUdpEndpoint);
            }
        }
    }

    if (res == E_SUCCESS || res == E_WOULD_BLOCK)
    {
        data.Append(L"\n");
        nickData.Append(L"\n");
       // __pEditAreaData->AppendText(nickData);
       // __pEditAreaData->RequestRedraw();
        //__pEditAreaData->AppendText(data);
        //__pEditAreaData->RequestRedraw();

        __pEditFieldtext->SetText(L"");
        __pEditFieldtext->RequestRedraw();

    }
    else
    {
        AppLog("Error sending data");
    }

    buffer.Clear();

    return res;
}

result
ChatForm::DisplayText(ByteBuffer& txBuffer, unsigned long buflen)
{
    String data;
    String nickData;

    char* pBuffer = null;
    result res = E_SUCCESS;
    //result res1 = E_SUCCESS;


    txBuffer.Flip();
    pBuffer = (char*) (txBuffer.GetPointer());
    data.SetCapacity(buflen + 2);
   // nickData.SetCapacity(buflen + 2);

    res = data.Append(pBuffer);
    //res = nickData.Append(pBuffer);
    TryReturn(res == E_SUCCESS, res, "Append Failed");

    txBuffer.Clear();

    if (data == L"__HELLO__")
    {
        EnableControl(true);
        __isConnected = true;
        return res;
    }

    if (__isConnected == false)
    {
        AppLog("Server hasn't received Initial message so all incoming content is discarded");
        // Remove created NetEndPoint
        delete __pUdpEndpoint;
        __pUdpEndpoint = null;
        return res;
    }

    if (data == L"__CLOSE__")
    {
        SceneManager* pSceneManager = SceneManager::GetInstance();
        AppAssert(pSceneManager);

        pSceneManager->GoBackward(BackwardSceneTransition(SCENE_CHATMAIN));

        OnClose();
        return res;
    }



    data.Append(L"\n");
    //nickData.Append(L"\n");
   // res = __pEditAreaData->AppendText(nickData);
    res = __pEditAreaData->AppendText(data);
    TryReturn(res == E_SUCCESS, res, "AppendText Failed");

    __pEditAreaData->RequestRedraw();

    return res;
}

result
ChatForm::ReceiveDataUDP(void)
{
    result res = E_SUCCESS;
    ByteBuffer txBuffer;
    unsigned long buflen = 0;

    // Get the size of data received
    __pSocket->Ioctl(NET_SOCKET_FIONREAD, buflen);

    txBuffer.Construct(buflen + 1);

    Ip4Address peerAddr1(NET_SOCKET_INADDR_ANY);
    unsigned short port = 0;
    NetEndPoint point(peerAddr1, port);

    res = __pSocket->ReceiveFrom(txBuffer, point);
    if (__pUdpEndpoint == null)
    {
        __pUdpEndpoint = new (std::nothrow) NetEndPoint(point);
    }
    else
    {
        if (__pUdpEndpoint->GetNetEndPoint() != point.GetNetEndPoint())
        {
            AppLog("It's a data from other endpoint so discard");
            return res;
        }
    }

    TryReturn(res == E_SUCCESS, res, "Receive Failed %d", res);

    res = DisplayText(txBuffer, buflen);
    txBuffer.Clear();

    return res;
}

result
ChatForm::ReceiveDataTCP(void)
{
    result res = E_SUCCESS;
    ByteBuffer txBuffer;
    String data;
    unsigned long buflen = 0;

    // Get the size of data received
    __pSocket->Ioctl(NET_SOCKET_FIONREAD, buflen);

    txBuffer.Construct(buflen + 1);

    res = __pSocket->Receive(txBuffer);
    TryReturn(res == E_SUCCESS, res, "Receive Failed");

    res = DisplayText(txBuffer, buflen);
    txBuffer.Clear();

    return res;
}

result
ChatForm::ReceiveData(void)
{
    result res = E_SUCCESS;

    if (__isTcp)
    {
        res = ReceiveDataTCP();
    }
    else
    {
        res = ReceiveDataUDP();
    }

    return res;
}

void
ChatForm::OnClose(void)
{
    result res = E_SUCCESS;
    AppLog("Socket Close Received");

    if (__isConstructed)
    {
        if (__pSocket != null)
        {
            SendDataCommand(L"__CLOSE__");

            res = __pSocket->RemoveSocketListener(*this);
            if (IsFailed(res))
            {
                AppLog("RemoveSocketListener failed %d", res);
            }

            __pSocket->Close();
        }

        if (__pTcpSocket != null)
        {
            __pTcpSocket->Close();
        }
    }

    delete __pSocket;
    delete __pTcpSocket;
    delete __pUdpEndpoint;

    __pUdpEndpoint = null;
    __pSocket = null;
    __pTcpSocket = null;
    __isConnected = false;
    __isConstructed = false;
}

void
ChatForm::EnableControl(bool val)
{
    if (val)
    {
        __pLabelWait->SetShowState(false);
        __pEditAreaData->SetEnabled(true);
        __pEditAreaData->SetShowState(true);
        __pLabelWait->RequestRedraw(true);
    }
    else
    {
        __pLabelWait->SetShowState(true);
        __pEditAreaData->SetEnabled(false);
        __pEditAreaData->SetShowState(false);
        __pLabelWait->RequestRedraw(true);
    }

    __pEditFieldtext->SetEnabled(val);

    __pForm->RequestRedraw();
    __pEditFieldtext->RequestRedraw();
    __pEditAreaData->SetKeypadEnabled(false);
    __pEditAreaData->SetViewModeEnabled(true);
    __pEditAreaData->RequestRedraw();
}

void
ChatForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                       const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
    AppLog("ChatForm::OnSceneActivatedN");

    Ip4Address* pAddress = null;

    if (pArgs != null)
    {
        if (pArgs->GetCount())
        {
            Boolean* pServer = dynamic_cast<Boolean*>(pArgs->GetAt(0));
            TryCatch(pServer != null, , "Failed to retrieve the client/server from the Array list.");

            __isServer = pServer->ToBool();

            if (__isServer)
            {
                Integer* pPort = dynamic_cast<Integer*>(pArgs->GetAt(1));
                TryCatch(pPort != null, , "Failed to retrieve the port number from the Array list.");

                __port = pPort->ToInt();

                Boolean* pProtocol = dynamic_cast<Boolean*>(pArgs->GetAt(2));
                TryCatch(pProtocol != null, , "Failed to retrieve the protocol type from the Array list.");

                __isTcp = pProtocol->ToBool();
            }
            else
            {
                pAddress = dynamic_cast<Ip4Address*>(pArgs->GetAt(1));
                TryCatch(pAddress !=null, , "Failed to retrieve the Ipaddress from the Array list.");

                Integer* pPort = dynamic_cast<Integer*>(pArgs->GetAt(2));
                TryCatch(pPort != null, , "Failed to retrieve the port number from the Array list.");

                __port = pPort->ToInt();

                Boolean* pProtocol = dynamic_cast<Boolean*>(pArgs->GetAt(3));
                TryCatch(pProtocol != null, , "Failed to retrieve the protocol type from the Array list.");

                __isTcp = pProtocol->ToBool();
            }
        }

        if (!__isTcp)
        {
            __pFooter = GetFooter();

            __pFooter->SetItemEnabled(0, true);
        }

        if (!__isServer)
        {
            __pClientIp = new (std::nothrow) Ip4Address(*pAddress);
        }
        else
        {
            __pClientIp = null;
        }

        StartChat();
    }

CATCH:
    pArgs->RemoveAll(true);
    delete pArgs;
}

void
ChatForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
                                        const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
    AppLog("ChatForm::OnSceneDeactivated");
}
